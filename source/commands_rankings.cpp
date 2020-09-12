#include "rankings.h"
#include "std_all.h"

void command_ranking(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	
	CString cst_message;
//	CTimer timer_crap;
//	timer_crap.reset();

	add_rankings(&cst_message, false);
	
	CString cst_date;
	cst_date.Format("%s",show_date_month_and_day());
	add_var(&cst_final, "st_date",cst_date);
	add_var(&cst_final, "st_news", ""+cst_message);
	add_var(&cst_final, "i_rank_command", ::c_request_main);
	add_var(&cst_final, "i_sent",c_command_ranking);
//	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
// 		timer_crap.stop_time();
//	LogMsg("Time to rank and send: %d", timer_crap.get_elapsed_milliseconds());

  }

