
#include "commands_yangs.h"
#include "location_data.h"

const int C_YANGS_DRAGON = 3;
const int C_YANGS_SPARROW = 1;
const int C_YANGS_TIGER = 2;
const int C_YANGS_SHOW_MENU = 100;
const int C_YANGS_GET_FORTUNE = 101;
const int C_YANGS_SEND = 102;
const int C_YANGS_GET_FORTUNE_SPARROW = 103;
const int C_YANGS_SEND_SPARROW = 104;
const int C_YANGS_BITCHY = 105;

class yangs_food
{
public:
	int i_cost[3];
  int i_mod[3];
  yangs_food()
  {
	i_cost[C_YANGS_DRAGON] = 10;
	i_mod[C_YANGS_DRAGON] = 10;

	i_cost[C_YANGS_SPARROW] = 20;
	i_mod[C_YANGS_SPARROW] = 5;

	i_cost[C_YANGS_TIGER] = 20;
	i_mod[C_YANGS_TIGER] = 10; //will MINUS
  };


} cls_yangs;
 

void command_yangs_bitchy(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	
		CString cst_temp;
		cst_temp.Format("`w%s: `$\"That's very interesting.  Now get the god damn teriyaki, put it in a box, and deliver it.\"\n\n", p_play[get_play(lpReq)].st_name);
		cst_main += cst_temp;
		cst_temp.Format("`wJune: `$\"Please leave now.\"");
		cst_main += cst_temp;
//		cst_temp.Format("`wYou: `$\"I will BURY you, bitch!\"\n\n");
//		cst_main += cst_temp;
		p_play[get_play(lpReq)].b_kicked_out_of_yangs = true;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
		add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}



void no_deliver_possible(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	
		CString cst_temp;
		cst_temp.Format("`w%s: `$\"If you could just deliver that to my parlor, that would be great, m'kay?\"\n\n", p_play[get_play(lpReq)].st_name);
		cst_main += cst_temp;
		cst_temp.Format("`wJune: `$\"Why don't you just get food for here?\"");
		cst_main += cst_temp;
		cst_temp.Format("\n\n`w%s: `$\"Ok, fine.\"\n\n", p_play[get_play(lpReq)].st_name);
		cst_main += cst_temp;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}

void command_get_name_delivery_tiger(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_main;
	
	if ( p_play[get_play(lpReq)].i_yangs_sent >= glo.i_yangs_per_day)
	{
		CString cst_temp;
		cst_temp.Format("`w%s: `$\"I would like a cursed tiger delivered, please.\"\n\n", p_play[get_play(lpReq)].st_name);
		cst_main += cst_temp;
		cst_temp.Format("`wJune: `$\"I'm sorry, my dad is out delivering food right now.  How about something for here?\"");
		cst_main += cst_temp;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);                                                 		
		add_button(&cst_final, 2, "Get Bitchy",c_request_yangs_callback, C_YANGS_BITCHY);                                                 		
		add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	}  else
	{
		add_var(&cst_final, "st_message", "`wJune: `$\"Ah.. someone you are not too fond of, I take it.  Who would this unlucky person be?\" ");
		add_var(&cst_final, "st_object", "i_getname");
		add_var(&cst_final, "i_return_command", c_request_yangs_callback);
		add_var(&cst_final, "i_return_command_sub", C_YANGS_GET_FORTUNE);
		
		add_var(&cst_final, "i_sent",c_command_object_add);
	}
	
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}

void command_get_name_delivery_sparrow(LPREQUEST lpReq)
{
 	CString cst_final;
	CString cst_main;

	if ( p_play[get_play(lpReq)].i_yangs_sent >= glo.i_yangs_per_day)
	{
	CString cst_temp;
		cst_temp.Format("`wYou: `$\"I would like a lucky sparrow delivered, please.\"\n\n");
		cst_main += cst_temp;
		cst_temp.Format("`wJune: `$\"I'm sorry, my dad is out delivering food right now.  How about something for here?\"");
		cst_main += cst_temp;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);                                                 		
		add_button(&cst_final, 2, "Get Bitchy",c_request_yangs_callback, C_YANGS_BITCHY);                                                 		
		add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
	} else
	{
	
	
	add_var(&cst_final, "st_message", "`wJune: `$\"That's very nice of you.  Who's the lucky person?\"");
    add_var(&cst_final, "st_object", "i_getname");
	add_var(&cst_final, "i_return_command", c_request_yangs_callback);
	add_var(&cst_final, "i_return_command_sub", C_YANGS_GET_FORTUNE_SPARROW);
	add_var(&cst_final, "i_sent",c_command_object_add);
	}
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}


