#include "globals.h"
#include "CSpeech.h"

void command_news(LPREQUEST lpReq)
  {
	init_location_info(lpReq, glo.i_game_age, "News"); 
	
	//show news
	command_news_front(lpReq);
  }
void command_news_front(LPREQUEST lpReq)
{
		online_struct *p = p_on[lpReq->i_on_index];
	//News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	CString cst_message;
 	//add information about highest ranked BP of the day
	//load today.txt file line by line and display it
	CString cst_today;
	
	//first let's add out own custom stuff to the news if needed

	if (glo.b_tourn_active)
	{
	  if (glo.i_game_age+5 > glo.i_tourn_days)
	  {
	    //game is almost over
		  if (glo.i_game_age == glo.i_tourn_days)
		  {
		    //this is the last day
			  cst_message += "`5Today is the final day of the tournament - a winner will be declared at midnight!\n\n";

		  }	  else
		  {
			 
			  if (!::tournament_ends_today())
			  {
			  //nearing last day
		      if (glo.i_game_age == glo.i_tourn_days-1)
			  {
			  	  //day before last day
				  cst_message += "`5Tomorrow is the last day of the tournament.\n\n";
			  } else
			  {

				  cst_message.Format("`5Only %d days left before a tournament winner shall be declared!\n\n",
					  glo.i_tourn_days-glo.i_game_age);
			  }
			  }
		  }
	  }
	}
	
	
	cst_today.Format("%snews_day_%d.txt",::st_data_path, p->status.i_location_callback);
	FILE *fp = open_file_cst(cst_today);
	
	CString cst_line;
	if (fp)
	{
		while (get_line_from_file_cst(fp, &cst_line))
		{
			cst_message += cst_line;
		}
		fclose(fp);
	} else
	{
		cst_message += "Nothing has happened today.";
	}
	
	
	int i_top_guy = ::get_top_player_of_the_day();

    CString cst_top_today;
	if (i_top_guy != -1)
	{
		//talk about highest seller of the day
		cst_top_today.Format("%s from %s with %d Bury Points.", p_play[i_top_guy].st_name,
			p_play[i_top_guy].st_funeral_name, p_play[i_top_guy].i_sp);
	} else
	{
		  //nobody has sold anything yet today
		 cst_top_today.Format("Sadly, no one has managed to sell anything yet today.");
	}
	 
	 //top guy from yesterday
	 
	 CString cst_top_yesterday;
	
	if ( glo.b_top_old_exists)
	{
		 cst_top_yesterday.Format("%s from %s with %d Bury Points.\n\n", glo.st_top_old_name,
		 glo.st_parlor_old, glo.i_top_bp_old);
	} else
	{
		  //there was no top seller from yesterday
		 cst_top_yesterday.Format("No one died yesterday.");
	}

	if (::tournament_ends_today())
	{
		int i_winner = ::get_top_player();
		cst_top_today.Format("Top seller of the universe is %s.", 
			p_play[i_winner].st_name);
	}

	CString cst_date;
//	cst_date.Format("Day %d",show_date_month_and_day());
	cst_date.Format("Day %d",p->status.i_location_callback);
	add_var(&cst_final, "st_date",cst_date);
	add_var(&cst_final, "st_top_today",cst_top_today);
	add_var(&cst_final, "st_top_yesterday",cst_top_yesterday);
	add_var(&cst_final, "st_news", cst_message);
	if (p->status.i_location_callback == glo.i_game_age)
	add_var(&cst_final, "i_yesterday", 0); else
	add_var(&cst_final, "i_yesterday", 1); 
	add_var(&cst_final, "i_sent",c_command_news);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
}

void command_news_crime(LPREQUEST lpReq)
{
	 online_struct *p = p_on[lpReq->i_on_index];
	//News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	CString cst_message;
 	//add information about highest ranked BP of the day
	//load today.txt file line by line and display it
	CString cst_today;
	cst_today.Format("%snews_crime_day_%d.txt",::st_data_path, p->status.i_location_callback);
	FILE *fp = open_file_cst(cst_today);
	CString cst_line;
	if (fp)
	{
		while (get_line_from_file_cst(fp, &cst_line))
		{
			cst_message += cst_line;
			
		}
	fclose(fp);
	} else
		cst_message += "There has been no crime reported today.";
	
	
	CString cst_date;
//	cst_date.Format("Day %d",show_date_month_and_day());
	cst_date.Format("Day %d",p->status.i_location_callback);
	add_var(&cst_final, "st_date",cst_date);
	add_var(&cst_final, "st_news", cst_message);
	if (p->status.i_location_callback == glo.i_game_age)
	add_var(&cst_final, "i_yesterday", 0); else
	add_var(&cst_final, "i_yesterday", 1); 
	add_var(&cst_final, "i_sent",c_command_news);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
}

void command_yesterday_news(LPREQUEST lpReq)
  {

	init_location_info(lpReq, glo.i_game_age-1, "News"); 
   	//show news
	command_news_front(lpReq);

  }

  void command_announcement(LPREQUEST lpReq)
  {
   	  //they want to post
	   	d_get_token_string(cst_message, "st_note");
		
	//	cst_message.Replace("\r\n", "");

		CString cst_temp;
		swear_check((CStringEx*)&cst_message);		
		
		format_for_local_cst(&cst_message);

		
		//let's strip all ` codes out
		cst_message.Remove('`');
		
		//setup url support
		CStringEx cst_work;
		cst_work = cst_message;



again:
		
		int i_start = cst_work.Find("[");
		int i_end = cst_work.Find("]");



		if (i_start != -1) if (i_end != -1)
		{
			CStringEx cst_url;

			cst_url = GetDelimitedField(cst_work, "[", "]");

			cst_work.Delete(i_start, (i_end-i_start)+1);
			cst_temp.Format("`#<a href=\"http://%s\" target=\"_blank\">%s`6</a>",
				cst_url, cst_url);
			//don't allow nested brackets
			cst_temp.Replace("[","");
			cst_temp.Replace("]","");

			cst_work.Insert(i_start, cst_temp);
			goto again;
		}
	
		if (p_play[get_play(lpReq)].b_male)
		{
		g_glo.p_speech->say("%s announces: <pitch middle = '-30'><rate speed = '0'> %s.", p_play[get_play(lpReq)].st_name,
			cst_work);
		} else
		{
		g_glo.p_speech->say("%s announces: <pitch middle = '10'><rate speed = '0'> %s.", p_play[get_play(lpReq)].st_name,
			cst_work);

		}


		cst_temp.Format("`8%s of %s makes an announcement:`6\r\n",p_play[get_play(lpReq)].st_name,
		p_play[get_play(lpReq)].st_funeral_name);
		add_to_news(cst_temp);
		add_to_news(cst_work+"\r\n\r\n");

	  //great, have them see the news they just posted
	  command_news(lpReq);
 
  }

