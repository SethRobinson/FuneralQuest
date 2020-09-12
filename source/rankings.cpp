#include "stdafx.h"
#include "player_struct.h"
#include "vars.h"
#include "online_struct.h"
#include "rankings.h"
#include "funeral_file.h"
#include "all.h"

CTimer timer_rank_html;

void init_ranking_timer(int i_timer)
{
  timer_rank_html.set_interval(i_timer);


}

//global everybody shares for ranking printing

struct_player_sort *p_sort;

CString convert_status_to_text(int i_status)
{
	switch (i_status)
	{
	case C_STATUS_CHAT:
		 return "in cafe";
		 break;

	case C_STATUS_LOGON:
		return "logon";
		break;

	default:
		return "on now";

	}

}



void add_rankings(CString *cst_message, bool b_file_force)
{
  //first rank the players
	
	update_and_rank_players();
	
	if (timer_rank_html.interval_reached())
	{
	  b_file_force = true;
	}
	//now output them to this string in a somewhat formatted way
	player_struct *p_pl;
	CString cst_temp;
	CString c_sex;
	CString cst_html;	 
	CString cst_extra;

	int i_count = 0;
	if (b_file_force)
	{
		unlink("public\\pages\\rankings.htm");
	     append_file("public\\pages\\rank_start.htm", "public\\pages\\rankings.htm");
	}

	for (int i=0; i < g_on.i_cur_players; i++)
//	for (int i=0; i < 50; i++)
	{
	  p_pl = &p_play[p_sort[i].i_play];

	  if (p_pl->b_active == false) continue;

	  if (p_pl->b_male) c_sex = "M"; else c_sex = "<font color=\"#FC54FC\">F</font>";
	  if (p_pl->p_on)
	  {
		  //on now
		  cst_temp.Format("%-20s%s  %-20s%10s %s\n",p_pl->st_name, c_sex, 
		  p_pl->st_funeral_name, int_to_cash(p_pl->i_exp), convert_status_to_text(p_pl->p_on->i_status));
	  } else
	  {
		  cst_extra = "";
		  if (p_pl->b_police_protected) cst_extra = "Protected";
		  //not on
		  cst_temp.Format("%-20s%s  %-20s%10s %s\n",p_pl->st_name, c_sex, 
		  p_pl->st_funeral_name, int_to_cash(p_pl->i_exp), cst_extra);

	  }
	 if (b_file_force)
	 {
	  if (p_pl->b_male) c_sex = "M"; else c_sex = "F";
	  cst_html.Format("<tr><td width=\"30\">%d</td><td width=\"212\">%s</td><td width=\"47\">%s</td><td width=\"206\">%s</td><td width=\"131\"><div align=\"right\">%s</div></td></tr>", 
	  i+1, p_pl->st_name, c_sex, p_pl->st_funeral_name, int_to_cash(p_pl->i_exp));
	 
      add_text((char*)(LPCSTR)cst_html, "public\\pages\\rankings.htm");
	 }
		
		  *cst_message += cst_temp;
	  i_count++;
	}
 
	while (i_count - ((i_count/19)*19) != 0)
	{
	  //not a multiple of 20, add padding
		*cst_message += "\n";
		i_count++;
	}

 	if (b_file_force)
	{
	   append_file("public\\pages\\rank_end.htm", "public\\pages\\rankings.htm");
	}
	

}

void add_rankings_alive(CString *cst_message)
{
  //first rank the players
	
	update_and_rank_players();
	
	//now output them to this string in a somewhat formatted way
	player_struct *p_pl;
	CString cst_temp;
	CString c_sex;
	CString cst_html;	 
	CString cst_extra;

	int i_count = 0;

	for (int i=0; i < g_on.i_cur_players; i++)
//	for (int i=0; i < 50; i++)
	{
	  p_pl = &p_play[p_sort[i].i_play];

	  if (p_pl->b_active == false) continue;
	  if (p_pl->b_police_protected) continue;

	  if (p_pl->b_male) c_sex = "M"; else c_sex = "<font color=\"#FC54FC\">F</font>";
	  if (p_pl->p_on)
	  {
		  //on now
		  cst_temp.Format("%-20s%s  %-20s%10s %s\n",p_pl->st_name, c_sex, 
		  p_pl->st_funeral_name, int_to_cash(p_pl->i_exp), convert_status_to_text(p_pl->p_on->i_status));
	  } else
	  {
		  cst_extra = "";
		  if (p_pl->b_police_protected) cst_extra = "Protected";
		  //not on
		  cst_temp.Format("%-20s%s  %-20s%10s %s\n",p_pl->st_name, c_sex, 
		  p_pl->st_funeral_name, int_to_cash(p_pl->i_exp), cst_extra);

	  }
		
		  *cst_message += cst_temp;
	  i_count++;
	}
 
	while (i_count - ((i_count/19)*19) != 0)
	{
	  //not a multiple of 20, add padding
		*cst_message += "\n";
		i_count++;
	}

 
}
  
