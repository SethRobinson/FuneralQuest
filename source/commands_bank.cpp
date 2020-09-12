#include "commands_bank.h"


void ConvertAllToGold(LPREQUEST lpReq);
void ConvertAllToWood(LPREQUEST lpReq);


const int C_BANK_EXCHANGE = 1;
const int C_BANK_EXCHANGE_GOLD = 2;
const int C_BANK_EXCHANGE_SILVER = 3;
const int C_BANK_EXCHANGE_WOOD = 4;

const int C_BANK_EXCHANGE_WOOD_TO_GOLD = 5;
const int C_BANK_EXCHANGE_WOOD_TO_SILVER = 6;

const int C_BANK_EXCHANGE_SILVER_TO_GOLD = 7;
const int C_BANK_EXCHANGE_SILVER_TO_WOOD = 8;

const int C_BANK_EXCHANGE_GOLD_TO_WOOD = 9;
const int C_BANK_EXCHANGE_GOLD_TO_SILVER = 10;


const int C_BANK_EXCHANGE_WOOD_TO_GOLD_S = 11;
const int C_BANK_EXCHANGE_WOOD_TO_SILVER_S = 12;

const int C_BANK_EXCHANGE_SILVER_TO_GOLD_S = 13;
const int C_BANK_EXCHANGE_SILVER_TO_WOOD_S = 14;

const int C_BANK_EXCHANGE_GOLD_TO_WOOD_S = 15;
const int C_BANK_EXCHANGE_GOLD_TO_SILVER_S = 16;

const int C_BANK_ALL_TO_GOLD = 17;
const int C_BANK_ALL_TO_WOOD = 18;


const int C_GOLD_EXCHANGE_VALUE = 4;
const int C_SILVER_EXCHANGE_VALUE = 2;
const int C_WOOD_EXCHANGE_VALUE = 1;

