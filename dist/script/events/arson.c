//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
  //declare some script-wide vars.  A var has to declared ABOVE where you use it, so we do it here for the whole script.
  int i_tag_cost = 6; //how much it costs the player to hire them for arson
  String st_packet;
  String st_target_name;
  String st_target_home_name;
  int i_temp;
}

void HandleFireAlarm()
{
  AddToNews("`7EARLY WARNING SAVES PARLOR\n`^A small fire was quickly put out at local business $st_target_home_name$ before any damage was done. \"It was a close call,\" said a relieved $st_target_name$.\n\n");
  
  //if someone is in the chat room, let them see this.
  BroadCastTV("You see some breaking news.  It looks like $st_target_name$'s parlor was saved by a smoke detector."; 
  
  //now we need to mail the actual guy and let him know what happened.
  MailText(i_my_result, "\n`yA mysterious fire was started on your property.  Thanks to your smoke detector the fire was put out before any serious damage was done.  The police chief awards you `w3`y gold tags!\n");
  MailPicture(i_my_result, "flash/stuff/fire_alarm.swf");
  MailContinueButton(i_my_result); //so they have to click to continue

  //let's actually give the target some tags
  ModTags(i_my_result, C_GOLD_TAG, 3);

  i_temp = i_tag_cost;
  i_tag_cost *= -1; //make it negative
  ModTags(i_my_id, C_GOLD_TAG, i_tag_cost); 

  st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
   //let the current guy know what happened
  st_packet = AddCustom(st_packet, "st_main", "`yYou watch from your window.\n\n`4The fire is put out before it can do any serious damage!\n\n`yDamn smoke detectors!");
  st_packet = AddCustom(st_packet, "st_url", "flash\\blank.swf");
  st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
  st_packet = AddButtonHome(st_packet, 1, "Continue"); //the last number is what gets put in i_last_result.

 
  
  SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
  st_packet = ""; //a good habit is to clear this after any send packet  
  
  //since the only option we're giving them is to go to the parlor, nothing will get run in this script after this
  
  //remove the gold tags
 
}


void Main()
{
 LogMsg("Running arson event for $st_my_name$.");

// st_packet = AddCustom(st_packet, "st_generic_info", "Father Smyth greets you warmly, a little too warmly.  You wonder if he has anything on under that robe.");
// st_packet = AddCustom(st_packet, "st_generic_status", "");
 st_packet = AddCustom(st_packet, "st_main", "`wThe Twins: `y\"We like to play with fire.  We'll discreetly pay a 'visit' to any funeral parlor in town for only $i_tag_cost$ gold tags.\"\n\nA fire will destroy 5 percent of the parlors gravestones and burn all wood tags. If the parlor has a fire alarm, there is only a 20 percent chance of success.");
 st_packet = AddCustom(st_packet, "st_url_bg", "flash\\background\\background_5.swf");
 st_packet = AddCustom(st_packet, "st_url", "flash\\people\\m_old_3.swf");
// st_packet = AddCustom(st_packet, "st_object", "i_generic"); //used to create the initial screen type

 main_menu:
 //add options
 st_packet = AddButton(st_packet, 2, "Hire Twins", 1); //the last number is what gets put in i_last_result.
 st_packet = AddButtonHome(st_packet, 1, "No Thanks"); //the last number is what gets put in i_last_result.
// st_packet = AddButtonScript(st_packet, 1, "Say hi", "script\\public\\crap.c"); //run another script through button

 //send the packet
  st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
  SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
 st_packet = ""; //clear it for a new packet 
 
if (i_my_result == 1)
   {
     //do they have enough gold tags?
    int i_gold_tags = GetTags(i_my_id, C_GOLD_TAG); //defined in init.c
    if (i_gold_tags < i_tag_cost)
        {
        //can't afford it
         st_packet = AddCustom(st_packet, "st_main", "`wThe Twins: `y\"You don't have enough gold tags, fool!  We shall come again...\"");
         st_packet = AddButtonHome(st_packet, 1, "Continue"); //the last number is what gets put in i_last_result.
         //send the packet
        st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
        SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
         //since the only option we're giving them is to go to the parlor, nothing will get run in this script after this
        }
    }
 
 //if we got here they want to do it.  Let's ask them which parlor to hit.
  st_packet = AddCustom(st_packet, "st_message", "Who would you like the Twins to pay a visit to?");
  st_packet = AddCustom(st_packet, "st_object", "i_getname"); //used to retrieve a name
  st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
  SendPacketAndWaitOverlay(st_packet); //overlay the existing screen.  Only for pop up message type of things
  st_packet = ""; //clear it for a new packet 
 
  //did they select a valid player?

  if (i_my_result == -1)
  {
    //they didn't select anyone
    st_packet = AddCustom(st_packet, "st_main", "`yThe twins look confused.  Better try again.");
    st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
    goto main_menu;
  }
 
 if (i_my_result == i_my_id)
  {
    //they select themselves?
      st_packet = AddCustom(st_packet, "st_main", "`yEven as a joke you don't think you should suggest the twins burn down your own building.  Try again.");
      st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
      goto main_menu;
  }

  //it's a valid player.  Let's make sure he isn't on right now

  int i_on_now = IsPlayerOnNow(i_my_result);
  st_target_name = GetNameFromID(i_my_result);
  st_target_home_name = GetHomeNameFromID(i_my_result);

  if (i_on_now == 1)
    {
     //can't burn someone if they are currently online
    st_packet = AddCustom(st_packet, "st_main", "`yThe twins shake their heads vigorously.  Apparently they saw $st_target_name$ working in the yard.");
    st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
    goto main_menu;
    }

  //let's perform the entire thing now

  //do they have a fire alarm installed?

  int i_alarm = GetCustomById(i_my_result, C_CUSTOM_FIRE_ALARM); //1 is yes

  if (i_alarm == 1)
   {
     //they have a fire alarm installed so there is a good chance they it will catch this fire.
     int i_caught_fire_in_time = RandomRange(1, 10);
     if (i_caught_fire_in_time > 2)
     {
       //fire alarm stopped it.  Let's show what happened, write email to the guy and add something to the daily log.
       HandleFireAlarm();
       return; //all done here
     }
   }

  int i_target_id = i_my_result; //remember this for later, even if i_result is overwritten

  float f_burned_graves = GetTotalBurials(i_target_id);
  LogMsg("Changin graves ($i_burned_graves$)");
  f_burned_graves *= 0.05; //reduce it to five percent
  LogMsg("Changed to ($f_burned_graves$)");
  int i_burned_graves = f_burned_graves;
  int i_tags_lost = GetTags(i_target_id, C_WOOD_TAG);

//let's actually take the crap away now
  i_temp = i_tag_cost;
  i_tag_cost *= -1; //make it negative
  ModTags(i_my_id, C_GOLD_TAG,  i_tag_cost); 

  int i_safe = GetCustomById(i_my_result, C_CUSTOM_SAFE); //set it to 1, this means they own it.

  if (i_safe == 0)
  {
  st_packet = AddCustom(st_packet, "st_main", "`yYou watch from your window.\n\n`wA fire engulfs $st_target_home_name$!\n\n`y$st_target_name$ loses `w$i_burned_graves$`y gravestone(s) and `w$i_tags_lost$`y wood tag(s).");
  }

  if (i_safe > 0)
   {
     st_packet = AddCustom(st_packet, "st_main", "`yYou watch from your window.\n\n`wA fire engulfs $st_target_home_name$!\n\n`y$st_target_name$ loses `w$i_burned_graves$`y gravestone(s).  You see a shiny safe through a window.");
   }
 
  st_packet = AddUpdateTags(st_packet); //visually update the tag count on the flash client
  st_packet = AddCustom(st_packet, "st_url", "flash\\places\\fire.swf");
  st_packet = AddCustom(st_packet, "b_kill_buttons", "1"); //cause the main menu  option buttons to fade out
  st_packet = AddButtonHome(st_packet, 1, "Continue"); //the last number is what gets put in i_last_result.

  int i_rand = RandomRange(1, 2);

  if (i_rand == 1)
  {
    AddToNews("`7PARLOR DAMAGED BY FIRE, ARSON SUSPECTED\n`^A small fire was not noticed until too late at $st_target_home_name$ today.  \"This isn't happening!\" said $st_target_name$, still in shock.\n\n");
  }

  if (i_rand == 2)
  {
    AddToNews("`7PARLOR GETS BURNED\n`^A small fire was not noticed until too late at $st_target_home_name$ today.  \"Faulty wiring my ass, someone is gonna pay for this!\" said a distraught $st_target_name$.\n\n");
  }
 
  //if someone is in the chat room, let them see this.
  BroadCastTV("You see some breaking news.  It looks like $st_target_name$'s parlor caught on fire."; 

     
  
   //send email to the the sap who got burned
  if (i_safe > 0)
  {
   //they owned a safe, only lost tags
   MailText(i_my_result, "\n`yA mysterious fire was started on your property.  You lost `4$i_burned_graves$`y gravestone(s) but your `wSafe`y protected all your tags.\n");
  }

  if (i_safe == 0)
  {
   //they got their tags burned too.
  MailText(i_my_result, "\n`yA mysterious fire was started on your property.  You lost `4$i_burned_graves$`y gravestone(s) and `4$i_tags_lost$`y wood tag(s).\n");
  //remove the enemies stuff
  SetTags(i_target_id, C_WOOD_TAG, 0); //all his wood tags got burned!
  }
 
  MailPicture(i_my_result, "flash/stuff/fire_alarm.swf");
  MailContinueButton(i_my_result); //so they have to click to continue

  
  
  i_temp = i_burned_graves;
  i_burned_graves *= -1; //make it negative
  ModTotalBurials(i_target_id, i_burned_graves); //bye bye, graves


  SendPacketAndWait(st_packet); //this sends the packet and will pause until a reply is received.
  st_packet = ""; //a good habit is to clear this after any send packet  

  goto main_menu;
}

void OnKill()
{
}
