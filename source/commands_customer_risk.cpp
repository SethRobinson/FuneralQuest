#include "commands_customer_risk.h"
 
void replace_words(CString *p_s, LPREQUEST lpReq)
{
 	
	if ( (lpReq->i_on_index > g_on.i_cur_people) || (lpReq->i_on_index < 0) )
	{
	  log_error("LpReq was corrupted in replace_words, i_on_index can't be %d.",lpReq->i_on_index);
	  return;
	}
	online_struct *p = p_on[lpReq->i_on_index];
	//SETHFIX
	p_s->Replace("*SIR*", play_sir(get_play(lpReq)));
	p_s->Replace("*NAME*", p_play[get_play(lpReq)].st_name);
	p_s->Replace("*PARLOR*", p_play[get_play(lpReq)].st_funeral_name);
    p_s->Replace("\\n","\n"); //change fake cr's to real ones	
}


void command_risk_accept(LPREQUEST lpReq)
{
   	  online_struct *p = p_on[lpReq->i_on_index];
 	  if (!p->status.b_customer_active)
	{
	  send_error_message(lpReq, "Can't accept, no customer exists");
	  return;
	}
	  if (p->status.i_special_button != ::c_request_double)
	  {
	  log_error("%s can't double with this customer! (accept)", p_play[get_play(lpReq)].st_name);
	  send_error_message(lpReq, "Huh?  Can't double with this customer.");
 	  return;
	  }
   	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;

  	//figure out how many SP's they get
		int i_sp = random_range(4, 10);
		
	
		CString cst_new;
		if (i_sp == 1)
			cst_new = "`$You earned one Bury Point, not bad.\n"; else
		{
			//more than one point earned
			
			cst_new.Format("`$You earned `w%d`$ Bury Points.\n", i_sp);
			
		}
     cst_message = a_risk[p->status.i_risk_type].cst_accept_message;
	replace_words(&cst_message, lpReq);

		cst_message += "\n\n"+cst_new;
		exp_mod(get_play(lpReq),i_sp); 
		
		
		p->status.b_customer_active = false;
		 LogMsg("%s didn't take a risk (%d luck) earned %d bp.",p_play[get_play(lpReq)].st_name,
		 p_play[get_play(lpReq)].i_luck, i_sp);
 
 //fill in the blanks
	add_sp(&cst_final, lpReq);
	add_var(&cst_final, "st_option_3", " Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", cst_message);
	add_var(&cst_final, "st_url", a_risk[p->status.i_risk_type].cst_accept_url);
	if (a_risk[p->status.i_risk_type].cst_accept_bg != "")
	add_var(&cst_final, "st_url_bg", a_risk[p->status.i_risk_type].cst_accept_bg);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
}



void command_risk_start(LPREQUEST lpReq)
{

 	  online_struct *p = p_on[lpReq->i_on_index];
	  if (!p->status.b_customer_active)
	{
	  send_error_message(lpReq, "Can't accept, no customer exists");
	  return;
	}

   p->status.i_special_button = c_request_double;
   p->status.i_risk_type = random(a_risk.GetSize());

 	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;


 //fill in the blanks
    cst_message = a_risk[p->status.i_risk_type].cst_start_message;
	replace_words(&cst_message, lpReq);
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "st_option_3", a_risk[p->status.i_risk_type].cst_accept);
	add_var(&cst_final, "i_command_3", c_request_risk_accept);
 	add_var(&cst_final, "st_option_2", a_risk[p->status.i_risk_type].cst_risk);
	add_var(&cst_final, "i_command_2", c_request_reaction);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", cst_message);
	add_var(&cst_final, "st_url", a_risk[p->status.i_risk_type].cst_start_url);
	if (a_risk[p->status.i_risk_type].cst_start_bg != "")
	add_var(&cst_final, "st_url_bg", a_risk[p->status.i_risk_type].cst_start_bg);
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);

}


void command_double(LPREQUEST lpReq)
 {
 
	

 }

void command_reaction(LPREQUEST lpReq)
{
   	int i_sp = 0;
	online_struct *p = p_on[lpReq->i_on_index];
 	  if (!p->status.b_customer_active)
	{
	  send_error_message(lpReq, "Can't accept, no customer exists");
	  return;
	}
	  if (p->status.i_special_button != ::c_request_double)
	  {
	  log_error("%s can't double with this customer! (reaction)", p_play[get_play(lpReq)].st_name);
 	  send_error_message(lpReq, "Huh?  Can't double with this customer.");
 return;
	  }
 
   	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;
    CString cst_got;

		if (get_random_yes_no(p_play[get_play(lpReq)].i_luck, 0.90f) == false)
    {
	  //they failed.
    cst_message = a_risk[p->status.i_risk_type].cst_fail_message;
	replace_words(&cst_message, lpReq);
	
	add_var(&cst_final, "st_url", a_risk[p->status.i_risk_type].cst_fail_url);
	if (a_risk[p->status.i_risk_type].cst_fail_bg != "")
	add_var(&cst_final, "st_url_bg", a_risk[p->status.i_risk_type].cst_fail_bg);

	   //i_sp = int((double(p_play[get_play(lpReq)].i_sp) * 0.45)+ double((random(p_play[get_play(lpReq)].i_sp+1) /4)));
	
	   i_sp = glo.i_risk; //new way
	   cst_got.Format("`$You lose `#%d`$ Bury Points.",i_sp); 
	  		exp_mod(get_play(lpReq),-i_sp); 

	  
	  LogMsg("%s took a risk (%d luck) lost %d bp.",p_play[get_play(lpReq)].st_name,
		 p_play[get_play(lpReq)].i_luck, i_sp);
 
		} else
		{
		 //success

	 cst_message = a_risk[p->status.i_risk_type].cst_success_message;
	replace_words(&cst_message, lpReq);
	
	add_var(&cst_final, "st_url", a_risk[p->status.i_risk_type].cst_success_url);
	if (a_risk[p->status.i_risk_type].cst_success_bg != "")
	add_var(&cst_final, "st_url_bg", a_risk[p->status.i_risk_type].cst_success_bg);


    //i_sp = 3+p_play[get_play(lpReq)].i_sp + random(random_range(3, 6)*4);
	
	 //new way
	i_sp = glo.i_risk;
	cst_got.Format("`$You earned `w%d`$ Bury Points!",i_sp); 
		exp_mod(get_play(lpReq),i_sp); 

			 LogMsg("%s took risk (%d luck) won %d bp.",p_play[get_play(lpReq)].st_name,
		 p_play[get_play(lpReq)].i_luck, i_sp);
 

		}
  	
	cst_message += "\n\n"+cst_got;
	p->status.b_customer_active = false;
	
 //fill in the blanks
	add_sp(&cst_final, lpReq);
	add_var(&cst_final, "st_option_3", " Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", cst_message);
	add_sp(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
}


