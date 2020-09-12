#include "commands_complicated.h"


const int C_BUY_CHOICE_YES = 1;
const int C_BUY_CHOICE_CAN_NOT_AFFORD = 2;

void command_complicated_finish(LPREQUEST lpReq, bool b_user_crashed)
 {
	//accept whatever we got.
	CString cst_final;
	//now let's actually give them the URL to load
	CString cst_url;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];

	//security

		  if (!p->status.b_customer_active)
	{
	  send_error_message(lpReq, "Can't accept, no customer exists");
	  return;
	}
	  if (p->status.i_special_button != c_request_complicated_customer)
	  {
	  log_error("%s should have a complicated customer!", p_play[get_play(lpReq)].st_name);
	  send_error_message(lpReq, "Huh?  Not this kind of customer.");
  	  return;
	  }

	//header for customer message
	CString cst_c_man = p->c_man();
	CString cst_c_young = p->c_a_young();
	CString cst_d_relation = p->d_relation();
	CString cst_casket = p->casket();
	CString cst_d_him = p->d_him();
	
	//get best picture available

	CString cst_customer;
	int i_bp = int( float(p->get_price_int()) * 0.0064f);
	CString cst_extra;

	//did they get any bonuses?
	 int i_closeness = 100;
	 int i_bonus_amount = glo.i_trade_bonus;

     if (p->get_price_int() == p->status.i_money_to_spend)
	 {
	 	 i_bonus_amount = 200;
		 //right on the money
	   	  cst_extra.Format("`wEXCELLENT! `$For making this funeral cost the max %s could pay you receive a `w%d`$ BP bonus!\n\n", p->c_he(), i_bonus_amount);
	i_bp += i_bonus_amount;
	
	CString cst_news;
	cst_news.Format("`7%s makes a perfect sale, the industry is in awe.\r\n\r\n",p_play[get_play(lpReq)].st_name);
	add_to_news(cst_news);
	 } else
	  if (number_is_close(p->get_price_int(), p->status.i_money_to_spend, i_closeness/2))
	{
	  //they got within 100 of the best possible price
	  cst_extra.Format("`wGREAT! `$For getting within %d of this customers limit you receive a `w%d`$ BP bonus!\n\n",i_closeness/2, i_bonus_amount+10);
	i_bp += (i_bonus_amount+10);
	} else

	 if (number_is_close(p->get_price_int(), p->status.i_money_to_spend, i_closeness))
	{
	  //they got within 100 of the best possible price
	  cst_extra.Format("`wNICE! `$For getting within %d of this customers limit you receive a `w%d`$ BP bonus!\n\n",i_closeness, i_bonus_amount);
	i_bp += i_bonus_amount;
	}


	if (b_user_crashed)
	{
	LogMsg("%s crashed, but we're giving him %d BP's off a %s guy", p_play[get_play(lpReq)].st_name, i_bp,
		p->c_income());
	broadcast_tv("\"%s has fainted in %s parlor.  Working too hard maybe?\" a guy reports.",  p_play[get_play(lpReq)].st_name, ::play_his(get_play(lpReq)));
	} else
	{
		cst_customer.Format("`w`$The service is a success!  Total bill is $%s.\n\n%sYou earn `w%d`$ Bury Points and a %s sales tag.",p->get_price(), cst_extra, i_bp,
			::get_casket_name(p->status.e_desired_casket));
	LogMsg("%s earned %d BP's off a %s guy", p_play[get_play(lpReq)].st_name, i_bp,
		p->c_income());
	}
	if (i_bp > 69)
	{
	   broadcast_tv("\"It appears %s just made a %d BP sale,\" a guy reports.",  p_play[get_play(lpReq)].st_name, i_bp);
	}
	
	p->status.i_special_button = 0;
	exp_mod(get_play(lpReq), i_bp);
	p_play[get_play(lpReq)].a_coffins[p->status.e_desired_casket]++;
	p_play[get_play(lpReq)].i_plots++;

	
	if (b_user_crashed)
	{
	  //send them mail about the sale.
		CString cst_temp;
		cst_message = "";
		  cst_temp.Format("\n`wYou fainted while dealing with a customer.`$\n\n");
	   cst_message += cst_temp;
	   cst_temp.Format("You vaguely remember riding in an ambulance.\n\n");
	   cst_message += cst_temp;
	    cst_temp.Format("`$When you get home you find `w%s `$Bury Points and a %s tag on the desk!",int_to_cash(i_bp),
			::get_casket_name(p->status.e_desired_casket));
	   cst_message += cst_temp;

	   send_packet(p->i_play, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_picture(p->i_play, "flash\\places\\hospital_ambulance.swf");
	   send_packet_continue_button(p->i_play);



	} else
	{
	add_sp(&cst_final, lpReq);
	add_var(&cst_final, "st_option_3", " Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", cst_customer);
	add_var(&cst_final, "st_url", "flash/stuff/money_bag_2.swf");
	  //flash/stuff/money_bag.swf
	add_var(&cst_final, "b_show_stats","0");
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
	}
 }

void complicated_add_options(CString *cst_final, LPREQUEST lpReq)
{

 
	add_var(cst_final, "st_option_3", "     Done");
	add_var(cst_final, "i_command_3", c_request_finish_sale);
	add_var(cst_final, "b_kill_buttons", 1);
	add_var(cst_final, "st_option_2", "  Soft Sell");
	add_var(cst_final, "i_command_2", c_request_soft_sell);

	add_var(cst_final, "st_option_1", "  Hard Sell");
	add_var(cst_final, "i_command_1", c_request_hard_sell);

	add_var(cst_final, "st_option_0", "   Sympathy");
	add_var(cst_final, "i_command_0", c_request_sympathy);

	add_var(cst_final, "st_option_a", "     Guilt");
	add_var(cst_final, "i_command_a", c_request_guilt);

	
	add_var(cst_final, "b_show_stats","1");
	add_var(cst_final, "i_sent",c_command_load_movie);

}

