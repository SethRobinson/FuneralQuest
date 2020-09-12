#pragma once
#include "all.h"

//linked list stack class for all player messages and such.
const int c_que_importance_normal=0; //normal, not guruanteed to be delivered
const int c_que_importance_high=1; //will be delivered no matter what

//message types
const int c_que_type_text = 0; //a text message to show the user
const int c_que_type_var_mod = 1; //a variable is going to be modified
const int c_que_type_var_set = 2; //a variable is going to be set
const int c_que_type_command = 3; //command to be sent to flash, like show url
const int c_que_type_small_command = 4; //like above, but won't pause message processing
const int c_que_type_chat = 5; //show up in chat only
const int c_que_type_simple_text = 6; //shows up in chat room or anywhere
const int c_que_type_add_to_log = 7; //writes to local text file


const int c_que_sub_type_nothing = 0;
const int c_que_sub_type_luck = 1;
const int c_que_sub_type_bp = 2;
const int c_que_sub_type_turns = 3;

const int c_que_no_expire = -1;

const int c_que_nobody = -1;

//the part I save
struct struct_que
{
 int c_type; //define what kind of structure this is
 int c_importance; //priority
 int i_from; //misc thing to keep track of who sent this
 int i_text_size; //how big the text buffer is
 int i_data_size; //how big the data info is
 int i_value; //misc value
 int i_sub_type; //further description of packet
 int i_expire;
};

//each message contains this
class class_que_packet
{
friend class class_que;

public:
 struct_que q;
	
 char *text;
 void *p_data;
 class_que_packet *next;

public:
 
	void delete_text()
	{
		if (text) delete text;
		 text = NULL;
		 q.i_text_size = 0;
	}

	void delete_data()
	{
		if (p_data) delete p_data;
		 p_data = NULL;
		 q.i_data_size = 0;
	}
	
	void set_text(char st_text[])
	{
	   //allocate memory and save this text, first delete any text if it existed..
		delete_text();
		q.i_text_size = strlen(st_text)+1;
		text = new char[q.i_text_size];
		strcpy(text, st_text);
		//remember how much text it was...
	}
	
  class_que_packet()
 {
   text = NULL;
   next = NULL;
   q.i_text_size = 0;
   q.i_data_size = 0;
   p_data = NULL;
 }


 ~class_que_packet()
 {
  	 //free memory used
	  delete_text();
	  delete_data();
	 //free our children, will recursively free everybody
	 if (next) delete next;
//	 log_msg("Released one.");
 }


};

//class to keep track of the link list, pop and push things and such
class class_que
{
public:
    class_que(); //construct
    ~class_que(); //deconstruct
    bool class_que::add_to_end(char message[], int = c_que_type_text, int = c_que_importance_normal
        , int = c_que_nobody, int = c_que_sub_type_nothing, int = 0, int = c_que_no_expire);
    bool class_que::add_to_end(const class_que_packet *que_to_add);

    void delete_list();
    bool get_reference_to_first(class_que_packet **que_temp);
    bool delete_first();
    bool save_to_hd(int i_player);
    bool load_from_hd(int i_player);

private:
    class_que_packet *first; //every linked list needs a first one
    static int i_count;
};

