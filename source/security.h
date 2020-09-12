	 	  #pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"

#include "maint.h" 
#include "CCustomArray.h"
#include "CTextParse.h"
#include "mmsystem.h"

class CSwear
{
public:
	CString cst_bad_word;
	CString cst_good_word;
};

class CBan
{
public:
	CString cst_ip;
    CString cst_comment;
};

typedef CArray<CSwear,CSwear> SwearArray;
typedef CArray<CBan,CBan> BanArray;

extern SwearArray a_swear;
extern BanArray a_ban;

void load_swear_filter_from_file();
void load_ban_filter_from_file();
bool cannot_log_in(LPREQUEST lpReq, int i_play);
bool cannot_log_in_new_user(LPREQUEST lpReq);
bool swear_check(CStringEx *cst_in);
bool is_banned(LPREQUEST lpReq);

