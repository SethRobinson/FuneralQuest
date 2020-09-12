#pragma once
#include "stdafx.h"
#include "online_struct.h"
#include "vars.h"
#include "broadcast.h"
#include "all.h"
#include "player_struct.h"
#include "funeral_util.h" 
#include "commands.h"
#include "COptions.h"
#include "commands_misc.h"


int get_id_from_full_or_partial_handle(int i_start, CString cst_search, CString *cst_name, int *i_his_id);
bool play_active(int i_play);
int get_play_from_logon_name(CString cst_name);
int get_play_from_handle(CString cst_name);
int how_many_people_are_online(int i_status_required);
int get_number_of_valid_players();
int get_random_player_excluding(int i_exclude); //or -1 to not exclude anyone
CString play_sir(int i_play);
CString play_he(int i_play);
void add_reward_text(CString *p_string, CString cst_name, int i_amount, int *p_i_things_changed);
CString play_his(int i_play);
CString play_He(int i_play);
CString play_him(int i_play);
int get_player_who_locked_this_guy(int i_target);
const CString get_casket_name(int i);
int GetValidPlayerID(int i_id);
int get_id_from_full_or_partial_handle_bar(CString cst_search);
