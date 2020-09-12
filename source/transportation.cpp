#include "transportation.h"

TransportArray a_transport;


CString get_transportation(int i_play)
{
 return a_transport[p_play[i_play].i_transportation_type].cst_name;

}


void load_transport_from_file()
{
    //clear all transportation first
    a_transport.RemoveAll();


 	CTransports temp;
  	//TRANSPORTATION DB ***********

	temp.cst_name = "Bicycle";
	temp.i_price = 4;
	temp.i_bonus_turns = 0;
	temp.cst_description.Format("(you start with this, you can trade it in for 2)");
	a_transport.Add(temp);


	temp.cst_name = "Budget Hearse";
	temp.i_price = 12;
	temp.i_bonus_turns = 2;
	temp.cst_description.Format("The ever humble Budget Hearse can fulfill all your transportation needs.  Children may laugh and point somewhat, but it sure beats a bike.");
	temp.cst_url = "flash\\hearse\\hearse1.swf";
	a_transport.Add(temp);
	  

	temp.cst_name = "Medalist";
	temp.i_price = 30;
	temp.i_bonus_turns = 4;
	temp.cst_url = "flash\\hearse\\hearse2.swf";
	temp.cst_description.Format("The new Medalist very simply is the greatest value available today in the funeral profession. Flowing body lines and a graceful roof make this coach a winner.");
	a_transport.Add(temp);

  
	  
	temp.cst_name = "Statesman";
    temp.i_price = 50;
	temp.i_bonus_turns = 7;
	temp.cst_url = "flash\\hearse\\hearse3.swf";
	temp.cst_description.Format("The paradigm of contemporary elegance and styling, the Statesman funeral coach gracefully integrates form and function with dramatic design.");
	a_transport.Add(temp);

	temp.cst_name = "Crown Sovereign";
	temp.i_price = 80;
	temp.cst_url = "flash\\hearse\\hearse4.swf";
	temp.i_bonus_turns = 10;
	temp.cst_description.Format("Beautifully proportioned, this new funeral coach reflects the prestige and styling of lasting beauty, an investment in your business future that is as distinguished and distinctive as the service you provide.");
	a_transport.Add(temp);

}