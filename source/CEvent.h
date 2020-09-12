#pragma once

#include "stdafx.h"
#include "CTextParse.h"

class CEvent: public CTextParse
{


public:
	
	char st_message[1000]; //holds text message
	char st_give_random[1000];
	char st_add_log[1000];
	char st_pic_file_name[255];
	char st_title[200];
	int i_mod_bp_low;
	int i_mod_luck_low;
	int i_mod_turns_low;
	int i_mod_bp_high;
	int i_mod_luck_high;
	int i_mod_turns_high;
	
	CEvent();
	~CEvent();


  bool CEvent::compute_event(int i_range, char fname[255]);
   int CEvent::get_random_count_for_range(CTextParse *p_parse, int i_range);
   bool CEvent::move_to_range_and_message(CTextParse *p_parse, int i_range, int i_message);
};


