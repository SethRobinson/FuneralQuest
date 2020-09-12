#include "commands_battle.h"

bool BattleAlreadyEnded(LPREQUEST lpReq);

const int C_BATTLE_RUN = 1;
const int C_BATTLE_ATTACK = 2;
const int C_BATTLE_YELL_WIN = 3;
const int C_BATTLE_YELL_LOST = 4;
const int C_BATTLE_GOT_YELL_WIN = 5;
const int C_BATTLE_GOT_YELL_LOST = 6;


//special fight stuff
const int C_BATTLE_WIN_NORMAL = 1;
const int C_BATTLE_WIN_SELF_DEFENSE = 2;

void command_battle_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
   	if (i_command_sub == C_BATTLE_RUN)
	{
		command_battle_run(lpReq);
		return;
	}

	if (i_command_sub == C_BATTLE_ATTACK)
	{
		command_battle_attack(lpReq);
		return;
	}
		if (i_command_sub == C_BATTLE_YELL_WIN)
	{
		command_battle_yell_win(lpReq);
		return;
	}
		if (i_command_sub == C_BATTLE_GOT_YELL_WIN)
	{
		command_battle_got_yell_win(lpReq);
		return;
	}
   	
		if (i_command_sub == C_BATTLE_YELL_LOST)
	{
		command_battle_yell_lost(lpReq);
		return;
	}
		if (i_command_sub == C_BATTLE_GOT_YELL_LOST)
	{
		command_battle_got_yell_lost(lpReq);
		return;
	}

}


CString get_hit_type_me(int i_hit)
{

 if (in_range(i_hit, 1, 2)) return "slap";
 if (in_range(i_hit, 3, 4)) return "sock";
 if (in_range(i_hit, 5, 6)) return "punch";
 if (in_range(i_hit, 7, 8)) return "kick";
 if (in_range(i_hit, 9, 10)) return "throw";
 if (in_range(i_hit, 11, 12)) return "squeeze";
 if (in_range(i_hit, 13, 14)) return "ninja";

 //default
 return "slam";
}

CString get_hit_type_him(int i_hit)
{

 if (in_range(i_hit, 1, 2)) return "slaps";
 if (in_range(i_hit, 3, 4)) return "socks";
 if (in_range(i_hit, 5, 6)) return "punches";
 if (in_range(i_hit, 7, 8)) return "kicks";
 if (in_range(i_hit, 9, 10)) return "throws";
 if (in_range(i_hit, 11, 12)) return "squeezes";
 if (in_range(i_hit, 13, 14)) return "ninja's";

 //default
 return "slams";
}



void add_battle_menu(CString * cst_main, CString *cst_final, LPREQUEST lpReq, bool b_add_buttons)
{
   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_p = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
 
	//let's add hitpoints and names
	CString cst_temp;
	cst_temp.Format("%s - %d of %d hitpoints", p_me->st_name, p_me->i_hp, p_me->i_hp_max);
	add_var(cst_final, "st_stats_me", cst_temp);
	

	cst_temp.Format("%s - %d of %d hitpoints", p_p->st_name, p_p->i_hp, p_p->i_hp_max);
	add_var(cst_final, "st_stats_him", cst_temp);

	add_var(cst_final, "i_hp_me", p_me->i_hp);
	add_var(cst_final, "i_hp_him", p_p->i_hp);
	
 if (b_add_buttons)
 {
	add_button(cst_final, 1, "Run", ::c_request_battle_callback, C_BATTLE_RUN);
	add_button(cst_final, 2, "Attack", ::c_request_battle_callback, C_BATTLE_ATTACK);
	}
}


void command_battle_run(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	//send original guy info about this battle

	if (BattleAlreadyEnded(lpReq))
	{
		LogError("Looks like %s tried to cheat by calling command_battle_run with a hacked client.", p_play[get_play(lpReq)].st_name);
		send_error_message(lpReq, "Your FQ client is acting oddly.  (Be careful, this has been logged...)");
		return;
	}

   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_him = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
   CString cst_message;
  	   cst_temp.Format("\n`wYou were attacked in your sleep by %s!`$\n\n",p_me->st_name);
	   cst_message += cst_temp;
	   cst_temp.Format("%s got scared and ran off.\n\n", play_He(p_me->i_index));
	   cst_message += cst_temp;
	   send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_picture(p->status.i_his_id, "flash\\people\\man_prowl.swf");
	   send_packet_continue_button(p->status.i_his_id);
	//now let's actually give them the URL to load
	cst_main.Format("You backflip through an open window and take off running.");
	//add battle menu
	add_var(&cst_final, "st_main", cst_main);
	add_button(&cst_final, 1, "Continue", ::c_request_comp, C_SUB_NONE);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	p->status.p_play = NULL; //SETH BUGFIX
}

