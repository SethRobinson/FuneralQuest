#include "maint.h"
#include "funeral.h" //for server reset stuff
#include "reset_game.h"
#include "command_script.h"
#include "std_all.h"

CAutoSave c_auto_save;
CAutoSave c_reg_check;
CAutoSave c_reconnect;

void add_to_news(CString cst_news)
{
 	CString cst_today;
//	cst_news.Replace("\n","\r\n");
	LogMsg("Adding to log: %s",cst_news);
	cst_today.Format("%snews_day_%d.txt",::st_data_path, glo.i_game_age);
	add_text((char *)(LPCSTR)cst_news, (char *)(LPCSTR)cst_today);
}

void add_to_news_section(int i_news_section, CString cst_news)
{

	CString cst_today;
//	cst_news.Replace("\n","\r\n");
	LogMsg("Adding to log: %s",cst_news);
	if (i_news_section == C_NEWS_SECTION_FRONT)
	cst_today.Format("%snews_day_%d.txt",::st_data_path, glo.i_game_age);
	if (i_news_section == C_NEWS_SECTION_CRIME)
	cst_today.Format("%snews_crime_day_%d.txt",::st_data_path, glo.i_game_age);
	add_text((char *)(LPCSTR)cst_news, (char *)(LPCSTR)cst_today);
}

void process_news_mail(CEvent *event)
{
  if (event->st_give_random[0] == 0) return; //not doing anything special today

  //does at least 1 valid player exist?
  if (::get_number_of_valid_players() < 1) return;

  int i_play = ::get_random_player_excluding(-1); //random player out of everybody

  
  LogMsg("Today's random event happening to %s.",p_play[i_play].st_name);


  	  int i_bonus_luck = random_range(event->i_mod_luck_low, event->i_mod_luck_high);
		int i_bp = random_range(event->i_mod_bp_low, event->i_mod_bp_high);
		int i_turns = random_range(event->i_mod_turns_low, event->i_mod_turns_high);

		int i_things_changed = 0;
		CString cst_reward;

		cst_reward.Format("");
		if (i_bp == 1)
		add_reward_text(&cst_reward, "Bury Point", i_bp, &i_things_changed); else
		add_reward_text(&cst_reward, "Bury Points", i_bp, &i_things_changed);
		
		add_reward_text(&cst_reward, "luck", i_bonus_luck, &i_things_changed);
		if (i_turns == 1)
		add_reward_text(&cst_reward, "turn", i_turns, &i_things_changed); else
        add_reward_text(&cst_reward, "turns", i_turns, &i_things_changed);

		if (i_things_changed > 0) 
		{
		  cst_reward += ".";
		}


  //email this guy the info
  CString cst_message;
  cst_message.Format("%s%s",
  event->st_give_random, cst_reward);
  cst_message.Replace("\\n","\n");
  send_packet_continue_button(i_play,glo.i_game_age+1);
  send_packet(i_play, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody,
	  glo.i_game_age+1);
  //now actually give them the luck
  if (i_bonus_luck != 0)
  send_packet_var_mod(i_play, c_que_sub_type_luck, i_bonus_luck,glo.i_game_age+1);
  if (i_bp != 0)
  send_packet_var_mod(i_play, c_que_sub_type_bp, i_bp,glo.i_game_age+1);
  if (i_turns != 0)
  send_packet_var_mod(i_play, c_que_sub_type_turns, i_turns,glo.i_game_age+1);
  send_packet_picture(i_play, event->st_pic_file_name,glo.i_game_age+1);
  send_packet_continue_button(i_play,glo.i_game_age+1);
  //special action to add to the log
  CStringEx cst_log = event->st_add_log;
  cst_log.Replace("*NAME*", p_play[i_play].st_name);
  cst_log.Replace("*HOME*", p_play[i_play].st_funeral_name);
  cst_log.Replace("\\n","\n");

  cst_log += "\n\n";
  send_packet_add_log(i_play, cst_log, glo.i_game_age+1);

}



bool tournament_ends_today()
{
  if (!glo.b_tourn_active) return false;
  if (glo.i_game_age == glo.i_tourn_days+1) return true;

  return false;
}

bool tournament_ends_tomorrow()
{
  if (!glo.b_tourn_active) return false;
  if (glo.i_game_age == glo.i_tourn_days) return true;

  return false;
}

