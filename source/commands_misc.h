#pragma once


#include "stdafx.h"
#include "online_struct.h"
#include "vars.h"
#include "broadcast.h"
#include "all.h"
#include "player_struct.h"
#include "funeral_util.h" 
#include "funeral_file.h"
#include "commands.h"
#include "mfc_all.h"
#include "CEvent.h"

void send_error_message(LPREQUEST lpReq, CString cst_error);
char * color_check();
 void add_turns(CString * pcst_temp, LPREQUEST lpReq);
 void add_money(CString * pcst_temp, LPREQUEST lpReq);
void add_exp(CString * pcst_temp, LPREQUEST lpReq);
 void add_sp(CString * pcst_temp, LPREQUEST lpReq);
 void add_luck(CString * pcst_temp, LPREQUEST lpReq);
  void command_quit(LPREQUEST lpReq);
    void command_no_business(LPREQUEST lpReq);
	void command_no_turns(LPREQUEST lpReq);
	CString get_rating_string(int i_rating_percent);
