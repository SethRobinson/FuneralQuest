  	#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"

void command_spirit(LPREQUEST lpReq);
void command_spirit_callback(LPREQUEST lpReq);
void command_spirit_psych(LPREQUEST lpReq);
int get_psych_cost(LPREQUEST lpReq);
int get_hp_cost(LPREQUEST lpReq);
int get_strength_cost(LPREQUEST lpReq);
void command_spirit_menu(LPREQUEST lpReq);
void command_spirit_hp(LPREQUEST lpReq);
void command_spirit_strength(LPREQUEST lpReq);






