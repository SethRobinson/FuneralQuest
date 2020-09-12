#pragma once
#include "player_struct.h"


//Don't change these much, I have a util that scans for the version # to make
//the installer...
const int i_min_version = 0;
const int i_max_version = 1;
const int c_max_karma_strings = 100;
const int c_max_karma_string_length = 80;
const int c_max_rating_strings = 100;
const int c_max_rating_string_length = 80;


   struct global_struct_loader
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades
	char buffer[4048];
} ;

struct global_struct
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades
	char buffer[256];
	int i_holder_position;
	bool b_extra_crap;
	int i_turns_per_day;
	int i_starting_money;
	int i_experience_per_day;
	char a_st_karma[::c_max_karma_strings][c_max_karma_string_length];
	char a_st_rating[::c_max_rating_strings][c_max_karma_string_length];
	int i_starting_karma;
	int i_starting_rating;
	int i_starting_wood;
	int i_starting_silver;
	int i_starting_gold;
	int i_game_age;
	time_t t_last_maint;
	int i_craphold;
	int i_starting_plots;
	int a_casket_sp[::c_max_coffin_types]; //sell points for selling coffins
	int i_plot_sp; //sell points for selling a plot
	bool b_top_old_exists; //information on who was the best seller yesterday
	char st_top_old_name[255]; //extra big in case of later..
	int i_top_bp_old;
	int i_top_index_old;
	char st_parlor_old[255];
	int i_starting_luck;
	char st_news[500];
	float f_luck_odds; 
	int i_yangs_per_day; //how many yangs can be sen per day
	int i_risk; //the amount they lose/get when doing a risk event
	bool b_tourn_active;
	int i_tourn_days;
	int i_trade_bonus;
	int i_bonus_luck;
	int i_bonus_exp;
	int i_visits_per_day; //how many yangs can be sen per day
	int i_starting_hp;
	int i_starting_strength;
	char st_admin_email[50];
    
    float f_normal_odds;  //odds of a normal event happening 0.10 is normal?
    float f_script_odds;  //odds of a script event happening 0.10 is normal?
    int i_tags_recievable_per_day;
	int i_name_change_cost;
    char st_buffer[3984];
}; 


extern global_struct_loader glo_loader;
extern global_struct glo;
bool file_config_save();
bool file_config_load();
extern char st_data_path[];
extern char st_mail_path[];