void command_bank_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
  	
	if (i_command_sub == C_BANK_EXCHANGE)
	{
		bank_exchange(lpReq);
		return;
	}
		if (i_command_sub == C_BANK_EXCHANGE_WOOD)
	{
		bank_exchange_wood(lpReq);
		return;
	}

		if (i_command_sub == C_BANK_EXCHANGE_SILVER)
	{
		bank_exchange_silver(lpReq);
		return;
	}
				if (i_command_sub == C_BANK_EXCHANGE_GOLD)
	{
		bank_exchange_gold(lpReq);
		return;
	}
		

		if (i_command_sub == C_BANK_EXCHANGE_WOOD_TO_GOLD)
	{
		bank_exchange_all(lpReq, ::c_coffin_wood, c_coffin_gold);
		return;
	}
	if (i_command_sub == C_BANK_EXCHANGE_WOOD_TO_GOLD_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_wood, c_coffin_gold, C_BANK_EXCHANGE_WOOD_TO_GOLD);
		return;
	}
  
	
	if (i_command_sub == C_BANK_EXCHANGE_WOOD_TO_SILVER)
	{
		bank_exchange_all(lpReq, ::c_coffin_wood, c_coffin_silver);
		return;
	}
		if (i_command_sub == C_BANK_EXCHANGE_WOOD_TO_SILVER_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_wood, c_coffin_silver, C_BANK_EXCHANGE_WOOD_TO_SILVER);
		return;
	}
  

  	if (i_command_sub == C_BANK_EXCHANGE_SILVER_TO_GOLD)
	{
		bank_exchange_all(lpReq, ::c_coffin_silver, c_coffin_gold);
		return;
	}
 		if (i_command_sub == C_BANK_EXCHANGE_SILVER_TO_GOLD_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_silver, c_coffin_gold, C_BANK_EXCHANGE_SILVER_TO_GOLD);
		return;
	}
   	if (i_command_sub == C_BANK_EXCHANGE_SILVER_TO_WOOD)
	{
		bank_exchange_all(lpReq, ::c_coffin_silver, c_coffin_wood);
		return;
	}
 		if (i_command_sub == C_BANK_EXCHANGE_SILVER_TO_WOOD_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_silver, c_coffin_wood, C_BANK_EXCHANGE_SILVER_TO_WOOD);
		return;
	}

	  	if (i_command_sub == C_BANK_EXCHANGE_GOLD_TO_SILVER)
	{
		bank_exchange_all(lpReq, ::c_coffin_gold, c_coffin_silver);
		return;
	}
 		if (i_command_sub == C_BANK_EXCHANGE_GOLD_TO_SILVER_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_gold, c_coffin_silver, C_BANK_EXCHANGE_GOLD_TO_SILVER);
		return;
	}

		
		if (i_command_sub == C_BANK_EXCHANGE_GOLD_TO_WOOD)
	{
		bank_exchange_all(lpReq, ::c_coffin_gold, c_coffin_wood);
		return;
	}

    		if (i_command_sub == C_BANK_EXCHANGE_GOLD_TO_WOOD_S)
	{
		bank_exchange_input(lpReq, ::c_coffin_gold, c_coffin_wood, C_BANK_EXCHANGE_GOLD_TO_WOOD);
		return;
	}

   if (i_command_sub == C_BANK_ALL_TO_GOLD)
	{
		ConvertAllToGold(lpReq);
		return;
	}
   if (i_command_sub == C_BANK_ALL_TO_WOOD)
	{
		ConvertAllToWood(lpReq);
		return;
	}


  


}
bool tag_exchange_ok(int i_play, LPREQUEST lpReq, int i_count, int i_from_type, int i_to_type, int *p_i_lost, int *p_i_gained)
{
 //if lpReq is null, no messages will be sent
   	CString cst_temp;
 	CString cst_final;

  	int i_from_value = 1;
	int i_to_value = 3;
	if (i_from_type == c_coffin_wood) i_from_value = C_WOOD_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_silver) i_from_value = C_SILVER_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_gold) i_from_value = C_GOLD_EXCHANGE_VALUE;

	if (i_to_type == c_coffin_gold) i_to_value = C_GOLD_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_silver) i_to_value = C_SILVER_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_wood) i_to_value = C_WOOD_EXCHANGE_VALUE;


  if (i_count == 0)
  {
    bank_exchange(lpReq);
	return false;
  }

  if (i_count < 0)
  {
 	cst_temp.Format("`wTeller: `$\"You are very amusing %s.\"", play_sir(get_play(lpReq)));
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_bank_callback, ::C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	return false;
  }

  if (i_count > p_play[i_play].a_coffins[i_from_type])
  {
 	cst_temp.Format("`wTeller: `$\"You can't exchange what you don't have.  I'm trying to be nice here, but please, think next time.\"");
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_bank_callback, ::C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	return false;
  }

  if ( ((i_from_value*i_count) / i_to_value) < 1)
  {
    //can't even trade one.
   	cst_temp.Format("`wTeller: `$\"You don't understand, do you. You can't trade just %d for a %s tag, alright?\"", i_count, get_casket_name(i_to_type));
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_bank_callback, ::C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	return false;
  }

  //they can exchange at least one.  Let's do it
  int i_to_gain = int((float(i_count) * (float(i_from_value)/ float(i_to_value)))); //will round low

  *p_i_lost = int((float(i_to_gain)*float(i_to_value))/float(i_from_value));

  *p_i_gained = i_to_gain;
  p_play[i_play].a_coffins[i_from_type] -= *p_i_lost;
  p_play[i_play].a_coffins[i_to_type] += *p_i_gained;
 return true;
}



void bank_exchange_all(LPREQUEST lpReq, int i_from_type, int i_to_type)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    d_get_token_string(cst_text, "st_extra_text"); //get the fortune

