#pragma once

#include "winsock.h"
#include "time.h"
class online_struct;

//Main player structure
const int c_max_coffin_types = 10;

const int c_coffin_wood = 0;
const int c_coffin_silver = 1;
const int c_coffin_gold = 2; 

const int C_CUSTOM_SIZE = 200;


//custom things we know about, 100 and < are reserved for us.  Compare with
//the init.c script to get these up to date.

const int C_CUSTOM_FIRE_ALARM = 0;  //1 is installed, 0 is not installed.
const int C_CUSTOM_SAFE = 1;  //1 is installed, 0 is not installed.
const int C_CUSTOM_PADLOCK = 2;  //1 is installed, 0 is not installed.
const int C_CUSTOM_LOCKPICK = 3;  //1 is installed, 0 is not installed.
const int C_CUSTOM_PADLOCK_STRENGTH = 4;  //100 is max, 0 is broken/about to break

const int c_transportation_bicycle = 0;

struct player_struct_loader
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades
	char buffer[10];
} ;



struct player_struct
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades
	char buffer[10];

	//***********

	char st_name[30]; //Name shown to players
	char st_logon[30];	//Name used to logon
	char st_password[30];  //Max 29 chars!
	char st_funeral_name[30];
	char st_email[50];
	bool b_active; //false if deleted
	bool b_want_email;	//Set to true if he wants spam (not used)
	bool b_male;   //True if player is male
	struct in_addr last_ip;	 //in_addr strict of last_ip (or current)
	struct in_addr old_ip;	//in_addr struct of old_ip (before last)
	online_struct *p_on; //save his online struct, NULL if not online
	time_t t_last_logon;
	time_t t_last_packet;
	int i_turns;
	int i_money;
	int i_exp;
	int i_karma; //start at 40 something?
	int i_rating; //start at 40 something?
	int a_coffins[c_max_coffin_types]; //only use 3, but just in case..
	bool b_incinerator_installed; //can you function as a creamatorium?
	int i_transportation_type; //bike, types of hearses
	int i_plots; //how many spots to bury you have
	int i_day_last_logged_on;
	int i_sp; //sell points earned today
	int i_luck;
	bool b_always; //can always log on, skips IP security check
	int i_yangs_sent;
	time_t t_account_created;
	int i_psych;
	bool b_kicked_out_of_yangs;
	int i_strength;
	int i_hp_max;
	int i_hp;
	int i_player_fights;
	bool b_can_be_attacked;
	int i_index; //our index in the p_play struct
	bool b_police_protected;
    int a_custom[C_CUSTOM_SIZE]; //used for script declared vars
	int i_tags_recieved_today; //how many tags people have given him today
    byte upgrade_space[96];

};

extern player_struct_loader player_loader;
extern player_struct *p_play, player;