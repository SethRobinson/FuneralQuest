//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
}

void Main()
{
  //this is the main screen of the church.  Let's send all the info over in a packet. 
 SetLocation("Church"); //shows up on status bar.  If you forget to set it, it's going to just say "Script".
 
 String st_packet;
 st_packet = AddCustom(st_packet, "st_generic_info", "Father Smyth greets you warmly, a little too warmly.  You wonder if he has anything on under that robe.");
 st_packet = AddCustom(st_packet, "st_generic_status", "");
 st_packet = AddCustom(st_packet, "st_main", "`wFather Smyth: `y\"Why hello there, $st_my_name$.  Isn't it a wonderfully fantastic day today?  Praise the Lord.\"");
  

 st_packet = AddCustom(st_packet, "st_url_bg", "flash\\background\\background_church.swf");
 st_packet = AddCustom(st_packet, "st_url", "flash\\people\\churchguy_h.swf");
 st_packet = AddCustom(st_packet, "st_object", "i_generic"); //used to create the initial screen type

 main_menu:
 //add options
 //st_packet = AddButton(st_packet, 1, "Say hi", 1); //the last number is what gets put in i_last_result.
 st_packet = AddButtonTownMap(st_packet, 1, "Leave"); //the last number is what gets put in i_last_result.
// st_packet = AddButtonScript(st_packet, 1, "Say hi", "script\\public\\crap.c"); //run another script through button

 //send the packet
 SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.

 st_packet = ""; //clear it for a new packet 
 
if (i_my_result == 1)
   {
    st_packet = AddCustom(st_packet, "st_main", "`wFather Smyth: `y\"Hi there.  I already said hi though.\"");
   }
 goto main_menu;


}

void OnKill()
{
}
