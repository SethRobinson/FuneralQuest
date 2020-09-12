//main control file for funeral quest

//********* defines and settings for build ***********




#include "funeral.h"

#include "new_user.cpp"
#include "player.cpp"
#include "commands.cpp"
#include "CEvent.h"
#include "maint.h"
#include "mmsystem.h"
#include "COptions.h"
#include "security.h"
#include "globals.h"
#include "CScriptEngine.h"
#include "command_script.h"
#include <direct.h>

#include "CSpeech.h"

void LogMsg(const LPCSTR lpFormat, ...);

bool g_b_server_started = false;

bool tokenize_input(LPREQUEST lpReq, CString cst_request);


//quick hack for me to test things, fix the development server
void ProcessAllPlayers()
{
    for (int i=0; i < g_on.i_cur_players; i++)
    {
        if (p_play[i].b_active)
        {
            //the guy is active
            if (p_play[i].a_custom[C_CUSTOM_PADLOCK] == 1)
            {
                p_play[i].a_custom[C_CUSTOM_PADLOCK_STRENGTH] = 100;
            }
        }
    }
    
}


bool server_init ( void )
{
 	
	//let's randomize things
	srand( (unsigned)time( NULL ) ); //true random
	HTTPSERVINFO si;
	memset(&glo,0,sizeof(glo)); //zero out all memory
	g_on.init(); //clear global online vars
	
	LogMsg("FuneralQuest Server %d.%d Initializing...",
		::i_max_version, ::i_min_version);
	char st_full_path[256];
	getdir(st_full_path);
	LogMsg("Changing directory to %s.",st_full_path);
	chdir(st_full_path);
	file_sys_load(); //load the system config
	strcpy(sys.st_path, st_full_path);
	init_options();
	file_config_load(); //load the game config
	time(&sys.t_boot_time); //set boot time;
	
	//Now init the server
					si.nPort = sys.i_port;
					
					si.lpRootDir = "public";
					si.hwnd      = g_hwnd_main;
					si.uMsgAsy   = UM_ASYNC;
					if (!StartHTTP(&si))
					{
					   log_error("Could not start server. Change some settings and restart the program.");
					}
	//load player data
	player_load();

 //   ProcessAllPlayers(); //used for testing things/fixing player accounts

	load_swear_filter_from_file();
	load_ban_filter_from_file();
	load_transport_from_file();

         //init the scripting engine, one time thing
    SAFE_DELETE(g_glo.p_script); //should be null, but if we init it at another
    //place in the program I want this here to remind me when I cut and paste..
    g_glo.p_script = new CScriptEngine();
    InitScriptFunctions(); //add the FQ specific functions
   //Init globals
    RunSinglePassScript("script\\init.c");
  
    //special case for brand-new games to run maint once
	if (glo.i_game_age == 0)
	{
	  //let's do some maint
	  maint_new_day();
	}
	
	//rank players to refresh the rankings.htm file
	CString cst_temp; 
	::init_ranking_timer(15*1000); //update every 15 minutes  
	add_rankings(&cst_temp, true); 
	build_server_settings_htm(); //build the html we show people
    AfxBeginThread(get_server_dat, NULL);
     SAFE_DELETE(g_glo.p_speech); //release if needed
	 g_glo.p_speech = new CSpeech();
	 g_glo.p_speech->set_mute(!sys.b_sapi_enabled);
	 g_glo.p_speech->say("<pitch middle = '-20'><rate speed = '0'>Welcome to Fewneral Quest.");
	 //hack
	 //cls_pro.b_validated = true;
  
  	g_b_server_started = true;
    return true;

}

bool shutdown_server()
{
  //party is over
	player_save();
	g_dlg_main->timer_kill(); //kill list bar updating timer
	g_on.online_delete_all();
	g_on.player_kill_all();
//	add_text("Normal shut down requested.\r\n", "log.txt");
	file_config_save();
	free_rankings();
	free_seth_buffer();
	StopHTTP();
    SAFE_DELETE(g_glo.p_speech); //release if needed

	return true;
}
 

CString cbut(CString cst)
{
  //center for buttons
	if (cst.GetLength() < 10)
	center_string_cst(&cst, 15);
	return cst;

}

void center_string_cst(CString *cst, int i_max)
{

  
  int i_len = cst->GetLength();
  if (i_len >= i_max) return; //can't add any spaces, it's too big
  int i_spaces_to_add = (i_max-i_len)/2;
  
  CString cst_temp;
  for (int i=0; i < i_spaces_to_add; i++)
	  cst_temp += ' ';

  cst->Insert(0, cst_temp);

}


int GetCountOfPeopleWithThisIPAddress(in_addr &inAdd)
{

	int count = 0;

	for (int i=0; i < g_on.i_cur_players; i++)
	{
		if (
			(p_play[i].last_ip.S_un.S_addr == inAdd.S_un.S_addr)
			|| (p_play[i].old_ip.S_un.S_addr == inAdd.S_un.S_addr)
			)
		{
			count++;
			//p_play[i].t_last_logon
		}
	}

	return count;
}

void send_logon_information(LPREQUEST lpReq)
{
	CString cst_final;
	CString cst_temp;
	cst_temp.Format("           <u><a href=\"http://www.rtsoft.com/fq\">"
             "FQServer %d.%d.</a></u> Copyright 2001-2011: <u><a href=\"http://www.rtsoft.com\">Robinson Technologies</a></u>  Uptime:%s",
i_max_version, i_min_version, get_uptime());

//	cst_temp.Format("hi");
	add_var(&cst_final, "st_intro_news",cst_temp);				  
	
	//add game stat info:
    int i_real_players = get_number_of_valid_players();
    if (glo.b_tourn_active)
	{

		
		cst_temp.Format("Game Info: Tournament Mode Active (day %d of %d) - %d of %d players on now.",
		glo.i_game_age, glo.i_tourn_days, how_many_people_are_online(-1), i_real_players );
	
		if (::tournament_ends_today())
		{
			int i_winner = ::get_top_player();
		 
            if (i_winner == -1)
            {
                //erm, there was no winner.  Let's assume they want to start a new game.
             
                    cst_temp.Format("New tournament starting tomorrow, please come back then!");
	
            }   else
            {
            cst_temp.Format("Game Info: Tournament is OVER, %s declared winner.  New game starting tomorrow.",
			  p_play[i_winner].st_name);
            }


		}
	} else
	{
	cst_temp.Format("Game Info: No Reset Mode Active (day %d) - %d of %d players on now.",
		glo.i_game_age, how_many_people_are_online(-1), i_real_players );
	}


    if (glo.i_game_age == 0)
    {
     cst_temp.Format("A brand new game of FQ will be starting tomorrow! (Server will auto-reset at midnight)");
    }


	CString cst_no_more = "";


	//if cst_no_more isn't null, the client will give them this message instead of letting them create a user.  keep in mind the
	//client could be hacked to ignore this



#ifdef _DEBUG
	LogMsg("Guy at %s loggong in", my_ip(lpReq));

	LogMsg("TimeT size is %d", sizeof(time_t));

	int ipCount = GetCountOfPeopleWithThisIPAddress( ((SOCKADDR_IN*)lpReq->lpSockAddr)->sin_addr);
	LogMsg("There are %d people with this persons IP address.", ipCount);
#endif

	if (cls_pro.i_max_players == 0)
	{
		//not initted yet?
		cst_no_more.Format("This server is not finished initializing.  Please try again in a few minutes. (click refresh)");
	}


	int i_temp = sys.i_max_players;
	
	if (i_temp > cls_pro.i_max_players) i_temp = cls_pro.i_max_players; 
	if (i_real_players >= i_temp)
	cst_no_more.Format("The town is full.  (this server allows %d players max)  Come back later!", i_temp);

		if (glo.b_tourn_active)
		{
			if (glo.i_game_age == glo.i_tourn_days+1)
			{
				//game is over and resetting tomorrow
		 	cst_no_more.Format("A new game is starting tomorrow, please come back then!");
			}
		}
        if (glo.i_game_age == 0)
        {
     		 	cst_no_more.Format("A new game is starting tomorrow, please come back then! (Or midnight tonight!)");
      }


	add_var(&cst_final, "st_no_more", cst_no_more);
	center_string_cst(&cst_temp, 120);
	add_var(&cst_final, "st_game_stats", cst_temp); 
	cst_temp = glo.st_news;
//	center_string_cst(&cst_temp, 120);
	add_var(&cst_final, "st_server_news", cst_temp);
	add_var(&cst_final, "i_sent","1");
	seth_ansi(&cst_final); //change ` symbols to colors and whatnot
	send_msg(lpReq, (LPCSTR)cst_final);
}


