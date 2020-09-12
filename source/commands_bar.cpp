//The bar room and chat stuff


#include "commands_bar.h"
#include "Globals.h"
#include "CSpeech.h"
		   

void AdjustTV(CStringEx *p_text, LPREQUEST lpReq);

bool g_tvOn = true;

		void command_bar(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	add_var(&cst_final, "st_object", "i_bar");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
 
  }
	

		void list_people_on(LPREQUEST lpReq)
		{
			CString cst_temp;
             int i_counter = 0;			
			 cst_temp.Format("\n`wName                 Status            Last hit");
			 send_to_chat(cst_temp, get_play(lpReq));
			 cst_temp.Format("`$-----------------------------------------------");
			 send_to_chat(cst_temp, get_play(lpReq));
             CString cst_time;
			 


			 for (int i=0; i < g_on.i_cur_people; i++)
			{
				if (p_on[i]) if (p_on[i]->b_logged_on)
				{
					i_counter++;
					get_time_difference_in_text(&cst_time, p_on[i]->t_last_packet);
					cst_temp.Format("%-20s %-12s  %s", p_play[p_on[i]->i_play].st_name,
					p_on[i]->cst_location, cst_time	);
					send_to_chat(cst_temp, get_play(lpReq));
				}
				
			}
	
		 


		}

		CString get_names_in_bar(LPREQUEST lpReq)
		{
			
			//who's here and in the bar right now?
			 CString cst_return;
			 int i_count = how_many_people_are_online(::C_STATUS_CHAT);
			 i_count--; //remove ourself from the count

			 if (i_count == 0)
			 {
			   cst_return = "`$You are the only one here.";
			   return cst_return;
			 }

			 cst_return.Format("`$Other people here:");

			 CString cst_temp;
			 for (int i=0; i < g_on.i_cur_people; i++)
			{
				if (p_on[i]) if (p_on[i]->b_logged_on) if (p_on[i]->i_status == C_STATUS_CHAT)
				{
					if (i != lpReq->i_on_index)
					{
					  //get this guys name
					    i_count--;
						if (i_count > 0)
						{
						  	//not the last name inthe list
							cst_temp.Format(" %s,", p_play[p_on[i]->i_play].st_name);
						} else
						{
					  			cst_temp.Format(" %s", p_play[p_on[i]->i_play].st_name);
				
						}
						cst_return += cst_temp;
					
					}
				}
				
			}
			
		return cst_return;	
		}

		void AdjustTV(CStringEx *p_text, LPREQUEST lpReq)        
		{
			//first get the target's name
			
			
			CStringEx c_target = GetField(p_text, ' ', 1);

			CString cst_send;

			if (c_target == "")
			{
				if (g_tvOn)
				{
					cst_send.Format("`5The TV is currently on.");

				} else
				{
					cst_send.Format("`5The TV is currently off, that stuff will rot your brain.");

				}

			} else
			if (c_target == "on" || c_target == "ON")
			{
				cst_send.Format("`5You turn the tv on.");
				g_tvOn = true;
			} else
			{
				cst_send.Format("`5You turn the tv off.");
				g_tvOn = false;
			}
				send_to_chat(cst_send, get_play(lpReq));
				return; //failed
		}


void ChatGiveTag(CStringEx *p_text, LPREQUEST lpReq)        
{
   //first get the target's name
   CStringEx c_target = GetField(p_text, ' ', 1);

   CString cst_send;
   int i_target = get_id_from_full_or_partial_handle_bar(c_target);

   
   if (i_target == -1)
   {
   cst_send.Format("`5You look around the room but don't see anyone with `w'%s'`5 in their name.", 
       c_target);
   send_to_chat(cst_send, get_play(lpReq));
    return; //failed
   }
   
   int i_tag_type = -1; //set the default to an illegal value
   CStringEx c_tag = GetField(p_text, ' ', 2);
   
   if (c_tag.CompareNoCase("wood") == 0)
       i_tag_type = c_coffin_wood; else
       if (c_tag.CompareNoCase("silver") == 0)
           i_tag_type = c_coffin_silver; else
           if (c_tag.CompareNoCase("gold") == 0)
               i_tag_type = c_coffin_gold; else
           {
               //error!
               cst_send.Format("`5Valid tag types are wood, silver and gold.");
               send_to_chat(cst_send, get_play(lpReq));
               return; //failed
               
           }
           
   int i_play = get_play(lpReq);

   //does he really have the tags?

       if (p_play[i_play].a_coffins[i_tag_type] < 1)
       {
           //doesn't have it
         cst_send.Format("`5%s looks in his pocket for a %s tag but comes up empty.",
           p_play[i_play].st_name, get_casket_name(i_tag_type));
         broadcast_chat(cst_send);
         return; //failed
       } 


     if (i_play == i_target)
   {
         //he wants to give himself a tag?  Ok, let's let him show other players he really has one. (or not)
         cst_send.Format("`5%s absently tosses a %s tag into the air and catches it.",
           p_play[i_play].st_name, get_casket_name(i_tag_type));
         broadcast_chat(cst_send);
         return; //failed
 
     }
 
     if (p_play[i_target].i_tags_recieved_today >= glo.i_tags_recievable_per_day)
     {
         if (p_play[i_target].i_tags_recieved_today == 0)
         {
             cst_send.Format("`5%s is unable to give `w%s `5a %s tag. (the admin has disabled tag transfer)",p_play[i_play].st_name, p_play[i_target].st_name,
                 get_casket_name(i_tag_type), p_play[i_target].i_tags_recieved_today);
             broadcast_chat(cst_send);
             return;
             
             
         }
         
         cst_send.Format("`5%s is unable to give `w%s `5a %s tag. (he's already received %d today)",p_play[i_play].st_name, p_play[i_target].st_name,
             get_casket_name(i_tag_type), p_play[i_target].i_tags_recieved_today);
         broadcast_chat(cst_send);
         return;
     }
     
     
   //let's actually move the tag now

   p_play[i_play].a_coffins[i_tag_type]--;
   p_play[i_target].a_coffins[i_tag_type]++;

   p_play[i_target].i_tags_recieved_today++;
   cst_send.Format("`5%s gives `w%s `5a %s tag.",p_play[i_play].st_name, p_play[i_target].st_name, get_casket_name(i_tag_type));
   broadcast_chat(cst_send);

}


