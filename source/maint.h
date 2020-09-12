#pragma once
#include "stdafx.h"
#include "player_struct.h"
#include "online_struct.h"
#include "funeral_file.h"
#include "file_system.h"
#include "all.h"
#include "mfc_all.h"
#include "rankings.h"
#include "player_file.h"
#include "CAutoSave.h"
#include "broadcast.h"
#include "Cevent.h"
#include "misc_utils.h"

#include "server_connect.h"
#include "CProtection.h"

void maint_time_check();
CString get_uptime();
void add_to_news(CString cst_news);
void maint_new_day();
void add_to_news_section(int i_news_section, CString cst_news);

const int C_NEWS_SECTION_FRONT = 0;
const int C_NEWS_SECTION_CRIME = 1;


extern CAutoSave c_reconnect;

bool tournament_ends_today();
