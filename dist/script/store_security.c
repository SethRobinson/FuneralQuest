//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
  //some script wide globals
  int i_smoke_price = 8; //price of smoke detector in gold tags
  int i_safe_price = 6;
  int i_padlock_price = 6;

  String st_packet;
  int i_smoke;
  int i_padlock;
  int i_safe;
  int i_temp;
  String st_temp;
  int i_gold_tags;
}


void DisplaySmokeDetector()
{
      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\fire_alarm.swf");
    
      st_packet = AddCustom(st_packet, "st_main", "`yA `wSmoke Detector`y will allow you to notice fires and call the fire department much quicker.  It retails here for `w$i_smoke_price$`y gold tags.\n\n`wEffect: `yReduce the risk of a fire getting large enough to cause damage to 20 percent.");
      st_packet = AddButton(st_packet, 1, "Back", 1); //Don't buy it
      st_packet = AddButton(st_packet, 2, "Buy it", 2); //Buy it!
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

       //first, do they already own it?  If so this would be a waste of money.
        if (i_smoke == 1)
       {
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"Your parlor is quite small.  You really don't need two of these things.\"\n\nYou nod in agreement.  Why waste the cash?");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
       }
      
       //can they actually afford this purchase?
   
      i_gold_tags = GetTags(i_my_id, C_GOLD_TAG);
      if (i_gold_tags < i_smoke_price)
      {
         //can't afford it
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"I'm sorry, you seem to be lacking in funds.  Perhaps another time?\"\n\nYou feel like a complete dumbass.  He's right.");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
      }

      //let's actually give them the alarm and remove their gold tags.
      SetCustomById(i_my_id, C_CUSTOM_FIRE_ALARM, 1); //set it to 1, this means they own it.
      
      i_gold_tags -= i_smoke_price;
      SetTags(i_my_id, C_GOLD_TAG, i_gold_tags);

      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\money_bag_2.swf");
      st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
      st_packet = AddCustom(st_packet, "st_main", "`yYou purchase the `wSmoke Detector`y for `w$i_smoke_price$`y gold tags.");
      st_packet = AddButton(st_packet, 1, "Continue", 1);
      SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
      st_packet = ""; //clear it for a new packet 
         return;
       }

}


void DisplayPadlock()
{
     st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\padlock.swf");
    
      st_packet = AddCustom(st_packet, "st_main", "`ySecure your parlor and stop intruders with a new steel laminated `wHeavy Padlock`y.  The shackle is crome plated and hardened to maximize its strength. Only `w$i_padlock_price$`y gold tags.\n\n`wEffect: `yCasual thieves will no longer be able to break into your parlor.");
      st_packet = AddButton(st_packet, 1, "Back", 1); //Don't buy it
      st_packet = AddButton(st_packet, 2, "Buy it", 2); //Buy it!
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

       //first, do they already own it?  If so this would be a waste of money.
        if (i_padlock == 1)
       {
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"Wait until the one you have breaks. Sorry, company policy.\"");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
       }
      
       //can they actually afford this purchase?
   
      i_gold_tags = GetTags(i_my_id, C_GOLD_TAG);
      if (i_gold_tags < i_padlock_price)
      {
         //can't afford it
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"I'm sorry, you seem to be lacking in funds.  Perhaps another time?\"\n\nYou mumble something about leaving the gold tags in your other pants.");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
      }

      //let's actually give them the alarm and remove their gold tags.
      SetCustomById(i_my_id, C_CUSTOM_PADLOCK, 1); //set it to 1, this means they own it.
      SetCustomById(i_my_id, C_CUSTOM_PADLOCK_STRENGTH, 100); //set to max hitpoints
      
      i_gold_tags -= i_padlock_price;
      SetTags(i_my_id, C_GOLD_TAG, i_gold_tags);

      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\money_bag_2.swf");
      st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
      st_packet = AddCustom(st_packet, "st_main", "`yYou purchase the `wHeavy Padlock`y for `w$i_padlock_price$`y gold tags.");
      st_packet = AddButton(st_packet, 1, "Continue", 1);
      SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
      st_packet = ""; //clear it for a new packet 
         return;
       }

}