void leave_pissed(CString *cst_message, LPREQUEST lpReq)
{
	online_struct *p = p_on[lpReq->i_on_index];


	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  	CString cst_final;
    CString cst_answer;
	
	int i_rand = random(4);

	if (i_rand == 0)
	cst_answer.Format("%s\"You know what? I really don't like the way you work. Bye.\"\n",cst_young_man);
	if (i_rand == 1)
	cst_answer.Format("%s\"Enough is enough.  I'm sick of you.\"\n",cst_young_man);
	if (i_rand == 2)
	cst_answer.Format("%s\"I just realized that you disgust me.  Sayoonara.\"\n",cst_young_man);
	if (i_rand == 3)
	cst_answer.Format("%s\"Can we cut the crap for a second here?  I guess not, bye.\"\n",cst_young_man);

//	LogMsg("%s loses a customer (anger)",p_play[get_play(lpReq)].st_name);
	broadcast_tv("\"A %s %s %s just left %s looking pissed,\" a reporter says.",p->c_income(),p->c_young(), p->c_man(),  p_play[get_play(lpReq)].st_funeral_name); 

    CString cst_other;	
	
	  if (get_number_of_valid_players() > 1)
	  {
	    //let's give this business to anothe valid player
		 int i_new_player = get_random_player_excluding(get_play(lpReq));
		 cst_other.Format("\n`$You watch %s through the window after %s leaves.  You see %s go into %s to see %s!",
			 p->c_him(), p->c_he(), p->c_him(), p_play[i_new_player].st_funeral_name, p_play[i_new_player].st_name);
	 
	    //don't forget to email the guy about it
	     CString cst_email;
		 int i_his_points = 5+random(5);
		 cst_email.Format("\n`$%s %s walks in with a dead body. \"Hello %s, I would like you to know I got treated like crap by"
			 " %s of %s. I know you'll do a better job!\"\n\nYou earn `w%d`$ Bury Points for stealing this sale!\n",
			 p->c_a_young(), p->c_man(), play_sir(i_new_player), p_play[get_play(lpReq)].st_name, p_play[get_play(lpReq)].st_funeral_name, 
			  i_his_points);
		 
	  send_packet(i_new_player, cst_email, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_picture_bg(i_new_player, "flash/background/background_2.swf");
	  send_packet_picture(i_new_player, p->status.st_url_save);
	  send_packet_var_mod(i_new_player, c_que_sub_type_bp, i_his_points);
	  send_packet_continue_button(i_new_player);
	   }
	
	
	
	add_var(&cst_final, "st_option_3", " Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", *cst_message+cst_answer+cst_other);
	add_var(&cst_final, "st_url", "flash/stuff/money_bag.swf");
			//flash/stuff/money_bag.swf
	add_var(&cst_final, "b_show_stats","0");
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	p->status.i_special_button = 0;

	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
}


void pissed_over_limit(CString *cst_message, LPREQUEST lpReq)
{
	online_struct *p = p_on[lpReq->i_on_index];


	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  	CString cst_final;
    CString cst_answer;
	
	int i_rand = random(3);

    		if (i_rand== 0)
			cst_answer.Format("%s\"That would put me over my limit.. I don't have time for this, bye.\"\n",cst_young_man);
			if (i_rand == 1)
			cst_answer.Format("%s\"Damn, you see, I can only spend $%s.  I'm leaving.\"\n",cst_young_man, int_to_cash(p->status.i_money_to_spend));
			if (i_rand == 2)
			cst_answer.Format("%s\"My visa only goes to $%s... I'll try next-door.\"\n",cst_young_man, int_to_cash(p->status.i_money_to_spend));
	


//	LogMsg("%s loses a customer (anger)",p_play[get_play(lpReq)].st_name);
	broadcast_tv("\"A %s %s %s just left %s looking pissed,\" a reporter says.",p->c_income(),p->c_young(), p->c_man(),  p_play[get_play(lpReq)].st_funeral_name); 

    CString cst_other;	
	
	  if (get_number_of_valid_players() > 1)
	  {
	    //let's give this business to anothe valid player
		 int i_new_player = get_random_player_excluding(get_play(lpReq));
		 cst_other.Format("\n`$You watch %s through the window after %s leaves.  You see %s go into %s to see %s!",
			 p->c_him(), p->c_he(), p->c_him(), p_play[i_new_player].st_funeral_name, p_play[i_new_player].st_name);
	 
	    //don't forget to email the guy about it
	     CString cst_email;
		 int i_his_points = 5+random(5);
		 cst_email.Format("\n`$%s %s walks in with a dead body. \"Hello %s, I would like you to know I got treated like crap by"
			 " %s of %s. I know you'll do a better job!\"\n\nYou earn `w%d`$ Bury Points for stealing this sale!\n",
			 p->c_a_young(), p->c_man(), play_sir(i_new_player), p_play[get_play(lpReq)].st_name, p_play[get_play(lpReq)].st_funeral_name, 
			  i_his_points);
		 
	  send_packet(i_new_player, cst_email, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	  //now actually give them the luck
	  send_packet_picture_bg(i_new_player, "flash/background/background_2.swf");
	  send_packet_picture(i_new_player, p->status.st_url_save);
	  send_packet_var_mod(i_new_player, c_que_sub_type_bp, i_his_points);
	  send_packet_continue_button(i_new_player);
	   }
	
	
	
	add_var(&cst_final, "st_option_3", " Continue");
	add_var(&cst_final, "i_command_3", c_request_main);
	add_var(&cst_final, "b_kill_buttons", 1);
	add_var(&cst_final, "st_main", *cst_message+cst_answer+cst_other);
	add_var(&cst_final, "st_url", "flash/stuff/money_bag.swf");
			//flash/stuff/money_bag.swf
	add_var(&cst_final, "b_show_stats","0");
	add_turns(&cst_final, lpReq);
	add_var(&cst_final, "i_sent",c_command_load_movie);
	p->status.i_special_button = 0;

	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
}


CString figure_complicated_total(LPREQUEST lpReq)
{
  	online_struct *p = p_on[lpReq->i_on_index];
   CString cst_temp;
   cst_temp.Format("\n\nTotal: $"+p->get_price());
  if (p->status.b_show_resistance)
   {
	 //add resistance info
	   CString cst_res;
	   cst_res.Format(" `5(Resistance: `#%.0f%%25`5)", p->status.f_cheapness*10);
	   cst_temp += cst_res;
   }
   
   return cst_temp; 

}

 void command_complicated_customer(LPREQUEST lpReq)
 {
	//this is the backbone of the game
	CString cst_final;
	//now let's actuall give them the URL to load
	CString cst_url;
	CString cst_message;
	CString cst_temp;
 	online_struct *p = p_on[lpReq->i_on_index];

	//header for customer message
	CString cst_c_man = p->c_man();
	CString cst_c_young = p->c_a_young();
	CString cst_d_relation = p->d_relation();
	CString cst_casket = p->casket();
	CString cst_d_him = p->d_him();
	CString cst_price = p->get_price();
	CString cst_c_young_man = p->c_Young_man();
	//get best picture available
	cst_url = p->c_flash_url();
	CString cst_psych = "";
	int i_psych_count = 0;

	if (get_random_yes_no(20+(p_play[get_play(lpReq)].i_psych*13), 0.98f))
	{
		//learned something
		cst_temp.Format("`wYou successfully analyze %s resistance.`$", p->c_his());
		p->status.b_show_resistance = true;
		if (i_psych_count > 0) cst_psych += " ";
		cst_psych += cst_temp;
		i_psych_count++;
		
	}
	
	if (p_play[get_play(lpReq)].i_psych > 0)
	{
		if (get_random_yes_no( (p_play[get_play(lpReq)].i_psych*14), 0.98f) == true)
		{
			//we learned mood.
			if (p->status.b_loved)
			{
				cst_temp.Format("`w%s seems heartbroken.`$", p->c_He());
			} else
			{
				cst_temp.Format("`w%s seems guilty.`$", p->c_He());
			}
			if (i_psych_count > 0) cst_psych += " ";
			cst_psych += cst_temp;
			i_psych_count++;
		}
	}


       	if (p_play[get_play(lpReq)].i_psych > 4)
	{
		if (get_random_yes_no( (p_play[get_play(lpReq)].i_psych*15)-65, 0.98f) == true)
		{
			//max spending limit raised
			cst_temp.Format("`wSpending limit doubled.`$");
			p->status.i_money_to_spend *= 2;
			if (i_psych_count > 0) cst_psych += " ";
			cst_psych += cst_temp;
			i_psych_count++;
		}
	}




	if (p_play[get_play(lpReq)].i_psych > 1)
	{
		if (get_random_yes_no( (p_play[get_play(lpReq)].i_psych*15)-25, 0.98f) == true)
		{
			//we learned max price.
			cst_temp.Format("`wYou estimate %s is worth about $%s.`$", p->c_he(), int_to_cash(p->status.i_money_to_spend));
			if (i_psych_count > 0) cst_psych += " ";
			cst_psych += cst_temp;
			i_psych_count++;
		}
	}
	
	 	if (p_play[get_play(lpReq)].i_psych > 2)
	{
		if (get_random_yes_no( (p_play[get_play(lpReq)].i_psych*15)-35, 0.98f) == true)
		{
			//we learned max price.
			cst_temp.Format("`wResistance cut in half.`$");
			p->status.f_cheapness *= 0.5;
			if (i_psych_count > 0) cst_psych += " ";
			cst_psych += cst_temp;
			i_psych_count++;
		}
	}
	
		if (p_play[get_play(lpReq)].i_psych > 3)
	{
		if (get_random_yes_no( (p_play[get_play(lpReq)].i_psych*15)-50, 0.98f) == true)
		{
			//we learned max price.
			cst_temp.Format("`wMood cut in half.`$");
			p->status.f_mood *= 0.5;
			if (i_psych_count > 0) cst_psych += " ";
			cst_psych += cst_temp;
			i_psych_count++;
		}
	}

 

	if (i_psych_count == 0)
	{

	 	cst_temp.Format("\n\n`$You are unable to analyze anything extra about %s.", p->c_him());
		cst_psych += cst_temp;
	
	} else
	{

	  cst_psych = "\n\n" + cst_psych;
	}

	
	cst_price =figure_complicated_total(lpReq);

	strcpy(p->status.st_url_save, cst_url);
	CString cst_customer;
	cst_customer += "`w"+cst_c_young_man+": `$\"Please, I need a funeral service for my "+ cst_d_relation+". ";
	cst_message = "I would need a plot and a "+cst_casket+" casket for "+cst_d_him+".\""
		+cst_psych+cst_price;
    
 
	add_turns(&cst_final, lpReq);
	p->status.i_special_button = ::c_request_complicated_customer;
	CString cst_bg;
	int i_rand = random(4)+1;
	cst_bg.Format("flash/background/background_%d.swf",i_rand);
	
	add_var(&cst_final, "st_url_bg", cst_bg);
	add_var(&cst_final, "st_mood", p->c_mood());
	add_var(&cst_final, "st_class", p->c_income());


	add_var(&cst_final, "st_main",cst_customer+cst_message);
	add_var(&cst_final, "st_url", cst_url);


	complicated_add_options(&cst_final, lpReq);

	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);

 }

  int get_new_option(LPREQUEST lpReq)
  {

   return random_range(0, a_option.GetSize()-1);
  }

  int buy_choice(int i_price, LPREQUEST lpReq)
  {
 	online_struct *p = p_on[lpReq->i_on_index];

	if (p->get_price_int()+i_price > p->status.i_money_to_spend) return C_BUY_CHOICE_CAN_NOT_AFFORD;

	return ::C_BUY_CHOICE_YES;
  }

  void command_complicated_soft_sell(LPREQUEST lpReq)
 {
	//this is the backbone of the game
	CString cst_final;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];

	//first pick something that don't already have
	int i_option = get_new_option(lpReq);



	CString cst_customer;
	
	int i_r = random(4);

	if (i_r == 0)
	{
	
	cst_customer.Format("`w%s: `$\"You know, your neighbor was in here recently.. he bought %s %s for only $%s. Impeccable taste.\"\n\n", 
		p_play[get_play(lpReq)].st_name,a_option[i_option].cst_a, a_option[i_option].cst_name,
		int_to_cash(a_option[i_option].i_price));
	} else
	if (i_r == 1)
	{
   	cst_customer.Format("`w%s: `$\"I can tell by the fine clothes you're wearing that you appreciate the finer things. How about %s %s for $%s?\"\n\n", 
		p_play[get_play(lpReq)].st_name,a_option[i_option].cst_a, a_option[i_option].cst_name,
		int_to_cash(a_option[i_option].i_price));
	} else
	if (i_r == 2)
	{
   	cst_customer.Format("`w%s: `$\"I don't mean to rush you, but this isn't really something you can wait a week thinking about. We do have %s %s for $%s...\"\n\n", 
		p_play[get_play(lpReq)].st_name,a_option[i_option].cst_a, a_option[i_option].cst_name,
		int_to_cash(a_option[i_option].i_price));
	} else
	
	if (i_r == 3)
	{
   	cst_customer.Format("`w%s: `$\"You know..  I bet your %s's social security death benefits would probably cover %s %s for $%s?\"\n\n", 
		p_play[get_play(lpReq)].st_name,p->d_relation(),a_option[i_option].cst_a, a_option[i_option].cst_name,
		int_to_cash(a_option[i_option].i_price));
	} else

	{
	 LogMsg("Error with choosing saying");

	}
	
	CString cst_answer;
  	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
   CString cst_price;
	

   
   
   
   if (frandom(10) < p->status.f_cheapness)
	{

	   //we're too cheap to buy it, get more mad
		p->status.f_mood += (0.4f + frandom(0.7f));
	
		if (p->status.f_mood >= 10)
		{
		  leave_pissed(&cst_customer, lpReq);
		  return;

		}


		int i_rand = random(6);
		if (i_rand == 0)
		cst_answer.Format("%s\"I really don't think my %s needs that.\"",cst_young_man,p->d_relation());
		else if (i_rand == 1)
		cst_answer.Format("%s\"Actually, my %s wanted a very simple funeral.\"",cst_young_man,p->d_relation());
		else if (i_rand == 2)
		cst_answer.Format("%s\"Um, no.\"",cst_young_man,p->d_relation());
		else if (i_rand == 3)
		{
			if (p->status.b_loved)
			cst_answer.Format("%s\"When my %s was alive %s asked me not to buy that.  I'm sorry.\"",cst_young_man,p->d_relation(), p->d_he());
		   else
			cst_answer.Format("%s\"I'm not gonna pay another $%s for an 'effing %s I barely knew!\"",cst_young_man, int_to_cash(a_option[i_option].i_price),p->d_relation());
		} 
		else if (i_rand == 4)
		cst_answer.Format("%s\"You've got to be kidding.\"",cst_young_man); 
		else if (i_rand == 5)
		cst_answer.Format("%s\"I'm not saying I can't afford that, but I don't think I really need it.\"",cst_young_man); 
	   
		 cst_answer += figure_complicated_total(lpReq);

	   add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
	return;
	}


	int i_purchase_choice = buy_choice(a_option[i_option].i_price, lpReq);

	float f_mood = p->status.f_mood; //average out emotions

	
	//float f_mood = (p->status.f_cheapness + p->status.f_mood)/2; //average out emotions

    if (i_purchase_choice == ::C_BUY_CHOICE_YES)
	{
		//p->status.f_mood += frandom(1); //always getting madder...
	

		//add it to our inventory
		p->add_option(i_option);
		
		if (f_mood < 2)
	 {
		  cst_answer.Format("%s\"Thank you, I'll take it.\"",cst_young_man);
	 } else
 	 if (f_mood < 4)
	 {
		  cst_answer.Format("%s\"You don't say? %s is something I want then.\"",cst_young_man, 
			  a_option[i_option].cst_name);
	 } else
 	 if (f_mood < 7)
	 {
		  cst_answer.Format("%s\"Fine, I'll take it.\"",cst_young_man);
	 } else
 	 if (f_mood < 9)
	 {
		  cst_answer.Format("%s\"I think I'm getting ripped off here but I'll take it.\"",cst_young_man);
	 } else
	{
		  cst_answer.Format("%s\"WHATEVER!! Just give it to me I guess.\"",cst_young_man);
	 }
	} else if (i_purchase_choice == ::C_BUY_CHOICE_CAN_NOT_AFFORD)
	{
   	
	   //can't afford it, unless we're in a really good mood we're splitting
		if (f_mood > 9)
		{
		    pissed_over_limit(&cst_message, lpReq);
            /*
            LogMsg("%s lost a %s customer by going over his limit of %s. (tried small)", p_play[get_play(lpReq)].st_name,
		    p->c_income(), int_to_cash(p->status.i_money_to_spend));
			
			int i_answer = random(3);
			if (i_answer == 0)
			cst_answer.Format("%s\"That would put me over my limit.. I don't have time for this, bye.\"",cst_young_man);
			if (i_answer == 1)
			cst_answer.Format("%s\"Damn, you see, I can only spend $%s.  I guess I'm out of here to find a cheaper place.\"",cst_young_man, int_to_cash(p->status.i_money_to_spend));
			if (i_answer == 2)
			cst_answer.Format("%s\"My visa only goes to $%s... Sorry, but I'm going home to call around.\"",cst_young_man, int_to_cash(p->status.i_money_to_spend));
		
				p->status.i_special_button = 0;

			add_var(&cst_final, "st_option_3", " Continue");
			add_var(&cst_final, "i_command_3", c_request_main);
			add_var(&cst_final, "b_kill_buttons", 1);
			add_var(&cst_final, "st_main", cst_customer+cst_answer);
			add_var(&cst_final, "st_url", "flash/stuff/money_bag.swf");
			//flash/stuff/money_bag.swf
			add_var(&cst_final, "b_show_stats","0");
			add_turns(&cst_final, lpReq);
			add_var(&cst_final, "i_sent",c_command_load_movie);
			seth_ansi(&cst_final);
			send_msg(lpReq, (LPCSTR)cst_final);
              */
			return;
			
		}  else
	  {
	  	  p->status.f_mood += 0.4f + frandom(1); //always getting madder...
		  cst_answer.Format("%s\"That would put me over my limit.. how about something cheaper?\"",cst_young_man);
	  }


	}


		 cst_answer += figure_complicated_total(lpReq);
	
	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
 }


 void command_complicated_hard_sell(LPREQUEST lpReq)
 {
	 
		//this is the backbone of the game
	CString cst_final;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];
   CString cst_price;
 

  	CString cst_customer;
 	CString cst_answer;
  	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  	
	//big things work kind of by an upgrade system, not like little things


	if (p->status.e_desired_casket == ::c_coffin_gold)
	{
		
		complicated_upgrade(lpReq);

	  return;
	};
	int i_price;
	
	if (p->status.e_desired_casket == ::c_coffin_wood)
	{
		i_price = C_COFFIN_SILVER_PRICE - ::C_COFFIN_WOOD_PRICE;		
    
		int i_rand = random(2);
		if (i_rand == 0)
		{
		cst_customer.Format("`w%s: `$\"Well, I suppose you COULD go with a wood casket.. except it really doesn't offer much protection from worms and the like.  How about a Silver Casket for only $%s additional?\"\n\n",
		p_play[get_play(lpReq)].st_name, int_to_cash(i_price));
		} else 
		if (i_rand == 1)
		{
		cst_customer.Format("`w%s: `$\"Wouldn't your %s prefer our traditional Silver Casket for only $%s more?\"\n\n",
		p_play[get_play(lpReq)].st_name, p->d_relation(), int_to_cash(i_price));
		}
	
	
	} else 
	if (p->status.e_desired_casket == ::c_coffin_silver)
	{
		i_price = C_COFFIN_GOLD_PRICE - ::C_COFFIN_SILVER_PRICE;		
		int i_rand = random(2);
		if (i_rand == 0)
		{
     	cst_customer.Format("`w%s: `$\"Just think of the beautiful memory picture you would have of your %s in a Gold Casket - a bargain for only $%s more!\"\n\n",
		p_play[get_play(lpReq)].st_name, p->d_relation(), int_to_cash(i_price));
		} else 
	  if (i_rand == 1)
	  {
     	cst_customer.Format("`w%s: `$\"A Gold Casket (only $%s additional) reflects character and station in life.. its suberb styling reflects a successful living.\"\n\n",
		p_play[get_play(lpReq)].st_name, int_to_cash(i_price));
	  } else log_error("Random choice for casket pitch is %d?",i_rand);
	} 

	 CString cst_silver = "silver";
			   if (p->status.e_desired_casket == ::c_coffin_silver) cst_silver = "gold";

	 if ( (frandom(10) < p->status.f_cheapness) || (p->status.f_cheapness > 8))
	{

	   //we're too cheap to buy it, get more mad
		p->status.f_mood += 0.6f + frandom(1);
	
		if (p->status.f_mood >= 10)
		{
		  leave_pissed(&cst_customer, lpReq);
		  return;

		}


		int i_rand = random(6);
		if (i_rand == 0)
		cst_answer.Format("%s\"I really don't think my %s needs that.\"",cst_young_man,p->d_relation());
		else if (i_rand == 1)
		cst_answer.Format("%s\"Actually, my %s wanted a very simple funeral.\"",cst_young_man,p->d_relation());
		else if (i_rand == 2)
		cst_answer.Format("%s\"A casket is a casket, right?\"",cst_young_man);
		else if (i_rand == 3)
		{
			if (p->status.b_loved)
			cst_answer.Format("%s\"No way, for that much it should have AC and a stereo!\"",cst_young_man,p->d_relation());
		   else
		   {
			  
			   cst_answer.Format("%s\"My %s doesn't deserve %s.\"",cst_young_man,p->d_relation(), cst_silver);
		   }
		} 
		else if (i_rand == 4)
		cst_answer.Format("%s\"I don't want to spend that much.\"",cst_young_man); 
		else if (i_rand == 5)
		cst_answer.Format("%s\"I've got the dough, but I would rather give to charity.\"",cst_young_man); 
	   
		 cst_answer += figure_complicated_total(lpReq);


	   add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
	return;
	}

   
	//we're not too cheap to buy it, but can we afford it?
		int i_purchase_choice = buy_choice(i_price, lpReq);

