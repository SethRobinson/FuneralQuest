  //the majority of client/server communication is here

#include "rankings.h"
#include "commands.h"
#include "commands_map.h"
#include "commands_misc.h"		  
#include "commands_getname.h"
#include "commands_send_mail.h"
#include "commands_bar.h"
#include "commands_city_hall.h"

#include "commands_yangs.h"
#include "commands_newday.cpp"
#include "commands_main.cpp"
#include "create_customer.h"
#include "command_random.h"
#include "commands_customer.h"
#include "commands_news.cpp"
#include "commands_rankings.cpp"
#include "commands_customer_risk.h"
#include "commands_complicated.h"
#include "commands_church.h"   
#include "location_data.h"		  
#include "commands_spirit.h"
#include "commands_comp.h"	  
#include "commands_hospital.h"
#include "commands_bank.h"
#include "command_store.h"
#include "command_options.h"
#include "command_script.h"


  void add_tags(CString *cst_final, int i_play)
  {

	add_var(cst_final, "i_coffin_wood", int_to_cash(p_play[i_play].a_coffins[::c_coffin_wood]));
	add_var(cst_final, "i_coffin_silver", int_to_cash(p_play[i_play].a_coffins[::c_coffin_silver]));
	add_var(cst_final, "i_coffin_gold", int_to_cash(p_play[i_play].a_coffins[::c_coffin_gold]));

  }
 
  

  void process_command(LPREQUEST lpReq)
{
   	d_get_token_int(i_command, "i_command");

   //only users who are logged on can use these commands.  Let's make sure they
   //are
      int i_play = p_on[lpReq->i_on_index]->i_play;
 
   CString cst_final;
 
   if (i_play == -1)
   {
	 //user not logged on
	   send_error_message(lpReq, "Connection with server died. (user not logged on)");
	   return;

   }
 
   p_play[i_play].p_on = p_on[lpReq->i_on_index]; //for easy access later



   //everything is fine, let's process his command.
if (sys.b_extra_debug)   
   LogMsg("%s: Command %d",my_ip(lpReq),i_command);

   switch(i_command)
   {
   case c_request_quit:
	   command_quit(lpReq);
	   break;
   case c_request_wait_for_customer:
       p_on[lpReq->i_on_index]->KillActiveScript();

       command_wait_for_customer(lpReq);
	   break;
   case c_request_stats:
    CommandScriptRunScript(lpReq, "Stats", "script\\stats.c");

   break;
   case c_request_news:
   command_news(lpReq);
   break;

   case c_request_front:
   command_news_front(lpReq);
   break;

   case c_request_crime:
   command_news_crime(lpReq);
   break;
   
   case c_request_ranking:
   command_ranking(lpReq);
   break;
			
    case c_request_map:
   command_map(lpReq);
   break;
  
   case c_request_yesterday:
   command_yesterday_news(lpReq);
   break;

    case c_request_announcement:
   command_announcement(lpReq);
   break;


   case c_request_main:
       //kill a script if its active
       p_on[lpReq->i_on_index]->KillActiveScript();
       
       command_main(lpReq);
   break;

   case c_request_accept:
   command_accept(lpReq);
   break;
   case c_request_double:
   command_double(lpReq);
   break;
   case c_request_reaction:
   command_reaction(lpReq);
   break;

   case c_request_yangs:
     p_on[lpReq->i_on_index]->KillActiveScript();
   
    command_yangs(lpReq);
   break;
   
   case c_request_yangs_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_yangs_callback(lpReq);
   break;
   
   case c_request_getname:
   command_getname(lpReq);
   break;
   case c_request_send_mail:
   command_send_mail(lpReq);
   break;
  case c_request_bar:

      command_bar(lpReq);
   break;

   case c_request_chat_start:
   command_chat_start(lpReq);
   break;

   case c_request_chat_update:
   command_chat_update(lpReq);
   break;

   case c_request_finish_sale:
   command_complicated_finish(lpReq, false);
   break;

   case c_request_soft_sell:
   command_complicated_soft_sell(lpReq);
   break;

   case c_request_hard_sell:
   command_complicated_hard_sell(lpReq);
   break;

   case c_request_guilt:
   command_complicated_guilt(lpReq);
   break;

   case c_request_sympathy:
   command_complicated_sympathy(lpReq);
   break;

   case c_request_risk_accept:
   command_risk_accept(lpReq);
   break;
   case c_request_city_hall:
   command_city_hall(lpReq);
   break;
  
   case c_request_spirit:
   command_spirit(lpReq);
   break;

    case c_request_spirit_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_spirit_callback(lpReq);
   break;

  case c_request_comp:
   command_comp(lpReq);
   break;

    case c_request_comp_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_comp_callback(lpReq);
   break;
   case c_request_battle_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_battle_callback(lpReq);
   break;

    case c_request_hospital:
   	   command_hospital(lpReq);
   break;

   case c_request_hospital_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_hospital_callback(lpReq);
   break;

    case c_request_bank:
   	   command_bank(lpReq);
   break;

   case c_request_bank_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_bank_callback(lpReq);
   break;


    case c_request_store:
             p_on[lpReq->i_on_index]->KillActiveScript();
  
        command_store(lpReq);
   break;

   case c_request_script:
   	   CommandScript(lpReq);
   break;


   case c_request_store_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   command_store_callback(lpReq);
   break;

     case c_request_options:
            p_on[lpReq->i_on_index]->KillActiveScript();
         command_options(lpReq);
   break;
    case c_request_options_callback:
        if (location_callback_ok(lpReq, i_command))
	   command_options_callback(lpReq);
   break;
     case c_request_script_callback:
   	   if (location_callback_ok(lpReq, i_command))
	   CommandScriptCallback(lpReq);
   break;



   default:
	   //if you get here, there was a problem.
	CString cst_error;
	cst_error.Format("Server unable to process request %d.",i_command);
	log_error("%s: Server error:  Can't process request %d?",my_ip(lpReq),i_command);
	   send_error_message(lpReq, cst_error);
	break;

   }


}