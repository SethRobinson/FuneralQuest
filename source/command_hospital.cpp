#include "commands_hospital.h"

const int C_HOSPITAL_LEAVE_ROOM = 1;

void command_hospital_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
   	if (i_command_sub == C_HOSPITAL_LEAVE_ROOM)
	{
		command_hospital_leave_room(lpReq);
		return;
	}

}



		void command_hospital(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("You are at the city hospital.  A nurse smiles at you.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar


	cst_temp.Format("`wNurse: `$\"Hi %s.  I don't recall you having a pick-up today.\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);

	
	add_var(&cst_final, "st_url_bg", "flash\\places\\hospital_desk.swf");
	add_var(&cst_final, "st_url", "");
	init_location_info(lpReq, c_request_hospital_callback, "Hospital"); 
	

	add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		

	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

 		void command_hospital_room(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("You are lying on a bed, it doesn't look like your own.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar

	cst_temp.Format("`$Your whole body aches.");
	add_var(&cst_final, "st_main", cst_temp);
 	init_location_info(lpReq, c_request_hospital, "Hospital"); 
 	add_var(&cst_final, "b_show_battle", "2"); //turn off battle meters in flash
 
	add_var(&cst_final, "st_url_bg", "flash\\places\\hospital_room.swf");
	add_var(&cst_final, "st_url", "flash\\blank.swf");
	init_location_info(lpReq, c_request_hospital_callback, "Hospital"); 
	add_button(&cst_final, 1, "Leave",c_request_hospital_callback, C_HOSPITAL_LEAVE_ROOM);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

	void command_hospital_leave_room(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("You are at the city hospital.  A nurse smiles at you.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar

	CString cst_prince;
	if (p_play[get_play(lpReq)].b_male) cst_prince = "prince charming"; else cst_prince = "sleeping beauty";
	cst_temp.Format("`wNurse: `$\"I see you finally woke up, %s.  Hope you'll be more careful next time.\"", cst_prince);
	add_var(&cst_final, "st_main", cst_temp);

	
	add_var(&cst_final, "st_url_bg", "flash\\places\\hospital_desk.swf");
	add_var(&cst_final, "st_url", "");

	add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		

	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }


