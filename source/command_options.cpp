#include "command_options.h"

const int C_OPTIONS_PROCESS_NAME = 1;
const int C_OPTIONS_NAME_INPUT = 2;


	 
void command_options_callback(LPREQUEST lpReq)
{
    online_struct *p = p_on[lpReq->i_on_index];
	
	d_get_token_int(i_command_sub, "i_command_sub");
	
  	
	if (i_command_sub == C_OPTIONS_NAME_INPUT)
	{
		change_name_input(lpReq);
		return;
	}
		if (i_command_sub == C_OPTIONS_PROCESS_NAME)
	{
		options_process_name(lpReq);
		return;
	}

}


		void command_options(LPREQUEST lpReq)
  {
  
	  //News screen.  Send all the info they need then load the screen.
	  CString cst_final;
	  //now let's actually give them the URL to load
	  CString cst_temp;
 
	  if (glo.i_name_change_cost < 1)
	  {
		  //can't afford it
		  add_var(&cst_final, "st_generic_info", cst_temp);
		  add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
		  add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have

		  cst_temp.Format("`$This server doesn't allow name changes.  You're stuck with what your parents gave you.");

		  add_var(&cst_final, "st_main", cst_temp);
		  //add_var(&cst_final, "st_url_bg", "flash\\stuff\\question.swf");
		  add_var(&cst_final, "st_url", "flash\\stuff\\question.swf");
		  init_location_info(lpReq, c_request_options_callback, "Change Name"); 
		  add_button(&cst_final, 1, "Continue",c_request_stats, ::C_SUB_NONE);                                                 		
		  add_var(&cst_final, "st_object", "i_generic");
		  add_var(&cst_final, "i_sent",c_command_object);
		  seth_ansi(&cst_final);
		  send_msg(lpReq, cst_final);	  return;


	  }



		cst_temp.Format("You stare at yourself in the mirror.\n\nMaybe it's time for a change.");
	add_var(&cst_final, "st_generic_info", cst_temp);
    add_var(&cst_final, "st_generic_status", ""); //blank out the status bar
 	add_var(&cst_final, "b_show_tags", "1"); //show how many casket tags they have


	cst_temp.Format("`$Announcing your new name in the newspaper will cost `w%d`$ gold tag(s).  Do it?", glo.i_name_change_cost);
	add_var(&cst_final, "st_main", cst_temp);
	//add_var(&cst_final, "st_url_bg", "flash\\stuff\\question.swf");
	add_var(&cst_final, "st_url", "flash\\stuff\\question.swf");
	init_location_info(lpReq, c_request_options_callback, "Change Name"); 
	add_button(&cst_final, 1, "No",c_request_stats, ::C_SUB_NONE);                                                 		
	add_button(&cst_final, 2, "Yes", c_request_options_callback, C_OPTIONS_NAME_INPUT);
	add_var(&cst_final, "st_object", "i_generic");
	add_var(&cst_final, "i_sent",c_command_object);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }


  void change_name_input(LPREQUEST lpReq)
  {

			CString cst_final;
	CString cst_temp;
	cst_temp.Format("`wEnter your new name.");
	
	add_var(&cst_final, "st_message", cst_temp);
	add_var(&cst_final, "st_extra_text", "");
    add_var(&cst_final, "st_object", "i_get_text");
	add_var(&cst_final, "i_return_command", c_request_options_callback);
	add_var(&cst_final, "i_return_command_sub", C_OPTIONS_PROCESS_NAME);
	add_var(&cst_final, "i_sent",c_command_object_add);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  }

void options_process_name(LPREQUEST lpReq)
{

	if (glo.i_name_change_cost < 1)
	{
		//avoid problem with potential hacking..
		return;
	}

   online_struct *p = p_on[lpReq->i_on_index];
   player_struct *p_me = &p_play[get_play(lpReq)];
   d_get_token_string(cst_name, "st_extra_text"); //get the name
   strip_dangerous_stuff(&cst_name);
   swear_check((CStringEx*)&cst_name);
    
    CString cst_temp;
	CString cst_final;
		if (cst_name.Compare("12ABORT12") == 0)
	{
			command_options(lpReq);
			return;
	}
 
		if (p_me->a_coffins[::c_coffin_gold] < glo.i_name_change_cost)
		{
		   //can't afford it
	  		cst_temp.Format("`$You don't have enough gold tags.  Go exchange your woods or something.", cst_name);
		add_var(&cst_final, "st_main", cst_temp);
		add_button(&cst_final, 1, "Continue",c_request_stats, C_SUB_NONE);                                                 		
		add_var(&cst_final, "i_sent",c_command_generic);
		seth_ansi(&cst_final);
		send_msg(lpReq, cst_final);
  		return;
	}


	if (cst_name.Compare(p_me->st_name) == 0)
	{
	  //it's identical to what they have..
	  	cst_temp.Format("`$Yeah, %s.. you like the sound of that... oh wait...", cst_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_options, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  	return;


	}
   



   //is the name ok?

		if (cst_name.GetLength() < 3)
	{
	//error message
	cst_temp.Format("`$What, are those initials?  Pick a longer name.");
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_options, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  	return;
	}
	
	if (cst_name.GetLength() > 20)
	{
	cst_temp.Format("`$Is that a name or a lecture?  Try something shorter.");
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_options, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  	return;
	}


	//see if someone is already using that logon name
    int i_used;
  
	i_used = get_play_from_handle(cst_name);

	if ( (i_used != -1) && (i_used != get_play(lpReq)) )
	{
	cst_temp.Format("`$Um, don't you think the real %s would kind of be pissed?  Try something original.", 
		cst_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_options, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);
  	return;
	}

  	//remove the tag
	p_me->a_coffins[::c_coffin_gold] -= glo.i_name_change_cost;
    
	//say stuff to log
		CString cst_news;
	cst_news.Format("`5NAME CHANGE: %s has changed %s name to %s.\r\n\r\n",
		p_me->st_name, ::play_his(p_me->i_index), cst_name);

	add_to_news_section(::C_NEWS_SECTION_FRONT,cst_news);



	strcpy(p_me->st_name, cst_name); //change name
	cst_temp.Format("`$You will now be known as `w%s`$.", cst_name);
	add_var(&cst_final, "st_main", cst_temp);
	add_button(&cst_final, 1, "Continue",c_request_stats, C_SUB_NONE);                                                 		
	add_var(&cst_final, "i_sent",c_command_generic);
	seth_ansi(&cst_final);
	send_msg(lpReq, cst_final);


//   command_hospital_room(lpReq);
}
