#include "commands_spirit.h"

const C_SPIRIT_PSYCH = 1;
const C_SPIRIT_MAIN = 2;

void command_spirit_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
	if (i_command_sub == C_SPIRIT_PSYCH)
	{
		command_spirit_psych(lpReq);
		return;
	}
	if (i_command_sub == C_SPIRIT_MAIN)
	{
		command_spirit(lpReq);
		return;
	}

	
}



		void command_spirit(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("The role of the funeral director is constantly changing, encompassing more and more responsibilities each year. (for a fee, of course)");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", "Taking classes at SU is a great way to expand your options."); //blank out the status bar

	CString cst_main;

	cst_temp.Format("`wClass schedules\n\n");
	cst_main += cst_temp;
	cst_temp.Format("`$PSY - Social Psychology (cost: `w%d`$ silver tags)", get_psych_cost(lpReq));
	cst_main += cst_temp;
	add_var(&cst_final, "st_main", cst_main);

	
	add_var(&cst_final, "st_url_bg", "flash\\places\\psych.swf");
	add_var(&cst_final, "st_url", "");
	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
	init_location_info(lpReq, c_request_spirit_callback, "Spirit U"); 
	
	add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
	add_button(&cst_final, 2, "Take PSY",c_request_spirit_callback, C_SPIRIT_PSYCH);                                                 		
	add_tags(&cst_final, lpReq);
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

		int get_psych_cost(LPREQUEST lpReq)
		{
		 return 10+ (p_play[get_play(lpReq)].i_psych)*3;
		}


		void command_spirit_psych(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	CString cst_temp;

	if (get_psych_cost(lpReq) > p_play[get_play(lpReq)].a_coffins[::c_coffin_silver])
	{
	   //can't afford it


	} else

	{
	
	//remove money
	p_play[get_play(lpReq)].a_coffins[c_coffin_silver] -= get_psych_cost(lpReq);
	//add stats
	p_play[get_play(lpReq)].i_psych++;

	cst_temp.Format("You study hard for about an hour.  Your psychology level is raised to `w%d`$!", 
		p_play[get_play(lpReq)].i_psych+1);
	cst_main += cst_temp;
	add_tags(&cst_final, lpReq);

	}
	
	
	add_var(&cst_final, "st_main", cst_main);
	add_button(&cst_final, 1, "Continue",c_request_spirit_callback, C_SPIRIT_MAIN);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
}



