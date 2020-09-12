#include "reset_game.h"
#include "command_script.h"

void reset_game()
{

 LogMsg("Deleting game data files...");
 delete_wildcard("data","*.txt");
 LogMsg("Deleting mail and messages...");
 delete_wildcard("data\\mail","*.dat");
 LogMsg("Deleting logs...");
 unlink("log.txt");
 unlink("errorlog.txt");
 unlink("data\\player.dat");

 //set some things in the config.dat file

 glo.b_top_old_exists = false;
 glo.i_game_age = 0;
 glo.t_last_maint = 0; //maint will get done right after this
  file_config_save();
  
  RunSinglePassScript("script\\reset.c");
  

}