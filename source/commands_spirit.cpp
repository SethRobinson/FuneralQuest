#include "commands_spirit.h"

const int C_SPIRIT_PSYCH = 1;
const int C_SPIRIT_MAIN = 2;
const int C_SPIRIT_STRENGTH = 3;
const int C_SPIRIT_HP = 4;
const int C_SPIRIT_LIBRARY = 5;
const int C_SPIRIT_PAPER_START = 6;
const int C_SPIRIT_PAPER = 7;
const int C_SPIRIT_HEROES = 8;
const int C_SPIRIT_LIBRARY_RESTART = 9;

void command_spirit_library(LPREQUEST lpReq);
void command_spirit_heroes(LPREQUEST lpReq);
	void command_spirit_library_restart(LPREQUEST lpReq);
		  void command_spirit_paper_start(LPREQUEST lpReq);
	   void command_spirit_paper(LPREQUEST lpReq);


void command_spirit_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
	if (i_command_sub == C_SPIRIT_PSYCH)
	{
		command_spirit_psych(lpReq);
		return;
	}

	if (i_command_sub == C_SPIRIT_STRENGTH)
	{
		command_spirit_strength(lpReq);
		return;
	}

	if (i_command_sub == C_SPIRIT_HP)
	{
		command_spirit_hp(lpReq);
		return;
	}

	if (i_command_sub == C_SPIRIT_MAIN)
	{
		command_spirit_menu(lpReq);
		return;
	}

		if (i_command_sub == C_SPIRIT_LIBRARY)
	{
		command_spirit_library(lpReq);
		return;
	}
  	if (i_command_sub == C_SPIRIT_LIBRARY_RESTART)
	{
		command_spirit_library_restart(lpReq);
		return;
	}
   	if (i_command_sub == C_SPIRIT_HEROES)
	{
		command_spirit_heroes(lpReq);
		return;
	}
	if (i_command_sub == C_SPIRIT_PAPER_START)
	{
		command_spirit_paper_start(lpReq);
		return;
	}
	if (i_command_sub == C_SPIRIT_PAPER)
	{
		command_spirit_paper(lpReq);
		return;
	}


}

	void add_class_schedules(CString *cst_final, LPREQUEST lpReq)
	{
		
		CString cst_temp;
		CString cst_main;
		cst_temp.Format("`wClass schedules\n\n");
		cst_main += cst_temp;
		cst_temp.Format("`$PSY - Social Psychology (cost: `w%d`$ gold tags)", get_psych_cost(lpReq));
		cst_main += cst_temp;
		cst_temp.Format("\n`$PE - Physical Education (cost: `w%d`$ gold tags)", get_hp_cost(lpReq));
		cst_main += cst_temp;
		cst_temp.Format("\n`$ST - Strength Training (cost: `w%d`$ gold tags)", get_strength_cost(lpReq));
		cst_main += cst_temp;
		add_var(cst_final, "st_main", cst_main);
		add_button(cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
		add_button(cst_final, 4, "Take PSY",c_request_spirit_callback, C_SPIRIT_PSYCH);                                                 		
		add_button(cst_final, 3, "Take PE",c_request_spirit_callback, C_SPIRIT_HP);		
		add_button(cst_final, 2, "Take ST",c_request_spirit_callback, C_SPIRIT_STRENGTH);                                                 		
		add_button(cst_final, 5, "Library",c_request_spirit_callback, C_SPIRIT_LIBRARY);                                                 		

		add_tags(cst_final, get_play(lpReq));
		
		
	}
		void command_spirit_menu(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	add_class_schedules(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }


		void command_spirit(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	//now let's actually give them the URL to load
  	cst_temp.Format("The role of the funeral director is constantly changing, encompassing more and more responsibilities each year. (for a fee, of course)");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", "Taking classes at SU is a great way to expand your options."); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\psych.swf");
	add_var(&cst_final, "st_url", "");
	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
 	init_location_info(lpReq, c_request_spirit_callback, "Spirit U"); 

	
	add_class_schedules(&cst_final, lpReq);

	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

		int get_psych_cost(LPREQUEST lpReq)
		{
		 return 4+ (p_play[get_play(lpReq)].i_psych)*2;
		}
      	int get_hp_cost(LPREQUEST lpReq)
		{
		 return int((p_play[get_play(lpReq)].i_hp_max)*1.2f);
		}

    	int get_strength_cost(LPREQUEST lpReq)
		{
		 return (p_play[get_play(lpReq)].i_strength)*3;
		}



		void command_spirit_psych(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	CString cst_temp;

	if (get_psych_cost(lpReq) > p_play[get_play(lpReq)].a_coffins[::c_coffin_gold])
	{
	   //can't afford it
	cst_temp.Format("You fumble through your pockets in vain - you don't have enough gold tags to pay for the class.");
	cst_main += cst_temp;
	} else

	{

    

	//remove money
	p_play[get_play(lpReq)].a_coffins[c_coffin_gold] -= get_psych_cost(lpReq);
	//add stats
	p_play[get_play(lpReq)].i_psych++;

    CString cst_extra = "";

     if (p_play[get_play(lpReq)].i_psych == 1)
     {
         cst_extra.Format("\n\n`$You have learned a new skill: `wMood reading");
     }
    
     if (p_play[get_play(lpReq)].i_psych == 2)
     {
         cst_extra.Format("\n\n`$You have learned a new skill: `wFinancial Estimation");
     }

     if (p_play[get_play(lpReq)].i_psych == 3)
     {
         cst_extra.Format("\n\n`$You have learned a new skill: `wCutting resistance in half");
     }
     
     if (p_play[get_play(lpReq)].i_psych == 4)
     {
         cst_extra.Format("\n\n`$You have learned a new skill: `wCutting mood in half");
     }
     if (p_play[get_play(lpReq)].i_psych == 5)
     {
         cst_extra.Format("\n\n`$You have learned a new skill: `wDoubling spending limit");
     }
     
     cst_temp.Format("You study hard for about an hour.  Your psychology level is raised to `w%d`$!%s", 
		p_play[get_play(lpReq)].i_psych+1, cst_extra);
	cst_main += cst_temp;
	add_tags(&cst_final, get_play(lpReq));

    
    
    }
	
	add_var(&cst_final, "st_main", cst_main);
	add_button(&cst_final, 1, "Continue",c_request_spirit_callback, C_SPIRIT_MAIN);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
}


void command_spirit_hp(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	CString cst_temp;

	if (get_hp_cost(lpReq) > p_play[get_play(lpReq)].a_coffins[::c_coffin_gold])
	{
	   //can't afford it
	cst_temp.Format("You fumble through your pockets in vain - you don't have enough gold tags to pay for the class.");
	cst_main += cst_temp;
	} else

	{
	
	//remove money
	p_play[get_play(lpReq)].a_coffins[c_coffin_gold] -= get_hp_cost(lpReq);
	//add stats
	int i_hp = random_range(4, 5);
	p_play[get_play(lpReq)].i_hp_max += i_hp;
	
	cst_temp.Format("You run laps around the gym.  You gain `w%d`$ hitpoints!", 
		i_hp);
	cst_main += cst_temp;
	add_tags(&cst_final, get_play(lpReq));
	}
	
	add_var(&cst_final, "st_main", cst_main);
	add_button(&cst_final, 1, "Continue",c_request_spirit_callback, C_SPIRIT_MAIN);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
}

void command_spirit_strength(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	CString cst_temp;

	if (get_strength_cost(lpReq) > p_play[get_play(lpReq)].a_coffins[::c_coffin_gold])
	{
	   //can't afford it
	cst_temp.Format("You fumble through your pockets in vain - you don't have enough gold tags to pay for the class.");
	cst_main += cst_temp;
	} else

	{
	
	//remove money
	p_play[get_play(lpReq)].a_coffins[c_coffin_gold] -= get_strength_cost(lpReq);
	//add stats
	int i_strength = 1;
	p_play[get_play(lpReq)].i_strength += i_strength;
	cst_temp.Format("You lift weights for a while.  You gain `w1`$ strength!", 
		i_strength);
	cst_main += cst_temp;
	add_tags(&cst_final, get_play(lpReq));
	}
	
	add_var(&cst_final, "st_main", cst_main);
	add_button(&cst_final, 1, "Continue",c_request_spirit_callback, C_SPIRIT_MAIN);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
}


void add_library_text(LPREQUEST lpReq, CString * cst_final)
{
 	CString cst_temp;	
	CString cst_main;
	cst_temp.Format("You are in the library.\n\n");
		cst_main += cst_temp;
		cst_temp.Format("`$Students are quietly studying here.  You see one nerdy looking kid hogging the lone library computer.\n\n");
		cst_main += cst_temp;
		cst_temp.Format("`$The book of deeds is sitting on a pedestal close by.");
		cst_main += cst_temp;
		add_var(cst_final, "st_main", cst_main);
		add_button(cst_final, 1, "Back",c_request_spirit_callback, C_SPIRIT_MAIN);                                                 		
		add_button(cst_final, 2, "Old Papers",c_request_spirit_callback, C_SPIRIT_PAPER_START);                                                 		
		add_button(cst_final, 3, "View Deeds",c_request_spirit_callback, C_SPIRIT_HEROES);                                                 		


}

	void command_spirit_library(LPREQUEST lpReq)
  {
   	CString cst_final;
	add_library_text(lpReq, &cst_final);
 	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

	void command_spirit_library_restart(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	//now let's actually give them the URL to load
  	cst_temp.Format("The role of the funeral director is constantly changing, encompassing more and more responsibilities each year. (for a fee, of course)");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", "Taking classes at SU is a great way to expand your options."); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\psych.swf");
	add_var(&cst_final, "st_url", "");
	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
 	init_location_info(lpReq, c_request_spirit_callback, "Spirit U"); 

	
	add_library_text(lpReq, &cst_final);
 
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);

 
  }

		void command_spirit_heroes(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	//now let's actually give them the URL to load
  	add_var(&cst_final, "st_url_bg", "flash\\places\\psych.swf");
	add_var(&cst_final, "st_url", "");
	add_var(&cst_final, "st_object", "i_view");
	add_var(&cst_final, "i_sent",c_command_object);
	

	//add text to book
	cst_temp.Format("The Book Of Deeds`$\n\n");
	cst_main += cst_temp;

	
	if (!load_file_into_cstring("deeds.txt", &cst_main))
	{
	  //no file exists so no heroes yet
	  cst_temp.Format("No one in Deathville has ever done anything worthing noting.\n\nIt's sad, really.\n");
	  cst_main += cst_temp;
	  int i_top_guy = ::get_top_player();

	  if (i_top_guy	!= -1)
	  {
	  cst_temp.Format("\nThere is a glimmer of hope in `w%s`$, but %s has a way to go.",
		  p_play[i_top_guy].st_name, ::play_he(i_top_guy));
	  	  cst_main += cst_temp;

	  } else
	  {

	   log_error("Error getting top player in view deeds.");
	  }
	}
	
	add_var(&cst_final, "st_view", cst_main);
	
	add_button(&cst_final, 1, "Back",c_request_spirit_callback, C_SPIRIT_LIBRARY_RESTART);                                                 		 
	

	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

 void command_spirit_paper(LPREQUEST lpReq)
  {
 
   d_get_token_int(i_day, "st_extra_text");


	 //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	//now let's actually give them the URL to load
  	add_var(&cst_final, "st_url_bg", "flash\\places\\psych.swf");
	add_var(&cst_final, "st_url", "");
	add_var(&cst_final, "st_object", "i_view");
	add_var(&cst_final, "i_sent",c_command_object);
	
	if ( (i_day < 1) || (i_day > glo.i_game_age-1))
	{
	  cst_temp.Format("You desperately hunt around for anything dealing with a day %d but fail.",
		  i_day);
	cst_main += cst_temp;
	} else
	{
	//add text to book
	cst_temp.Format("You find a yellowing paper from day %d, it looks like it may have been peed on.`$\n\n",
		i_day);
	cst_main += cst_temp;
	CString f_name;
	
	f_name.Format("data\\news_day_%d.txt",i_day);
	load_file_into_cstring(f_name, &cst_main);
	
	//make it legible over black
	cst_main.Replace("`7", "`w");
	cst_main.Replace("`^", "`$");

	}

	
	add_var(&cst_final, "st_view", cst_main);
	
	add_button(&cst_final, 1, "Back",c_request_spirit_callback, C_SPIRIT_LIBRARY_RESTART);                                                 		 
	

	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

	  void command_spirit_paper_start(LPREQUEST lpReq)
{

		  CString cst_final;
	CString cst_main;
	CString cst_temp;
	cst_temp.Format("Which old newspaper would you like to look at?  You can choose between 1 and %d.",
		glo.i_game_age-1);
	add_var(&cst_final, "st_message", cst_temp);
	//clear the default
	add_var(&cst_final, "st_extra_text", "");
	add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_spirit_callback);
	add_var(&cst_final, "i_return_command_sub", C_SPIRIT_PAPER);
    add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}




