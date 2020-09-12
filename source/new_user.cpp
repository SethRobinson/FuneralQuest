
#include "security.h"
#include "Globals.h"
#include "CSpeech.h"

void process_new_user(LPREQUEST lpReq)
{
	if (sys.b_ip_check)
		if (cannot_log_in_new_user(lpReq)) return;


	CString cst_final;
	CString cst_temp;
	d_get_token_string(cst_logon, "logon");
	d_get_token_string(cst_name, "name");
	d_get_token_string(cst_funeral_name, "f_name");
	d_get_token_string(cst_password, "password");
	d_get_token_string(cst_password_v, "password_v");
	d_get_token_string(cst_male, "b_male");
	d_get_token_string(cst_email, "email");

	strip_dangerous_stuff(&cst_name);
	strip_dangerous_stuff(&cst_funeral_name);
	swear_check((CStringEx*)&cst_name);
	swear_check((CStringEx*)&cst_funeral_name);

	int i_real_players = get_number_of_valid_players();
	int i_temp = sys.i_max_players;

	if (i_temp > cls_pro.i_max_players) i_temp = cls_pro.i_max_players; 
	if (i_real_players >= i_temp)
	{
		CString cst_no_more;
		cst_no_more.Format("The town is full.  (this server allows %d players max)  Come back later!", i_temp);
		add_var(&cst_final, "g_st_error", cst_no_more);
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","4"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;

	}
	if (cst_name.GetLength() < 3)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour `wname is too short.  Try a longer one.");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","4"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_name.GetLength() > 20)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour name is too long.  Try a shorter one.");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","4"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	//see if someone is already using that logon name
	int i_used;
	i_used = get_play_from_handle(cst_name);
	if (i_used != -1)
	{
		//someone is already using this logon name, don't allow it
		//error message
		add_var(&cst_final, "g_st_error", "`wSomeone is already using the name "+cst_name+". Please try another.");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","4"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}


	if (cst_logon.GetLength() < 3)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour logon name is too short.  Try a longer one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_logon.GetLength() > 20)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour logon name is too long.  Try a shorter one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}



	i_used = get_play_from_logon_name(cst_logon);

	if (i_used != -1)
	{
		//someone is already using this logon name, don't allow it
		//error message
		add_var(&cst_final, "g_st_error", "`wSomeone is already using the logon name of "+cst_logon+". Please try another.");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","3"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}



	if (cst_funeral_name.GetLength() < 3)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour funeral parlor name is too short.  Try a longer one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_funeral_name.GetLength() > 20)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour funeral parlor name is too long.  Try a shorter one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_password.GetLength() > 20)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour password is too long.  Try a shorter one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_password.GetLength() < 1)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour password is too short.  Try a longer one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_password.Compare(cst_password_v) != 0)
	{
		add_var(&cst_final, "g_st_error", "`wYour passwords don't match!  Try again, genius.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;

	}

	if (cst_password.GetLength() < 1)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wYour password is too short.  Try a longer one.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	if (cst_email.GetLength() < 4)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wEmail address required.  (Only used for game purposes)");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","3"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}
	if (cst_email.GetLength() > 45)
	{
		//error message
		add_var(&cst_final, "g_st_error", "`wEmail address is too big.  Try a smaller one.");
		seth_ansi(&cst_final);
		add_var(&cst_final, "i_sent","3"); //4 means go back to first new user screen
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}

	//Passed all tests.  Create the account.

	if (!g_on.create_new_player(lpReq))
	{
		//error creating account
		add_var(&cst_final, "g_st_error", "Unknown server error trying to creating account - Please email the admin.");
		add_var(&cst_final, "i_sent","3"); //2 means unknown user in this case
		seth_ansi(&cst_final);
		send_msg(lpReq, (LPCSTR)cst_final);
		return;
	}


	//Make this a little easier..
	int i_play = p_on[lpReq->i_on_index]->i_play;

	//Copy vars to player record
	if (cst_male.CompareNoCase("1") == 0)
		p_play[i_play].b_male = true; else p_play[i_play].b_male = false;
	p_play[i_play].i_index = i_play; //remember our index in the player file
	strcpy(p_play[i_play].st_name, cst_name);
	strcpy(p_play[i_play].st_logon, cst_logon);
	strcpy(p_play[i_play].st_funeral_name, cst_funeral_name);
	strcpy(p_play[i_play].st_password, cst_password);
	strcpy(p_play[i_play].st_email, cst_email);
	p_play[i_play].b_active = true;

	//set starting stats
	p_play[i_play].i_money = glo.i_starting_money;

	//don't have to set these, "new day" function will do it
	//p_play[i_play].i_exp = glo.i_experience_per_day;
	//p_play[i_play].i_turns = glo.i_turns_per_day;
	p_play[i_play].i_karma = glo.i_starting_karma;
	p_play[i_play].i_rating = glo.i_starting_rating;
	p_play[i_play].i_luck = glo.i_starting_luck;
	p_play[i_play].i_hp_max = glo.i_starting_hp;
	p_play[i_play].i_hp = glo.i_starting_hp;
	p_play[i_play].b_can_be_attacked = true;
	p_play[i_play].i_strength = glo.i_starting_strength;

	/*
	p_play[i_play].a_coffins[::c_coffin_wood] = glo.i_starting_wood;
	p_play[i_play].a_coffins[::c_coffin_silver] = glo.i_starting_silver;
	p_play[i_play].a_coffins[::c_coffin_gold] = glo.i_starting_gold;
	p_play[i_play].i_plots = glo.i_starting_plots;
	*/
	//if (cannot_log_in(lpReq)) return;
	if (sys.b_sound)
	{

		CString cst_temp;
		cst_temp.Format("%s\\sounds\\logon.wav",sys.st_path);
		//	LogMsg("Playing %s",cst_temp);
		PlaySound((LPCSTR)cst_temp, NULL, SND_FILENAME | SND_ASYNC);

	}

	sys.i_logons_today++;
	sys.i_logons_total++;


	g_on.logon(lpReq, i_play);

	//let's announce some things
	broadcast("%s has just moved into town!",p_play[i_play].st_name);
	CString cst_temp2;

	cst_temp2.Format("`5A new parlor (%s) run by %s has been opened!\r\n\r\n",
		p_play[i_play].st_funeral_name, p_play[i_play].st_name);
	//cst_temp2.MakeUpper();
	g_glo.p_speech->say("<pitch middle = '-20'><rate speed = '0'>%s has moved into town.", p_play[i_play].st_name);


	/*

	int i_rand = random(4);

	if (i_rand == 0)
	cst_temp2.Format("`7STIFF COMPETITION OR DEAD WEIGHT?\r\n`^The funeral industry welcomes %s to town. \"I know some will see me as a threat, but hey, there are enough bodies to go around,\" consoles %s, the owner.\r\n\r\n",
	p_play[i_play].st_funeral_name, p_play[i_play].st_name);
	if (i_rand == 1)
	cst_temp2.Format("`7NEW FUNERAL HOME TURNING HEADS\r\n`^%s is a brand new full service death care facility. \"We're the only place in town who won't RIP you off!\" lamely joked %s, the owner.\r\n\r\n",
	p_play[i_play].st_funeral_name, p_play[i_play].st_name);
	if (i_rand == 2)
	cst_temp2.Format("`7NEW PARLOR OPEN FOR BUSINESS\r\n`^%s, a new death care provider has entered the fray. \"A town can always use one more,\" explains %s, the owner.\r\n\r\n",
	p_play[i_play].st_funeral_name, p_play[i_play].st_name);
	if (i_rand == 3)
	cst_temp2.Format("`7NEW PARLOR OPEN FOR BUSINESS\r\n`^%s, a new death care provider has entered the fray. \"I vow to treat dead people like live people,\" promises %s, the owner.\r\n\r\n",
	p_play[i_play].st_funeral_name, p_play[i_play].st_name);

	*/

	add_to_news(cst_temp2);


	command_news(lpReq); //news is the first thing new players see

}

