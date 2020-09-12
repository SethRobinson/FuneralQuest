#pragma once
#include "CTimer.h"


//for sorting the players, we'll need a special struct

struct struct_player_sort
{
  int i_exp;
  int i_play;
};


int get_top_player();
int get_top_player_of_the_day();

bool update_and_rank_players();
void free_rankings();
void add_rankings(CString *cst_message, bool b_file_too);
int get_percent_of_ranking_of_player(int i_play);
CString convert_status_to_text(int i_status);
void init_ranking_timer(int i_timer);	
int get_third_highest_player();
int get_second_highest_player();
void add_rankings_alive(CString *cst_message);
int get_player_rank(int i_play);