//	float f_mood = (p->status.f_cheapness + p->status.f_mood)/2; //average out emotions

        //made simple 12-4-02
        float f_mood =p->status.f_mood; //average out emotions

	
	if (i_purchase_choice == ::C_BUY_CHOICE_YES)
	{
	  	p->status.e_desired_casket++;
		if (f_mood < 7)
		{
		int i_rand = random(3);
		if (i_rand == 0)
		cst_answer.Format("%s\"Alright, I'll take it.\"",cst_young_man); 
		else if (i_rand == 1)
		cst_answer.Format("%s\"Ok, I'll do the upgrade.\"",cst_young_man); 
		else if (i_rand == 2)
		cst_answer.Format("%s\"Well, since you put it that way, ok.\"",cst_young_man); 
		} else
		{
		cst_answer.Format("%s\"Fine, I'll take the damn %s casket, ok?\"",cst_young_man, cst_silver); 
		}

	}  else if (i_purchase_choice == ::C_BUY_CHOICE_CAN_NOT_AFFORD)
	{

	   //can't afford it, unless we're in a really good mood we're splitting
		if (f_mood > 9)
		{
 	    
        	  pissed_over_limit(&cst_customer, lpReq);
	
              /*
            
            LogMsg("%s lost a %s customer by going over his limit of %s. (tried big)", p_play[get_play(lpReq)].st_name,
		    p->c_income(), int_to_cash(p->status.i_money_to_spend));

			cst_answer.Format("%s\"That casket would put me over my limit.. I don't have time for this, bye.\"",cst_young_man);
			add_var(&cst_final, "st_option_3", " Continue");
			add_var(&cst_final, "i_command_3", c_request_main);
			add_var(&cst_final, "b_kill_buttons", 1);
			add_var(&cst_final, "st_main", cst_customer+cst_answer);
			add_var(&cst_final, "st_url", "flash/stuff/money_bag.swf");
			//flash/stuff/money_bag.swf
			add_var(&cst_final, "b_show_stats","0");
			add_turns(&cst_final, lpReq);
			add_var(&cst_final, "i_sent",c_command_load_movie);
			seth_ansi(&cst_final);
				p->status.i_special_button = 0;

			send_msg(lpReq, (LPCSTR)cst_final);
                */
			
			return;
			
		}  else
	  {
	  	  p->status.f_mood += 0.4f + frandom(1);
		  cst_answer.Format("%s\"That casket would put me over my limit.. how about something cheaper?\"",cst_young_man);
	  }
	}
   
	cst_answer +=	::figure_complicated_total(lpReq);

	
	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);

	 
 }





 void command_complicated_guilt(LPREQUEST lpReq)
 {
   	CString cst_final;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];
   CString cst_price;
  	CString cst_customer;
 	CString cst_answer;
  	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  

	//guilt them out, only works if they don't really love their son
	if (p->status.f_mood >= 10)
	{
	   	cst_customer.Format("`$You are interrupted!\n\n");
		leave_pissed(&cst_customer, lpReq);
		return;
	}


   	int i_rand = random(5);


	if (i_rand == 0)
	{
	  	cst_customer.Format("`w%s: `$\"The funeral is not occasion for a display of cheapness, %s.\"\n\n",
		p_play[get_play(lpReq)].st_name, p->c_sir());
	} else
	if (i_rand == 1)
	{
	   	cst_customer.Format("`w%s: `$\"%s,  let me ask you one simple thing.  Did you hate %s?  Why else wouldn't you make this funeral something to be proud of?\"\n\n",
		p_play[get_play(lpReq)].st_name, p->c_Sir(), p->d_him());
	} else
	if (i_rand == 2)
	{
	   	cst_customer.Format("`w%s: `$\"Some believe an expensive funeral absolves the purchaser from any sin.  I happen to believe this, my friend.\"\n\n",
		p_play[get_play(lpReq)].st_name);
	} else
	if (i_rand == 3)
	{
	   	cst_customer.Format("`w%s: `$\"I would stay away from cremation.  Your loved one deserves better than 'bake and shake' - people would talk.\"\n\n",
		p_play[get_play(lpReq)].st_name);
	} else
	if (i_rand == 4)
	{
	   	cst_customer.Format("`w%s: `$\"Resistance to a 'proper' funeral will send the wrong message, wouldn't you agree?\"\n\n",
		p_play[get_play(lpReq)].st_name);
	} else

	{
	   log_error("Error!");
	}

	//increase anger a bit
	p->status.f_mood +=  frandom(1); //get more mad regardless of what happens


	if (p->status.b_loved)
	{
	    p->status.f_mood += 1 +frandom(2); //get more mad regardless of what happens
	   //this advice doesn't mean jack to us
    	int i_rand = random(3);
   	  	if (i_rand == 0)
		cst_answer.Format("%s\"I LOVED %s! Don't you get it? None of that stuff matters to me!\"",cst_young_man, p->d_him());
		else
   	  	if (i_rand == 1)
		cst_answer.Format("%s\"My %s was %s a %s for god-sake! How can you say something like that?\"",cst_young_man,p->d_relation(),
		p->d_smashed(), p->d_fridge());
		else
   	  	if (i_rand == 2)
		cst_answer.Format("%s\"If you want to keep my business you're going to stop saying stuff like that.\"",cst_young_man);

	} else
	{

	   //they will take this advice to heart
    //this advice doesn't mean jack to us
       p->status.f_cheapness -= 0.7f + frandom(2); //get cheaper
	   fforce_range(&p->status.f_cheapness, 0, 10, "changing cheapness");
	   
		int i_rand = random(4);
   	  	if (i_rand == 0)
		cst_answer.Format("%s\"You're right. I should be a little looser with my money.\"", cst_young_man);
		else
   	  	if (i_rand == 1)
		cst_answer.Format("%s\"Good call.  I guess it would be appropriate to spend more on my %s.\"",cst_young_man,p->d_relation());
		else
   	  	if (i_rand == 2)
		cst_answer.Format("%s\"You've got a point there.. It makes me mad but you're right.\"",cst_young_man);
	  	if (i_rand == 3)
		cst_answer.Format("%s\"I agree in principle.. but I only want to spend $%s on this.\"",cst_young_man,int_to_cash(p->status.i_money_to_spend));

	}

   
   cst_answer +=figure_complicated_total(lpReq);

	
	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);


 }

 void command_complicated_sympathy(LPREQUEST lpReq)
 {
   	CString cst_final;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];
   CString cst_price;
  	CString cst_customer;
 	CString cst_answer;
  	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  

	//guilt them out, only works if they don't really love their son
	if (p->status.f_mood >= 10)
	{
	   	cst_customer.Format("`$You are interrupted!\n\n");
		leave_pissed(&cst_customer, lpReq);
		return;
	}


   	int i_rand = random(5);


	if (i_rand == 0)
	{
	  	cst_customer.Format("`w%s: `$\"%s, I understand you've been through a lot, please tell me what happened.\"\n\n",
		p_play[get_play(lpReq)].st_name, p->c_Sir());
	} else
	if (i_rand == 1)
	{
	   	cst_customer.Format("`w%s: `$\"I know how you feel.  I know someone who lost a %s once.\"\n\n",
		p_play[get_play(lpReq)].st_name, p->d_relation());
	} else
	if (i_rand == 2)
	{
	   	cst_customer.Format("`w%s: `$\"So you two were close?  %s sounds very special.\"\n\n",
		p_play[get_play(lpReq)].st_name, p->d_He());
	} else
	if (i_rand == 3)
	{
	   	cst_customer.Format("`w%s: `$\"I'm not a licensed therapist.. but keep talking, get the pain out.\"\n\n",
		p_play[get_play(lpReq)].st_name);
	} else
	if (i_rand == 4)
	{
	   	cst_customer.Format("`w%s: `$\"Any time a %s is involved in an accident it's going to be traumatic.\"\n\n",
		p_play[get_play(lpReq)].st_name, p->d_fridge());
	} else

	{
	   log_error("Error!");
	}

	//increase anger a bit
	p->status.f_mood += 0.4f + frandom(1); //get more mad regardless of what happens


	if (!p->status.b_loved)
	{
	    p->status.f_mood += 1 +frandom(2); //get more mad regardless of what happens
	   //this advice doesn't mean jack to us
    	int i_rand = random(3);
   	  	if (i_rand == 0)
		cst_answer.Format("%s\"Sorry, I don't read ya, pard.\"",cst_young_man);
		else
   	  	if (i_rand == 1)
		cst_answer.Format("%s\"I don't go for all this touchy feeling crap, ok?  Don't hug me.\"",cst_young_man);
		else
   	  	if (i_rand == 2)
		cst_answer.Format("%s\"Um yeah.. except I actually have another %s, so it ain't so bad.  No more wasting my time, ok?\"",cst_young_man,
		p->d_relation());

	} else
	{

	   //they will take this advice to heart
    //this advice doesn't mean jack to us
       p->status.f_cheapness -=( 0.7f + frandom(2)); //get more likely to spend money
	   fforce_range(&p->status.f_cheapness, 0, 10, "changing cheapness");
	   
		int i_rand = random(4);
   	  	if (i_rand == 0)
		cst_answer.Format("%s\"It's just.. I feel like %s is just in another room, you know?\"", cst_young_man, p->d_he());
		else
   	  	if (i_rand == 1)
		cst_answer.Format("%s\"I still have dreams.. of %s getting %s a %s over and over.\"",cst_young_man,p->d_him(),
		p->d_smashed(), p->d_fridge());
		else
   	  	if (i_rand == 2)
		cst_answer.Format("%s\"I mean, usually someone's %s is close.. but we were REALLY close if you know what I mean.\"",cst_young_man, p->d_relation());
	  	if (i_rand == 3)
		cst_answer.Format("%s\"I appreciate what you're trying to do but I need to get this over with..for $%s or less..\"",cst_young_man, int_to_cash(p->status.i_money_to_spend));

	}

   cst_answer += figure_complicated_total(lpReq);

	
	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);


 }


  void complicated_upgrade(LPREQUEST lpReq)
 {
	 
		//this is the backbone of the game
	CString cst_final;
	CString cst_message;
 	online_struct *p = p_on[lpReq->i_on_index];
   CString cst_price;
 

  	CString cst_customer;
 	CString cst_answer;
  	CString cst_young_man;
	cst_young_man.Format("`w%s: `$",p->c_Young_man());
  	
	//big things work kind of by an upgrade system, not like little things

    //advance upgrade wanted
	int i_upgrade = p->status.e_big_upgrade+1;

	if (i_upgrade > a_upgrade.GetUpperBound())
	{
	  //nothing else to buy.
	cst_answer.Format("`yThis %s has already bought everything big you can think of.  It's a great feeling.",p->c_man());
      cst_answer += figure_complicated_total(lpReq);

	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
     return;
    }
	
  CString cst_last = "Gold Casket";
  if (p->status.e_big_upgrade != ::C_UPGRADE_NONE)
  {
    cst_last = a_upgrade[p->status.e_big_upgrade].cst_name;

  }
		int i_rand = random(3);
		if (i_rand == 0)
		{
		cst_customer.Format("`w%s: `$\"A simple %s may be enough for some.  But tell me, don't the words '%s' (only $%s additional) give you goosebumps?\"\n\n",
		p_play[get_play(lpReq)].st_name, cst_last, a_upgrade[i_upgrade].cst_name,int_to_cash(a_upgrade[i_upgrade].i_price));
		} else 
		if (i_rand == 1)
		{
		cst_customer.Format("`w%s: `$\"I would feel guilty if I didn't mention we do offer a %s for a small fee of $%s.\"\n\n",
		p_play[get_play(lpReq)].st_name, a_upgrade[i_upgrade].cst_name,int_to_cash(a_upgrade[i_upgrade].i_price));
		}

		if (i_rand == 2)
		{
		cst_customer.Format("`w%s: `$\"%s's are a great idea for an enternal slumber. A fee of $%s will be charged, alright?\"\n\n",
		p_play[get_play(lpReq)].st_name, a_upgrade[i_upgrade].cst_name,int_to_cash(a_upgrade[i_upgrade].i_price));
		}
	
	 if ( (frandom(10) < p->status.f_cheapness) || (p->status.f_cheapness > 8))
	{

	   //we're too cheap to buy it, get more mad
		p->status.f_mood += 0.7f + frandom(1);
	
		if (p->status.f_mood >= 10)
		{
		  leave_pissed(&cst_customer, lpReq);
		  return;

		}


		int i_rand = random(6);
		if (i_rand == 0)
		cst_answer.Format("%s\"No.\"",cst_young_man);
		else if (i_rand == 1)
		cst_answer.Format("%s\"Actually I want a simple funeral for my %s.\"",cst_young_man,p->d_relation());
		else if (i_rand == 2)
		cst_answer.Format("%s\"I would rather spend money getting drunk after this.\"",cst_young_man);
		else if (i_rand == 3)
		{
			if (p->status.b_loved)
			cst_answer.Format("%s\"God, can't you see this is hurting me talking about this now?\"",cst_young_man);
		   else
		   {
			  
			   cst_answer.Format("%s\"In case you haven't noticed, I wasn't overly fond of my %s.  Forget it.\"",cst_young_man,p->d_relation());
		   }
		} 
		else if (i_rand == 4)
		cst_answer.Format("%s\"You just said $%d right? I hope you're kidding.\"",cst_young_man, a_upgrade[i_upgrade].i_price); 
		else if (i_rand == 5)
		cst_answer.Format("%s\"I've got the dough, but I would rather spend it on something that matters.\"",cst_young_man); 
	   
		 cst_answer += ::figure_complicated_total(lpReq);

	   add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);
	return;
	}

   
	//we're not too cheap to buy it, but can we afford it?
		int i_purchase_choice = buy_choice(a_upgrade[i_upgrade].i_price, lpReq);

