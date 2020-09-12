
#include "online_util.h"
#include "commands_complicated.h"


bool online_global::online_update(LPREQUEST lpReq)
{
	//online_maint(); //every so many minutes
	int i_index = find_online_guy(lpReq);

	if (i_index == -1) 
	{
		i_index = create_new(lpReq);
		if (i_index == -1)
		{
		log_error("online_update - Failed to create_new?");
			return false; 
		
		}
	}

	if (p_on[i_index] == NULL)
	{
		log_error("online_update - can't update a null.");
		return false;

	}
	
	//Ok, we have our guys online account number.
	time(&p_on[i_index]->t_last_packet);	//update time of last packet
	lpReq->i_on_index = i_index;
	p_on[i_index]->i_hits++; //add a hit
	g_dlg_main->user_list_update(lpReq); //Update list view control
	return true; //no error
}

int online_global::find_online_guy(LPREQUEST lpReq)
{
	SOCKADDR_IN * sock;
	sock = (SOCKADDR_IN*) lpReq->lpSockAddr; //Cast as SOCKADDR_IN

	for (int i=0; i <g_on.i_cur_people; i++)
	{
		if (p_on[i] != NULL)
			if (p_on[i]->ip.S_un.S_addr == sock->sin_addr.S_un.S_addr)	//is IP address the same?
			{
				return i; //found match
			}
	}

	return -1; //Couldn't make a match
}


int online_global::create_new(LPREQUEST lpReq)
{
	//We should check for empty slots first
	int i_on = -1;

	for (int i=0; i < g_on.i_cur_people; i++)
	{
		if (p_on[i] == NULL)
		{
			i_on = i;
			break;
		}
	}

	if (i_on == -1)
	{
		//Add a person
		this->i_cur_people++;
		p_on = (online_struct **)realloc(p_on, sizeof(LONG)*i_cur_people);
		i_on = (i_cur_people-1);
	}

	//Create enough memory for this

	p_on[i_on] = new online_struct;
	if (p_on[i_on] == NULL)
	{
		log_error("Error initting p_on record %d.",i_on);
		return -1;
	}
	memset(&p_on[i_on]->status,0,sizeof(communication_status));

	//initted it.  Let's set the defaults
	p_on[i_on]->i_index = i_on;
	p_on[i_on]->i_list_view = -1;
	time(&p_on[i_on]->t_last_packet);	//update time of last packet
	lpReq->i_on_index = i_on;
	p_on[i_on]->init(lpReq);

	return i_on;
}


bool online_global::online_delete_all()
{

	for (int i=0; i <g_on.i_cur_people; i++)
	{
		if (p_on[i])
			online_delete_guy(i);
	}

	i_cur_people = 0;
	SAFE_FREE(p_on);
	return true;
}

bool online_global::online_delete_guy(int i_guy)
{
	assert(i_guy >= 0);

	//first log off user completely
	p_on[i_guy]->KillActiveScript(); //if a script was going, kill it.  It gets a chance to do last
	//minute things, but shouldn't run any Wait() type of things.

	if (p_on[i_guy] != NULL)	//Used?
	{
		//to give people the sale they were working on
		if (p_on[i_guy]->status.b_customer_active)
			if (p_on[i_guy]->status.i_special_button ==  c_request_complicated_customer)
			{
				//they were in the middle of a sale!
				REQUEST req_hold;
				p_on[i_guy]->lpReq = &req_hold;
				p_on[i_guy]->lpReq->i_on_index = i_guy;
				p_on[i_guy]->lpReq->Socket = NULL;
				command_complicated_finish(p_on[i_guy]->lpReq, true);
				p_on[i_guy]->lpReq = NULL;
			}

			//to not allow people to lose carrier in battle, uncomment this
			/*
			//if he's currently in battle, let's make him lose it
			if (p_on[i_guy]->status.p_play)
			{
			if (p_on[i_guy]->status.i_location_callback == c_request_battle_callback)
			{
			//they are fighting another player right now
			p_play[p_on[i_guy]->i_play].i_hp = 0;
			CString cst_final; //crap
			CString cst_main; //crap;
			//recreate a partial LpReq struct for this to work
			REQUEST req_hold;
			p_on[i_guy]->lpReq = &req_hold;
			p_on[i_guy]->lpReq->i_on_index = i_guy;
			p_on[i_guy]->lpReq->Socket = NULL;
			somebody_died(p_on[i_guy]->lpReq, &cst_final, &cst_main);
			p_on[i_guy]->lpReq = NULL;
			}

			}
			*/
			if (p_on[i_guy]->b_logged_on)
			{
				//turn off his ON flag in the player.dat file too
				p_play[p_on[i_guy]->i_play].p_on = NULL;

				//also save any (high priority) pending messages that were not able to be delivered
				p_on[i_guy]->que.save_to_hd(p_on[i_guy]->i_play);
			}

			//remove from list
			g_dlg_main->user_list_remove(p_on[i_guy]->i_list_view);

			SAFE_DELETE(p_on[i_guy]);

	} else
	{
		log_error("Error - there is no one online on p_on %d.",i_guy);
	}

	return true;
}

int empty_slot()
{
	//is anybody deleted?  If so, let's overwrite their account.
	for (int i=0; i < g_on.i_cur_players; i++)
	{
		if (p_play[i].b_active == false)
		{
			//found an empty slot!
			return i;
		}
	}

	//no empty slots.  Guess we'll have to dynamically create
	//a new one. Oh well, not our job.
	return -1;
}


bool online_global::create_new_player(LPREQUEST lpReq)
{
	//Let's create a new player.
	int i_index;

	i_index = empty_slot(); //fill empty slots

	if (i_index == -1)
	{
#ifdef _DEBUG
		LogMsg("Adding record to player data.");
#endif
		//no empty slots, let's make a new one
		i_cur_players++;
		i_index = i_cur_players-1;
		//reinit memory
		p_play = (player_struct *)realloc(p_play, sizeof(player_struct)*i_cur_players);
		if (p_play == NULL)
		{
			log_error("Error initting memory for new player.");
			return false;
		}
	}
	memset(&p_play[i_index],0,sizeof(player_struct)); //Clear this record

	p_on[lpReq->i_on_index]->i_play = i_index;

	LogMsg("Creating player account %d.",i_index);
	//delete any old mail
	CString cst_temp;
	cst_temp.Format("data\\mail\\%d.dat",i_index);
	unlink(cst_temp);


	this->logon(lpReq, p_on[lpReq->i_on_index]->i_play);


	return true;
}

void online_global::init()
{
	g_on.i_cur_people = 0;
	g_on.i_cur_players = 0;

}

bool online_global::player_kill_all()
{
	for (int i =0; i < g_on.i_cur_players; i++)
	{
	}

	//de-init memory for player record structure
	SAFE_FREE(p_play);
	return true;
}

void online_global::update_list_view_times()
{
	for (int i=0; i <g_on.i_cur_people; i++)
	{
		if (p_on[i] != NULL)
			g_dlg_main->user_list_update_time(p_on[i]);
	}

}

int online_global::logon(LPREQUEST lpReq, int i_play)
{
	p_on[lpReq->i_on_index]->b_logged_on = true;
	p_on[lpReq->i_on_index]->i_play = i_play;

	//fill in info about last and old IP address
	SOCKADDR_IN * sock;
	sock = (SOCKADDR_IN*) lpReq->lpSockAddr;
	p_on[lpReq->i_on_index]->i_status = ::C_STATUS_PLAY;

	p_play[i_play].old_ip = p_play[i_play].last_ip;
	p_play[i_play].last_ip = sock->sin_addr;
	p_play[i_play].p_on = p_on[lpReq->i_on_index];
	time(&p_play[i_play].t_last_logon);

	//load any pending mail data
	p_on[lpReq->i_on_index]->que.load_from_hd(i_play);

	//update list view with new info
	g_dlg_main->user_list_update(lpReq);


	return 1;
}

