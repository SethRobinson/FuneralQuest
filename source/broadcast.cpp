#include "stdafx.h"
#include "online_struct.h"
#include "vars.h"
#include "broadcast.h"
#include "all.h"
#include "player_struct.h"
#include "funeral_util.h"
#include "maint.h"
#include "std_all.h"
#include "commands_bar.h"
const int c_broadcast_everybody = -1;


bool add_que_messages (CString *pcst_final, CString *pcst_message, LPREQUEST lpReq)
{
    //add qued messages to the list, handle special cases if required
	int i_on_index = lpReq->i_on_index;
	class_que_packet *que_temp;
	CString cst_temp;
	
	while (p_on[i_on_index]->que.get_reference_to_first(&que_temp))
	{
		//a packet in que exists, handle it
		if (que_temp->q.i_expire != ::c_que_no_expire)
		{
		  if (que_temp->q.i_expire <= glo.i_game_age)
		  {
		    //this packet has expired, delete and ignore it
		  	p_on[i_on_index]->que.delete_first();
			continue;
		  }

		}
	
		if (que_temp->q.c_type == ::c_que_type_add_to_log)
		{
	   		//insert this text into the command string
			add_to_news(que_temp->text);
		}
	
		
		if (que_temp->q.c_type == ::c_que_type_command)
		{
	   		//insert this text into the command string
			cst_temp.Format("%s", que_temp->text);
		    *pcst_final += cst_temp;
			//don't display any more for now
		   p_on[i_on_index]->que.delete_first(); //delete the one we're on
		   return false; //have more to handle after this
		}

		if (que_temp->q.c_type == ::c_que_type_small_command)
		{
	   		//insert this text into the command string
			cst_temp.Format("%s", que_temp->text);
		    *pcst_final += cst_temp;
		}

	    	
		if (que_temp->q.c_type == ::c_que_type_text)
		{
	   		//it's just a text message, easy to handle
			cst_temp.Format("`w%s\n", que_temp->text);
		    *pcst_message += cst_temp;
		    LogMsg("%s got: %s",p_play[get_play(lpReq)].st_name, que_temp->text);
		}
  	if (que_temp->q.c_type == ::c_que_type_simple_text)
		{
	   		//it's just a text message, easy to handle
			cst_temp.Format("`w%s\n", que_temp->text);
		    *pcst_message += cst_temp;
		}

	
		if (que_temp->q.c_type == ::c_que_type_var_mod)
		{
	   		//we're supposed to change a variable, and probably
			//send the change too
			if (que_temp->q.i_sub_type == ::c_que_sub_type_luck)
			{
				luck_mod(get_play(lpReq),que_temp->q.i_value); 
				add_var(pcst_final, "i_luck", p_play[get_play(lpReq)].i_luck);
			}

				if (que_temp->q.i_sub_type == ::c_que_sub_type_bp)
			{
				exp_mod(get_play(lpReq),que_temp->q.i_value); 
				add_var(pcst_final, "i_exp", p_play[get_play(lpReq)].i_exp);
				add_var(pcst_final, "i_sp", p_play[get_play(lpReq)].i_sp);
			}

		}
		
		
		//packet handled, delete it forever
		p_on[i_on_index]->que.delete_first();
	}

	return false; //no special handling required
}


bool add_que_messages_chat (CString *pcst_message, LPREQUEST lpReq)
{
    //add qued messages to the list, handle special cases if required
	int i_on_index = lpReq->i_on_index;
	class_que_packet *que_temp, *que_first;
	CString cst_temp;
	if (!p_on[i_on_index]->que.get_reference_to_first(&que_temp)) return false; //no messages
	que_first = que_temp; //remember who was first
	 class_que new_que;


	while (p_on[i_on_index]->que.get_reference_to_first(&que_temp))
	{
		//a packet in que exists, handle it
		 	
		if (que_temp->q.c_type == ::c_que_type_chat)
		{
	   		//insert this text into the command string
			cst_temp.Format("%s\r", que_temp->text);
		    *pcst_message += cst_temp;
			 
		} else

			if (que_temp->q.c_type == ::c_que_type_simple_text)
		{
	   		//it's just a text message, easy to handle
			cst_temp.Format("`w%s\r", que_temp->text);
		    *pcst_message += cst_temp;
		} else


		{
		  //we don't want to process this one yet.. move to  a temp stack
		  new_que.add_to_end(que_temp);

		}
		//packet handled, delete it forever
		p_on[i_on_index]->que.delete_first();
	}

	//add the ones we were saving now that it's empty

	while(new_que.get_reference_to_first(&que_temp))
	{
	  p_on[i_on_index]->que.add_to_end(que_temp);
	  new_que.delete_first();
	}

	//ok, all done.  new_que will automatically be deleted.
	
	return false; //no special handling required
}


