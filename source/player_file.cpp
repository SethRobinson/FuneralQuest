#include "player_file.h"

   bool player_load(void)
{
  //Let's load all our players
 	CString cst_temp;
	cst_temp.Format("%splayer.dat",::st_data_path);
 	  if (!exist_cst(cst_temp))
  {
    //File doesn't exist yet.
	  LogMsg("Player.dat file doesn't exist yet, not loading it.");
	  return false;
  }

  	FILE * fp;

    if ( (fp=fopen(cst_temp, "rb")) == NULL)
	{
	  //Not able to create config file. Let's set the defaults
	  LogMsg("Can't load player.dat file");
	  //version and filesize will be set on save each time.
	  return false;
	}


	//load header

	if (fread(&player_loader, sizeof(player_loader), 1, fp) <= 0)
	{
	  log_error("Couldn't read player.day header.  Corrupted file?  Delete it maybe.");
	  fclose(fp);
	  return false;
	}

	LogMsg("Loading players. (%d.%d version)",player_loader.i_max_version, player_loader.i_min_version);

	
	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on player.dat file.");
	}
	
	
	g_on.i_cur_players = 0;

	while (1)
	{
		//more to load
		if (!fread(&player, player_loader.i_file_size, 1,fp))
		{
			break;
			//LogMsg("Error with fread loading player.dat file.");
		} else
		{

	 //add account to real player data array
	g_on.i_cur_players++;

	//reinit memory
	p_play = (player_struct *)realloc(p_play, sizeof(player_struct)*g_on.i_cur_players);
	if (p_play == NULL)
	{
	  log_error("Error initting memory while loading players.");
  	  fclose(fp);
	  return false;
	}

	//copy record to new array slot we made
	memcpy(&p_play[g_on.i_cur_players-1], &player, sizeof(player_struct));
	 p_play[g_on.i_cur_players-1].p_on = NULL;
	 
	 p_play[g_on.i_cur_players-1].i_index = g_on.i_cur_players-1; //upgrade old versions
		if (p_play[g_on.i_cur_players-1].i_hp_max == 0)
		{
		  //upgrade from 0.62 or earlier version
			p_play[g_on.i_cur_players-1].i_hp_max = 10;
			p_play[g_on.i_cur_players-1].i_hp = 10;
	       	p_play[g_on.i_cur_players-1].b_can_be_attacked = true;
			p_play[g_on.i_cur_players-1].i_strength = 2;

		}
		
		
		}
	}

	fclose(fp);
	return true;

}


   bool player_save()
   {

   if (g_on.i_cur_players == 0) return true; //no players to save at this time

 p_play[0].i_max_version = i_max_version;
 p_play[0].i_min_version = i_min_version;
 p_play[0].i_file_size = sizeof(player_struct);


 //Now let's actually save the bitch.
 FILE * fp;
 	CString cst_temp;
	cst_temp.Format("%splayer.dat",::st_data_path);
 
 if ( (fp=fopen(cst_temp, "wb")) == NULL)
	 {
	  log_error("Error saving player.dat.  Out of HD space or something?");
	  return false;
	 }

	 for (int i=0; i < g_on.i_cur_players; i++)
	 {
	 	 //save each player
		 if (fwrite(&p_play[i], sizeof(player_struct), 1, fp) <= 0)
		 {
			log_error("Wrote zero bytes to player.dat file?  Something is wrong.");
			fclose(fp);
			return false;
		 }
	 }
  
 	//done saving.
	 fclose(fp);
	// LogMsg("Player.dat file saved.");
	 return true;


   }