if ( (cst_text == "12ABORT12"))
{
   bank_exchange(lpReq);
   return;
}
	
	int i_tags = atol(cst_text);
	int i_traded = 0;
	int i_lost, i_gained;

	if (!tag_exchange_ok(get_play(lpReq), lpReq, i_tags, i_from_type, i_to_type, &i_lost, &i_gained))
	{
	   //something went wrong, it was handled
	   return;
	}

	CString cst_temp;
	cst_temp.Format("`wTeller: `$\"Very good, I have converted %d %s tag(s) to %d %s tag(s). Any other tags you would like to convert?\"", i_lost, get_casket_name(i_from_type),
		i_gained, get_casket_name(i_to_type));
	add_var(&cst_final, "st_main", cst_temp);
	add_tags(&cst_final, get_play(lpReq));
	add_button(&cst_final, 2, "Gold", c_request_bank_callback, C_BANK_EXCHANGE_GOLD);
	add_button(&cst_final, 3, "Silver", c_request_bank_callback, C_BANK_EXCHANGE_SILVER);
	add_button(&cst_final, 4, "Wood", c_request_bank_callback, C_BANK_EXCHANGE_WOOD);
	add_button(&cst_final, 1, "Back",c_request_bank, ::C_SUB_NONE);                                                 		
     
	add_var(&cst_final, "i_sent",c_command_generic);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }
	 	
	void bank_exchange_input(LPREQUEST lpReq, int i_from_type, int i_to_type, int i_callback)
  {
 	int i_from_value = 0;
	int i_to_value = 0;
	if (i_from_type == c_coffin_wood) i_from_value = C_WOOD_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_silver) i_from_value = C_SILVER_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_gold) i_from_value = C_GOLD_EXCHANGE_VALUE;

	if (i_to_type == c_coffin_gold) i_to_value = C_GOLD_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_silver) i_to_value = C_SILVER_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_wood) i_to_value = C_WOOD_EXCHANGE_VALUE;


	int i_hold_from = i_to_value/i_from_value;
	int i_hold_to = 1;
	
	if (i_from_type == c_coffin_silver) if (i_to_type == c_coffin_gold) i_hold_from = 2;

	if (i_hold_from < 1)
	{
	  //use other method
		i_hold_to = i_from_value/i_to_value;
		i_hold_from = 1;
	}


		CString cst_final;
	CString cst_temp;
	cst_temp.Format("`wTeller: `$\"%d %s tag(s) = %d %s tag(s).  How many %s tags would you like to exchange?\"",
		i_hold_from, get_casket_name(i_from_type), i_hold_to, get_casket_name(i_to_type) , get_casket_name(i_from_type));
	add_var(&cst_final, "st_message", cst_temp);
	add_var(&cst_final, "st_extra_text", p_play[get_play(lpReq)].a_coffins[i_from_type]);
    add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_bank_callback);
	add_var(&cst_final, "i_return_command_sub", i_callback);
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }



void bank_exchange(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("`wTeller: `$\"Which kind tag would you like to exchange?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 2, "Gold", c_request_bank_callback, C_BANK_EXCHANGE_GOLD);
	add_button(&cst_final, 3, "Silver", c_request_bank_callback, C_BANK_EXCHANGE_SILVER);
	add_button(&cst_final, 4, "Wood", c_request_bank_callback, C_BANK_EXCHANGE_WOOD);
	add_button(&cst_final, 1, "Back",c_request_bank, ::C_SUB_NONE);                                                 		
    
	add_var(&cst_final, "i_sent",c_command_generic);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }


   void bank_exchange_wood(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("`wTeller: `$\"What would you like to convert your Wood to?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 2, "Gold", c_request_bank_callback,  C_BANK_EXCHANGE_WOOD_TO_GOLD_S);
	add_button(&cst_final, 3, "Silver", c_request_bank_callback,  C_BANK_EXCHANGE_WOOD_TO_SILVER_S);
	add_button(&cst_final, 1, "Back",c_request_bank_callback, C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }
  void bank_exchange_silver(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("`wTeller: `$\"What would you like to convert your Silver to?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 2, "Gold", c_request_bank_callback,  C_BANK_EXCHANGE_SILVER_TO_GOLD_S);
	add_button(&cst_final, 3, "Wood", c_request_bank_callback,  C_BANK_EXCHANGE_SILVER_TO_WOOD_S);
	add_button(&cst_final, 1, "Back",c_request_bank_callback, C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

    void bank_exchange_gold(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("`wTeller: `$\"What would you like to convert your Gold to?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 2, "Silver", c_request_bank_callback,  C_BANK_EXCHANGE_GOLD_TO_SILVER_S);
	add_button(&cst_final, 3, "Wood", c_request_bank_callback,  C_BANK_EXCHANGE_GOLD_TO_WOOD_S);
	add_button(&cst_final, 1, "Back",c_request_bank_callback, C_BANK_EXCHANGE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }

    void AddBankCommands(CString &cst_final, LPREQUEST lpReq)
    {
        add_tags(&cst_final, get_play(lpReq));
        add_button(&cst_final, 4, "All to wood", c_request_bank_callback, C_BANK_ALL_TO_WOOD);
        add_button(&cst_final, 3, "All to gold", c_request_bank_callback, C_BANK_ALL_TO_GOLD);
        add_button(&cst_final, 2, "Exchange", c_request_bank_callback, C_BANK_EXCHANGE);
        add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
        
        add_var(&cst_final, "st_object", "i_generic");
        add_var(&cst_final, "i_sent",c_command_object);
        
    }
		void command_bank(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("You are at the city bank.  This facility allows you to exchange casket sales tags and transfer them to other accounts.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar


	cst_temp.Format("`wTeller: `$\"How's business, %s?\"", p_play[get_play(lpReq)].st_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have
	add_var(&cst_final, "st_url_bg", "flash\\places\\bank.swf");
	add_var(&cst_final, "st_url", "");
	init_location_info(lpReq, c_request_bank_callback, "Bank"); 
	

    AddBankCommands(cst_final, lpReq);
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }


void ConvertAllTagsToWood(LPREQUEST lpReq)
{
  int i_play = get_play(lpReq); //get the players id for quicker referencing

  p_play[i_play].a_coffins[c_coffin_wood] += p_play[i_play].a_coffins[c_coffin_silver] * C_SILVER_EXCHANGE_VALUE;
  p_play[i_play].a_coffins[c_coffin_wood] += p_play[i_play].a_coffins[c_coffin_gold] * C_GOLD_EXCHANGE_VALUE;

  //remove the tags we exchanged, if any
  p_play[i_play].a_coffins[c_coffin_silver] = 0;
  p_play[i_play].a_coffins[c_coffin_gold] = 0;

}

void ConvertWoodTagsToGold(LPREQUEST lpReq)
{
   int i_play = get_play(lpReq); //get the players id for quicker referencing

  int i_from_type = c_coffin_wood;
  int i_to_type = c_coffin_gold;
  int i_count = p_play[i_play].a_coffins[i_from_type]; //how many we want to trade in
  
  int i_from_value;
  int i_to_value;

    if (i_from_type == c_coffin_wood) i_from_value = C_WOOD_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_silver) i_from_value = C_SILVER_EXCHANGE_VALUE;
	if (i_from_type == c_coffin_gold) i_from_value = C_GOLD_EXCHANGE_VALUE;

	if (i_to_type == c_coffin_gold) i_to_value = C_GOLD_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_silver) i_to_value = C_SILVER_EXCHANGE_VALUE;
	if (i_to_type == c_coffin_wood) i_to_value = C_WOOD_EXCHANGE_VALUE;

   if ( ((i_from_value*i_count) / i_to_value) < 1)
  {
    //can't even trade one.
   
	return;
  }

  //they can exchange at least one.  Let's do it
  int i_to_gain = int((float(i_count) * (float(i_from_value)/ float(i_to_value)))); //will round low

  int i_lost = 0;
  int i_gained = 0;

  i_lost = int((float(i_to_gain)*float(i_to_value))/float(i_from_value));

  i_gained = i_to_gain;
  p_play[i_play].a_coffins[i_from_type] -= i_lost;
  p_play[i_play].a_coffins[i_to_type] += i_gained;


}


void ConvertAllToWood(LPREQUEST lpReq)
{
   	CString cst_temp;
    CString cst_final;
    
    //first convert all to wood
    ConvertAllTagsToWood(lpReq);
    
    cst_temp.Format("`wA teller converts all your tags to Wood for you.");
    add_var(&cst_final, "st_main", cst_temp);
    AddBankCommands(cst_final, lpReq);
    
    add_var(&cst_final, "i_sent",c_command_generic);
    seth_ansi(&cst_final);
    send_msg(lpReq, cst_final);
    
}



void ConvertAllToGold(LPREQUEST lpReq)
{
    
   	CString cst_temp;
    CString cst_final;
    
    
    //first convert all to wood
    ConvertAllTagsToWood(lpReq);
    
    //now convert back to gold
    ConvertWoodTagsToGold(lpReq);
    
    
    cst_temp.Format("`wA teller converts all your tags to Gold for you.");
    add_var(&cst_final, "st_main", cst_temp);
    
    AddBankCommands(cst_final, lpReq);
    
    add_var(&cst_final, "i_sent",c_command_generic);
    seth_ansi(&cst_final);
    send_msg(lpReq, cst_final);
    
    
    
}