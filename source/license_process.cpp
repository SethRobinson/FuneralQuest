//Code to understand what the license server is saying.  Seth Robinson '01

#include "license_process.h"
#include "license_vars.h"
#include "server_connect.h"
#include "file_system.h"
#include "CProtection.h"

void process_message_from_server(LPREQUEST lpReq)
{
	 if (!cls_pro.is_validated())
     {
     
     d_get_token_string_no_return(cst_message, "message");
      log_msg_thread_safe(cst_message);
     }
}


void process_verification(LPREQUEST lpReq)
{
	//removed drm
	
}


void process_welcome_from_server(LPREQUEST lpReq)
{
	//let's send our logon info 
   	CString cst_final;
	add_var(&cst_final, "command", C_L_COMMAND_LOGON_REQUEST_FROM_CLIENT);
	add_var(&cst_final, "lname", sys.st_license_logon);
	add_var(&cst_final, "lpass", sys.st_license_password);
	add_var(&cst_final, "ldescrip", sys.st_license_description);
	add_var(&cst_final, "email", glo.st_admin_email);
	add_var(&cst_final, "game_name", sys.st_license_game_name);
	add_var(&cst_final, "advertise", (int)sys.b_advertise);
	add_var(&cst_final, "tourney", (int)glo.b_tourn_active);
	add_var(&cst_final, "day", (int)glo.i_game_age);
	add_var(&cst_final, "port", sys.i_port);
	add_var(&cst_final, "id", cls_pro.get_serial());
	add_var(&cst_final, "force_ip", sys.st_force_ip);

	convert_and_send_to_server(&cst_final);
}

bool process_messages_from_license_server(CString *p_cst_m)
{
      REQUEST Req;
	  LPREQUEST lpReq = &Req;
	  memset(lpReq, 0, sizeof(Req));
	  
	  if (!tokenize_input(&Req, *p_cst_m))
	  {
	    log_msg_thread_safe("Unable to understand response from master server.");
		return false;
	  }
 	//save the important stuff

	  get_token_int(&Req, "command", &Req.nMethod);
//	  log_msg_thread_safe("Got command %d from master server.",Req.nMethod);

	  switch (Req.nMethod)
	  {
	  case C_L_COMMAND_MESSAGE_FROM_SERVER:
	   process_message_from_server(&Req);
	  break;

	  case C_L_COMMAND_WELCOME_FROM_SERVER:
	   process_welcome_from_server(&Req);
	  break;

	  case C_L_COMMAND_LOGON_VERIFICATION_FROM_SERVER:
	   process_verification(&Req);
	  break;

	  default:
	  log_msg_thread_safe("Unknown command from license server, ignoring it.  Maybe we need to upgrade?");

	  }
   return true; //success?
}