void command_yangs_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];

	d_get_token_int(i_command_sub, "i_command_sub");
	
	if (i_command_sub == C_YANGS_DRAGON)
	{
		command_yangs_number_4(lpReq);
		return;
	}
		if (i_command_sub == C_YANGS_SPARROW)
	{
		command_get_name_delivery_sparrow(lpReq);
		return;
	}

	if (i_command_sub == C_YANGS_TIGER)
	{
		command_get_name_delivery_tiger(lpReq);
		return;
	}
	
	if (i_command_sub == C_YANGS_SHOW_MENU)
	{
		command_yangs_show_menu(lpReq);
		return;
	}
		if (i_command_sub == C_YANGS_GET_FORTUNE)
	{
		command_yangs_get_fortune(lpReq);
		return;
	}
		if (i_command_sub == C_YANGS_GET_FORTUNE_SPARROW)
	{
		command_yangs_get_fortune_sparrow(lpReq);
		return;
	}

		if (i_command_sub == C_YANGS_SEND)
	{
		command_yangs_dev(lpReq);
		return;
	}
	
		if (i_command_sub == C_YANGS_SEND_SPARROW)
	{
		command_yangs_dev_sparrow(lpReq);
		return;
	}
		if (i_command_sub == C_YANGS_BITCHY)
	{
		command_yangs_bitchy(lpReq);
		return;
	}
	
}

void yangs_add_menu_buttons(LPREQUEST lpReq, CString *cst_final)
{

	add_button(cst_final, 4, "Order #1",c_request_yangs_callback, C_YANGS_DRAGON);
 	add_button(cst_final, 1, "Leave",c_request_map, C_SUB_NONE);
	add_button(cst_final, 2, "Order #3",c_request_yangs_callback, C_YANGS_TIGER);
	add_button(cst_final, 3, "Order #2",c_request_yangs_callback, C_YANGS_SPARROW);
}

void yangs_add_menu(LPREQUEST lpReq, CString *cst_main)
{
		   CString cst_temp;
		 	*cst_main += "`wThe Menu\n\n";
			cst_temp.Format("`w#1`$ - Lucky Dragon (adds `w10 `$luck, for here only) Cost: `w5`$ Bury Points\n");
			*cst_main += cst_temp;
			cst_temp.Format("`w#2`$ - Lucky Sparrow (adds `w5 `$luck, for delivery only) Cost: `w20`$ Bury Points\n");
			*cst_main += cst_temp;
			cst_temp.Format("`w#3`$ - Cursed Tiger (removes `w10`$ luck, for delivery only) Cost: `w20`$ Bury Points\n");
			*cst_main += cst_temp;
			
}

void command_yangs_get_fortune(LPREQUEST lpReq)
{
   d_get_token_int(i_his_id, "i_his_id");

   if (i_his_id == -1)
   {
	 //they couldn't find a name, justshow the menu again
	command_yangs_show_menu(lpReq);
	return;
   }

	CString cst_final;
	CString cst_main;
	add_var(&cst_final, "st_message", "`wJune: `$\"If you would like to write a custom fortune, enter it now.\"");
	//clear the default
	add_var(&cst_final, "st_extra_text", "");
	add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_yangs_callback);
	add_var(&cst_final, "i_return_command_sub", C_YANGS_SEND);
    add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}

void command_yangs_get_fortune_sparrow(LPREQUEST lpReq)
{
   d_get_token_int(i_his_id, "i_his_id");

   if (i_his_id == -1)
   {
	 //they couldn't find a name, justshow the menu again
	command_yangs_show_menu(lpReq);
	return;
   }

	CString cst_final;
	CString cst_main;
	add_var(&cst_final, "st_message", "`wJune: `$\"If you would like to write a custom fortune, enter it now.\"");
	//clear the default
	add_var(&cst_final, "st_extra_text", "");
	add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_yangs_callback);
	add_var(&cst_final, "i_return_command_sub", C_YANGS_SEND_SPARROW);
    add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}



