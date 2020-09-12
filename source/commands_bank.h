#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"

void command_bank(LPREQUEST lpReq);
void command_bank_callback(LPREQUEST lpReq);
void bank_exchange(LPREQUEST lpReq);
void bank_exchange_wood(LPREQUEST lpReq);
void bank_exchange_all(LPREQUEST lpReq, int i_from_type, int i_to_type);
void bank_exchange_input(LPREQUEST lpReq, int i_from_type, int i_to_type, int i_callback);
void bank_exchange_silver(LPREQUEST lpReq);
void bank_exchange_gold(LPREQUEST lpReq);
  
 