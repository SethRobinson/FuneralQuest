
#include "stdafx.h"
#include "all.h"
#include "file_system.h"
#include "funeral_file.h"
const char g_st_file_name[] = "system.dat";
char st_data_path[]="data\\";
char st_mail_path[]="data\\mail\\";



sys_struct sys;
sys_struct_loader sys_loader;





bool file_sys_load()
{

   	switch_to_current_dir();
	memset(&sys,0,sizeof(sys)); //zero out everything
	//Set defaults
	sys.i_port = 80;
	sys.i_anon_kick_minutes = 1;
	sys.i_player_kick_minutes = 4;
	sys.i_auto_save_minutes = 30;
	sys.b_sound = true;
	sys.b_ip_check = true;
	sys.b_advertise = true;
	strcpy(sys.st_license_description, "Vanilla settings.");
	strcpy(sys.st_license_logon, "demo");
	strcpy(sys.st_license_password, "demo");
	strcpy(sys.st_license_game_name, "FQ Original");
	sys.b_maint_delete = true;
	sys.i_max_players = 800; 
	sys.i_inactive_days = 15;
	sys.b_swear_filter = true;
	sys.b_sapi_enabled = true;
	FILE * fp;

    if ( (fp=fopen(g_st_file_name, "rb")) == NULL)
	{
	  //Not able to create sys file. Let's set the defaults
	  LogMsg("Can't load %s file, creating a new one with defaults.", g_st_file_name);
	  //version and filesize will be set on save each time.
	  return (file_sys_save());
	}

	//load header

	if (fread(&sys_loader, sizeof(sys_loader), 1, fp) <= 0)
	{
	  log_error("Couldn't read %s header.  Corrupted file?  Delete it maybe.",
		  g_st_file_name);
	  fclose(fp);
	  return false;
	}

	LogMsg("Loading system settings. (%d.%d version)",sys_loader.i_max_version, sys_loader.i_min_version);

	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on sys.dat file.");
	}
	if (!fread(&sys, sys_loader.i_file_size, 1,fp))
	{
	 log_error("Error with fread loading sys.dat file.");
	}

	fclose(fp);

	//make any updates if we have to
		//make any updates if we have to
	if (sys.i_max_version == 0) if ((sys.i_min_version < 77) )
	{
	  sys.b_sapi_enabled = true; //talking defaults to on
	}
	return true;
}


bool file_sys_save()
{

	switch_to_current_dir();
 
	
	//Set some values
 sys.i_max_version = i_max_version;
 sys.i_min_version = i_min_version;
 sys.i_file_size = sizeof(sys);


 //Now let's actually save the bitch.
 FILE * fp;
     if ( (fp=fopen(g_st_file_name, "wb")) == NULL)
	 {
	  log_error("Error saving %s.  Out of HD space or something?", g_st_file_name);
	  return false;
	 }

	 if (fwrite(&sys, sizeof(sys), 1, fp) <= 0)
	 {
		log_error("Wrote zero bytes to %s file?  Something is wrong.");
		fclose(fp);
		return false;
	 }
  
 	//done saving.
	 fclose(fp);
//	 LogMsg("sys file saved.");
	 return true;


}