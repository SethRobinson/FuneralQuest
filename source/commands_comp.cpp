#include "commands_comp.h"
#include "std_all.h"
const int C_COMP_LOOK = 1;
const int C_COMP_VISIT = 2;
const int C_COMP_MENU = 3;
const int C_COMP_GET_NAME = 4;
const int C_COMP_GOT_NAME = 5;
const int C_COMP_TRY_DOOR = 6;
const int C_COMP_STEAL = 7;
const int C_COMP_ATTACK = 8;
const int C_COMP_PICKLOCK = 9;
const int C_COMP_INSIDE_MENU = 10;

	void CommandPickLock(LPREQUEST lpReq);
     void SendInsideMenu(LPREQUEST lpReq);

  void command_comp_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
   	if (i_command_sub == C_COMP_LOOK)
	{
		command_comp_look(lpReq);
		return;
	}

	if (i_command_sub == C_COMP_MENU)
	{
		command_comp_menu(lpReq);
		return;
	}

	   if (i_command_sub == C_COMP_GET_NAME)
	{
		command_comp_get_name(lpReq);
		return;
	}

	   	   if (i_command_sub == C_COMP_GOT_NAME)
	{
		command_comp_got_name(lpReq);
		return;
	}

	 if (i_command_sub == C_COMP_TRY_DOOR)
	{
		command_comp_try_door(lpReq);
		return;
	}

	  if (i_command_sub == C_COMP_INSIDE_MENU)
	{
		SendInsideMenu(lpReq);
		return;
	}

     
     if (i_command_sub == C_COMP_STEAL)
	{
		command_comp_steal(lpReq);
		return;
	}
   		  if (i_command_sub == C_COMP_PICKLOCK)
	{
		CommandPickLock(lpReq);
		return;
	}
	  if (i_command_sub == C_COMP_ATTACK)
	{
		command_comp_attack(lpReq);
		return;
	}



} 

 	void command_comp_look(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	
	CString cst_message;
//	CTimer timer_crap;
//	timer_crap.reset();

	add_rankings_alive(&cst_message);
	
	CString cst_date;
	cst_date.Format("%s",show_date_month_and_day());
	add_var(&cst_final, "st_date",cst_date);
	add_var(&cst_final, "st_news", ""+cst_message);
	add_var(&cst_final, "i_rank_command", ::c_request_comp);
	add_var(&cst_final, "i_sent",c_command_ranking);
	//	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
// 		timer_crap.stop_time();
//	LogMsg("Time to rank and send: %d", timer_crap.get_elapsed_milliseconds());
 }


  void add_comp_menu(CString *cst_final, LPREQUEST lpReq)
  {
	  CString cst_temp;
	  CString cst_main;
	  if (p_play[get_play(lpReq)].i_player_fights >= glo.i_visits_per_day)
	  {
	  cst_temp.Format("You look at your watch, you should really get back to your parlor.\n");
	  } else
	  {
		  if ((glo.i_visits_per_day-p_play[get_play(lpReq)].i_player_fights) == 1)
		  {
		  cst_temp.Format("You look at your watch, you have time to visit `w1`$ more parlor today.\n");
		  } else
		  {
		  cst_temp.Format("You look at your watch, you have time to visit `w%d`$ more parlors today.\n", glo.i_visits_per_day-p_play[get_play(lpReq)].i_player_fights);
		  }
	  }
	  cst_main += cst_temp;
	  add_var(cst_final, "st_main", cst_main);
	  add_button(cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
	  add_button(cst_final, 2, "Look",c_request_comp_callback, ::C_COMP_LOOK);                                                 		
	  add_button(cst_final, 3, "Visit",c_request_comp_callback, ::C_COMP_GET_NAME);                                                 		
  }

  void command_comp_get_name(LPREQUEST lpReq)
{
 	CString cst_final;
	CString cst_main;

	if ( p_play[get_play(lpReq)].i_player_fights >= glo.i_visits_per_day)
	{
	    CString cst_temp;
		cst_temp.Format("Is today look at everyone's parlor day?  No, it's not.  Maybe tomorrow.");
		cst_main += cst_temp;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Continue",c_request_comp_callback, C_COMP_MENU);                                                 		
		add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	} else
  
  {
	add_var(&cst_final, "st_message", "`$Who's funeral home would you like to pay a little visit to?");
    add_var(&cst_final, "st_object", "i_getname");
	add_var(&cst_final, "i_return_command", c_request_comp_callback);
	add_var(&cst_final, "i_return_command_sub", C_COMP_GOT_NAME);
	add_var(&cst_final, "i_sent",c_command_object_add);
	}
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}



  	void command_comp_menu(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
    add_comp_menu(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }


		void command_comp(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	//now let's actually give them the URL to load
  	cst_temp.Format("A dense fog covers the row upon row of funeral homes. You see disturbed, emotionally unstable people"
" trying to choose which parlor to patronize.  You fight the urge to hand out business cards, you"
" have more important things to attend to.");
	
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\palors.swf");
	add_var(&cst_final, "st_url", "flash\\blank.swf");
//	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
 	init_location_info(lpReq, c_request_comp_callback, "Parlors"); 
	add_comp_menu(&cst_final, lpReq);
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }	

	 	void command_comp_in_front(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	online_struct *p = p_on[lpReq->i_on_index];

	 
	 //now let's actually give them the URL to load
  	cst_temp.Format("You are standing in front of a funeral parlor.  The sign above the door proudly announces the name, \"%s\". ", p_play[p->status.i_his_id].st_funeral_name);
	
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\funeral_enemy.swf");
	add_var(&cst_final, "st_url", "flash\\music\\music_sneak.swf");
//	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
   	if (p->status.i_his_id == get_play(lpReq))
	{
		//Uh, we can't attack ourself...
		cst_main.Format("This parlor looks strangely familiar.\n\nYou decide to go inside...",
			p_play[p->status.i_his_id].st_name);
		add_button(&cst_final, 1, "Continue", ::c_request_main, ::C_SUB_NONE);
  	    add_var(&cst_final, "st_main", cst_main);
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
		return;
	}

	if ( p_play[p->status.i_his_id].p_on != NULL)
	{
		//they must be logged on right now
		cst_main.Format("You see %s through the window talking earnestly to a young couple, perhaps later would be better...",
			p_play[p->status.i_his_id].st_name);
		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);
  	    add_var(&cst_final, "st_main", cst_main);
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
		return;
	}
	

		if (p_play[p->status.i_his_id].b_police_protected)
	{
		
		cst_main.Format("You park your %s a few blocks away from %s.  As you approach you see %s talking to some cops, maybe you should come back after things cool down.",
			get_transportation(get_play(lpReq)),p_play[p->status.i_his_id].st_funeral_name, p_play[p->status.i_his_id].st_name);
	 	add_var(&cst_final, "st_url_bg", "flash\\places\\funeral_enemy_police.swf");

		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);
  	    add_var(&cst_final, "st_main", cst_main);
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
	    return;
	}



     //everything is ok
 
	if (!p->status.p_play)
	{
	 	//error, they shouldn't be here if this isn't set
		return;

	}


	//is someone else screwing with our girl?
	int i_guy = get_player_who_locked_this_guy(p->status.i_his_id);
	if (i_guy != -1)
	{
		//they must be logged on right now
		cst_main.Format("As you take your position behind a bush, you are surprised to see %s already sitting there.\n\n`w%s: `$\"Oh, hey %s.  Tell ya what, %s's mine first, ok?\"\n\nYou agree to come back later.",
			p_play[i_guy].st_name, p_play[i_guy].st_name, p_play[get_play(lpReq)].st_name,  play_he(p->status.i_his_id));
		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);
  	    add_var(&cst_final, "st_main", cst_main);
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
		return;


	}
	//we're now officially doing something with this guy.  
	//remove a turn
	p_play[get_play(lpReq)].i_player_fights++;
	
	
	p->status.b_locked = true;
	cst_main.Format("The lights are off, the parlor is silent.  %s must be sleeping inside.\n\n"
	"Nobody seems to be looking, you could try the door-knob if you like.\n\nYou see a `w%s`$ parked nearby.",
			p_play[p->status.i_his_id].st_name,get_transportation(p->status.i_his_id));
		add_button(&cst_final, 2, "Try Door", ::c_request_comp_callback, ::C_COMP_TRY_DOOR);
		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);
	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }	

  void DrawDoorMenu(CString &cst_final)
  {
        add_button(&cst_final, 2, "Pick Lock", ::c_request_comp_callback, ::C_COMP_PICKLOCK);
		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);

  }
  
  
  void SendInsideMenu(LPREQUEST lpReq)
  {

      	CString cst_final;
	CString cst_temp;
	CString cst_main;
    	online_struct *p = p_on[lpReq->i_on_index];
  	if (!p->status.p_play)
	{
	 	//error, they shouldn't be here if this isn't set
		return;

	}

      //now let's actually give them the URL to load
  	cst_temp.Format("You have broken into %s.", p->status.p_play->st_funeral_name);
	
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\desk_enemy.swf");
	add_var(&cst_final, "st_url", "");
//	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have

    cst_main.Format("You quickly slip in and close the door.  You hear %s softly snoring in the back room."
, p->status.p_play->st_name);
   

   
    if (p->status.p_play->a_custom[C_CUSTOM_SAFE] > 0)
    {
        //add text about the safe
        cst_temp.Format("\n\nYou see a large combination `wSafe`$ in the corner.  You won't be able to steal tags.");
        cst_main += cst_temp;
    }
    

    //let's add text about their HP
    cst_temp.Format("\n\nJudging from the room, you would guess %s has `w%d`$ hitpoints.",
     p->status.p_play->st_name, p->status.p_play->i_hp_max);
   cst_main += cst_temp;








	//we're now officially doing something with this guy.  
	p->status.b_locked = true;
	
    if (p->status.p_play->a_custom[C_CUSTOM_SAFE] == 0)
    {
      add_button(&cst_final, 2, "Steal", ::c_request_comp_callback, ::C_COMP_STEAL);
    }
    add_button(&cst_final, 1, "Attack", ::c_request_comp_callback, ::C_COMP_ATTACK);
	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);

  }


	void command_comp_try_door(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	online_struct *p = p_on[lpReq->i_on_index];
  	if (!p->status.p_play)
	{
	 	//error, they shouldn't be here if this isn't set
		return;

	}

    if (p->status.p_play->a_custom[C_CUSTOM_PADLOCK] > 0)
    {
        //they have a lock on the door.  We have to do special stuff.
	p->status.b_locked = true;

    if (p_play[get_play(lpReq)].a_custom[C_CUSTOM_LOCKPICK] > 0)
    {
        cst_main.Format("There is a heavy padlock on the door at `w%d percent strength`y. What do you want to do?\n\nYou are holding `w%s`y lockpick(s)."
         ,p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH], int_to_cash(p_play[get_play(lpReq)].a_custom[C_CUSTOM_LOCKPICK]));
    } else
    {
        cst_main.Format("There is a heavy padlock on the door at `w%d percent strength`y.  What do you want to do?",
           p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH] );
        
    }
    
      DrawDoorMenu(cst_final);

	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
    return;
    }


    //let them in

	SendInsideMenu(lpReq); 
  }	


    void CommandPickLock(LPREQUEST lpReq)
    {
        //News screen.  Send all the info they need then load the screen.
        CString cst_final;
        CString cst_temp;
        CString cst_main;
        online_struct *p = p_on[lpReq->i_on_index];
        if (!p->status.p_play)
        {
            //error, they shouldn't be here if this isn't set
            return;
            
        }
        
        if (p_play[get_play(lpReq)].a_custom[C_CUSTOM_LOCKPICK] < 1)
        {
            //can't lockpick, no picks.
            cst_main.Format("You attempt to pick the lock with your finger.  No go.  Perhaps if you had a real lockpick...");
            DrawDoorMenu(cst_final);
            add_var(&cst_final, "st_main", cst_main);
            add_var(&cst_final, "i_sent",c_command_generic);
            seth_ansi(&cst_final);
            send_msg(lpReq, cst_final);
            return;
        }
        
       
        CString cst_parlor_owner;
        int i_amount_weakened = random_range(5, 10);
        p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH] -= i_amount_weakened;

        if (p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH] < 1)
        {
            //broke the lock right off
            p->status.p_play->a_custom[C_CUSTOM_PADLOCK] = 0;
            p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH] = 0;
            
            cst_main.Format("As you are picking the lock the whole thing breaks and falls off!  Too bad your lockpick got stuck inside it.");
            
            //tell guy about it
            cst_parlor_owner.Format("`yYour padlock is `4damaged beyond repair`y by a prowler and falls off!");
            send_packet(p->status.i_his_id, cst_parlor_owner, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
            
        }  else
        {
            //weakened the lock.
            cst_main.Format("`yYou successfully pick the lock.  Your lockpick breaks as you try to get it out which causes the padlock `w%d`y points of damage.", 
                i_amount_weakened);
            //tell guy about it
            cst_parlor_owner.Format("`yYour padlock is weakened by `4%d`y down to `w%d percent strength`y by a prowler.",
                i_amount_weakened,p->status.p_play->a_custom[C_CUSTOM_PADLOCK_STRENGTH] );
            send_packet(p->status.i_his_id, cst_parlor_owner, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
            
        }

        add_button(&cst_final, 1, "Continue", ::c_request_comp_callback, ::C_COMP_INSIDE_MENU);
        //remove a lockpick
        p_play[get_play(lpReq)].a_custom[C_CUSTOM_LOCKPICK]--;
        
        add_var(&cst_final, "st_main", cst_main);
        add_var(&cst_final, "i_sent",c_command_generic);
        seth_ansi(&cst_final);
        send_msg(lpReq, cst_final);
        return;
    }
    
    

		void command_comp_got_name(LPREQUEST lpReq)
{
 
	CString cst_final;
	CString cst_main;
		
	d_get_token_int(i_his_id, "i_his_id");
   
   if (i_his_id == -1)
   {
	   //they couldn't find a name, just show the menu again
	   command_comp_menu(lpReq);
	   return;
   }

   if ( (!in_range(i_his_id, 0, g_on.i_cur_players)) || (!play_active(i_his_id)) )
   {
	   command_comp_menu(lpReq);
	   return;
   }

  	  if (p_play[get_play(lpReq)].i_player_fights >= glo.i_visits_per_day)
	  {
	   command_comp_menu(lpReq);
	   return;
	  }


	

	//entrance granted
	online_struct *p = p_on[lpReq->i_on_index];
	p->status.i_his_id = i_his_id; //save player #
	p->status.p_play = &p_play[i_his_id];
	command_comp_in_front(lpReq);
}

			void command_comp_steal(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	online_struct *p = p_on[lpReq->i_on_index];
  	if (!p->status.p_play)
	{
	 	//error, they shouldn't be here if this isn't set
		return;
	}

if (p->status.p_play->a_custom[C_CUSTOM_SAFE] > 0)
{
     log_error("Player shouldn't be able to steal from this guy, he has a safe! Are they trying to hack FQ?");
     return;


}
	 //steal stuff
  
	//which item should they steal?
	
	int i_stolen = 0;
	CString cst_type;
	i_stolen = steal_random_caskets(3, &cst_type, &p_play[get_play(lpReq)], p->status.p_play);

	if (i_stolen != -1)
	{
	  //they have some to steal.
		if (i_stolen == 0)
		{
	       cst_temp.Format("You begin wildly opening drawers and things looking for anything of value.");
		} else
		{
		  if (i_stolen == 1)
		  {
		     cst_temp.Format("You look around and find `w1 %s`$ tag hidden on top of the refrigerator!",
				 cst_type);
		  } else
		  {
		     cst_temp.Format("You look around and find `w%d %s`$ tags in a drawer!", i_stolen,
				 cst_type);
			 }

		}

	} else
	{
	  cst_temp.Format("You look around but can't find anything of value.");
	}

	cst_main += cst_temp;
	CString cst_sexy = "pajamas";
	if (p_play[get_play(lpReq)].b_male) if (!p->status.p_play->b_male)
		cst_sexy = "lingerie";
	cst_temp.Format("\n\nYou are startled by a noise behind you.  It's %s in %s %s! You crash through a window and run like hell.",
		p->status.p_play->st_name, play_his(p->status.i_his_id), cst_sexy);
	cst_main += cst_temp;
	//now write mail.
	CString cst_message;
				
  	        cst_temp.Format("\n`wYou caught %s sneaking around inside your parlor!`$\n",p_play[get_play(lpReq)].st_name);
			cst_message += cst_temp;
			   
			if (i_stolen == 1)
			{
			  cst_temp.Format("You scare %s off.  You notice `w%d %s tag `$is missing.", play_him(get_play(lpReq)), i_stolen, cst_type);
		
			} else if (i_stolen > 1)
			{
			  cst_temp.Format("You scare %s off.  You notice `w%d %s tags `$are missing.",play_him(get_play(lpReq)), i_stolen, cst_type);

			} else
			{
			  //stole nothing
			  cst_temp.Format("You scare %s off before %s could steal anything.",play_him(get_play(lpReq)), play_he(get_play(lpReq)));
			}
				cst_message += cst_temp;
			
		send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
        send_packet_picture(p->status.i_his_id, "flash\\people\\man_prowl.swf");
	    send_packet_continue_button(p->status.i_his_id);
		
		//now actually give them the luck
    	//	send_packet_continue_button(p->status.i_his_id);
		add_button(&cst_final, 1, "Leave", ::c_request_comp, ::C_SUB_NONE);
		add_var(&cst_final, "st_main", cst_main);
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
	}	


		void command_comp_attack(LPREQUEST lpReq)
  {
	online_struct *p = p_on[lpReq->i_on_index];
  	if (!p->status.p_play)
	{
	 	//error, they shouldn't be here if this isn't set
		return;

	}

	//setup the attack then send it over the main attack engine
	int i_me = get_play(lpReq);
    int i_enemy = p->status.i_his_id;
	
	//max out hitpoints
	p_play[i_me].i_hp = p_play[i_me].i_hp_max;
	p_play[i_enemy].i_hp = p_play[i_enemy].i_hp_max;

		command_battle(lpReq);
		}



//me is always the guy doing the stealing
//will return -1 if there is nothing to steal, 0 if they just fail

int steal_random_caskets(int i_max_to_steal, CString *cst_type, player_struct *p_me, player_struct *p_him)
{

 	int i_tag_type = random(3);
    int i_stolen = 0;
	int i_max_temp = 3;
	*cst_type = "error";
	if (i_tag_type == ::c_coffin_wood)
	{
	  *cst_type = "wooden";
	}

	if (i_tag_type == ::c_coffin_silver)
	{
	  *cst_type = "silver";
	}
	
	if (i_tag_type == ::c_coffin_gold)
	{
	  *cst_type = "gold";
	}

	if (p_him->a_coffins[i_tag_type] > 0)
	{
	  //they have some to steal.
		i_max_temp = random_range(0, i_max_to_steal);
		if (i_max_temp > p_him->a_coffins[i_tag_type]) i_max_temp = p_him->a_coffins[i_tag_type];
		i_stolen = i_max_temp;

	//actually mode tag(s)
	p_him->a_coffins[i_tag_type] -= i_stolen;
	p_me->a_coffins[i_tag_type] += i_stolen;
	} else
	{
	  //they just don't have any
		return -1;

	}
 return i_stolen;
}