void add_random_news_sayings()
{
   CString cst_header;
   CString cst_message;


 if (tournament_ends_today())
 {

	 CString cst_temp;
 	 int i_winner = ::get_top_player();	 
  	 if (i_winner != -1)
	 {
  add_to_news("`7TOURNAMENT WINNER HAS BEEN DECLARED!\r\n");
   cst_temp.Format("With %s Bury Points and %s plots, %s has been declared the victor.",
	   int_to_cash(p_play[i_winner].i_exp), ::int_to_cash(p_play[i_winner].i_plots), p_play[i_winner].st_name);
   add_to_news("`^" +cst_temp + "\r\n\r\n");
	 }
   //also let's do this
      add_to_news("`7GIANT METEOR TO DESTROY THE WORLD AT MIDNIGHT!\r\n");
   cst_temp.Format("\"This is a real bummer, but at least we have time to read our mail and stuff.  But who will bury us?,\" comments a local astronomer.");
   add_to_news("`^" +cst_temp + "\r\n\r\n");

   

   //also add to the book of deeds
   
   	 if (i_winner != -1)
	 {
  cst_temp.Format("%s won the 'Best Funeral Directory In The Universe' award.\r\n",p_play[i_winner].st_name);
   add_text((LPCSTR)cst_temp, "deeds.txt");
	 }
   return;
 }


   CEvent event;
   //default to money bags in case we're doing a player event too
   strcpy(event.st_pic_file_name, "flash/stuff/money_bag_2.swf");

   event.compute_event(glo.i_game_age, "news_blurbs.txt");

   cst_message = event.st_message;
   cst_header = event.st_title;
	   


   add_to_news("`7" +cst_header + "\r\n");
   add_to_news("`^" +cst_message + "\r\n\r\n");


   //great, but is some lucky guy going to get extra stuff today?

   process_news_mail(&event);

}


