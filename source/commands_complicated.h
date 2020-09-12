 #pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"

void command_complicated_customer(LPREQUEST lpReq);
void command_complicated_finish(LPREQUEST lpReq, bool b_user_crashed);

void command_complicated_soft_sell(LPREQUEST lpReq);
void command_complicated_hard_sell(LPREQUEST lpReq);
void command_complicated_guilt(LPREQUEST lpReq);
void command_complicated_sympathy(LPREQUEST lpReq);
void complicated_upgrade(LPREQUEST lpReq);



