#pragma once 

void broadcast(LPCSTR lpFormat, ...);
bool add_que_messages (CString *pcst_final, CString *pcst_message, LPREQUEST lpReq);
bool add_que_messages_chat (CString *pcst_message, LPREQUEST lpReq);
void send_packet_var_mod(int i_to, int u_sub_type, int i_new_value, int = c_que_no_expire);
void send_packet_var_set(int i_to, int u_sub_type, int i_new_value);
void send_packet_continue_button(int i_to, int = c_que_no_expire);
  void send_packet_picture(int i_to, const CString cst_url, int = c_que_no_expire);
 
void send_packet_picture_bg(int i_to, CString cst_url, int = c_que_no_expire);
void send_packet_mail(int i_to, CString *cst_content, int i_from);
void broadcast_chat(LPCSTR lpFormat, ...);
 void send_to_chat(CString p_text, int i_to);
void send_packet_text(int i_to, CString cst_text);
void send_packet(int i_to, CString cst_message, int u_importance, int u_type, 
				 int i_from, int = c_que_no_expire);
void send_packet_add_log(int i_to, const CString cst_log,int i_expire);
void broadcast_packet(int i_to, CString cst_message, int u_importance, int u_type, 
					  int i_from);
void broadcast_chat_to(int i_id, LPCSTR lpFormat, ...);
void broadcast_tv(LPCSTR lpFormat, ...);

 


 


