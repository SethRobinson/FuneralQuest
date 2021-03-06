//note:  i_my_id will already exist and contain the player id of the player this script is attached to

//This script is kind of a special case.  It's run when a player clicks the stats button.  Because the
//stats screen is a special custom screen, many commands like setting a pic don't work because there
//is no pic window on that screen.  So be careful when modifying this script.

void OnCreate()
{
}

void Main()
{
 String st_packet;
 String st_text = "`y";
 
  //extract some extra info about this player

 int i_smoke = GetCustomById(i_my_id, C_CUSTOM_FIRE_ALARM); //1 is yes

 int i_padlock = GetCustomById(i_my_id, C_CUSTOM_PADLOCK); //1 is yes
 int i_safe = GetCustomById(i_my_id, C_CUSTOM_SAFE); //1 is yes
 int i_pad_strength =  GetCustomById(i_my_id, C_CUSTOM_PADLOCK_STRENGTH); //100 is max, 0 is none

 if (i_smoke == 1)
  {
     st_text += "Smoke detector installed.\n";
  }
 
 if (i_padlock == 1)
  {
     st_text += "Your parlor entrance is padlocked. (`w$i_pad_strength$ percent strength`y)\n";
  }
 if (i_safe == 1)
  {
     st_text += "You own a safe.\n";
  }
 if (st_text == "`y")
  {
    //no special messages, let's show the default.
    st_text += "Your stats.";
  }
 st_packet = AddCustom(st_packet, "st_temp", st_text);
  
 main_menu:
 //add options
 st_packet = AddButtonChangeName(st_packet, 2, "Name");
 st_packet = AddButtonHome(st_packet, 1, "Back");
// st_packet = AddButtonScript(st_packet, 1, "Say hi", "script\\public\\crap.c"); //run another script through button

 //special command to add all stat info to our packet (it's a lot of info, but the stat screen uses it..)
 st_packet = AddUpdateStats(st_packet); //only useful to use this in stats.c 

 //send the packet
 SendPacketAndWaitStats(st_packet); //this sends the packet and will pause until a reply is received.
 st_packet = ""; //clear it for a new packet 
 
//this will never get run currently, but if we added some customs buttons it would.

if (i_my_result == 1)
   {
   

   }
 
}

void OnKill()
{
}
