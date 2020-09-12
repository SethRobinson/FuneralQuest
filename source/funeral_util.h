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
#include "file_system.h"
#include "resource.h"
#include "fqdlg.h"
#include "shared_util.h" //shared between the license server and the server
#include "transportation.h"


const int C_SUB_NONE = 0;


CString cbut(CString cst);
void center_string_cst(CString *cst, int i_max);

 

#define d_get_token_string(x,y) CString x; if (!get_token_st(lpReq, y, &x)) {send_fail(lpReq); return;}
#define d_get_token_int(x,y) int x; if (!get_token_int(lpReq, y, &x)) {send_fail(lpReq); return;}

#define d_get_token_string_no_return(x,y) CString x; if (!get_token_st(lpReq, y, &x)) {log_error("Server read error.");}
#define d_get_token_int_no_return(x,y) int x; if (!get_token_int(lpReq, y, &x)) {log_error("Server read error.");}

 void add_var(CString *cst_buff, CString cst_var_name, int i_value);
 void add_var(CString *cst_buff, CString cst_var_name, CString cst_var_value);
 void luck_mod(int i_play, int i_value);
 void seth_ansi(CString *pcst);
 void send_msg(LPREQUEST lpReq, CString cst_final);
 void exp_mod(int i_play, int i_value);
 void turns_mod(int i_play, int i_value);
 bool get_token_st(LPREQUEST lpReq, char name[255], CString * cst_var);
void send_fail(LPREQUEST lpReq);
  bool get_token_int(LPREQUEST lpReq, char name[255], int * cst_var);
  bool get_random_yes_no(int i_luck, float f_weight);
  void load_strings(CString cst_fname, bool b_rating);
 void log_msg_thread_safe(LPCSTR lpFormat, ...);
bool tokenize_input(LPREQUEST lpReq, CString cst_request);
void free_seth_buffer();
void add_button(CString *cst_final, int i_but, CString cst_option, int i_command, int i_sub_command);
void exp_mod_force_total(int i_play, int i_value);
void log_msg_not_threadsafe(LPCSTR lpFormat, ...);
void StripSethAnsi(CString *pcst);



