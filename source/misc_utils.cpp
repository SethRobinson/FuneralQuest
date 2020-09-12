#include "misc_utils.h"

#include <assert.h>



const CString get_casket_name(int i)
{
 CString cst_casket = "Unknown";
 if (i == ::c_coffin_wood) cst_casket = "Wood";
 if (i == ::c_coffin_silver) cst_casket ="Silver";
 if (i == ::c_coffin_gold) cst_casket = "Gold";

 return cst_casket;
}


CString play_sir(int i_play)
{
  if (p_play[i_play].b_male) return "sir"; else return "ma'am";

}

CString play_he(int i_play)
{
  if (p_play[i_play].b_male) return "he"; else return "she";

}

CString play_He(int i_play)
{
  if (p_play[i_play].b_male) return "He"; else return "She";

}

CString play_his(int i_play)
{
  if (p_play[i_play].b_male) return "his"; else return "her";

} 

CString play_him(int i_play)
{
  if (p_play[i_play].b_male) return "him"; else return "her";

}



void add_reward_text(CString *p_string, CString cst_name, int i_amount, int *p_i_things_changed)
{

  if (i_amount == 0) return; //nothing to add

  //add something
  *p_i_things_changed += 1;

  if (*p_i_things_changed == 1)
  {
    //first time
	  CString cst_temp;
	  
      if (i_amount > 0)
      {
      
      cst_temp.Format("\n\n`$You earn `w%d`$ %s", i_amount, cst_name);
      } else
      {
      cst_temp.Format("\n\n`4You lose `w%d`4 %s", int(abs(i_amount)), cst_name);

      }
	  
      *p_string += cst_temp;

  } else
  {
    //add it
	  CString cst_temp;
	  if (i_amount > 0)
      {
      
      cst_temp.Format(" and `w%d`$ %s", i_amount, cst_name);
      } else
      {
      cst_temp.Format(" and `w%d`4 %s", int(abs(i_amount)), cst_name);

      }
	  *p_string += cst_temp;
  }

}


int get_number_of_valid_players()
{
	int i_counter = 0;
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  if (p_play[i].b_active) i_counter++;
	}
    return i_counter;
}

	int get_random_player_excluding(int i_exclude) //or -1 to not exclude anyone
	{
        //some emergency failsafes
        int i_valid_player = get_number_of_valid_players();
        if (i_exclude > -1)
        {
        
          if (i_valid_player < 2)
          {
              return -1;
          }
        } else
        {
          if (i_valid_player < 1)
          {
              return -1;
          }

        }
		
        int i_chosen = -1;
		
		while (1)
		{

		i_chosen = random(g_on.i_cur_players);
		if (i_exclude != i_chosen)
		if (play_active(i_chosen)) return i_chosen;
		}

	  
      return -1;

	}
	int how_many_people_are_online(int i_status_required)
		{
			int i_counter = 0;
			for (int i=0; i < g_on.i_cur_people; i++)
			{
				if (p_on[i]) if (p_on[i]->b_logged_on) 
				{
					if (i_status_required != -1)
					{
						if (p_on[i]->i_status == i_status_required)
						{
				        	i_counter++;
						}
					} else
					{
					  //no status required
				        	i_counter++;


					}
				}
				
			}
			
			return i_counter;
		}



bool play_active(int i_play)
{
 if (p_play[i_play].b_active) return true;
 return false;
}

int GetValidPlayerID(int i_id)
{
    
   	if (!(in_range(i_id, 0, g_on.i_cur_players)))
    {
        assert(0); 
        return -1; //invalid
    }
    
    
    if (p_play[i_id].b_active == false)
    {
        assert(0); 
        //player is deleted
        return -1;
    } 

    //a-ok
    return i_id;
}




int get_id_from_full_or_partial_handle_bar(CString cst_search)
{
    
    //loop through and see if any of the names match
    for (int i=0; i < g_on.i_cur_players; i++)
    {
        if (play_active(i)) if (p_play[i].p_on)
        {
            if (p_play[i].p_on->i_status == C_STATUS_CHAT)
            {
                
                if (stristr(p_play[i].st_name, cst_search))
                {
                    return i;
                }
            }
        }
        
    }
    
    //couldn't find anything
    return -1;
}


  int get_id_from_full_or_partial_handle(int i_start, CString cst_search, CString *cst_name, int *i_his_id)
  {

	  if (!in_range(i_start, 0, g_on.i_cur_players))
	  {
		 *i_his_id = -1;
		 log_error("Got %d for getname id", i_start);
	//	 LogMsg
		 //we got sent bad data
		 return -1; 

	  }
  
	CStringEx cst_temp_name;
	

   //loop through and see if any of the names match
	for (int i=i_start; i < g_on.i_cur_players; i++)
	{
	 	if (play_active(i))
		{
			cst_temp_name.Format("%s",p_play[i].st_name);

			if (FindNoCase(cst_temp_name, cst_search) != -1)
		{
		 	cst_name->Format("%s",p_play[i].st_name); 
			*i_his_id = i;
			return i;
		}
		}

	}


	*i_his_id = -1;
   //couldn't find anything
	return -1;
  }
   int get_play_from_logon_name(CString cst_name)
{
	
	//loop through and see if any of the names match
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	 	if (play_active(i))
		if (cst_name.CompareNoCase(p_play[i].st_logon) == 0)
		{
		 return i;
		}

	}

	//could not match logon name with anyone
	return -1;
}


   int get_player_who_locked_this_guy(int i_target)
   {
	   int i_return = -1; //nobody (default)
	   
	   //loop through and see if any of the names match
	   for (int i=0; i < g_on.i_cur_players; i++)
	   {
		   if (play_active(i)) if (p_play[i].p_on)
		   {
			   //this guy is on now
			   if (p_play[i].p_on->status.i_his_id == i_target)
			   {
				   if (p_play[i].p_on->status.b_locked)
					   i_return = i;
			   }
			   
		   }
		   
	   }
	   
	   return i_return;
	   
   }
   
   int get_play_from_handle(CString cst_name)
{
	
	//loop through and see if any of the names match
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	 	if (play_active(i))
	//		cst_name.CompareNoCase(
			if (cst_name.CompareNoCase(p_play[i].st_name) == 0)
		{
		 return i;
		}

	}

	//could not match logon name with anyone
	return -1;
}