#include "stdafx.h"

#include "funeral_file.h"
#include "funeral_util.h"

void LogMsg(const LPCSTR lpFormat, ...);

global_struct_loader glo_loader;
global_struct glo;

const float C_F_DEFAULT_LUCK_ODDS = 0.08f;
const float C_F_DEFAULT_NORMAL_ODDS = 0.08f;
const float C_F_DEFAULT_SCRIPT_ODDS = 0.05f;
           
const int C_DEFAULT_NAME_CHANGE_COST =1;

const int C_I_TAGS_RECIEVABLE_PER_DAY = 1;

bool file_config_load()
{
	memset(&glo,0,sizeof(glo)); //zero out everything
	glo.i_experience_per_day = 5;
	glo.i_starting_money = 500;
	glo.i_turns_per_day = 15;
	glo.i_starting_gold = 0;
	glo.i_starting_silver = 2;
	glo.i_starting_wood = 5;
	glo.i_starting_karma = 50;
	glo.i_starting_rating = 40;
	glo.a_casket_sp[::c_coffin_wood] = 1;
	glo.a_casket_sp[::c_coffin_silver] = 2;
	glo.a_casket_sp[::c_coffin_gold] = 3;
	glo.i_plot_sp = 1;
	glo.i_starting_luck = 50;
	
    glo.i_yangs_per_day = 1;
	glo.i_risk = 50;		
	glo.b_tourn_active = false;
	glo.i_tourn_days = 30; //one month default
	glo.i_bonus_exp = 20;
	glo.i_bonus_luck = 25;
	glo.i_trade_bonus = 25;
	glo.i_visits_per_day = 2;
	glo.i_starting_hp = 10;
	glo.i_starting_strength = 3;
	strcpy(glo.st_admin_email, "(none)");
	glo.f_normal_odds = C_F_DEFAULT_NORMAL_ODDS;
	glo.f_luck_odds = C_F_DEFAULT_LUCK_ODDS;
    glo.f_script_odds =  C_F_DEFAULT_SCRIPT_ODDS;
    //Set defaults
    glo.i_tags_recievable_per_day = C_I_TAGS_RECIEVABLE_PER_DAY;
	glo.i_name_change_cost = C_DEFAULT_NAME_CHANGE_COST;

	FILE * fp;
	CString cst_temp;
	cst_temp.Format("%sconfig.dat",::st_data_path);
    if ( (fp=fopen(cst_temp, "rb")) == NULL)
	{
	  //Not able to create config file. Let's set the defaults
	  LogMsg("Can't load config.dat file, creating a new one with defaults.");
	  //version and filesize will be set on save each time.
	  load_strings("ratings.txt", true);
	  load_strings("karma.txt", false);
	  return (file_config_save());
	}

	//load header

	if (fread(&glo_loader, sizeof(glo_loader), 1, fp) <= 0)
	{
	  log_error("Couldn't read config.day header.  Corrupted file?  Delete it maybe.");
	  fclose(fp);
	  return false;
	}

	LogMsg("Loading game config. (%d.%d version)",glo_loader.i_max_version, glo_loader.i_min_version);
	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on global.dat file.");
	}
	if (!fread(&glo, glo_loader.i_file_size, 1,fp))
	{
	 log_error("Error with fread loading global.dat file.");
	}

	fclose(fp);

	//make any updates if we have to
	if (glo.i_max_version == 0) if ((glo.i_min_version == 6) || (glo.i_min_version == 62))
	{
	  //update a few things
		glo.i_visits_per_day = 2;
	    glo.i_bonus_exp = 20;
	   	glo.i_starting_hp = 10;
	    glo.i_starting_strength = 3;
        glo.f_normal_odds = C_F_DEFAULT_NORMAL_ODDS;
        glo.f_luck_odds = C_F_DEFAULT_LUCK_ODDS;
        glo.f_script_odds = C_F_DEFAULT_SCRIPT_ODDS;
        glo.i_tags_recievable_per_day = C_I_TAGS_RECIEVABLE_PER_DAY;


	}

	if (glo.i_max_version == 0) if ((glo.i_min_version == 70) || (glo.i_min_version == 72)
        || (glo.i_min_version == 78))
	{
	  //update a few things
		glo.i_bonus_exp = 20;
	   	glo.i_starting_hp = 10;
	    glo.i_starting_strength = 3;
        glo.f_normal_odds = C_F_DEFAULT_NORMAL_ODDS;
        glo.f_luck_odds = C_F_DEFAULT_LUCK_ODDS;
        glo.f_script_odds = C_F_DEFAULT_SCRIPT_ODDS;
        glo.i_tags_recievable_per_day = C_I_TAGS_RECIEVABLE_PER_DAY;


	}

 	if ( (glo.i_max_version == 0) && (glo.i_min_version == 79) )
    {
          glo.f_script_odds = C_F_DEFAULT_SCRIPT_ODDS;
          glo.i_tags_recievable_per_day = C_I_TAGS_RECIEVABLE_PER_DAY;


    }

 	if ( 
        ((glo.i_max_version == 0) && (glo.i_min_version == 92) )
         ||
        ((glo.i_max_version == 0) && (glo.i_min_version == 91) )

         ||
        ((glo.i_max_version == 0) && (glo.i_min_version == 90) )
        )
    {
          glo.i_tags_recievable_per_day = C_I_TAGS_RECIEVABLE_PER_DAY;
    }

	if ( (glo.i_max_version == 0) && (glo.i_min_version < 99) )
	{
		glo.i_name_change_cost = C_DEFAULT_NAME_CHANGE_COST;
	}
		
	//load strings

   	  load_strings("ratings.txt", true);
	  load_strings("karma.txt", false);

	return true;
}


bool file_config_save()
{
 //Set some values
 glo.i_max_version = i_max_version;
 glo.i_min_version = i_min_version;
 glo.i_file_size = sizeof(glo);


 //Now let's actually save the bitch.
 	FILE * fp;
	CString cst_temp;
	cst_temp.Format("%sconfig.dat",::st_data_path);
    
 if ( (fp=fopen(cst_temp, "wb")) == NULL)
	 {
	  log_error("Error saving config.dat.  Out of HD space or something?");
	  return false;
	 }

	 if (fwrite(&glo, sizeof(glo), 1, fp) <= 0)
	 {
		log_error("Wrote zero bytes to config.dat file?  Something is wrong.");
		fclose(fp);
		return false;
	 }
  
 	//done saving.
	 fclose(fp);
	 //LogMsg("Config file saved.");
	 return true;


}