//This is run after logging in for the first time.  Don't do anything that requires a wait such as a send packet
//command.  The player's id who is logging in is placed into i_my_id.  You can modify turns and things here.

void Main()
{
 //uncomment this to prove it's actually running at every newday per player
  
//  string st_name = GetNameFromID(i_my_id);
//  LogMsg("Script newday_player.c processed for $i_my_id$. ($st_name$) Giving 20 turns.");
//  ModTurns(i_my_id, 20);

}
