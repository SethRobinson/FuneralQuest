#include "COptions.h"

OptionArray a_option;
OptionArray a_upgrade;
RisksArray a_risk;

void load_risks_from_file()
{
   a_risk.RemoveAll(); //reset it
   CRisks rtemp;

   //LogMsg("Loading risk texts from risks.txt.");
   CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   cls_parse.open("risks.txt");
  
	//ready to read actual stuff!
	char *p_buffer;
 	char st_crap[255];

		while(cls_parse.get_next_line(&p_buffer))
	{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "start_message") == 0)
		{
		   //clear vars/set defaults
		   rtemp.cst_start_message = cls_parse.get_word(2);
		   
		} else

		if (strcmp(st_crap, "start_url") == 0)
		{
		   rtemp.cst_start_url = cls_parse.get_word(2);
		} else
		if (strcmp(st_crap, "start_url_bg") == 0)
		{
		   rtemp.cst_start_bg = cls_parse.get_word(2);
		} else

		if (strcmp(st_crap, "normal") == 0)
		{
		   rtemp.cst_accept = cls_parse.get_word(2);
		} else
		if (strcmp(st_crap, "risk") == 0)
		{
		   rtemp.cst_risk = cls_parse.get_word(2);
		} else
     	if (strcmp(st_crap, "fail_message") == 0)
		{
		   rtemp.cst_fail_message = cls_parse.get_word(2);
		} else

     	if (strcmp(st_crap, "fail_url") == 0)
		{
		   rtemp.cst_fail_url = cls_parse.get_word(2);
		} else

   	    if (strcmp(st_crap, "fail_url_bg") == 0)
		{
		   rtemp.cst_fail_bg = cls_parse.get_word(2);
		} else
     	if (strcmp(st_crap, "success_message") == 0)
		{
		   rtemp.cst_success_message = cls_parse.get_word(2);
		} else

     	if (strcmp(st_crap, "success_url") == 0)
		{
		   rtemp.cst_success_url = cls_parse.get_word(2);
		} else

   	    if (strcmp(st_crap, "success_url_bg") == 0)
		{
		   rtemp.cst_success_bg = cls_parse.get_word(2);
		} else

		if (strcmp(st_crap, "normal_message") == 0)
		{
		   rtemp.cst_accept_message = cls_parse.get_word(2);
		} else

     	if (strcmp(st_crap, "normal_url") == 0)
		{
		   rtemp.cst_accept_url = cls_parse.get_word(2);
		} else

   	    if (strcmp(st_crap, "normal_url_bg") == 0)
		{
		   rtemp.cst_accept_bg = cls_parse.get_word(2);
		} else
	    if (strcmp(st_crap, "add_risk") == 0)
		{
		  //add this risk
		 a_risk.Add(rtemp);
		} else
		{

			  //nothing we know
		
		}



	}
	


}



void setup_risks()
{

}

void load_hard_sell_from_file()
{
   a_upgrade.RemoveAll(); //reset it
   COptionItem rtemp;

   //LogMsg("Loading risk texts from risks.txt.");
   CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   cls_parse.open("hard_sell.txt");
   
	//ready to read actual stuff!
	char *p_buffer;
 	char st_crap[255];

		while(cls_parse.get_next_line(&p_buffer))
	{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "reference") == 0)
		{
		   //clear vars/set defaults
		   rtemp.cst_a = cls_parse.get_word(2);
		   
		} else

		if (strcmp(st_crap, "name") == 0)
		{
		   rtemp.cst_name = cls_parse.get_word(2);
		} else
	
   	    if (strcmp(st_crap, "price") == 0)
		{
		   rtemp.i_price = atol(cls_parse.get_word(2));
		} else
	    if (strcmp(st_crap, "add_item") == 0)
		{
			  //add this risk
		 a_upgrade.Add(rtemp);
		} else
		{

			  //nothing we know
		
		}

	}

}



void load_soft_sell_from_file()
{
  a_option.RemoveAll();

  COptionItem rtemp;

   //LogMsg("Loading risk texts from risks.txt.");
   CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   cls_parse.open("soft_sell.txt");
   
	//ready to read actual stuff!
	char *p_buffer;
 	char st_crap[255];

		while(cls_parse.get_next_line(&p_buffer))
	{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "reference") == 0)
		{
		   //clear vars/set defaults
		   rtemp.cst_a = cls_parse.get_word(2);
		   
		} else

		if (strcmp(st_crap, "name") == 0)
		{
		   rtemp.cst_name = cls_parse.get_word(2);
		} else
	
   	    if (strcmp(st_crap, "price") == 0)
		{
		   rtemp.i_price = atol(cls_parse.get_word(2));
		} else
	    if (strcmp(st_crap, "add_item") == 0)
		{
			//add this risk
		 a_option.Add(rtemp);
		} else
		{
		  //nothing we know
		}

	}

}


void init_options()
{

  	 load_risks_from_file();
	 load_soft_sell_from_file();
	 load_hard_sell_from_file();
}