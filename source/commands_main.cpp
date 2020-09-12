#include "security.h"



void command_main(LPREQUEST lpReq)

{
	CString cst_final;
	CString cst_url; //no pic
	cst_url = "flash/places/desk.swf";
	
	CString cst_message;
	cst_message = "Error, you should never see this message, young fella.";
	//did we just log on?
	online_struct *p = p_on[lpReq->i_on_index];
   	init_location_info(lpReq, 0, "At Home"); 

	if (!p->b_gave_logon_message)
	{
	  	//should this person be able to log on?  Let's do security stuff.
		p->b_gave_logon_message = true;
	
		if (p_play[get_play(lpReq)].i_day_last_logged_on != glo.i_game_age)
		{
		  //guy just logged on for the first time this day
			cst_url = "flash/morning_coffee.swf";
			new_day_logon(&cst_message, lpReq);
		
			add_logon_stats(&cst_final, ::get_play(lpReq));
			add_var(&cst_final, "st_url", cst_url);
			add_var(&cst_final, "st_object", "m_main");
			add_var(&cst_final, "st_main", cst_message);
			add_var(&cst_final, "st_option_3", "Continue");
			add_var(&cst_final, "i_command_3", c_request_main);
			add_var(&cst_final, "b_kill_buttons", 1);

			
			add_var(&cst_final, "i_sent",c_command_object);
			seth_ansi(&cst_final);
			send_msg(lpReq, (LPCSTR)cst_final);
			return;
			

		} else
		{
		
		//Guy logs on, but he's already been on today.
		cst_message.Format("`$Welcome back, `w%s`$.  Unfinished business?\n", p_play[get_play(lpReq)].st_name);
		}
	

	} else
	{
	  	//already was logged on.
		
		if (p_play[get_play(lpReq)].i_day_last_logged_on != glo.i_game_age)
		{
		  //but wait!  Maint has been run.. while the guy was online!
	  	     cst_url = "flash/morning_coffee.swf";
			  online_day_logon(&cst_message, lpReq);
			 add_logon_stats(&cst_final, ::get_play(lpReq));
			add_var(&cst_final, "st_url", cst_url);
			add_var(&cst_final, "st_object", "m_main");
			add_var(&cst_final, "st_main", cst_message);
			add_var(&cst_final, "st_option_3", "Continue");
			add_var(&cst_final, "i_command_3", c_request_main);
			add_var(&cst_final, "b_kill_buttons", 1);
			add_var(&cst_final, "i_sent",c_command_object);
			seth_ansi(&cst_final);
			send_msg(lpReq, (LPCSTR)cst_final);
			return;

		} else
		{
		  //standard run of the mill showing the main screen
			cst_message = "`$";


		}
	}


   	//add numbers that show on main screen
	add_logon_stats(&cst_final, ::get_play(lpReq));
	add_var(&cst_final, "st_url", cst_url);
	add_var(&cst_final, "st_object", "m_main");


	
	if (!add_que_messages(&cst_final, &cst_message, lpReq))
	{
	//no messages required special handling, let's let the player play
		
	}

	if (cst_message.CompareNoCase("`$") == 0)
	{
	  //they didn't get any special messages, show the default.
		cst_message = "`$You are sitting at your desk.\n";
	   if (p_play[get_play(lpReq)].b_police_protected)
		   cst_message = "`$You are sitting at your desk.  Police are outside, you feel safe.\n";
	}
	add_var(&cst_final, "st_main", cst_message);
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);


}
