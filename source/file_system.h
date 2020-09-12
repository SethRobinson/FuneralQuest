#pragma once

bool file_sys_save();
bool file_sys_load();

struct sys_struct_loader
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades
	char buffer[256];
};


struct sys_struct
{
	int i_max_version; //version number (the big part)
	int i_min_version; //version number (the small part)
	int i_file_size; //easier to do upgrades

	int i_player_kick_minutes;
	int i_port;
	int i_anon_kick_minutes;
	int i_auto_save_minutes;
	bool b_extra_debug;
	time_t t_boot_time;
	bool b_log;
	char st_local_ip[256];
	bool b_sound;
	char st_path[256];
	int i_logons_today;
	bool b_ip_check;
	bool b_advertise;
	char st_license_logon[60];
	char st_license_password[60]; 
	char st_license_description[255];
	char st_license_game_name[80];
	int i_logons_total;
	int i_hits_today;
	int i_hits_total;
	int i_max_players; //how many the admin wants to allow
	bool b_maint_delete; //delete players who have 0 exp at maint?
	int i_inactive_days; //delete players who don't log on in this many days
	bool b_swear_filter; //on to filter public chat/messages
	char st_force_ip[80];
	bool b_sapi_enabled; //server can talk
    int i_max_online; //how many players can play at once
	char buffer[496];
};


extern sys_struct sys;

extern  sys_struct_loader sys_loader;

