#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"


void command_store(LPREQUEST lpReq);
void command_store_callback(LPREQUEST lpReq);
void store_hearse(LPREQUEST lpReq);
void store_look(LPREQUEST lpReq, int i_num);
void store_buy_hearse(LPREQUEST lpReq);


 