void DisplaySafe()
{
      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\safe.swf");
    
      st_packet = AddCustom(st_packet, "st_main", "`yOur commercial `wSafe`y is the perfect way to product your valuables.  Features steel castors and automatic re-locking system.  Only `w$i_safe_price$`y gold tags.\n\n`wEffect: `ySales tags cannot be stolen or damaged in a fire.");
      st_packet = AddButton(st_packet, 1, "Back", 1); //Don't buy it
      st_packet = AddButton(st_packet, 2, "Buy it", 2); //Buy it!
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

       //first, do they already own it?  If so this would be a waste of money.
        if (i_safe == 1)
       {
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"You already own a Safe.  You shouldn't need two.\"");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
       }
      
       //can they actually afford this purchase?
   
      i_gold_tags = GetTags(i_my_id, C_GOLD_TAG);
      if (i_gold_tags < i_safe_price)
      {
         //can't afford it
        st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");  
        st_packet = AddCustom(st_packet, "st_main", "`wElija: `y\"I'm sorry, you seem to be lacking in funds.  Perhaps another time?\"\n\nYou nod in agreement.");
        st_packet = AddButton(st_packet, 1, "Continue", 1);
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
        st_packet = ""; //clear it for a new packet 
        return;
      }

      //let's actually give them the alarm and remove their gold tags.
      SetCustomById(i_my_id, C_CUSTOM_SAFE, 1); //set it to 1, this means they own it.
      
      i_gold_tags -= i_safe_price;
      SetTags(i_my_id, C_GOLD_TAG, i_gold_tags);

      st_packet = AddCustom(st_packet, "st_url", "flash\\stuff\\money_bag_2.swf");
      st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
      st_packet = AddCustom(st_packet, "st_main", "`yYou purchase the `wSafe`y for `w$i_safe_price$`y gold tags.");
      st_packet = AddButton(st_packet, 1, "Continue", 1);
      SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
      st_packet = ""; //clear it for a new packet 
         return;
       }

}


void Main()
{
  //this is the 'supplies' sub-menu in the store.  The store is a special case because I didn't want to
  //redo purchasing transportion options in script, so I attached individual buttons to script.
 main_menu:

 st_packet = AddCustom(st_packet, "st_generic_info", "You are in the supplies section.");
 st_packet = AddCustom(st_packet, "st_generic_status", "");
 
 String st_items = "`wElijah: `y\"This town seems accident prone.  Investing in security is a great idea.\"\n\n";

 st_temp = "";
 i_smoke = GetCustomByID(i_my_id, C_CUSTOM_FIRE_ALARM); //1 is yes
 i_padlock = GetCustomByID(i_my_id, C_CUSTOM_PADLOCK);
 i_safe = GetCustomByID(i_my_id, C_CUSTOM_SAFE);
 int i_pad_strength = GetCustomByID(i_my_id, C_CUSTOM_PADLOCK_STRENGTH);
 
 if (i_padlock == 1)
  {
    //they already own this
    st_temp = " (`walready own one at $i_pad_strength$ percent strength`y)";
  }

 st_items += "`w1`y - Heavy Padlock - Cost: `w$i_padlock_price$`y gold tags$st_temp$\n";
st_items = StringExpand(st_items); //so we can reuse st_temp without them all being the same when it's finally expanded 
 st_temp = "";
 if (i_smoke == 1)
  {
    //they already own this
    st_temp = " (`walready own one`y)";
  }

 st_items += "`w2`y - Smoke Detector - Cost: `w$i_smoke_price$`y gold tags$st_temp$\n";
st_items = StringExpand(st_items); //so we can reuse st_temp without them all being the same when it's finally expanded 
st_temp = "";
if (i_safe == 1)
  {
    //they already own this
    st_temp = " (`walready own one`y)";
  }
 st_items += "`w3`y - Safe - Cost: `w$i_safe_price$`y gold tags$st_temp$\n";
 
 st_packet = AddCustom(st_packet, "st_main", st_items);

 //st_packet = AddCustom(st_packet, "st_url_bg", "flash\\background\\background_church.swf");
 st_packet = AddCustom(st_packet, "st_url", "flash\\people\\store_guy.swf");


 //add options
 st_packet = AddButtonStore(st_packet, 1, "Back"); //the last number is what gets put in i_last_result.
 st_packet = AddButton(st_packet, 4, "View #1", 2); //padlock
 st_packet = AddButton(st_packet, 3, "View #2", 3); //smoke detector
 st_packet = AddButton(st_packet, 2, "View #3", 4); //Safe

 //send the packet
 SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
 //LogMsg("Got reply for $i_my_id$. Result code is $i_my_result$."); 
 st_packet = ""; //clear it for a new packet 
 
if (i_my_result == 2)
   {
     //show info on the padlock
      DisplayPadlock();
   }
if (i_my_result == 3)
   {
     //show info on the smoke detector
      DisplaySmokeDetector();
   }

if (i_my_result == 4)
   {
     //show info on the smoke detector
      DisplaySafe();
   }

  goto main_menu;

}

void OnKill()
{
}