//true signals an error

bool BattleAlreadyEnded(LPREQUEST lpReq)
{
	online_struct *p = p_on[lpReq->i_on_index];
	player_struct *p_him = p->status.p_play;
	player_struct *p_me = &p_play[get_play(lpReq)];

	if (!p || !p_him || !p_me)
	{
		return true;
	}

	if (p_me->i_hp <= 0) { return true;}
	if (p_him->i_hp <= 0) {return true;}

	return false; //nothing looks strange here
}

void command_battle_attack(LPREQUEST lpReq)
  {
 	CString cst_final;
	CString cst_temp;
	CString cst_main;
	
	if (BattleAlreadyEnded(lpReq))
	{
		LogError("Looks like %s tried to cheat by calling command_battle_attack with a hacked client.", p_play[get_play(lpReq)].st_name);
		send_error_message(lpReq, "Your FQ client is acting oddly.  (Be careful, this has been logged...)");
		return;
	}
	
	//now let's actually give them the URL to load
  	  battle_hit(lpReq, &cst_main);
	  
	    if (somebody_died(lpReq, &cst_final, &cst_main))
	  {
	    //death handler will take care of everything
		  return;
	  }

	  //are we dead? if not..
	  battle_get_hit(lpReq, &cst_main);
	  
    if (somebody_died(lpReq, &cst_final, &cst_main))
	  {
	    //death handler will take care of everything
		  return;

	  }
	  
	  add_battle_menu(&cst_main, &cst_final, lpReq, true);


	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
}

//setup battle screen
void command_battle(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	CString cst_temp;
	//now let's actually give them the URL to load
  	cst_temp.Format("You are in battle.");
	CString cst_main;
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
 	add_var(&cst_final, "st_url_bg", "flash\\places\\funeral_enemy.swf");
	add_var(&cst_final, "st_url", "flash\\music\\music_fight.swf");
	add_var(&cst_final, "b_show_battle", "1"); //turn on battle meters in flash
    online_struct *p = p_on[lpReq->i_on_index];
    p->status.i_location_callback = ::c_request_battle_callback;

	//show intro to battle stuff
     player_struct *p_p = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];

  	//send MAX hitpoints over
	add_var(&cst_final, "i_hp_max_me", p_me->i_hp_max);
	add_var(&cst_final, "i_hp_max_him", p_p->i_hp_max);

	//make a random saying:
	CString cst_rand;
	int i_random = random(6);

	if (i_random == 0) cst_rand.Format("Stop stealing my customers!");
	if (i_random == 1) cst_rand.Format("I will BURY you!"); 
	if (i_random == 2) cst_rand.Format("Messing with me was a grave mistake, dear %s.", p_play[p->status.i_his_id].st_name); 
	if (i_random == 3) 
	{
		if (p_play[p->status.i_his_id].b_male)
		cst_rand.Format("Discount this, bastard!"); else
		cst_rand.Format("Discount this, bitch!");
	}
	if (i_random == 4) cst_rand.Format("I was in this town first!"); 
	if (i_random == 5) cst_rand.Format("You're an amateur! You don't belong here!"); 


	cst_temp.Format("`w\"%s\" `$you scream as you break into %s room.\n\n", cst_rand, play_his(p->status.i_his_id), play_his(p->status.i_his_id));
	cst_main += cst_temp;

	

	//who swings first?
	if (p_play[get_play(lpReq)].i_plots > p_play[p->status.i_his_id].i_plots)
	{
	  //we get first attack
	  cst_temp.Format("%s is completely surprised, you get first swing!\n\n", p_play[p->status.i_his_id].st_name);
	  cst_main += cst_temp;
	
	} else
	{
	  //he gets first attack
	  cst_temp.Format("%s's eyes flick open, %s knew you were in the house!\n\n", p_play[p->status.i_his_id].st_name, play_he(p->status.i_his_id));
  	  cst_main += cst_temp;
	  battle_get_hit(lpReq, &cst_main);

	  if (somebody_died(lpReq, &cst_final, &cst_main))
	  {
	    //death handler will take care of everything
		  return;
	  }
	}
    	//add battle menu

	add_battle_menu(&cst_main, &cst_final, lpReq, true);
	add_var(&cst_final, "st_main", cst_main);
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }	


