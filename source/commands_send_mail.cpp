#include "commands_send_mail.h"

void command_send_mail(LPREQUEST lpReq)
  {
 
     d_get_token_string(cst_input, "st_input"); //get message
			
	 d_get_token_int(i_his_id, "i_his_id"); //get address
	CString cst_final;

	 
	if ( (!in_range(i_his_id, 0, g_on.i_cur_players)) || (!play_active(i_his_id)) )
	{
	add_var(&cst_final, "st_object", "i_message");
	add_var(&cst_final, "st_message", "Can't email, player doesn't exist?");
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	return;
	} 
	

	//let's send the email
   
    send_packet_mail(i_his_id, &cst_input, get_play(lpReq));

	//if the guy is online and chatting let's let him know.
	broadcast_chat_to(i_his_id, "You have mail waiting for you at your parlor."); 

	//tell the log about it
    LogMsg("%s wrote email to %s: %s",p_play[get_play(lpReq)].st_name,
		p_play[i_his_id].st_name, cst_input);

	add_var(&cst_final, "i_sent",c_command_callback);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }

