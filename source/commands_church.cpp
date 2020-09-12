#include "commands_church.h"

		void command_church(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("Father Smyth greets you warmly, a little too warmly.  You wonder if he has anything on under that robe.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar


	cst_temp.Format("`wFather Smyth: `$\"Why hello there, %s.  Isn't it a wonderfully fantastic day today?  Praise the Lord.\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);

	
	add_var(&cst_final, "st_url_bg", "flash\\background\\background_church.swf");
	add_var(&cst_final, "st_url", "flash\\people\\churchguy_h.swf");
	init_location_info(lpReq, c_request_church, "Church"); 
	

	add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		

	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

