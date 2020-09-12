#include "commands_customer.h"  
#include "location_data.h"

void command_wait_for_customer(LPREQUEST lpReq)
 {
 	if (p_play[get_play(lpReq)].i_turns < 1)
	{
	  //no turns left, dumbass!
      command_no_turns(lpReq);
	  return;
	}
	
	//this is the backbone of the game
	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;

	if (frandom(1) < glo.f_normal_odds)

	{
	  //super duper random event
		command_random_event_free_bp(lpReq);
		return;
	}
	
	/*
	i_random = random(10)+1;
	if (i_random ==	1)
	{
	  //no business hapenning
		command_no_business(lpReq);
		return;

	}
	  */

	create_random_customer(lpReq);

	online_struct *p = p_on[lpReq->i_on_index];


	if (frandom(1) < glo.f_luck_odds)
	{
	  //risk event
  	//let's do a luck/risk event
	command_risk_start(lpReq);
	return;

	}

   	if (frandom(1) < glo.f_script_odds)
	{
	  //scripted event

     //let's setup the script callback default.
    init_location_info(lpReq, c_request_script_callback, "Home");

    CString cst_script_fname = "script\\script_event.c";

    //load this script
    online_struct *p = p_on[lpReq->i_on_index];
    p->InitScript((LPCSTR)cst_script_fname);
    return;
	}

 
    
  	p_play[get_play(lpReq)].i_turns--; //deduct a turn
 
    command_complicated_customer(lpReq);
	return;
}	


  void command_accept(LPREQUEST lpReq)
 {
 	  online_struct *p = p_on[lpReq->i_on_index];
	  if (!p->status.b_customer_active)
	{
	  send_error_message(lpReq, "Can't accept, no customer exists");
	  return;
	}
	
	//this is the backbone of the game
	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;
	
	//header for customer message
	CString cst_c_man = p->c_man();
	CString cst_c_young = p->c_a_young();
	CString cst_d_relation = p->d_relation();
	CString cst_casket = p->casket();
	CString cst_d_him = p->d_him();
	
	//get best picture available
	cst_url = p->c_flash_url();
	
	CString cst_customer = "`wTHE CEREMONY IS A SUCCESS!\n\n";
	
	int i_rand = random(2);
	if (i_rand == 0) cst_url = "flash/places/funeral-ceremony.swf";
	if (i_rand == 1) cst_url = "flash/places/funeral-ceremony2.swf";
	//increment coffins sold counter for this coffin type
	p_play[get_play(lpReq)].a_coffins[p->status.e_desired_casket]++;
	
	//incrememnt the plots sold
	p_play[get_play(lpReq)].i_plots++;
	
	//figure out how many SP's they get
	int i_sp = glo.i_plot_sp;

	i_sp += glo.a_casket_sp[p->status.e_desired_casket];
	
	if (i_sp == 1)
	cst_message = "`$You earned one Bury Point, not bad.\n"; else
	{
	  //more than one point earned
	 	
		cst_message.Format("`$You earned `w%d`$ Bury Points.\n", i_sp);

	}

	p_play[get_play(lpReq)].i_sp += i_sp;
	p_play[get_play(lpReq)].i_exp += i_sp;
	p->status.b_customer_active = false;
	add_sp(&cst_final, lpReq);
	add_exp(&cst_final, lpReq);
	add_var(&cst_final, "st_main",cst_customer+cst_message);
	add_var(&cst_final, "st_url", cst_url);
	add_var(&cst_final, "st_url_bg", "flash/blank.swf"); //blank out the background

	add_var(&cst_final, "st_option_3", "Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);

	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 }
