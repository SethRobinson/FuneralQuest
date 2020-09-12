#pragma once

#include "globals.h"
#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"
#include "maint.h"
#include "location_data.h"

void CommandScript(LPREQUEST lpReq);
void InitScriptFunctions();
void CommandScriptCallback(LPREQUEST lpReq);
bool RunSinglePassScript(char *p_fname);
void CommandScriptRunScript(LPREQUEST lpReq, CString st_location, CString st_script);
bool RunSinglePassScriptAndSetPlayerID(char *p_fname, int i_play);