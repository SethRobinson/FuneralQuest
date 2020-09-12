 
#pragma once

#include "funeral_util.h"
#include "misc_utils.h"
#include <afxtempl.h>
 

void init_location_info(LPREQUEST lpReq, int i_callback, CString cst_location_name);
bool location_callback_ok(LPREQUEST lpReq, int i_command);