void process_highest_seller_of_the_day()
{
  //first get the highest seller
	int i_index = get_top_player_of_the_day();
 	  CString cst_message;

	if (i_index != -1)
	{
	  //well, there was one so let's do something about it.
	  //give him crap
	  //fill in info so tomorrow we'll know who it was
	  glo.b_top_old_exists = true;
	  glo.i_top_bp_old = p_play[i_index].i_sp;
	  glo.i_top_index_old = i_index;
	  strcpy(glo.st_top_old_name, p_play[i_index].st_name);
	  strcpy(glo.st_parlor_old, p_play[i_index].st_funeral_name);
	  cst_message.Format("`wCongratulations!\n`$You were the top seller for yesterday.  You earn `w%s`$ luck and `w%s`$ Bury Points.\n",
		  int_to_cash(glo.i_bonus_luck), int_to_cash(glo.i_bonus_exp));
	 	if (!tournament_ends_tomorrow())
		{
 
	  send_packet(i_index, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_var_mod(i_index, c_que_sub_type_luck, glo.i_bonus_luck);
	  send_packet_var_mod(i_index, c_que_sub_type_bp, glo.i_bonus_exp);
		}

	} else
		glo.b_top_old_exists = false;





}


void process_highest_parlor_of_the_day()
{
  //first get the highest seller
	int i_index = get_top_player();
  	  CString cst_message;

	if (i_index != -1)
	{
	  //well, there was one so let's do something about it.
	  //give him crap
	  //fill in info so tomorrow we'll know who it was
	  cst_message.Format("`wCongratulations!\n`$You ran the top parlor in town yesterday.  You earn `w%s`$ luck and `w%s`$ Bury Points.\n",
		  int_to_cash(glo.i_bonus_luck), int_to_cash(glo.i_bonus_exp));
	  
	  send_packet(i_index, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_var_mod(i_index, c_que_sub_type_luck, glo.i_bonus_luck);
	  send_packet_var_mod(i_index, c_que_sub_type_bp, glo.i_bonus_exp);

	} else
		glo.b_top_old_exists = false;

    
 	i_index = get_second_highest_player();

	if (i_index != -1)
	{
 	  cst_message.Format("`wCongratulations!\n`$You were the second highest parlor in town yesterday. You earn `w%s`$ luck and `w%s`$ Bury Points.\n",
		  int_to_cash(int(glo.i_bonus_luck*0.5)), int_to_cash(int(glo.i_bonus_exp*0.5)));
	  
	  send_packet(i_index, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_var_mod(i_index, c_que_sub_type_luck, int(glo.i_bonus_luck*0.5));
	  send_packet_var_mod(i_index, c_que_sub_type_bp, int(glo.i_bonus_exp*0.5));
	}

	i_index = get_third_highest_player();

	if (i_index != -1)
	{
 	  cst_message.Format("`wCongratulations!\n`$You were the third highest parlor in town yesterday. You earn `w%s`$ luck and `w%s`$ Bury Points.\n",
		  int_to_cash(int(glo.i_bonus_luck*0.25)), int_to_cash(int(glo.i_bonus_exp*0.25)));
	  
	  send_packet(i_index, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_var_mod(i_index, c_que_sub_type_luck, int(glo.i_bonus_luck*0.25));
	  send_packet_var_mod(i_index, c_que_sub_type_bp, int(glo.i_bonus_exp*0.25));
	}


}


void do_maint_on_user(int i_index)
{
  //it's past midnight; process this pumpkin
  	player_struct *p = &p_play[i_index];
	p->i_sp = 0; //reset BP's for the day
	p->i_yangs_sent = 0; //can send more yangs now
    p->i_tags_recieved_today = 0; //can recieve more tags now via /give

	if (::tournament_ends_today())
	{
		p->i_yangs_sent = ::glo.i_yangs_per_day; //don't let them send yangs

	}


	//should we delete this user?
	if (p->i_day_last_logged_on + sys.i_inactive_days < glo.i_game_age)
	{
	  //delete this guy
		broadcast("%s closes shop and leaves town.", p->st_name);
		p->b_active = false;
	}

		if (p->i_day_last_logged_on + 1 < glo.i_game_age)
	{
	  //police protection leaves
		p->b_police_protected = false;
	}



	if (sys.b_maint_delete && (p->p_on == NULL))
	{
	  if (p->i_exp == 0)
	  {
		broadcast("%s closes shop and leaves town without a single sale.", p->st_name);
		p->b_active = false;
	  }
	}

    if (p->b_active)
    {
        //run our script for it
       RunSinglePassScriptAndSetPlayerID("script\\maint_player.c", i_index);

    }

}

void process_users_for_maint()
{
    //itterate through users and do things to them.  Sexual things. 
    //Just kidding about the sexual part.  (mostly)
    
    for (int i=0; i < g_on.i_cur_players; i++)
    {
        if (p_play[i].b_active)
        {
            //the guy is active
            do_maint_on_user(i);
        }
    }
    
    //Or was I?
}


void process_daily_news()
{
  //nothing to do really

}

CString get_uptime()
{
  //how long ago did the server boot?
  	CString cst_temp;
	static time_t t_now;
	time(&t_now);
	static double dbl_elapsed_seconds;
	
	dbl_elapsed_seconds = difftime(t_now, sys.t_boot_time);
	if (dbl_elapsed_seconds < 1) dbl_elapsed_seconds = 1; //avoid devide by zero, JIC.

	if (dbl_elapsed_seconds > 59) 
	cst_temp.Format("%6.0f Min", dbl_elapsed_seconds / 60);
	 else
	cst_temp.Format("%6.0f Sec", dbl_elapsed_seconds);

	 if ((dbl_elapsed_seconds / 60) > 59)
	 {
	   //more than 59 minutes, let's just say hours and be done with it.
	 	cst_temp.Format("%6.0f Hour(s)", (dbl_elapsed_seconds/60)/60);


	 }
   return cst_temp;
}

void maint_new_day()
{
   	LogMsg("Running maint.");
	
		//do we need to reset the game?
		if (glo.b_tourn_active)
		{
			if (glo.i_game_age == glo.i_tourn_days+1)
			{
			  //let's do it
			shutdown_server();
			LogMsg("Resetting game...");
			reset_game();
			server_init();
			return;


			}

		}

	//first advance the global day counter
    //first delete yesterdays news
	process_highest_seller_of_the_day();
	if (!tournament_ends_tomorrow())
	{
	process_highest_parlor_of_the_day();
	}
   
	process_daily_news();
   glo.i_game_age++;
   process_users_for_maint();
  //great.  Now how about we do a little thing call making a new log file?
    add_random_news_sayings();

   //save time and date we ran this maint on
   time(&glo.t_last_maint); 
 //run other maint stuff?
 //save config
   file_config_save();
 //write a line to a stats file
   CString cst_temp;
   cst_temp.Format("%s %s:Logons=%d, hits=%d\r\n",show_small_date(), show_army_time(), sys.i_logons_today, sys.i_hits_today); 
   add_text((char*)(LPCSTR)cst_temp, "stats.txt");
   //reset system stats
   sys.i_logons_today = 0;
   sys.i_hits_today = 0;
   file_sys_save();
   
   RunSinglePassScript("script\\maint.c");
   //let's reconnect to the FQ License server
   force_reconnect();

}


void maint_time_check()
{

	//this is run every 6 seconds or so as defined in winders_timer.cpp

	//get today	(in days since 1972?)
	time_t ltime;
	time( &ltime );
	tm today = *localtime( &ltime );
	long time_now = (long)today.tm_mday + 256L*(long)today.tm_mon + 65536L*(long)today.tm_year;

	//get last saved maint day
	tm last = *localtime( &glo.t_last_maint );
	long time_last = (long)last.tm_mday + 256L*(long)last.tm_mon + 65536L*(long)last.tm_year;

	if (time_last != time_now)
	{
	
		
		maint_new_day();

	}
	
	//time to autosave?
    if (c_auto_save.minutes_elapsed(sys.i_auto_save_minutes))
	{
		LogMsg("Performing autosave.");
		file_config_save();
		file_sys_save();
		player_save();
	}


}
