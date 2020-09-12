#include "command_random.h"
#include "Cevent.h"



void command_random_event_free_bp(LPREQUEST lpReq)
 {
   //random sayings when you have no turns


  	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;
	CString cst_header;
	CString cst_reward;

		cst_header = "`wSPECIAL EVENT\n\n";

		
		CEvent event;
		event.compute_event(p_play[get_play(lpReq)].i_exp, "events.txt");
		cst_message.Format("`$%s", event.st_message);
		cst_url = event.st_pic_file_name;
		
		int i_bonus_luck = random_range(event.i_mod_luck_low, event.i_mod_luck_high);
		int i_bp = random_range(event.i_mod_bp_low, event.i_mod_bp_high);
		int i_turns = random_range(event.i_mod_turns_low, event.i_mod_turns_high);

		int i_things_changed = 0;

		cst_reward.Format("");
		if (abs(i_bp) == 1)
		add_reward_text(&cst_reward, "Bury Point", i_bp, &i_things_changed); else
		add_reward_text(&cst_reward, "Bury Points", i_bp, &i_things_changed);
		
		add_reward_text(&cst_reward, "luck", i_bonus_luck, &i_things_changed);
		if (abs(i_turns) == 1)
		add_reward_text(&cst_reward, "turn", i_turns, &i_things_changed); else
        add_reward_text(&cst_reward, "turns", i_turns, &i_things_changed);

		if (i_things_changed > 0) 
		{
		  cst_reward += ".";
		}
	
	exp_mod(get_play(lpReq), i_bp);
	luck_mod(get_play(lpReq), i_bonus_luck);
	turns_mod(get_play(lpReq), i_turns);
	add_sp(&cst_final, lpReq);
	add_exp(&cst_final, lpReq);
	add_luck(&cst_final, lpReq);
	add_var(&cst_final, "st_main", cst_header+cst_message+cst_reward);
	add_var(&cst_final, "st_url", cst_url);
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 }


void command_random_event(LPREQUEST lpReq)
 {
   //random sayings when you have no turns


  	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;
	CString cst_header;
		cst_url = "flash/people/m_adult_4.swf";

		cst_header = "`wSPECIAL EVENT - VISIT FROM OPAL\n\n";
		cst_message = "`$\"Greetings, friend.\"\n\n";
	add_var(&cst_final, "st_main", cst_header+cst_message);
	add_var(&cst_final, "st_url", cst_url);
	add_var(&cst_final, "st_option_2", "Accept");
	add_var(&cst_final, "i_command_2", c_request_reaction);
	add_var(&cst_final, "st_option_3", "No thanks");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 

 }