void battle_get_hit( LPREQUEST lpReq, CString *cst_main)
{
   //let the enemy swing
   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_p = p->status.p_play;
   int i_hit = int (frandom(float(p_p->i_strength+1)/2) + float(p_p->i_strength+1)/2);
   CString cst_temp;
   
   if ( (random(20) == 0) || (i_hit == 0))
   {
	   cst_temp.Format("%s swings but you sidestep the blow.", p_p->st_name);
	   i_hit = 0;
   } else
   {
	   /*
	   if (random(20) == 0)
	   {
		   //power move	   
		   i_hit *= 2;
		   cst_temp.Format("%s `wsoul smashes `$you for `4%d`$ damage!", p_p->st_name, i_hit);
		   
		   
	   }
	   
	   else
	   	 */
		 {
		   //made some contact
		   cst_temp.Format("%s %s you for `4%d`$ damage.", p_p->st_name,get_hit_type_him(i_hit), i_hit);
	   }
   }
	   p_play[get_play(lpReq)].i_hp -= i_hit;
  //add text
  *cst_main += (cst_temp + "\n\n");

}

void battle_hit( LPREQUEST lpReq, CString *cst_main)
{
  
	//if one of us is dead, this should never happen...


	
   //let's swing
   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_p = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
   int i_hit = int (frandom(float(p_me->i_strength+1)/2) + float(p_me->i_strength+1)/2);
   CString cst_temp;
   
   if ( /* (random(20) == 0) ||*/ (i_hit == 0))
   {
	   cst_temp.Format("%s dodges your swing.", p_p->st_name);
	   i_hit = 0;
   } else
   {
	   if (random(10) == 0)
	   {
		   //power move	   
		   i_hit *= 2;
		   cst_temp.Format("You `wsoul smash`$ %s for `4%d`$ damage!", p_p->st_name, i_hit);
		   
	   }
	   else
	   {
		   //made some contact
		   cst_temp.Format("You %s %s for `4%d`$ damage.", get_hit_type_me(i_hit), p_p->st_name, i_hit);
	   }
   }
  p_p->i_hp -= i_hit;
  //add text
  *cst_main += (cst_temp + "\n\n");

}