void ShowMyInfo(LPREQUEST lpReq)
{
    //build a string with common info and show it to them in  the chat room.

   CString cst_info;

   int i_play = get_play(lpReq);

   cst_info.Format("`5You have `w%d`5 turns left today and `w%d`5 parlor visits.\nToday's BP's: `w%s`5, total BP's: `w%s`5.\nCasket tags: `w%d`5 Wood, `w%d`5 Silver, `w%d`5 Gold.",
       p_play[i_play].i_turns, glo.i_visits_per_day - p_play[i_play].i_player_fights,
       int_to_cash(p_play[i_play].i_sp), int_to_cash(p_play[i_play].i_exp),
       
       
       p_play[i_play].a_coffins[c_coffin_wood],
       p_play[i_play].a_coffins[c_coffin_silver],
       p_play[i_play].a_coffins[c_coffin_gold]);
   send_to_chat(cst_info, get_play(lpReq));
}

		void process_chat_command(CStringEx *p_text, LPREQUEST lpReq)
		{

		 CStringEx cst_command = GetField(p_text, ' ',0);

		 if (cst_command == "?")
		 {
		     CString cst_hold;
             cst_hold = "\n** `$Cafe chat help **`w\n\n";
             cst_hold +=  "`w/yell `$to send a message to everyone\n`w/me`$ to do a custom action\n";
             cst_hold +=  "`w/all `$to see who is online now\n";
             cst_hold +=  "`w/give name tag_type `$to give a tag. Example: `w/give Seth gold\n";
             cst_hold +=  "`w/info `$to show how many BP's, turns and tags you have now.\n";
			 cst_hold +=  "`w/tv on (or off) `$to control the cafe tv.\n";

             send_to_chat(cst_hold,get_play(lpReq));
		 } else	
			 
         if (cst_command.CompareNoCase("who") == 0)
		 {
		   send_to_chat(get_names_in_bar(lpReq), get_play(lpReq));
		 }  else
           if (cst_command.CompareNoCase("info") == 0)
		 {
		   ShowMyInfo(lpReq);
		 }  else
              if (cst_command.CompareNoCase("give") == 0)
		 {
		   ChatGiveTag(p_text, lpReq);
		 }  else
	
			if (cst_command.CompareNoCase("tv") == 0)
		 {
			 AdjustTV(p_text, lpReq);
			}  else


         if (cst_command.CompareNoCase("me") == 0)
		 {
		   	 p_text->Delete(0, 3); //remove the me and space part
			 swear_check(p_text);
			 broadcast_chat("`$%s %s", p_play[get_play(lpReq)].st_name, *p_text);
		 }  else
  		 
         if (cst_command.CompareNoCase("yell") == 0)
		 {
		   	 p_text->Delete(0, 5); //remove the me and space part
			 swear_check(p_text);
			 broadcast("%s yells: %s", p_play[get_play(lpReq)].st_name, *p_text);
			
			 if (p_play[get_play(lpReq)].b_male)
			 {
			 g_glo.p_speech->say("%s yells:<pitch middle = '-30'> %s.", p_play[get_play(lpReq)].st_name,
			*p_text);
			 } else
			 {
			   //female
		   	 g_glo.p_speech->say("%s yells:<pitch middle = '10'> %s.", p_play[get_play(lpReq)].st_name,
			*p_text);
		
			 }


		 }  else
         if (cst_command.CompareNoCase("all") == 0)
		 {
			list_people_on(lpReq);
		 }  else
		 
		 {
		   send_to_chat("Unknown command.  Do /? for the list.", get_play(lpReq));

		}

		}	
			
		
		void command_chat_start(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
	//now let's actually give them the URL to load
	//tell others he's here, if there are any...
	init_location_info(lpReq, ::C_SUB_NONE, "in cafe");
	p_on[lpReq->i_on_index]->i_status = ::C_STATUS_CHAT;
   time(&p_on[lpReq->i_on_index]->t_last_chat_packet);
  

	broadcast_chat("`5%s saunters in.", p_play[get_play(lpReq)].st_name);

	CString cst_text;

	cst_text.Format("`$The `wPale Horse `$has been here since before you can remember.  A great place to gossip"
" or just sip a mochachino and watch tv.\n\nYou always wondered why they had that"
" big glass bottle on the roof.\n\n%s\nEnter /? for help.\n\n",
		get_names_in_bar(lpReq));
	add_var(&cst_final, "st_message", cst_text);
	add_var(&cst_final, "st_object", "i_chat");
	add_var(&cst_final, "i_chat_speed", "2000");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
   
	
  }

		void process_chat_input(CStringEx *cst_input, LPREQUEST lpReq)
		{
		    //process line by line
		
			int i_index = -1;

			while(1)
			{
			i_index = cst_input->Find("|",0);
			CStringEx cst_text;
			if (i_index != -1)
			cst_text = GetField(cst_input, '|', 0); else
			cst_text = *cst_input;
			
         	if (cst_text.GetLength() > 0)
			{

				
				if (cst_text[0] == '/')
				{
				  	
					//it's a special command
					LogMsg("%s does a %s", p_play[get_play(lpReq)].st_name, cst_text);
					cst_text.Delete(0,1); //take off the special char
					process_chat_command(&cst_text, lpReq);

				} else
				{
			
					//let's swear check it
					swear_check(&cst_text);
					
					broadcast_chat("`$%s: `w%s<font size=\"12\">",p_play[get_play(lpReq)].st_name,cst_text);
				
                    //remove special codes, sounds better for speaking
                    StripSethAnsi(&cst_text);
                    
                    if (p_play[get_play(lpReq)].b_male)
					{
					g_glo.p_speech->say("%s says:<pitch middle = '-30'> %s.", p_play[get_play(lpReq)].st_name,
			cst_text);
					} else
					{
					g_glo.p_speech->say("%s says:<pitch middle = '10'> %s.", p_play[get_play(lpReq)].st_name,
			cst_text);

					}

				}
			}

			if (i_index == -1)
			{
			  //all done, no | char was found
			  return;
			}
			
			//more to process
			cst_input->Delete(0,i_index+1);
			}

		}


			void command_chat_update(LPREQUEST lpReq)
  {
   //News screen.  Send all the info they need then load the screen.
 	CString cst_final;
    d_get_token_string(cst_input, "st_input"); //get the chat
	if (cst_input.GetLength() > 0)
    {
    
	process_chat_input((CStringEx*)&cst_input, lpReq);
    //they actually said something, reset their special chat timer
    	time(&p_on[lpReq->i_on_index]->t_last_chat_packet);
    }
	//add the latest chat if there is any
	CString cst_chat;

    //this a hack so we won't count the auto "update chat" packet as input, this
    //way people can't sit in the chat room and idle.

    p_on[lpReq->i_on_index]->t_last_packet = p_on[lpReq->i_on_index]->t_last_chat_packet;
    add_que_messages_chat(&cst_chat, lpReq);
	add_var(&cst_final, "st_message", cst_chat);
	add_var(&cst_final, "i_sent",c_request_chat_update);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);


  }





