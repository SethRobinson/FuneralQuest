#include "Security.h"

SwearArray a_swear;
BanArray a_ban;



bool is_banned(LPREQUEST lpReq)
{
	 CStringEx cst_my_ip = my_ip(lpReq);
 for (int i=0; i < a_ban.GetSize(); i++)
 {
	 if (FindNoCase(cst_my_ip, a_ban[i].cst_ip) >= 0)
	 {
	   //IP is banned.
		 LogMsg("Ignoring IP %s, on ban list. (%s)",
			 cst_my_ip, a_ban[i].cst_comment);
		 	 return true;
	 }
 }


return false;
}


//returns true if user should get kicked for swearing (not used)
bool swear_check(CStringEx *cst_in)
{
	if (!sys.b_swear_filter) return false;
	//replace swear words with better words
 	 for (int i=0; i < a_swear.GetSize(); i++)
	 {
		 ReplaceNoCase(*cst_in, a_swear[i].cst_bad_word, a_swear[i].cst_good_word);
		
	 }
 return false;

}


void load_swear_filter_from_file()
{
	a_swear.RemoveAll(); //reset it
	CSwear rtemp;
	
	if (!exist("swear_words.txt")) log_error("Can't find swear_words.txt.");
	CTextParse cls_parse;
	cls_parse.open("swear_words.txt");
	
	//ready to read actual stuff!
	char *p_buffer;
	char st_crap[255];
	
	while(cls_parse.get_next_line(&p_buffer))
	{
		strcpy(st_crap, cls_parse.get_word(1));
		
		if (strcmp(st_crap, "addword") == 0)
		{
			//clear vars/set defaults
			rtemp.cst_bad_word = cls_parse.get_word(2);
			rtemp.cst_good_word = cls_parse.get_word(3);
			//add this word 
			a_swear.Add(rtemp);
		}
		
		{
			//nothing we know
		}
		
	}
	
}

void load_ban_filter_from_file()
{
	a_ban.RemoveAll(); //reset it
	CBan rtemp;
	if (!exist("ban.txt")) log_error("Can't find ban.txt.");
	CTextParse cls_parse;
	cls_parse.open("ban.txt");
	
	//ready to read actual stuff!
	char *p_buffer;
	char st_crap[255];
	
	while(cls_parse.get_next_line(&p_buffer))
	{
		strcpy(st_crap, cls_parse.get_word(1));
		
		if (strcmp(st_crap, "ban") == 0)
		{
			//clear vars/set defaults
			rtemp.cst_ip = cls_parse.get_word(2);
			rtemp.cst_comment = cls_parse.get_word(3);
			//add this word 
			a_ban.Add(rtemp);
		}
		
		{
			//nothing we know
		}
		
	}
	
}


bool cannot_log_in_new_user(LPREQUEST lpReq)
{
 

  SOCKADDR_IN * sock;
  sock = (SOCKADDR_IN*) lpReq->lpSockAddr;
 
 //cycle through players and see if this IP exists
 for (int i=0; i < g_on.i_cur_players; i++)
 {
	 //make sure it's not us
	 if ( p_play[i].b_active && (p_play[i].b_always == false))
	 {
		 //only will check those who have logged on today
		 if (p_play[i].i_day_last_logged_on == glo.i_game_age)
			 
		 {
		   if ( (p_play[i].last_ip.S_un.S_addr == sock->sin_addr.S_un.S_addr) && (p_play[i].b_always == false))
		   {
               CString cst_t;
			   cst_t.Format("User with IP %s not allowed to create a new user. (already on as %s (%s) today)",
				   my_ip(lpReq),  p_play[i].st_name  , inet_ntoa(p_play[i].last_ip));
			  	 
			   log_error(cst_t);
			  send_error_message(lpReq, "Sorry, this game has activated a rule that only allows "
				 "one player per IP address per day.");
   	          

			 return true;


		   }
		 }

	 }


 }



 return false;
};

bool cannot_log_in(LPREQUEST lpReq, int i_play)
{
 
  SOCKADDR_IN * sock;
  sock = (SOCKADDR_IN*) lpReq->lpSockAddr;
 
 if (p_play[i_play].b_always)
 {
   //no security check done with this account, it's been marked "always alllowed on" in the editor
    return false;

 }

 //cycle through players and see if this IP exists
 for (int i=0; i < g_on.i_cur_players; i++)
 {
	 //make sure it's not us
	 if ( (i != i_play) && (p_play[i].b_active) && (p_play[i].b_always == false))
	 {
		 //only will check those who have logged on today
		 if (p_play[i].i_day_last_logged_on == glo.i_game_age)
			 
		 {
		   if (p_play[i].last_ip.S_un.S_addr == sock->sin_addr.S_un.S_addr)
		   {
               CString cst_t;
			   cst_t.Format("User with IP %s can't logon as %s because he already played as %s (%s) today.",
				   my_ip(lpReq), p_play[i_play].st_name, 
				   p_play[i].st_name
				   , inet_ntoa(p_play[i].last_ip));
			  	 
			   log_error(cst_t);
			 	 broadcast("%s was kicked, someone with that IP already played today.", p_play[i_play].st_name);
			   send_error_message(lpReq, "Sorry, this game has activated a rule that only allows "
				 "one player per IP address per day.");
   	          

			 return true;


		   }
		 }

	 }


 }



 return false;
};