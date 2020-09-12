#include "commands_getname.h"


//they send us a partial name, we give back a real name and that person's
//id number.  

void command_getname(LPREQUEST lpReq)
  {
 
     d_get_token_string(cst_input, "st_input"); //get name they want
			
	 d_get_token_int(i_his_id, "i_his_id"); //get name they want
			
	 CString cst_name; //name we will return
	 if (cst_input.GetLength() == 0) cst_input = "skljdskljdslkj"; //bad search

	 get_id_from_full_or_partial_handle(i_his_id, cst_input, &cst_name, &i_his_id);
	 
	CString cst_final;
	
	add_var(&cst_final, "st_object", "i_namenext");
	add_var(&cst_final, "i_his_id", i_his_id);
	add_var(&cst_final, "st_message", cst_name);
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }


