#include "command_store.h"
#include "command_script.h"

const int C_STORE_HEARSE = 100;
const int C_STORE_BUY_HEARSE = 101;
const int C_STORE_SUPPLIES = 102;
const int C_STORE_SECURITY = 103;

void command_store_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
	
	if (i_command_sub == C_STORE_HEARSE)
	{
		store_hearse(lpReq);
		return;
	}

    	if (i_command_sub == C_STORE_SUPPLIES)
	{
        //let's switch to using a script now.
         CommandScriptRunScript(lpReq, "Store", "script\\store_supplies.c");
		return;
	}


    	if (i_command_sub == C_STORE_SECURITY)
	{
        CommandScriptRunScript(lpReq, "Store", "script\\store_security.c");
		return;
	}


 	if (i_command_sub == C_STORE_BUY_HEARSE)
	{
		store_buy_hearse(lpReq);
		return;
	}


	if (i_command_sub > 0) if (i_command_sub < 5)
	{
		store_look(lpReq, i_command_sub);
	}
    
}




   void store_hearse(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
  	add_var(&cst_final, "st_url_bg", "flash\\places\\store.swf");
	add_var(&cst_final, "st_url", "flash\\people\\store_guy.swf");


  CString cst_temp;
	cst_temp.Format("`wElijah: `$\"We carry nothing but the best in funeral transportation.\"\n\n", p_play[get_play(lpReq)].st_name);
	CString cst_message;
	cst_message += cst_temp;
	CString cst_num;
	//add each item
	for (int i=1; i < a_transport.GetSize(); i++)
	{
	  cst_temp.Format("`w%d`$ - %s - Cost: `w%d`$ gold tags.\n", i, a_transport[i].cst_name, 
	  a_transport[i].i_price);
	  cst_message += cst_temp;
	
	  cst_num.Format("View #%d",i);  
	  add_button(&cst_final, (6-i),cst_num, c_request_store_callback, i);
	}
	
	
	add_var(&cst_final, "st_main", cst_message);
	add_button(&cst_final, 1, "Back",c_request_store, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }





	void command_store(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;

	
	cst_temp.Format("Di-Mart is not only an official DuraRest (tm) reseller but also carries many other useful supplies for the death care industry.\n\nA really creepy looking guy in a suit smiles at you.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
	add_var(&cst_final, "st_url_bg", "flash\\places\\store.swf");
	add_var(&cst_final, "st_url", "flash\\people\\store_guy.swf");


	cst_temp.Format("`wElijah: `$\"What can I get for you today, %s?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_tags(&cst_final, get_play(lpReq));
	init_location_info(lpReq, c_request_store_callback, "Store"); 
	add_button(&cst_final, 4, "Security", c_request_store_callback, ::C_STORE_SECURITY);                                                 		
	add_button(&cst_final, 3, "Supplies", c_request_store_callback, ::C_STORE_SUPPLIES);                                                 		
	add_button(&cst_final, 2, "Hearses", c_request_store_callback, ::C_STORE_HEARSE);                                                 		
	add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
    
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

   	void store_look(LPREQUEST lpReq, int i_num)
  {
   //Take a close loko at a hearse
    online_struct *p = p_on[lpReq->i_on_index];
	CString cst_final;
	CString cst_message;
	//now let's actually give them the URL to load
    CString cst_temp;
	add_var(&cst_final, "st_url_bg", a_transport[i_num].cst_url);
	add_var(&cst_final, "st_url", "flash\\blank.swf");

	cst_temp.Format("`wThe %s`$ - %s\n\n", a_transport[i_num].cst_name,
		a_transport[i_num].cst_description, a_transport[i_num].i_bonus_turns);
	cst_message += cst_temp;
	cst_temp.Format("Effect: `w%d`$ extra turns per day.\n\n", a_transport[i_num].i_bonus_turns);
	cst_message += cst_temp;
	cst_temp.Format("After trading in your %s for `w%d`$ gold tags, the cost will be `w%d`$ gold tags.",
	get_transportation(get_play(lpReq)), a_transport[p_play[get_play(lpReq)].i_transportation_type].i_price/2,
	a_transport[i_num].i_price - (a_transport[p_play[get_play(lpReq)].i_transportation_type].i_price/2)); 
	cst_message += cst_temp;
	
	add_var(&cst_final, "st_main", cst_message);
	
	p->status.i_risk_type = i_num; //remember which one we were looking at
	add_button(&cst_final, 2, "Buy it", c_request_store_callback, ::C_STORE_BUY_HEARSE);                                                 		
	add_button(&cst_final, 1, "Back",c_request_store_callback, C_STORE_HEARSE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }


		void store_buy_hearse(LPREQUEST lpReq)
  {
   //Take a close loko at a hearse
    online_struct *p = p_on[lpReq->i_on_index];
    player_struct *p_me = &p_play[get_play(lpReq)];
 
	CString cst_final;
	CString cst_message;
    CString cst_temp;
	
	if ( (p->status.i_risk_type < 1) || (p->status.i_risk_type > a_transport.GetUpperBound())  )
	{
	 return; //shouldn't be here
	}

	int i_num = p->status.i_risk_type; //this is the one they want to buy 
	//can they afford it?

	int i_price = a_transport[i_num].i_price - (a_transport[p_play[get_play(lpReq)].i_transportation_type].i_price/2);

	if (i_price > p_me->a_coffins[::c_coffin_gold])
	{
	  //they can't afford this
	  cst_message.Format("`wElijah: `$\"I'm sorry, I really don't have time to waste.  You obviously cannot afford a luxury sedan such as the %s.\"",
	  a_transport[i_num].cst_name);
	  add_button(&cst_final, 1, "Back",c_request_store_callback, C_STORE_HEARSE);                                                 		
	  add_var(&cst_final, "st_main", cst_message);
	  add_var(&cst_final, "i_sent",c_command_generic);
	  seth_ansi(&cst_final);
	   send_msg(lpReq, cst_final);
 	  return;
	}


	//they can afford it.  Sell it to them.

	
	
	//now let's actually give them the URL to load
//	add_var(&cst_final, "st_url_bg", "flash\\misc\\);
	add_var(&cst_final, "st_url", "flash\\stuff\\money_bag_2.swf");
	cst_message.Format("You trade in your %s for a brand new %s!\n\nYou have time for an extra `w%d`$ customers today.",
	a_transport[p_me->i_transportation_type].cst_name, a_transport[i_num].cst_name, a_transport[i_num].i_bonus_turns);
	cst_message += cst_temp;
	p_me->a_coffins[::c_coffin_gold] -= i_price;
	p_me->i_transportation_type = i_num;
	p_me->i_turns += a_transport[i_num].i_bonus_turns;
	add_tags(&cst_final, p_me->i_index);
	add_button(&cst_final, 1, "Back",c_request_store_callback, C_STORE_HEARSE);                                                 		
	  add_var(&cst_final, "st_main", cst_message);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }
