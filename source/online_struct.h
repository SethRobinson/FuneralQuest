#pragma once

#include "vars.h"
#include "COptions.h"
#include "message_que.h"
#include "funeral_util.h"
#include "player_struct.h"
#include "misc_utils.h"

typedef CArray<int,int> IntArray;

const int C_STATUS_LOGON = 0;
const int C_STATUS_PLAY = 1;
const int C_STATUS_CHAT = 2;


const int C_COFFIN_WOOD_PRICE = 800;     
const int C_COFFIN_SILVER_PRICE = 2600;
const int C_COFFIN_GOLD_PRICE = 5300;    

const int c_age_range_kid = 0;
const int c_age_range_teen = 1;
const int c_age_range_adult = 2;
const int c_age_range_old = 3;
const int c_max_age_range = 4;

const int c_relation_none = 0;
const int c_relation_sibling = 1;
const int c_relation_parent = 2;
const int c_relation_child = 3;
const int c_relation_spouse = 4;
const int c_relation_cousin = 5;
const int c_relation_grand_parent = 6;
const int c_relation_lover = 7;
const int c_relation_grand_child = 8;
const int c_relation_friend = 9;

const int C_UPGRADE_NONE = -1;


//income levels

const int c_income_poor = 0;
const int c_income_average = 1;
const int c_income_rich = 2;
const int c_income_super_rich =3; //unlike Seth!


//


struct communication_status
{
  bool b_customer_active;
  bool b_customer_sex_male;
  int i_customer_age;
  bool b_deceased_sex_male;
  int i_deceased_age;
  int e_relation;
  int e_income;
  int e_desired_casket;
  int e_big_upgrade;
  int e_death_type;
  int e_death_object;
  int i_price;
  int i_money_to_spend;
  int i_special_button;
  float f_mood; //0 is not mad, 10 is pissed
  float f_cheapness; //0 is not cheap, 10 won't buy anything
  bool b_loved; //true if they really loved the person they are burying
  int i_risk_type; //for the double event
  char st_url_save[255];
  bool b_show_resistance;
  int i_location_callback;
  int i_his_id;
  player_struct *p_play; //holds pointer to our enemy or friend we're dealing with
  bool b_locked;
};

class online_struct
{
public:
	const CString c_man();
	const CString online_struct::c_young();
	const CString online_struct::c_he();
	const CString online_struct::c_He();

	const CString online_struct::c_his();
	const CString online_struct::d_relation();
	const CString online_struct::c_flash_url();
    const CString online_struct::d_him();
	const CString online_struct::casket();
	const CString online_struct::c_a_young();
    const CString online_struct::c_income();
	const CString online_struct::c_mood(); 
	const CString online_struct::d_smashed();
    const CString online_struct::d_fridge();  
	const CString online_struct::get_price();
	const int online_struct::get_price_int();
	const CString online_struct::c_Young_man();
	bool online_struct::add_option(int i_new);
	const CString online_struct::c_Sir();
	const CString online_struct::c_sir();
	const CString online_struct::d_He();
	const CString online_struct::d_he();
	const CString online_struct::c_him();
    void online_struct::KillActiveScript();
    void online_struct::KillActiveScriptWithoutRunningOnKill();

    int online_struct::InitScript(const char *st_script_file);

	struct in_addr ip; //IP
	bool b_logged_on;
	int i_play; //his player account, applicable if logged on
	LPREQUEST lpReq; //pointer to the hardcore ip/port information
	time_t t_last_packet;
	time_t t_last_chat_packet;
	time_t t_logon;
	void online_struct::init(LPREQUEST lpReq);
	int i_index; //Number to access this record
	int i_list_view; //his index number on the list view control
	int i_hits; //How many requests he's done
	IntArray a_options; //reset for each customer, holds extra stuff they've bought
	bool b_gave_logon_message;
	class_que que; //holds messages we're supposed to display in a linked list
	//variables to record the current state and communication
	int i_status; //is he chatting or what?
	communication_status status; //saves current activity, customer etc
	CString cst_location; //text info on where they are, shows up when listing players
	online_struct();	//initialization
    int m_i_script; //if not C_SCRIPT_NONE, a script is active.
};


//helper class to keep organized
class online_global
{
public:
	int logon(LPREQUEST lpReq, int i_play);
	void update_list_view_times();
	bool player_kill_all();
	void init();
	bool create_new_player(LPREQUEST lpReq);
	bool online_delete_guy(int i_guy);
	bool online_delete_all();

  bool online_update(LPREQUEST lpReq);
  int find_online_guy(LPREQUEST lpReq);
  int create_new(LPREQUEST lpReq);

	int i_cur_people; //people who are online right now
  	int i_cur_players; //players in the database
	
};

extern online_struct **p_on;

extern online_global g_on;

int get_play(LPREQUEST lpReq);
extern int C_MAX_KILL_TEXTS;
extern int C_MAX_KILL_METHOD_TEXTS;

int get_range_from_age(int i_age);


