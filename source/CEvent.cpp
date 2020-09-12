#include "CEvent.h"
#include "funeral_file.h"

//this kind of orchestrates loading and parsing event information files and then
//making it easy to grab the information.  Relies on Seth's CTextParse class.

//Seth Robinson 2001

CEvent::CEvent()
{
   //set defaults
   strcpy( st_message, "No message.");
   strcpy(st_pic_file_name, "flash/places/embalming-room.swf");
   st_title[0] = 0;
   i_mod_turns_low = 0;
   i_mod_bp_low = 0;
   i_mod_luck_low = 0;
   i_mod_turns_high = 0;
   i_mod_bp_high = 0;
   i_mod_luck_high = 0;
   st_give_random[0] = 0;
   st_add_log[0] = 0;
}

CEvent::~CEvent()
{


}

bool CEvent::move_to_range_and_message(CTextParse *p_parse, int i_range, int i_message)
{
   	char st_crap[255];
	int i_amount_in_range = 0;
	bool b_in_range = false;	
	char *p_buffer;
	while(p_parse->get_next_line(&p_buffer))
	{
	 //	Msg(p_buffer);
		strcpy(st_crap, p_parse->get_word(1));
		if (strcmp(st_crap, "range") == 0)
		{
		   if (b_in_range == true)
		   {
			 //what the.. we must have left our range.
 	        return false;
		   }
			
		  //found a range marker 
		  //is this in our range?
	      int i_low = atoi(p_parse->get_word(2));
		  int i_high = atoi(p_parse->get_word(3));
		 // Msg("Found range of %d and %d",i_low, i_high);
		  if (in_range(i_range, i_low, i_high))
		  {
		    //this is our range!
			b_in_range = true;
		  }
		} else
			if (strcmp(st_crap, "start_entry") == 0)
			{
			  if (b_in_range) i_amount_in_range++;
			  if (i_amount_in_range == i_message)
			  {
			    //this is where we want to be, already to read the actual message.
				  return true;
			  }
			  
			
			}
	}

	return false;
}


int CEvent::get_random_count_for_range(CTextParse *p_parse, int i_range)
{
   	char st_crap[255];
	int i_amount_in_range = 0;
	bool b_in_range = false;	
	char *p_buffer;
	while(p_parse->get_next_line(&p_buffer))
	{
	 //	Msg(p_buffer);
		strcpy(st_crap, p_parse->get_word(1));
		if (strcmp(st_crap, "range") == 0)
		{
		   if (b_in_range == true)
		   {
			 //what the.. we must have left our range.
 	        return i_amount_in_range;
		   }
			
		  //found a range marker 
		  //is this in our range?
	      int i_low = atoi(p_parse->get_word(2));
		  int i_high = atoi(p_parse->get_word(3));
		 // Msg("Found range of %d and %d",i_low, i_high);
		  if (in_range(i_range, i_low, i_high))
		  {
		    //this is our range!
			b_in_range = true;
		  }
		} else
			if (strcmp(st_crap, "start_entry") == 0)
			{
			  if (b_in_range) i_amount_in_range++;
			}
	}

	return i_amount_in_range;
}

bool CEvent::compute_event(int i_range, char fname[255])
{
   CTextParse cls_parse;
  // cls_parse.set_path(st_data_path);
   cls_parse.open(fname);

   //first let's get the message
   //how many choices do we have?
	int i_count = get_random_count_for_range(&cls_parse,i_range);
  
	if (i_count == 0)
	{
	 log_error("Error, no messages exist in %s in this range!", fname);
	 return false;
	}

  
	//open file again - class automatically handles closing it and such, not problem there
	cls_parse.open(fname);
	//pick a random message and move to that place to read it
	if (!move_to_range_and_message(&cls_parse, i_range, random(i_count)+1))
	{
	  return false;
	}

	//ready to read actual stuff!
	char *p_buffer;
 	char st_crap[255];

		while(cls_parse.get_next_line(&p_buffer))
	{
	 	strcpy(st_crap, cls_parse.get_word(1));
	
		if (strcmp(st_crap, "start_entry") == 0)
		{
		   	//finished reading
			return true;
		} else

		if (strcmp(st_crap, "message") == 0)
		{
		   	strcpy(st_message, cls_parse.get_word(2));
		} else
	if (strcmp(st_crap, "give_random") == 0)
		{
		   	strcpy(st_give_random, cls_parse.get_word(2));
		} else

		if (strcmp(st_crap, "add_log") == 0)
		{
		   	strcpy(st_add_log, cls_parse.get_word(2));
		} else

	
		if (strcmp(st_crap, "title") == 0)
		{
		   	strcpy(st_title, cls_parse.get_word(2));
		} else

				if (strcmp(st_crap, "set_pic") == 0)
		{
			strcpy(this->st_pic_file_name, cls_parse.get_word(2));
		} else

		if (strcmp(st_crap, "mod_bp") == 0)
		{
			i_mod_bp_low = atoi(cls_parse.get_word(2));
			i_mod_bp_high = atoi(cls_parse.get_word(3));
		} else
		if (strcmp(st_crap, "mod_luck") == 0)
		{
			i_mod_luck_low = atoi(cls_parse.get_word(2));
			i_mod_luck_high = atoi(cls_parse.get_word(3));
		} else
		if (strcmp(st_crap, "mod_turns") == 0)
		{
			i_mod_turns_low = atoi(cls_parse.get_word(2));
			i_mod_turns_high = atoi(cls_parse.get_word(3));
		} else



			{

			  //nothing we know
			}



	}
	
	
	
	//processed ok
	return true;
}