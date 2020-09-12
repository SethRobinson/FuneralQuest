  	#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"
#include "commands_battle.h"

void command_comp(LPREQUEST lpReq);
void add_comp_menu(CString *cst_final, LPREQUEST lpReq);
void command_comp_callback(LPREQUEST lpReq);
void command_comp_look(LPREQUEST lpReq);
void command_comp_menu(LPREQUEST lpReq);
void command_comp_get_name(LPREQUEST lpReq);
void command_comp_got_name(LPREQUEST lpReq); 
void command_comp_try_door(LPREQUEST lpReq);
void command_comp_steal(LPREQUEST lpReq);
void command_comp_attack(LPREQUEST lpReq);
int steal_random_caskets(int i_max_to_steal, CString *cst_type, player_struct *p_me, player_struct *p_him);