bool CheckForValidPlayer(player_struct *pPlayer)
{
	if (
		(pPlayer->i_min_version < 0 || pPlayer->i_min_version > 300)

		|| 
		(pPlayer->i_max_version < 0 || pPlayer->i_max_version > 300)
		)
	{
		LogError("Invalid data detected!");
		return false;
	}
	

	return true;
}
bool somebody_died(LPREQUEST lpReq, CString * cst_final, CString * cst_main)
{
   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_him = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
   CString cst_message;
   CString cst_temp;	
   CString cst_prepare;

   if (!CheckForValidPlayer(p_him)) {return false;}

   //did we kill them?
   if (p_him->i_hp < 1)
   {
	   p_him->i_hp = 0;
	   //enemy died
	   //send email
	   cst_temp.Format("\n`wYou were attacked in your sleep by %s!`$\n\n",p_me->st_name);
	   cst_message += cst_temp;
	   cst_temp.Format("You are badly injured.  You vaguely remember riding in an ambulance.\n\n");
	   cst_message += cst_temp;
	   p_him->b_police_protected = true;
	   int i_stolen = 0;
	   CString cst_type;
	   //let's steal some stuff while we're here
	   i_stolen = steal_random_caskets(4, &cst_type, p_me, p_him);
	   //figure out how much BP we lose/get
	   int i_won_bp = 0;
	   int i_lost_bp = 0;
	   figure_battle_bp(p_me /*winner*/, p_him, &i_won_bp, &i_lost_bp, C_BATTLE_WIN_NORMAL);
	   CString cst_temp;
	   if (i_stolen < 1) cst_temp.Format(" Nothing was stolen."); else
	   {
		   if (i_stolen == 1)
			   cst_temp.Format(" `w%d`$ %s tag was stolen from your pocket.", i_stolen, cst_type); else
			   cst_temp.Format(" `w%d`$ %s tags were stolen from your pockets.", i_stolen, cst_type);
	   }
	  	   		broadcast_tv("It looks like %s was taken to the hospital.",  p_him->st_name);
 
	   cst_prepare.Format("`$You lose `w%s `$Bury Points. %s",int_to_cash(i_lost_bp),cst_temp);
	   cst_message += cst_prepare;
	   send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_picture(p->status.i_his_id, "flash\\stuff\\battle_lose.swf");
	   send_packet_continue_button(p->status.i_his_id);
	   
	  if (lpReq->Socket != NULL)
	  {
	   //now finish sending stuff to the guy
	   if (i_stolen < 1)
		   cst_temp.Format("You go through %s pockets but find nothing.",play_his(p_him->i_index)); else
	   {
		   if (i_stolen == 1)
			   cst_temp.Format("You go through %s pockets and find `w%d `$%s tag.",play_his(p_him->i_index), i_stolen, cst_type); else
			   cst_temp.Format("You go through %s pockets and find `w%d `$%s tags.",play_his(p_him->i_index), i_stolen, cst_type);
	   }
	   cst_message.Format("%s crumples to the floor. You have won the battle!\n\nYou earn `w%s `$Bury Points. %s",
		   p_him->st_name, int_to_cash(i_won_bp), cst_temp);
	   *cst_main += cst_message;
	   add_battle_menu(cst_main, cst_final, lpReq, false);
   	    add_var(cst_final, "st_url", "flash\\stuff\\money_bag_2.swf");

	   add_button(cst_final, 1, "Continue", c_request_battle_callback, ::C_BATTLE_YELL_WIN); 
	   add_var(cst_final, "st_main", *cst_main);
	   add_var(cst_final, "i_sent",c_command_generic);
	   seth_ansi(cst_final);
	   send_msg(lpReq, *cst_final);
	  }
	   return true;  //signal that we handled it
}
 
   

  if (p_me->i_hp < 1)
   {
	   p_me->i_hp = 0;
	   //enemy died
	   //send email
	   cst_temp.Format("\n`wYou were attacked in your sleep by %s!`$\n\n",p_me->st_name);
	   cst_message += cst_temp;
	   cst_temp.Format("You woke up and kicked %s ass!\n\n", play_his(p_me->i_index));
	   cst_message += cst_temp;
	   int i_won_bp = 0;
	   int i_lost_bp = 0;
	   figure_battle_bp(p_him /*winner*/, p_me, &i_won_bp, &i_lost_bp, C_BATTLE_WIN_SELF_DEFENSE);
	   CString cst_temp;
	   cst_prepare.Format("`$You earn `w%s `$Bury Points. (applied to day %d)",int_to_cash(i_won_bp), glo.i_game_age );
	   cst_message += cst_prepare;
	   send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_picture(p->status.i_his_id, "flash\\places\\hospital_ambulance.swf");
	   send_packet_continue_button(p->status.i_his_id);
	   		broadcast_tv("It looks like %s was taken to the hospital.",  p_me->st_name);
				
	  if (lpReq->Socket != NULL)
	  {
	   
	   //now finish sending stuff to the guy
	   cst_message.Format("You fall down, unable to get up.\n\nYou lose `w%s `$Bury Points.",
		   int_to_cash(i_lost_bp));
	   *cst_main += cst_message;
	   add_battle_menu(cst_main, cst_final, lpReq, false);
       add_var(cst_final, "st_url", "flash\\stuff\\battle_lose.swf");
	   	add_var(cst_final, "st_url_bg", "flash\\blank.swf");

	   add_button(cst_final, 1, "Continue", c_request_battle_callback, ::C_BATTLE_YELL_LOST); 
	   add_var(cst_final, "st_main", *cst_main);
	   add_var(cst_final, "i_sent",c_command_generic);
	   seth_ansi(cst_final);
	   send_msg(lpReq, *cst_final);
	  }	 else
	  {
	   //person is not online, he must have been kicked for inactivity, let's let him know he lost
	  	  cst_message = "";
		  cst_temp.Format("\n`wYou fainted while fighting %s.`$\n\n",p_him->st_name);
	   cst_message += cst_temp;
	   cst_temp.Format("You vaguely remember riding in an ambulance.\n\n");
	   cst_message += cst_temp;
	    cst_temp.Format("`$You lose `w%s `$Bury Points.",int_to_cash(i_lost_bp));
	   cst_message += cst_temp;

	   send_packet(p_me->i_index, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_picture(p_me->i_index, "flash\\stuff\\battle_lose.swf");
	   send_packet_continue_button(p_me->i_index);
	 
	   //write about it in the news
	   CString cst_quote;			
		cst_quote.Format("`8%s assaulted %s (who had fainted) in self defense!\r\n\r\n",
		p_him->st_name, p_me->st_name); 
		add_to_news_section(::C_NEWS_SECTION_CRIME, cst_quote);

	  }
       return true;  //signal that we handled it
}
  

return false;
}


void figure_battle_bp(player_struct *p_winner, player_struct *p_loser, int *p_i_win_bp, int *p_i_lose_bp
					   , int i_battle_type)
{
	if (i_battle_type == ::C_BATTLE_WIN_NORMAL)
	{
       *p_i_win_bp = int(p_loser->i_exp*0.02);
	   *p_i_lose_bp = int(p_loser->i_exp*0.01);
	   exp_mod_force_total(p_loser->i_index, -*p_i_lose_bp);
	   exp_mod_force_total(p_winner->i_index, *p_i_win_bp);
	}


	if (i_battle_type == ::C_BATTLE_WIN_SELF_DEFENSE)
	{
       	//everybody gets half
		*p_i_win_bp = int(p_loser->i_exp*0.01);
	   *p_i_lose_bp = int(p_loser->i_exp*0.005);
	   exp_mod_force_total(p_loser->i_index, -*p_i_lose_bp);
	   exp_mod_force_total(p_winner->i_index, *p_i_win_bp);
	}

}



void command_battle_yell_win(LPREQUEST lpReq)
{
 	CString cst_final;
	CString cst_main;
	add_var(&cst_final, "st_message", "`wYou hear police sirens in the distance... you can yell something as you leave or just hit ENTER to continue.");
	add_var(&cst_final, "st_extra_text", "");
    add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_battle_callback);
	add_var(&cst_final, "i_return_command_sub", C_BATTLE_GOT_YELL_WIN);
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
	
}	

