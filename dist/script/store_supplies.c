//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
 String st_temp;
 int i_temp;
 int i_lockpick;
 String st_packet;
 int i_lockpick_price = 1; 
 int i_gold_tags;
}


void DisplayLockpickInfo()
{
    menu: 
     st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\lockpick.swf");
    
      st_packet = AddCustom(st_packet, "st_main", "Have you ever accidently locked yourself out of your parlor?  A `wLockpick`y can 'let' you back in!  These are `w$i_lockpick_price$`y gold tag each.  (You currently have `w$i_lockpick$`y lockpick(s))\n\n`wEffect: `yWill instantly open any parlor front door.  One time use.");
      st_packet = AddButton(st_packet, 1, "Back", 1); //Don't buy it
      st_packet = AddButton(st_packet, 2, "Buy one", 2); //Buy it!
      SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
      st_packet = ""; //clear it for a new packet 

      if (i_my_result == 1)
       {
         //go back
        return;
       }

      if (i_my_result == 2)
       {
        //buy it!

      
      
       //can they actually afford this purchase?
   
      i_gold_tags = GetTags(i_my_id, C_GOLD_TAG);
      if (i_gold_tags < i_lockpick_price)
      {
         //can't afford it
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"You don't have a single gold tag at the moment?  That's too bad.  These are great little lockpicks.\"");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
      }

      //let's actually give them the alarm and remove their gold tags.
      ModCustomById(i_my_id, C_CUSTOM_LOCKPICK, 1); //add one
      i_lockpick += 1;
      
      i_gold_tags -= i_lockpick_price;
      SetTags(i_my_id, C_GOLD_TAG, i_gold_tags);

      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\money_bag_2.swf");
      st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
      st_packet = AddCustom(st_packet, "st_main", "`yYou purchase a `wLockpick`y for `w$i_lockpick_price$`y gold tag.");
      st_packet = AddButton(st_packet, 1, "Continue", 1);
      SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
      st_packet = ""; //clear it for a new packet 
       goto menu;    
  
       }

}

void Main()
{
  //this is the 'supplies' sub-menu in the store.  The store is a special case because I didn't want to
  //redo purchasing transportion options in script, so I attached individual buttons to script.
 main_menu:

 st_packet = AddCustom(st_packet, "st_generic_info", "You are in the supplies section.");
 st_packet = AddCustom(st_packet, "st_generic_status", "");
  
String st_items = "`wElijah: `y\"We carry a wide assortment of useful items.\"\n\n";


i_lockpick =  GetCustomById(i_my_id, C_CUSTOM_LOCKPICK); //how many lockpicks the player has now

 //st_packet = AddCustom(st_packet, "st_url_bg", "flash\\background\\background_church.swf");
st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");

st_items += "`w1`y - Lockpick - Cost: `w$i_lockpick_price$`y gold tag\n";

st_packet = AddCustom(st_packet, "st_main", st_items);

 //add options
 st_packet = AddButtonStore(st_packet, 1, "Back"); //the last number is what gets put in i_last_result.
 st_packet = AddButton(st_packet, 2, "View #1", 1);
 //send the packet
 SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
 //LogMsg("Got reply for $i_my_id$. Result code is $i_my_result$."); 

 st_packet = ""; //clear it for a new packet 
 
if (i_my_result == 1)
   {
   DisplayLockPickInfo();
   }

 goto main_menu;
}

void OnKill()
{
}
