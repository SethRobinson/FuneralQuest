#include "command_script.h"

void base_new_day(CString *cst_message, LPREQUEST lpReq, CString *cst_bonus)
{
   //result daily stats
  	CString cst_temp;
	int i_play = get_play(lpReq); //for speed and easy of use

   //reset coffins sold today
   //for (int i=0; i < c_max_coffin_types; i++)
   //p_play[i_play].a_coffins[i] = 0;
   
//   p_play[i_play].i_plots = 0; //don't reset plots

   //refill turns they get today, deleting any they have not used
   p_play[i_play].i_turns = glo.i_turns_per_day;
   

//do some emergency fixes if needed to fix a corrupt database
   if (p_play[i_play].i_transportation_type > a_transport.GetCount())
   {
	   LogError("%s had an illegal transportation value of %d, setting to 0.", p_play[i_play].st_name, p_play[i_play].i_transportation_type);
	   p_play[i_play].i_transportation_type = 0;
   }

if (p_play[i_play].i_exp < 0)
{
	LogError("%s had negative bury points?  Setting to 0.", p_play[i_play].st_name, p_play[i_play].i_transportation_type);
	p_play[i_play].i_exp = 0;
}

   //give any extra turn bonus they have coming to them
   p_play[i_play].i_turns += a_transport[p_play[i_play].i_transportation_type].i_bonus_turns;

   //reset SP for the day
   p_play[i_play].i_sp = 0;
   p_play[i_play].b_kicked_out_of_yangs = false; //no longer kicked out
   
   //update last day logged on to today
   p_play[i_play].i_day_last_logged_on = glo.i_game_age;
   p_play[i_play].i_player_fights = 0; //have not fought anyone yet today
   p_play[i_play].b_police_protected = false; //by by police
//   CString cst_temp;
//   cst_temp.Format("\n\n");
   //`$For logging on today, you receive `w%d`$ Bury Points.\n",
	 //  glo.i_experience_per_day);
 //  *cst_message += cst_temp;
//   p_play[i_play].i_exp += glo.i_experience_per_day;
  
 RunSinglePassScriptAndSetPlayerID("script\\newday_player.c", i_play);
 
   
   if (::tournament_ends_today())
   {
	  //don't let them do anything
	   p_play[i_play].i_turns = 0;
	   p_play[i_play].i_player_fights = ::glo.i_visits_per_day;

   }


if (p_play[i_play].i_transportation_type > 0)
	{
	  //they get a turn bonus.
		cst_temp.Format("Your %s gives you time for `w%d`$ extra customers. ",
			get_transportation(i_play), a_transport[p_play[i_play].i_transportation_type].i_bonus_turns);
		*cst_bonus += cst_temp;
	}

if (p_play[i_play].b_police_protected)
{
  cst_temp.Format("There are police patrolling your parlor.");
  		*cst_bonus += cst_temp;

}

}


void new_day_logon (CString * cst_message, LPREQUEST lpReq)
{
	CString cst_bonus;
   base_new_day(cst_message, lpReq, &cst_bonus);


	

	if (p_play[get_play(lpReq)].i_day_last_logged_on == 0) //first time to log on, a new user!
{


  cst_message->Format("`wWelcome to Funeral Quest, %s.  Good luck.`^\n\n`$\
You unlock the front door and enter your beloved %s \
Funeral Parlor.  Ah, it's great to be alive.\n\n%s"
	   , p_play[get_play(lpReq)].st_name, p_play[get_play(lpReq)].st_funeral_name, cst_bonus);
  


} else
{

  cst_message->Format("`wWelcome back, %s!`^\n\n`$\
You unlock the front door and enter your beloved %s \
Funeral Parlor.  Ah, it's great to be alive.\n\n%s"
	   , p_play[get_play(lpReq)].st_name, p_play[get_play(lpReq)].st_funeral_name, cst_bonus);
  }

  
 
}


void online_day_logon (CString * cst_message, LPREQUEST lpReq)
{
  	CString cst_temp_first;
	CString cst_bonus;
	base_new_day(cst_message, lpReq, &cst_bonus);


	
	cst_temp_first.Format("`wYou're eyes grow heavy.  You wake up the next morning, still at your desk.\n\n");
   
   CString cst_temp;
   cst_temp.Format("`$You quickly freshen up.. who knows what great things await %s Funeral Parlor today?\n\n%s",
	p_play[get_play(lpReq)].st_funeral_name, cst_bonus);
   *cst_message = cst_temp_first+cst_temp;
//   send_packet_text(get_play(lpReq), cst_temp_first+cst_temp);
//   send_packet_continue_button(get_play(lpReq));
   

}