void add_logon_stats(CString * pcst_temp, int i_play)
{
  //update the remote guys stats.. add it to this control string
  add_var(pcst_temp, "i_money", int_to_cash(p_play[i_play].i_money));
  add_var(pcst_temp, "i_exp", int_to_cash(p_play[i_play].i_exp));
  add_var(pcst_temp, "i_turns", int_to_cash(p_play[i_play].i_turns));
  add_var(pcst_temp, "i_sp", int_to_cash(p_play[i_play].i_sp));
  add_var(pcst_temp, "i_luck", p_play[i_play].i_luck);
  add_tags(pcst_temp, i_play);
}
void check_name_and_password(LPREQUEST lpReq)
{
	d_get_token_string(cst_logon, "st_logon");
	d_get_token_string(cst_password, "st_password");

	int i_play = -1; //-1 means couldn't find the guy
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  //are we this guy?
	  if (p_play[i].b_active)
	  {
	   if ( (compare(p_play[i].st_logon, (char*) (LPCSTR) cst_logon))
		   && (compare(p_play[i].st_password, (char*)(LPCSTR) cst_password)) )
	   {

		//first do some security checks
	    //if (this_ip_has_been_on
		   if (sys.b_ip_check)
		   if (cannot_log_in(lpReq, i)) return;

		   if (cls_pro.i_max_on == 0)
		   {
			 //not initted yet?
		   send_error_message(lpReq, "This server is not finished initializing.  Please try again in a few minutes.");
   	  	   return;
		   }
		   
		   int i_on_now =  how_many_people_are_online(-1);

           if ( (i_on_now  >= cls_pro.i_max_on) || ( (sys.i_max_online != 0) && (i_on_now >= sys.i_max_online) )  )
		  {
		    //license doesn't allow this many people to be on at once
			  CString cst_crap;
			  cst_crap.Format("Sorry, there are already %d players logged on right now which is the max this server will allow.  Please try again in a few minutes."
				,cls_pro.i_max_on);
			  send_error_message(lpReq, cst_crap);
   	         	return;
		  }
	  
		    if (cls_pro.i_max_players <= i)
		  {
		    //can't log on, higher than our limit
			  CString cst_crap;
			  cst_crap.Format("Sorry, user %d on a server with a max of %d users cannot log on. (Did you admin forget to renew his license?)"
				, i, cls_pro.i_max_players);
			  send_error_message(lpReq, cst_crap);
   	         	return;
		  }
	  
	
		  //make sure nobody is screwing with us
			if (::get_player_who_locked_this_guy(i) != -1)
			{
			  CString cst_crap;
			  cst_crap.Format("Someone is currently poking around your parlor...please log back on in a few minutes!"); 
			  send_error_message(lpReq, cst_crap);
   	         	return;
			}
		  
		  
		  g_on.logon(lpReq, i);
	   
	   	
		//turn them on
	    if (p_on[lpReq->i_on_index] == NULL)
		{
		 //something is wrong.
			LogMsg("User was kicked before we could send him welcome message.");
			return;
		}
	  	LogMsg("Connection from %s (%s)", p_play[get_play(lpReq)].st_name, my_ip(lpReq));
		broadcast("%s logged on.",p_play[get_play(lpReq)].st_name);
		g_glo.p_speech->say("<pitch middle = '-20'><rate speed = '0'>%s logged on.", p_play[get_play(lpReq)].st_name);

			
		command_news(lpReq);
		if (sys.b_sound)
		{
  	    
			CString cst_temp;
			cst_temp.Format("%s\\sounds\\logon.wav",sys.st_path);
		//	LogMsg("Playing %s",cst_temp);
			PlaySound((LPCSTR)cst_temp, NULL, SND_FILENAME | SND_ASYNC);

		}
			//inc logons today
	      sys.i_logons_today++;
	      sys.i_logons_total++;
		return;
	   }
		    

	  }

	}


   	CString cst_final;
	add_var(&cst_final, "i_sent","2"); //2 means unknown user in this case
	send_msg(lpReq, (LPCSTR)cst_final);
   
}



void request_process(LPREQUEST lpReq, CString cst_request)
{
  
	cst_request.Replace("/request.xp?","");

  int i_hold = cst_request.Find('?');
  if (i_hold == -1)
  {
    //There isn't a ? seperator. Is this a Flash 6 bug or maybe there really
	  //should be one?
    i_hold = cst_request.Find('&');

	if (i_hold == -1)
	{
	  //still not there?  fine
	i_hold = cst_request.GetLength();

	}

 

  }
  if ( (i_hold < 1) || (i_hold > d_max_token_size -1))
  {
    //why is this so big?  Send back an error.
	SendError(lpReq, HTTP_STATUS_BADREQUEST1);
	CloseConnection(lpReq);
	return;
  }

  strcpy(lpReq->command.data, cst_request.Left(i_hold));
  cst_request.Delete(0, i_hold); //remove command
  if (cst_request.GetLength() > 0)
  if (cst_request.GetAt(0) == '?') cst_request.Delete(0, 1); //Delete extra ?

  //First things first.  Let's tokenize any extra input given to us.
  if (!tokenize_input(lpReq, cst_request))
  {
    //Error reading tokens.  Probably one was too big?
	log_error("Got bad packet from IP %s of %s.",my_ip(lpReq), cst_request);
  	send_fail(lpReq); //tell the Flash client something went wrong
	return;
  }

	
  //Figure who this is, compare IP's etc
retry:
   if (!g_on.online_update(lpReq))
   {
	  log_error("Error initting memory, ignoring request.");
  	  send_fail(lpReq); //tell the Flash client something went wrong
	  return;  
   }
  int i_play = p_on[lpReq->i_on_index]->i_play;
   //i_play will be -1 if they are not logged on
 
  //Check command
  // LogMsg("Processing command %s.",lpReq->command.data);

  if (compare(lpReq->command.data, "checklogon"))
  {
    //Intro screen.  Check IP and see if they are already logged on.
	  if (i_play != -1)
	  {
	    //logoff person who was on here!
	  	  broadcast("%s logged off.", p_play[get_play(lpReq)].st_name);
		  g_on.online_delete_guy(lpReq->i_on_index);
		  goto retry;
	  }
	  
	  send_logon_information(lpReq);
 	  return;
  }

  if (compare(lpReq->command.data, "logon"))
  {
     if (i_play != -1)
	  {
	    //logoff person who was on here!
	  	 broadcast("%s logged off.", p_play[get_play(lpReq)].st_name);
   	     g_on.online_delete_guy(lpReq->i_on_index);
	 		  goto retry;

	 }
	 //Check name and password
	  check_name_and_password(lpReq);
	  return;
  }

  if (compare(lpReq->command.data, "new"))
  {
     if (i_play != -1)
	  {
	    //logoff person who was on here!
   	     g_on.online_delete_guy(lpReq->i_on_index);
 		  goto retry;

	 }
	 //Check name and password
	  process_new_user(lpReq);
	  return;
  }
   if (compare(lpReq->command.data, "command"))
  {
    //process the actual command.
	   if (lpReq->i_on_index != -1)
       {
       
       p_on[lpReq->i_on_index]->lpReq = lpReq; //remember this for the duration of this
       //hit.
      
       //also make sure the p_play p_on pointer is set for easy access.
       
       }
       process_command(lpReq);
	  return;
  }


}