void command_battle_got_yell_win(LPREQUEST lpReq)
{
   
   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_him = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
   
				d_get_token_string(cst_text, "st_extra_text"); //get the fortune
//    	    	strip_dangerous_stuff(&cst_text);
            	swear_check((CStringEx*)&cst_text);

				CString cst_news;
			CString cst_quote;			
			
			if ( (cst_text == "12ABORT12") || (cst_text.GetLength() < 2))
			{
				//they don't want to speak to the press
				cst_quote.Format("`8%s brutally assaulted %s!\r\n\r\n",
					p_me->st_name, p_him->st_name); 
			 
			} else
			{
				cst_quote.Format("`8%s brutally assaulted %s!`6\r\n\"%s\" `8justified %s.\r\n\r\n",
				p_me->st_name,	p_him->st_name,  cst_text,  p_me->st_name); 
		
			
			  CString cst_message;
	   cst_message.Format("`$While recovering you can't get what %s yelled while escaping out of your head...\n\n`w\"%s\"`$",
	   p_me->st_name,  cst_text);
	   send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	     send_packet_picture(p->status.i_his_id, "flash\\places\\hospital_room.swf");
	
	   send_packet_continue_button(p->status.i_his_id);
			}
			
  		add_to_news_section(::C_NEWS_SECTION_CRIME, cst_quote);
		   command_comp(lpReq);

		   	p->status.p_play = NULL; //SETH BUGFIX
	
}

void command_battle_yell_lost(LPREQUEST lpReq)
{
 	CString cst_final;
	CString cst_main;
	add_var(&cst_final, "st_message", "`wYou feel yourself being lifted into an ambulance.  Mutter something or just hit ENTER to continue.");
	add_var(&cst_final, "st_extra_text", "");
    add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_battle_callback);
	add_var(&cst_final, "i_return_command_sub", C_BATTLE_GOT_YELL_LOST);
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);

	
	
}

 void command_battle_got_yell_lost(LPREQUEST lpReq)
{

   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_him = p->status.p_play;
   player_struct *p_me = &p_play[get_play(lpReq)];
  	
				d_get_token_string(cst_text, "st_extra_text"); //get the fortune
    	    	//strip_dangerous_stuff(&cst_text);
            	swear_check((CStringEx*)&cst_text);

				CString cst_news;
			CString cst_quote;			
			
			if ( (cst_text == "12ABORT12") || (cst_text.GetLength() < 2))
			{
				//they don't want to speak to the press
				cst_quote.Format("`8%s assaulted %s in self defense!\r\n\r\n",
					p_him->st_name, p_me->st_name); 
			 
			} else
			{
				cst_quote.Format("`8%s assaulted %s in self defense!`6\r\n\"%s\" `8moaned %s from the ambulance.\r\n\r\n",
					p_him->st_name, p_me->st_name, cst_text,  p_me->st_name); 
	   	    
		

	   //let's also send this stuff to  the guy who lives here
	   CString cst_message;
	   cst_message.Format("`$As %s is leaving you hear %s moaning from the ambulance...\n\n`w\"%s\"`$",
	   p_me->st_name, ::play_him(p_me->i_index), cst_text);
	     send_packet_picture(p->status.i_his_id, "flash\\places\\hospital_ambulance.swf");
	 
	   send_packet(p->status.i_his_id, cst_message, ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
	   send_packet_continue_button(p->status.i_his_id);
	 		}
		   add_to_news_section(::C_NEWS_SECTION_CRIME,cst_quote);
		   command_hospital_room(lpReq);

		   p->status.p_play = NULL; //SETH BUGFIX
	
}