void command_yangs_show_menu(LPREQUEST lpReq)
{
	
	CString cst_final;
	CString cst_main;
	yangs_add_menu(lpReq, &cst_main);
	yangs_add_menu_buttons(lpReq, &cst_final);
	add_var(&cst_final, "st_main", cst_main);
    add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}


		
		void command_yangs(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
    CString cst_temp;
	cst_temp.Format("What could be luckier than a box of delectable grilled teriyaki "
		"spread over hot rice? Your mouth waters at the very thought of it.");
	add_var(&cst_final, "st_generic_info", cst_temp);
   	CString cst_main;

	if (p_play[get_play(lpReq)].b_kicked_out_of_yangs == true)
	{
	  //don't let them in
	add_var(&cst_final, "st_generic_status", "June glances at you nervously."); //blank out the status bar
		cst_temp.Format("`w%s: `$\"Hello? I'd like some service here!\"\n\n", p_play[get_play(lpReq)].st_name);
		cst_main += cst_temp;
		cst_temp.Format("`wJune: `$\"Just leave, asshole.\"");
		cst_main += cst_temp;
//		cst_temp.Format("`wYou: `$\"I will BURY you, bitch!\"\n\n");
//		cst_main += cst_temp;
		add_var(&cst_final, "st_main", cst_main);
		add_button(&cst_final, 1, "Leave",c_request_map, ::C_SUB_NONE);                                                 		
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	add_var(&cst_final, "st_url_bg", "flash\\places\\yangs.swf");
	add_var(&cst_final, "st_url", "");
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	return;

	}
	
	
	
	add_var(&cst_final, "st_generic_status", "June waits for you to order."); //blank out the status bar
	yangs_add_menu(lpReq, &cst_main);
	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "st_url_bg", "flash\\places\\yangs.swf");
	add_var(&cst_final, "st_url", "");
	init_location_info(lpReq, c_request_yangs_callback, "Yang's"); 
	//add options
	yangs_add_menu_buttons(lpReq, &cst_final);
	//create new screen
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }


		
		void command_yangs_number_4(LPREQUEST lpReq)                                                                                               		
		{                                                                                                                                          		
			//News screen.  Send all the info they need then load the screen.                                                                      		
			CString cst_final;                                                                                                                     		
			//now let's actually give them the URL to load                                                                                         		
			if (p_play[get_play(lpReq)].i_luck >= 40)                                                                                              		
			{                                                                                                                                      		
				//eating yangs won't help luck, don't let them waste their money                                                                   		
				add_var(&cst_final, "st_main", "`$Maybe later.  You especially like to eat Yang's when you're feeling down on your luck. (like less than 40 percent for instance)");        		
			} else                                                                                                                                 		
				if (p_play[get_play(lpReq)].i_sp < 5)                                                                                              		
				{                                                                                                                                  		
					//can't afford it                                                                                                              		
					add_var(&cst_final, "st_main", "`$You check your wallet.. shoot, not enough Bury Points!");                                            		
				} else                                                                                                                             		
				{                                                                                                                                  		
					//give them the Yangs                                                                                                          		
					add_var(&cst_final, "st_main", "`$You relish every bite of your General Tso's Chicken.\n\nYou gain `w10`$ luck.");             		
					exp_mod(get_play(lpReq), -5);                                                                                                  		
					p_play[get_play(lpReq)].i_luck += 10;                                                                                          		
					add_luck(&cst_final, lpReq);                                                                                                   		
					add_exp(&cst_final, lpReq);                                                                                                    		
					add_sp(&cst_final, lpReq);                                                                                                     		
				}                                                                                                                                  		
				
				add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);                                                 		
				add_var(&cst_final, "i_sent",c_command_generic);                                                                                   		
				seth_ansi(&cst_final);                                                                                                             		
				send_msg(lpReq, cst_final);                                                                                                        		
		}		                                                                                                                                   		
                                                                                                                                           		
		
