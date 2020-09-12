//This is run as the server starts up and inits the scripting system.  Don't use anything that would cause
//a delay here.


//*********** ANY CHANGES TO THIS SCRIPT REQUIRE YOU TO RESTART THE SERVER!!!!!! ************
void Main()
{
  //declare global variables to be used system wide, these are constants but since the scripting
  //system doesn't support constants we use the C_ prefix to make it clear to us.
  global int C_WOOD_TAG = 0;
  global int C_SILVER_TAG = 1;
  
  global int C_GOLD_TAG = 2;


  global int C_CUSTOM_FIRE_ALARM = 0; //so we don't have to remember index #'s
  global int C_CUSTOM_SAFE = 1; //so we don't have to remember index #'s
  global int C_CUSTOM_PADLOCK = 2; //so we don't have to remember index #'s
  global int C_CUSTOM_LOCKPICK = 3; //so we don't have to remember index #'s
  global int C_CUSTOM_PADLOCK_STRENGTH = 4; //0 means none, 100 is full strength. At 0 it breaks.

  LogMsg("Scripting system initted, ran init.c.");

}
