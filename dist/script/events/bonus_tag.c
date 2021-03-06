//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
  //declare some script-wide vars.  A var has to declared ABOVE where you use it, so we do it here for the whole script.
   String st_packet;
   int i_temp;
}


void Main()
{
 int i_tag_type = RandomRange(0,2); //pick a random kind of tag to give them

 String st_tag_type = "wood";

 if (i_tag_type == 1)
   {
      st_tag_type = "silver";
   }

 if (i_tag_type == 2)
   {
      st_tag_type = "gold";
   }

 String st_place = "davenport";

 LogMsg("Giving bonus $st_tag_type$ tag to $st_my_name$.");

 i_temp = RandomRange(0, 5);
 if (i_temp == 1)
  {
    st_place = "shed";
   }

 if (i_temp == 2)
  {
    st_place = "desk";
   }
 if (i_temp == 3)
  {
    st_place = "embalming fluids";
   }
 if (i_temp == 4)
  {
    st_place = "stereo";
   }
 if (i_temp == 5)
  {
    st_place = "parlor";
   }

 st_packet = AddCustom(st_packet, "st_main", "`yWhile cleaning behind the $st_place$ you find a `w$st_tag_type$ tag`y.");
 st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\money_bag_2.swf");
 //st_packet = AddCustom(st_packet, "st_url_bg", "flash\\people\\m_old_3.swf");
// st_packet = AddCustom(st_packet, "st_object", "i_generic"); //used to create the initial screen type

 //actually add the tag
  ModTags(i_my_id, i_tag_type, 1); 
  st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
 

 main_menu:
 //add options
 st_packet = AddButtonHome(st_packet, 1, "Continue"); //the last number is what gets put in i_last_result.
// st_packet = AddButtonScript(st_packet, 1, "Say hi", "script\\public\\crap.c"); //run another script through button

 //send the packet
  st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
  SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
  st_packet = ""; //clear it for a new packet 
  goto main_menu;
}

void OnKill()
{
}
