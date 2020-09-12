#include "commands_map.h"

void command_map(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	int i_old_status =p_on[lpReq->i_on_index]->i_status;
	
	p_on[lpReq->i_on_index]->i_status = ::C_STATUS_PLAY;

	if (i_old_status == C_STATUS_CHAT)
	{
	  //tell the other guys we left chat, it's only fair
		broadcast_chat("`5%s steps outside.", p_play[get_play(lpReq)].st_name);
	}

    p_on[lpReq->i_on_index]->KillActiveScript(); //in case it was active

    //clear location info
    init_location_info(lpReq, C_SUB_NONE, "Town Map");
	
    //clear any script we were running inside a building
 
	CString cst_final;
	//now let's actually give them the URL to load
	add_var(&cst_final, "st_object", "i_map");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