void broadcast_packet(int i_to, CString cst_message, int u_importance, int u_type, 
					  int i_from)
{
   //itterate through all players and add this message to their que
	//Note:  Shouldn't this check p_play and use it's p_on for checking?  This way should
	//work as well but keep this in mind if you make any changes...
	for (int i=0; i < g_on.i_cur_people; i++)
	{
	  if (p_on[i]) if (p_on[i]->b_logged_on)
	  {
		  if ( (i_to == c_broadcast_everybody) || (i_to == p_on[i]->i_play))
		  //he's really online, add the message to his private que
		p_on[i]->que.add_to_end((char*) (LPCSTR) cst_message, u_type, u_importance, i_from);
	  }

	}


} 

void broadcast_packet_chat(int i_to, CString cst_message, int u_importance, int u_type, 
					  int i_from)
{
  	for (int i=0; i < g_on.i_cur_people; i++)
	{
	  if (p_on[i]) if (p_on[i]->b_logged_on) if (p_on[i]->i_status == C_STATUS_CHAT)
	  {
	   	  if ( (i_to == c_broadcast_everybody) || (i_to == p_on[i]->i_play))
	 //he's really online, add the message to his private que
		p_on[i]->que.add_to_end((char*) (LPCSTR) cst_message, u_type, u_importance, i_from);
	  }

	}


}


 void send_packet_var_mod(int i_to, int u_sub_type, int i_new_value, int i_expire)
{
   //send packet to someone, offline or online
	if (!p_play[i_to].b_active)
	{
	  Msg("Tried to send var packet to inactive player (%d)",i_to);
	  return;
	}

	if (p_play[i_to].p_on)
	{
	   //he's on now, send online packet
		p_play[i_to].p_on->que.add_to_end(NULL, ::c_que_type_var_mod, ::c_que_importance_high, ::c_que_nobody,
			u_sub_type, i_new_value, i_expire	);
	   return;
	}


	//make temporary que struct for him
	class_que que_temp;
 	//load all this guys old mail if it exists
	que_temp.load_from_hd(i_to);
	que_temp.add_to_end(NULL, ::c_que_type_var_mod, ::c_que_importance_high, ::c_que_nobody,
	u_sub_type, i_new_value	, i_expire);

	que_temp.save_to_hd(i_to);

}

  void send_packet_var_set(int i_to, int u_sub_type, int i_new_value)
{
   //send packet to someone, offline or online
	if (!p_play[i_to].b_active)
	{
	  Msg("Tried to send var packet to inactive player (%d)",i_to);
	  return;
	}

	if (p_play[i_to].p_on)
	{
	   //he's on now, send online packet
		p_play[i_to].p_on->que.add_to_end(NULL, ::c_que_type_var_set, ::c_que_importance_high, ::c_que_nobody,
			u_sub_type, i_new_value	);
	   return;
	}


	//make temporary que struct for him
	class_que que_temp;
 	//load all this guys old mail if it exists
	que_temp.load_from_hd(i_to);
	que_temp.add_to_end(NULL, ::c_que_type_var_mod, ::c_que_importance_high, ::c_que_nobody,
	u_sub_type, i_new_value);

	que_temp.save_to_hd(i_to);

}


  void send_packet_mail(int i_to, CString * cst_content, int i_from)
  {
    
    CString cst_final;
	CString cst_message;
   //send mail
 
  //now let's send the quotable version
 CStringEx x_content = *cst_content;
 x_content.Replace("\r\n", "\n");
 cst_message.Format("`$You have received a letter from `w%s`$:\n\n%s",p_play[i_from].st_name, x_content);
  send_packet(i_to, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);

  
  //let's only quote the new stuff
  int i_new_start = ReverseFindString(x_content,  "\n>");
  if (i_new_start != -1)
  {
      i_new_start += 1; //go  past the \n> part
	  int i_after_cr = x_content.Find("\n", i_new_start);
	if (i_after_cr != -1)
	{
	  i_new_start = i_after_cr+1;
	  if (x_content[i_new_start] == '\n') i_new_start++; //get that blank c/r too
	}

    //delete the part we don't need anymore
	x_content.Delete(0, i_new_start);
  }


 x_content.Replace("\n", "\n>");

  add_var(&cst_final, "st_quote", ">"+x_content+"\n\n");
	send_packet(i_to, cst_final, ::c_que_importance_high, ::c_que_type_small_command, c_que_nobody); //from nobody

	cst_final = "";
  //give them an option to reply to it
	add_var(&cst_final, "st_option_2", "    Reply");
	add_var(&cst_final, "i_command_2", c_request_reply);
	add_var(&cst_final, "i_his_id", i_from);

	add_var(&cst_final, "st_option_3", "Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	send_packet(i_to, cst_final, ::c_que_importance_high, ::c_que_type_command, c_que_nobody); //from nobody



  }
  
  void send_to_chat(CString p_text, int i_to)
  {
	if (!in_range(i_to, 0,g_on.i_cur_players))
	{
	  log_error("Send_to_chat tried to send to player %d.",i_to);
	  return;

	}
  	if (!p_play[i_to].b_active)
	{
	  log_error("Tried to send var packet to inactive player (%d)",i_to);
	  return;
	}

   send_packet(i_to, p_text, ::c_que_importance_normal, ::c_que_type_chat, ::c_que_nobody);



  }
  void send_packet_continue_button(int i_to, int i_expire)
  {
    CString cst_final;
	add_var(&cst_final, "st_option_3", "Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	send_packet(i_to, cst_final, ::c_que_importance_high, ::c_que_type_command, c_que_nobody, i_expire); //from nobody

  }

   void send_packet_picture(int i_to, const CString cst_url,int i_expire)
  {
    CString cst_final;
	add_var(&cst_final, "st_url", cst_url);
	send_packet(i_to, cst_final, ::c_que_importance_high, ::c_que_type_small_command, c_que_nobody, i_expire); //from nobody
  }	 

    void send_packet_add_log(int i_to, const CString cst_log,int i_expire)
  {
   	send_packet(i_to, cst_log, ::c_que_importance_high, ::c_que_type_add_to_log, c_que_nobody, i_expire); //from nobody
  }


   void send_packet_picture_bg(int i_to, CString cst_url, int i_expire)
  {
    CString cst_final;
	add_var(&cst_final, "st_url_bg", cst_url);
	send_packet(i_to, cst_final, ::c_que_importance_high, ::c_que_type_small_command, c_que_nobody, i_expire); //from nobody
  }

   void send_packet_text(int i_to, CString cst_text, int i_expire)
  {
	send_packet(i_to, cst_text, ::c_que_importance_high, ::c_que_type_text, c_que_nobody, i_expire); //from nobody
  }

void send_packet(int i_to, CString cst_message, int u_importance, int u_type, 
					  int i_from, int i_expire)
{
   //send packet to someone, offline or online
	if (!p_play[i_to].b_active)
	{
	  Msg("Tried to send packet to inactive player (%d)",i_to);
	  return;
	}

	if (p_play[i_to].p_on)
	{
	   //he's on now, send online packet
		p_play[i_to].p_on->que.add_to_end((char*) (LPCSTR) cst_message, u_type, u_importance, i_from,c_que_sub_type_nothing,0,i_expire);
	   return;
	}

	//he's offline.  Let's save it to his mail file.

	if (u_importance == ::c_que_importance_normal) return; //not important, ignore this
	//packet

	//make temporary que struct for him
	class_que que_temp;
 	//load all this guys old mail if it exists
	que_temp.load_from_hd(i_to);
	que_temp.add_to_end((char*) (LPCSTR) cst_message, u_type, u_importance, i_from,c_que_sub_type_nothing,0,i_expire);
	que_temp.save_to_hd(i_to);

}


void broadcast(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s:%s",show_time(),szBuf);
//	OutputDebugString(cst_final);
	broadcast_packet(c_broadcast_everybody, cst_final, c_que_importance_normal, 
	c_que_type_simple_text, c_que_nobody);
	LogMsg(cst_final);
}

void broadcast_chat(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s",szBuf);
//	OutputDebugString(cst_final);
	broadcast_packet_chat(c_broadcast_everybody, cst_final, c_que_importance_normal, 
	c_que_type_chat, c_que_nobody);
	LogMsg(cst_final);
}

void broadcast_tv(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("`7The TV: `w%s",szBuf);
//	OutputDebugString(cst_final);
	
	if (g_tvOn)
	{
		broadcast_packet_chat(c_broadcast_everybody, cst_final, c_que_importance_normal, 
			c_que_type_chat, c_que_nobody);
	}
	
	LogMsg(cst_final);
}


void broadcast_chat_to(int i_id, LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s",szBuf);
//	OutputDebugString(cst_final);
	broadcast_packet_chat(i_id, cst_final, c_que_importance_normal, 
	c_que_type_chat, c_que_nobody);
	LogMsg(cst_final);
}