void command_yangs_dev(LPREQUEST lpReq)
{
			
			d_get_token_int(i_his_id, "i_his_id"); //get name they want
			d_get_token_string(cst_fortune, "st_extra_text"); //get the fortune
			
			if (cst_fortune == "12ABORT12")
			{
			   //hit the aboart button on the fortune entry dialog
				command_yangs_show_menu(lpReq);
			  return;

			}
			
			CString cst_final;
			//have they already sent too many yang's today?
			if ( p_play[get_play(lpReq)].i_yangs_sent >= glo.i_yangs_per_day)
			{
				
				CString cst_t;
				cst_t.Format("`$\"We only allow %d delivery per day,\" Mr Yang tells you apologetically. ", glo.i_yangs_per_day);
				add_var(&cst_final, "st_main", cst_t);
			} else
				//is it a valid name?
					if (i_his_id == get_play(lpReq))
			{
			  no_deliver_possible(lpReq);
			 				return;

			} else
		
				
				if ( (!in_range(i_his_id, 0, g_on.i_cur_players)) || (!play_active(i_his_id)) )
				{
					add_var(&cst_final, "st_main", "`$Error delivering food, he wasn't home.");
				} else
				{
					//valid so far
					if (p_play[get_play(lpReq)].i_sp < 20)
					{
						add_var(&cst_final, "st_main", "`$You don't have enough Bury Points.  You realize you would just look stupid trying to order now...");
						//can't afford it
					} else
					{
						//good to go!
						
						
						add_var(&cst_final, "st_main", "`$Mr. Yang leaves to deliver the food.");
						p_play[get_play(lpReq)].i_yangs_sent++;
						exp_mod(get_play(lpReq), -20);
						add_exp(&cst_final, lpReq);
						add_sp(&cst_final, lpReq);
						
						//send package
						CEvent event;
						event.compute_event(p_play[i_his_id].i_exp, "yangs.txt");
						
						if (cst_fortune == "")
						{
							//they didn't enter a fortune, let's use a default
							cst_fortune.Format("%s", event.st_message);
							
						}
						
						//add to chat
						
						broadcast_tv("A Yang's commercial comes on. It looks like %s in the background.",  p_play[i_his_id].st_funeral_name);
						
						
						//did someone see us?
						
						if (random(4) == 1)
						{
							CString cst_news;
							
							int i_choose = random(4);
							if (i_choose == 0)
							{
								cst_news.Format("`7SPOTLIGHT ON OUR TOWN\r\n`^Local mortician %s is seen buying some Yang's for delivery.\r\n\r\n", p_play[get_play(lpReq)].st_name);
								add_to_news(cst_news);
							}
							if (i_choose == 1)
							{
								cst_news.Format("`7YANG'S NOW DELIVERS\r\n`^\"Yes, delivery is very popular.  You see this package from %s? I'm going deliver it all the way to %s, no extra charge!\" Mr Yang smiles.\r\n\r\n",
									p_play[get_play(lpReq)].st_name, p_play[i_his_id].st_funeral_name);
								add_to_news(cst_news);
							}
							
							if (i_choose == 2)
							{
								cst_news.Format("`7YANG'S DOING WELL\r\n`^\"Ah, yes.  Business is very good today! People like %s buying so much delivery,\" Mr Yang comments.\r\n\r\n",
									p_play[get_play(lpReq)].st_name, play_he(get_play(lpReq)));
								add_to_news(cst_news);
							}
							if (i_choose == 3)
							{
								cst_news.Format("`7YANG'S MAKES A GREAT GIFT\r\n`^Looking for that special gift?  How about food? Today %s was seen buying some, seems popular.\r\n\r\n", p_play[get_play(lpReq)].st_name);
								
								add_to_news(cst_news);
								
								
							}
						}
						
						CString cst_message;
						cst_message.Format("\n`$Some delicious smelling chinese food has been left on your doorstep."
							"\nYour fortune cookie reads:\n`w\"%s\"\n`$You lose `w10`$ luck!\n",cst_fortune);
						send_packet(i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
						//now actually give them the luck
						send_packet_var_mod(i_his_id, c_que_sub_type_luck, -10);
						send_packet_picture(i_his_id, "flash/chinese_takeout.swf");
						send_packet_continue_button(i_his_id);
						LogMsg("%s sent Yang's to %s, cookie: %s", p_play[get_play(lpReq)].st_name, p_play[i_his_id].st_name,
							cst_fortune);
					}
					
				}
				
				add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);
				add_var(&cst_final, "i_sent",c_command_generic);
				seth_ansi(&cst_final);
				send_msg(lpReq, cst_final);
				
}
  
  
  
 
void command_yangs_dev_sparrow(LPREQUEST lpReq)
{
			
	d_get_token_int(i_his_id, "i_his_id"); //get name they want
	d_get_token_string(cst_fortune, "st_extra_text"); //get name they want

		if (cst_fortune == "12ABORT12")
			{
			   //hit the aboart button on the fortune entry dialog
				command_yangs_show_menu(lpReq);
				return;

			}
		
	
	CString cst_final;
	//have they already sent too many yang's today?
	if ( p_play[get_play(lpReq)].i_yangs_sent >= glo.i_yangs_per_day)
	{
		
			CString cst_t;
				cst_t.Format("`$\"We only allow %d delivery per day,\" Mr Yang tells you apologetically. ", glo.i_yangs_per_day);
				add_var(&cst_final, "st_main", cst_t);
			} else
				//is it a valid name?
			if (i_his_id == get_play(lpReq))
			{
			  no_deliver_possible(lpReq);
			  return;
			} else
				
				if ( (!in_range(i_his_id, 0, g_on.i_cur_players)) || (!play_active(i_his_id)) )
				{
					add_var(&cst_final, "st_main", "`$Error delivering food, he wasn't home.");
				} else
				{
					//valid so far
					if (p_play[get_play(lpReq)].i_sp < cls_yangs.i_cost[C_YANGS_SPARROW])
					{
						add_var(&cst_final, "st_main", "`$You don't have enough Bury Points.  You realize you would just look stupid trying to order now...");
						//can't afford it
					} else
					{
						//good to go!
						
						
						add_var(&cst_final, "st_main", "`$Mr. Yang leaves to deliver the food.");
						p_play[get_play(lpReq)].i_yangs_sent++;
						exp_mod(get_play(lpReq), -(cls_yangs.i_cost[C_YANGS_SPARROW]));
						add_exp(&cst_final, lpReq);
						add_sp(&cst_final, lpReq);
						
						//send package
						CEvent event;
						event.compute_event(p_play[i_his_id].i_exp, "yangs_nice.txt");
						
						if (cst_fortune == "")
						{
							//they didn't enter a fortune, let's use a default
							cst_fortune.Format("%s", event.st_message);
							
						}
						
						//add to chat
						
						broadcast_tv("A Yang's commercial comes on. It looks like %s in the background.",  p_play[i_his_id].st_funeral_name);
						
						
						//did someone see us?
						/*
						if (random(4) == 1)
						{
							CString cst_news;
							
							int i_choose = random(4);
							if (i_choose == 0)
							{
								cst_news.Format("`7SPOTLIGHT ON OUR TOWN\r\n`^Local mortician %s is seen buying some Yang's for delivery.\r\n\r\n", p_play[get_play(lpReq)].st_name);
								add_to_news(cst_news);
							}
							if (i_choose == 1)
							{
								cst_news.Format("`7YANG'S NOW DELIVERS\r\n`^\"Yes, delivery is very popular.  You see this package from %s? I'm going deliver it all the way to %s, no extra charge!\" Mr Yang smiles.\r\n\r\n",
									p_play[get_play(lpReq)].st_name, p_play[i_his_id].st_funeral_name);
								add_to_news(cst_news);
							}
							
							if (i_choose == 2)
							{
								cst_news.Format("`7YANG'S DOING WELL\r\n`^\"Ah, yes.  Business is very good today! People like %s buying so much delivery,\" Mr Yang comments.\r\n\r\n",
									p_play[get_play(lpReq)].st_name, play_he(get_play(lpReq)));
								add_to_news(cst_news);
							}
							if (i_choose == 3)
							{
								cst_news.Format("`7YANG'S MAKES A GREAT GIFT\r\n`^Looking for that special gift?  How about food? Today %s was seen buying some, seems popular.\r\n\r\n", p_play[get_play(lpReq)].st_name);
								
								add_to_news(cst_news);
								
								
							}
						}
						  */

						CString cst_message;
						cst_message.Format("\n`$%s has sent you some chinese food."
							"\nYour fortune cookie reads:\n`w\"%s\"\n`$You gain `w%d`$ luck!\n", p_play[get_play(lpReq)].st_name, cst_fortune, cls_yangs.i_mod[C_YANGS_SPARROW]);
						send_packet(i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
						//now actually give them the luck
						send_packet_var_mod(i_his_id, c_que_sub_type_luck,  cls_yangs.i_mod[C_YANGS_SPARROW]);
						send_packet_picture(i_his_id, "flash/chinese_takeout.swf");
						send_packet_continue_button(i_his_id);
						LogMsg("%s sent nice Yang's to %s, cookie: %s", p_play[get_play(lpReq)].st_name, p_play[i_his_id].st_name,
							cst_fortune);
					}
					
				}
				
				add_button(&cst_final, 1, "Continue",c_request_yangs_callback, C_YANGS_SHOW_MENU);
				add_var(&cst_final, "i_sent",c_command_generic);
				seth_ansi(&cst_final);
				send_msg(lpReq, cst_final);
}
  
  
  
 


  
  