int get_top_player_of_the_day()
{
  //return the player who has the most BP's

int i_index = -1;
int i_highest_points = 0;	
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  if (p_play[i].b_active)
	  if (p_play[i].i_day_last_logged_on == glo.i_game_age)
	  
		  if (p_play[i].i_sp > i_highest_points)
	  {
	    //mark guy
		  i_index = i;
		  i_highest_points = p_play[i].i_sp;
	  }

	}

  return i_index;
}

int get_top_player()
{
  //return the player who has the most BP's

int i_index = -1;
int i_highest_points = 0;	
	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  if (p_play[i].b_active) 
	  if (p_play[i].i_exp > i_highest_points)
	  {
	    //mark guy
		  i_index = i;
		  i_highest_points = p_play[i].i_exp;
	  }

	}

  return i_index;
}

int get_second_highest_player()
{
  //return the player who has the most BP's
int i_index = -1;
int i_highest_points = 0;	

int i_ignore = get_top_player();

	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  if (p_play[i].b_active) if (i != i_ignore) 
	  if (p_play[i].i_exp > i_highest_points)
	  {
	    //mark guy
		  i_index = i;
		  i_highest_points = p_play[i].i_exp;
	  }

	}

  return i_index;
}

int get_third_highest_player()
{
  //return the player who has the most BP's
int i_index = -1;
int i_highest_points = 0;	

int i_ignore = get_top_player();
int i_ignore_also = get_second_highest_player();

	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  if (p_play[i].b_active) if (i != i_ignore) if (i != i_ignore_also) 
	  if (p_play[i].i_exp > i_highest_points)
	  {
	    //mark guy
		  i_index = i;
		  i_highest_points = p_play[i].i_exp;
	  }

	}

  return i_index;
}



void debug_ranking_print()
{
  for (int i=0; i < g_on.i_cur_players; i++)
  {
    LogMsg("#%d is %s with %d points.",i,p_play[p_sort[i].i_play].st_name, p_sort[i].i_exp);

  }

}

inline int player_sort_compare (const void *p_guy1, const void *p_guy2)
{
   	struct_player_sort *sort_1 = (struct_player_sort*)p_guy1;
   	struct_player_sort *sort_2 = (struct_player_sort*)p_guy2;
	
	//is guy1 higher than guy2?
//	LogMsg("Comparing %d to %d",sort_1->i_exp , sort_2->i_exp);

	if (sort_1->i_exp == sort_2->i_exp) return 0; //same!
	if (sort_1->i_exp > sort_2->i_exp) return -1; else return 1;
  
}


bool update_and_rank_players()
{

 	if (g_on.i_cur_players == 0)
	{
	  	//no players exist yet
		p_sort = NULL;
	  return false;

	}
	//first setup our special ranking structure
  //does it have enough memory?  (were any new players added or is this our first time?)
  p_sort = (struct_player_sort*) realloc(p_sort, sizeof(struct_player_sort)*g_on.i_cur_players);

  //put values in - can't use the real p_play because we're going to move everything around
   for (int i=0; i < g_on.i_cur_players; i++)
  {
       p_sort[i].i_play = i; 
	   p_sort[i].i_exp = p_play[i].i_exp;
  }


  if (!p_sort)
  {
    log_error("Error getting memory to rank players.");
	return false;
  }

  
  //perform the actual sorting
  qsort(p_sort, g_on.i_cur_players, sizeof(struct_player_sort), player_sort_compare); 
 // debug_ranking_print();
  return true;
}


int get_percent_of_ranking_of_player(int i_play)
{
  update_and_rank_players();

  //now find the player
  int i_found = -1;
  for (int i=0; i < g_on.i_cur_players; i++)
  {
    if (p_sort[i].i_play == i_play)
	{
	  i_found = i;
	}
  }

  if (i_found == -1)
  {
    log_error("Error in get_percent_of_ranking_of_player, can't find player");
	return 1;
  }

  int i_percent = int( float(i_found) / float(g_on.i_cur_players)*100);

//  LogMsg("Percent is %d.",i_percent);

  return (100-i_percent);
}

int get_player_rank(int i_play)
{
  update_and_rank_players();

  //now find the player
  for (int i=0; i < g_on.i_cur_players; i++)
  {
    if (p_sort[i].i_play == i_play)
	{
	  return i+1;
	}
  }

  log_error("Unable to get player rank!");
  return -1;
}


void free_rankings()
{
  //free any memory structures associated with rankinsg
  SAFE_FREE(p_sort);

}
