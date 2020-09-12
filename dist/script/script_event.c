//note:  i_my_id will already exist and contain the player id of the player this script is attached to.

void OnCreate()
{
}

void Main()
{
  //this gets called by the engine a random amount when they wait for customers.  The frequency can be specified in the
//server game option dialog.

 //pick a random script event

 int i_random = RandomRange(1,2); 

 if (i_random == 1)
  {
    //LogMsg("Running arson..");
    RunScriptNoReturn("script\\events\\arson.c"); //runs this script and attaches the "_my_" vars to it.
    //nothing will get run here after this, this script dies. 
  }

 if (i_random == 2)
  {
    //play finds a tag
    RunScriptNoReturn("script\\events\\bonus_tag.c"); //runs this script and attaches the "_my_" vars to it.
    //nothing will get run here after this, this script dies. 
  }

}

void OnKill()
{
}