//	float f_mood = (p->status.f_cheapness + p->status.f_mood)/2; //average out emotions
        //changed 12-4-02 to be simpler
        float f_mood =p->status.f_mood; //average out emotions

	
	if (i_purchase_choice == ::C_BUY_CHOICE_YES)
	{
	  	p->status.e_big_upgrade++;
	
		if (f_mood < 7)
		{
		int i_rand = random(3);
		if (i_rand == 0)
		cst_answer.Format("%s\"Alright, I'll take it.\"",cst_young_man); 
		else if (i_rand == 1)
		cst_answer.Format("%s\"Ok.\"",cst_young_man); 
		else if (i_rand == 2)
		cst_answer.Format("%s\"Maybe you're right.  Ok.\"",cst_young_man); 
		} else
		{
		cst_answer.Format("%s\"Fine, whatever, ok?\"",cst_young_man); 
		}

	}  else if (i_purchase_choice == ::C_BUY_CHOICE_CAN_NOT_AFFORD)
	{

	   //can't afford it, unless we're in a really good mood we're splitting
		if (f_mood > 9)
		{
		
            	  leave_pissed(&cst_customer, lpReq);
	
/*
            
            cst_answer.Format("%s\"That would put me over my limit.. I don't have time for this, bye.\"",cst_young_man);
			add_var(&cst_final, "st_option_3", " Continue");
			add_var(&cst_final, "i_command_3", c_request_main);
			add_var(&cst_final, "b_kill_buttons", 1);
			add_var(&cst_final, "st_main", cst_customer+cst_answer);
			add_var(&cst_final, "st_url", "flash/stuff/money_bag.swf");
			//flash/stuff/money_bag.swf
			add_var(&cst_final, "b_show_stats","0");
			add_turns(&cst_final, lpReq);
			add_var(&cst_final, "i_sent",c_command_load_movie);
				p->status.i_special_button = 0;

			seth_ansi(&cst_final);
			send_msg(lpReq, (LPCSTR)cst_final);
  */
  return;

  
		}  else
	  {
		  	p->status.f_mood += 0.4f + frandom(1);
			  cst_answer.Format("%s\"That would put me over my limit.. how about something cheaper?\"",cst_young_man);
	  }
	}
   
   cst_answer += ::figure_complicated_total(lpReq);

	
	add_var(&cst_final, "st_main",cst_customer+cst_answer);
	add_var(&cst_final, "st_mood", p->c_mood());
	complicated_add_options(&cst_final, lpReq);
	seth_ansi(&cst_final);
	send_msg(lpReq, (LPCSTR)cst_final);

	 
 }


 