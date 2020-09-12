#pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"
#include "security.h"

void command_options(LPREQUEST lpReq);
void command_options_callback(LPREQUEST lpReq);
void change_name_input(LPREQUEST lpReq);
void options_process_name(LPREQUEST lpReq);
