#pragma once
#include "commands.h"
#include "server_connect.h"
#include "security.h"
#include "transportation.h"
#include "server_settings.h"
#include "message_que.h"
#include "broadcast.h"
#include "funeral_util.h"
#include "player_file.h"
#include "online_util.h"
#include "online_struct.h"


extern bool g_b_server_started;

bool server_init ( void );
bool shutdown_server();
void add_logon_stats(CString * pcst_temp, int i_play);
void add_default_main_screen(CString * pcst_final, LPREQUEST lpReq);
void command_news(LPREQUEST lpReq);
void command_news_front(LPREQUEST lpReq);
void command_news_crime(LPREQUEST lpReq);


