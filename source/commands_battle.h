  	#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"
#include "commands_comp.h"
#include "security.h"	
#include "commands_hospital.h"

void command_battle_yell_win(LPREQUEST lpReq);
void command_battle_got_yell_win(LPREQUEST lpReq);

void command_battle(LPREQUEST lpReq);
void command_battle_run(LPREQUEST lpReq);
void command_battle_attack(LPREQUEST lpReq);
void command_battle_callback(LPREQUEST lpReq);
void battle_get_hit( LPREQUEST lpReq, CString *cst_main);
void battle_hit( LPREQUEST lpReq, CString *cst_main);
bool somebody_died(LPREQUEST lpReq, CString * cst_final, CString * cst_main);
void figure_battle_bp(player_struct *p_winner, player_struct *p_loser, int *p_i_win_bp, int *p_i_lose_bp
					   , int i_battle_type);
void command_battle_got_yell_lost(LPREQUEST lpReq);
void command_battle_yell_lost(LPREQUEST lpReq);





