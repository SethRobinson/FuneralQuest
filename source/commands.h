#pragma once

#include <afxwin.h>         // MFC core and standard components

//commands we receive from the flash client

const int c_request_quit= 1;
const int c_request_wait_for_customer= 2;
const int c_request_stats = 3;
const int c_request_main = 4;
const int c_request_news = 5;
const int c_request_accept = 6;
const int c_request_yesterday = 7;
const int c_request_announcement = 8;
const int c_request_ranking = 9;
const int c_request_double = 10;
const int c_request_reaction = 11;
const int c_request_map = 12;
const int c_request_yangs = 13;
const int c_request_yangs_callback = 14;
const int c_request_getname = 17;
const int c_request_getnext = 18;
//const int c_request_yangs_dev = 19;
const int c_request_send_mail = 20;
const int c_request_reply = 21;
const int c_request_bar = 22;
const int c_request_chat_start = 23;
const int c_request_chat_update = 24;
const int c_request_soft_sell = 25;
const int c_request_hard_sell = 26;
const int c_request_sympathy = 27;
const int c_request_complicated_customer = 28;
const int c_request_guilt = 29;
const int c_request_finish_sale = 30;
const int c_request_risk_accept = 31;
const int c_request_city_hall = 32;
const int c_request_church = 34;
const int c_request_graveyard = 35;
const int c_request_spirit = 36;
const int c_request_hospital = 37;	 
const int c_request_comp = 38;	 
const int c_request_bank = 39;	 
const int c_request_store = 40;	 
const int c_request_front = 41;	 
const int c_request_crime = 42;	 
const int c_request_options = 43;
const int c_request_script = 44;


const int c_request_spirit_callback = 100;
const int c_request_hospital_callback = 101;
const int c_request_comp_callback = 102;
const int c_request_battle_callback = 103;
const int c_request_bank_callback = 104;
const int c_request_store_callback = 105;
const int c_request_callback_news_today = 106;
const int c_request_callback_news_yesterday = 107;
const int c_request_options_callback = 108;
const int c_request_script_callback = 109;


//commands we send to the flash client

const int c_command_quit= 1;
const int c_command_load_movie= 2;
const int c_command_stats = 3;
const int c_command_main = 4;
const int c_command_news = 5;
const int c_command_ranking = 9;
const int c_command_object = 10;  //load object, could be anything
const int c_command_object_add = 14;  //load object, could be anything
const int c_command_callback = 16;  //load object, could be anything
const int c_command_generic = 33;  //refresh a generic dialog

void add_tags(CString *cst_final, int i_play);
 
