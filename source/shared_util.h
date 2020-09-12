#pragma once

#include "stdafx.h"
#include "all.h"
#include "mfc_all.h"



void format_for_web_cst(CString *cst_temp);
  void strip_dangerous_stuff(CString *cst_temp);
 void format_for_local(char *st_buff);
 void format_for_web(char *st_buff);
 bool swear_fix(CString *cst_message);
void format_for_local_cst(CString *cst_temp);