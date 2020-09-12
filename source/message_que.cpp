#include "stdafx.h"
#include "message_que.h"
#include "funeral_file.h"
#include "funeral_util.h"


class_que::class_que()
{
  //init everything
	this->first = NULL;
}

class_que::~class_que()
{
  //delete everything
	delete_list();
}

void class_que::delete_list()
{
	if (first)
	delete first; //this will kill the list and it's children, using
	//recursion behavior in the class_que_packet class.
	first = NULL;
}

bool class_que::add_to_end(char message[], int c_type, int c_importance,
					  int i_from, int i_sub_type, int i_var_mod, int i_expire)
{
	//add a new packet to the end of the list
	
	class_que_packet *que_temp, *que_new;
	que_temp = first;

	//create new packet, fill in values
	que_new = new class_que_packet; //create it
	if (!que_new) return false; //couldn't initialize
	que_new->q.c_importance = c_importance;
	que_new->q.c_type = c_type;
	que_new->q.i_from = i_from;
	que_new->q.i_sub_type = i_sub_type;
	que_new->q.i_value = i_var_mod;
	que_new->q.i_expire = i_expire;
	if (message)
	que_new->set_text(message);
	//find end of list.  Could keep track of end later for speed, would
	//be easy to add.

	//test for special cases
	if (!que_temp)
	{
	  	  //add first one
	 first = que_new;
	 //LogMsg("Added first one.");
	 return true;
	}

	while (que_temp->next)
	que_temp = que_temp->next;

	//ok, we're here, add the new guy.

	que_temp->next = que_new;
//	LogMsg("Added one.");

  return true;
}

//add directy from packet data

bool class_que::add_to_end(const class_que_packet *que_to_add)
{
	//add a new packet to the end of the list
	
	class_que_packet *que_temp, *que_new;
	que_temp = first;

	//create new packet, fill in values
	que_new = new class_que_packet; //create it
	if (!que_new) return false; //couldn't initialize
	que_new->q.c_importance = que_to_add->q.c_importance;
	que_new->q.c_type = que_to_add->q.c_type;
	que_new->q.i_from = que_to_add->q.i_from;
	que_new->q.i_sub_type = que_to_add->q.i_sub_type;
	que_new->q.i_value = que_to_add->q.i_value;
	que_new->q.i_expire = que_to_add->q.i_expire;
	if (que_to_add->text)
	que_new->set_text(que_to_add->text);
	//find end of list.  Could keep track of end later for speed, would
	//be easy to add.

	//test for special cases
	if (!que_temp)
	{
	  	  //add first one
	 first = que_new;
	 //LogMsg("Added first one.");
	 return true;
	}

	while (que_temp->next)
	que_temp = que_temp->next;

	//ok, we're here, add the new guy.

	que_temp->next = que_new;
//	LogMsg("Added one.");

  return true;
}


bool class_que::get_reference_to_first(class_que_packet **que_temp)
{
 if (!first) 
 {
  	 *que_temp = NULL;
	 return false;
 }
 //return reference to the first item in the stack
 *que_temp = first;
 return true; 														  
}

bool class_que::delete_first()
{
   //delete the first item in the stack
	if (!first) return false; //can't delete anything, nothing exists
	
	class_que_packet *que_temp = first;
	first = first->next;

	//now actually delete the old first
	que_temp->next = NULL; //don't let the recursive deletion chain kick in
	delete que_temp;

	return true;
}

//save and empty ourself
bool class_que::save_to_hd(int i_player)
{
	if (!first) return true; //can't delete anything, nothing exists

  CString cst_fname;
   cst_fname.Format("%s\\%d.dat",st_mail_path,i_player);
   
   //should never need to delete it actually
   //if (exist_cst(cst_fname)) unlink((LPCSTR)cst_fname); //delete

   //open file for writing
   FILE *fp = fopen((LPCSTR)cst_fname, "wb");

   if (!fp)
   {
	   log_error("Error, can't open %s for writing",cst_fname);
	   return false;
   }
   //walk through the packets
   	class_que_packet *que_temp;
	CString cst_temp;

	while (get_reference_to_first(&que_temp))
	{
		if (que_temp->q.c_importance == ::c_que_importance_high)
		{
		int i_result = 0;
		//a packet in que exists, save it
	   	int i_packet_size = sizeof(struct_que);
		fwrite(&i_packet_size, sizeof(i_packet_size),1,fp);
		fwrite(&que_temp->q, i_packet_size, 1,fp);
		//now let's write out our text and data, after the packet
		if (que_temp->q.i_text_size != 0)
		fwrite(que_temp->text, que_temp->q.i_text_size,1,fp);
		if (que_temp->q.i_data_size != 0)
		fwrite(que_temp->p_data, que_temp->q.i_data_size,1,fp);
		//packet handled, delete it forever
		}
		delete_first();
	}

   fclose(fp);

  return true;
}

//load's mail data and populates the list
//returns false on error
bool class_que::load_from_hd(int i_player)
{
   CString cst_fname;
   cst_fname.Format("%s\\%d.dat",st_mail_path,i_player);

   //should never need to delete it actually
   //if (exist_cst(cst_fname)) unlink((LPCSTR)cst_fname); //delete

   //open file for writing
   FILE *fp = fopen((LPCSTR)cst_fname, "rb");

   if (!fp)
   {
	   //mail data for this player does not exist, so we won't load it
	   return true;
   }
   //walk through the packets
   	class_que_packet *que_temp, *p_new;

	//first load size of packet
	int i_packet_size = 0;

	while ( (fread(&i_packet_size, sizeof(i_packet_size),1,fp)) != 0)
	{
	  //packet exists
//		LogMsg("Reading packet");
		//init memory for new packet
		p_new = new class_que_packet(); //this sets defaults too

		fread(&p_new->q, i_packet_size,1,fp);  //load over our packet
		
		if (p_new->q.i_text_size != 0)
		{
//			LogMsg("Initted %d bytes for text..",p_new->q.i_text_size);
			p_new->text = new char[p_new->q.i_text_size];	 
		   fread(p_new->text, p_new->q.i_text_size,1,fp);
		} //if i_text_size is 0, then text would already be NULL, we don't need to set it
		
		if (p_new->q.i_data_size != 0)
		{
			p_new->p_data = new byte[p_new->q.i_data_size];
			fread(p_new->p_data, p_new->q.i_data_size,1,fp);
		}
		p_new->next = NULL;
//		LogMsg("Loaded msg: %s",p_new->text);
		//actually add the packet
	if (!first)
	{
	  	  //add first one
	 first = p_new;
	 //LogMsg("Added first one.");
	 
	} else
	{
	que_temp = first;
	while (que_temp->next)
	que_temp = que_temp->next;
	//ok, we're here, add the new guy.
	que_temp->next = p_new;
	}
	}

   fclose(fp);

   //delete the file we loaded to stop any possible cheating from a crash
     unlink((LPCSTR)cst_fname); //delete

 

  return true;


}
