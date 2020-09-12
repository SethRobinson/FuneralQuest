#include "commands_misc.h"
  

void send_error_message(LPREQUEST lpReq, CString cst_error)
{
	  LogMsg("Sending error message info to user : %s",cst_error);
      CString cst_final;
	  add_var(&cst_final, "g_st_error", cst_error);
	  add_var(&cst_final, "i_sent","-1"); //-1 means server connection died
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);

}


char * color_check()
  {

   return "\n`1one `2two `3three `4four `5five `6six `7seven `8eight\n `9nine `10ten `!eleven `@twelve `#thirt `$fourten `^sixteen `&seventeen `*eighteen";
  }


void add_turns(CString * pcst_temp, LPREQUEST lpReq)
{
  add_var(pcst_temp, "i_turns", p_play[get_play(lpReq)].i_turns);
}


void add_money(CString * pcst_temp, LPREQUEST lpReq)
{
  add_var(pcst_temp, "i_money", int_to_cash(p_play[get_play(lpReq)].i_money));
}

void add_exp(CString * pcst_temp, LPREQUEST lpReq)
{
  add_var(pcst_temp, "i_exp", int_to_cash(p_play[get_play(lpReq)].i_exp));
}
void add_sp(CString * pcst_temp, LPREQUEST lpReq)
{
  add_var(pcst_temp, "i_sp", int_to_cash(p_play[get_play(lpReq)].i_sp));
}

void add_luck(CString * pcst_temp, LPREQUEST lpReq)
{
  add_var(pcst_temp, "i_luck", p_play[get_play(lpReq)].i_luck);
}



 void command_quit(LPREQUEST lpReq)
 {
  //they want to quit for the day, let's log them off and send client quit packet
  //log off
	 broadcast("%s logged off.", p_play[get_play(lpReq)].st_name);
	 g_on.online_delete_guy(lpReq->i_on_index);
	CString cst_final;
	add_var(&cst_final, "i_sent",c_command_quit);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 }

  void command_no_business(LPREQUEST lpReq)
  {
   //random sayings when you have no turns


  	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;

	int i_random = (abs(rand()%8))+1;
  		cst_url = "flash/wait.swf";
	CEvent event;
	event.compute_event(p_play[get_play(lpReq)].i_exp, "no_customers.txt");
	cst_message = event.st_message;
	p_play[get_play(lpReq)].i_turns--; //deduct a turn
	add_var(&cst_final, "st_main", "`$"+cst_message+"\n\n(one turn wasted)");
	add_var(&cst_final, "st_url", cst_url);
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 


  }

  void command_no_turns(LPREQUEST lpReq)
  {
   //random sayings when you have no turns


  	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;

	int i_random = (abs(rand()%3))+1;
  		cst_url = "flash/closed.swf";

	if (i_random == 1)
	{
		cst_message = "You are out of turns for the day, come back tomorrow!";
	}
	if (i_random == 2)
	{
		cst_message = "It's closing time, you'll have to wait until tomorrow.";
	}
 	if (i_random == 3)
	{
		cst_message = "You look at your watch - it's late.  Maybe tomorrow.";
	}
	add_var(&cst_final, "st_main", "`$"+cst_message);
	add_var(&cst_final, "st_url", cst_url);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 


  }

  CString get_karma_string(LPREQUEST lpReq)
  {
   CString cst_description = "(No text for this karma level.  Email Seth some ideas!)";
   //protection

   force_range(&p_play[get_play(lpReq)].i_karma, 1, 100, "Karma");
   
   if (glo.a_st_karma[p_play[get_play(lpReq)].i_karma-1][0] != 0)
	   cst_description = glo.a_st_karma[p_play[get_play(lpReq)].i_karma];

   CString cst_final;
   cst_final.Format("%d - %s", p_play[get_play(lpReq)].i_karma, cst_description);
   return (cst_final);
  }

  CString get_rating_string(int i_rating_percent)
  {
     CString cst_description  = "(No text for this rating.  Email Seth some ideas!)";
 
	   //protection
   force_range(&i_rating_percent, 1, 100, "Rating");
  
   
   if (glo.a_st_rating[i_rating_percent-1][0] != 0)
	   cst_description = glo.a_st_rating[i_rating_percent-1];

	 
	 
	 CString cst_final;
   //cst_final.Format("%d %% - %s", i_rating_percent, cst_description);
   cst_final.Format("%s", cst_description);
   return (cst_final);
  }
