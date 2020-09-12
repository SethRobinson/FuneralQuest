#include "stdafx.h"
#include "online_struct.h"
#include "CScriptEngine.h"
#include "globals.h"
#include "uni.h"

int get_play(LPREQUEST lpReq)
{
 return p_on[lpReq->i_on_index]->i_play;
}


int online_struct::InitScript(const char *st_script_file)
{

                if (m_i_script != C_SCRIPT_NONE)
                {
                    log_error("Script shouldn't be initted yet.");
                    KillActiveScriptWithoutRunningOnKill();
                }


				m_i_script = g_glo.p_script->CreateScript();
                //let's add any script specific variables it needs
                
                //***************** Don't change the order of these vars.  I reference them by index
                //for speed later, so this would screw up everything.
                int i_temp_var = g_glo.p_script->GetScript(m_i_script)->AddVar("i_my_id", ::C_SCRIPT_VAR_INT);
                 g_glo.p_script->GetScript(m_i_script)->GetVar(i_temp_var)->SetInt(this->i_play);
             
                
                i_temp_var = g_glo.p_script->GetScript(m_i_script)->AddVar("i_my_result", ::C_SCRIPT_VAR_INT);
             
                i_temp_var = g_glo.p_script->GetScript(m_i_script)->AddVar("st_my_name", ::C_SCRIPT_VAR_WCHAR);
                //set the name value now, it won't be changing
                g_glo.p_script->GetScript(m_i_script)->GetVar(i_temp_var)->SetVar(C_SCRIPT_VAR_WCHAR, 0,0, 
                    uni(p_play[i_play].st_name).us_data);

                i_temp_var = g_glo.p_script->GetScript(m_i_script)->AddVar("st_my_text", ::C_SCRIPT_VAR_WCHAR);
               
                
                
                
               
                
                int i_result = g_glo.p_script->load_and_compile_script(st_script_file, m_i_script);
                if (i_result != ::C_SCRIPT_NONE)
                {
                    //successfull load
                   	   g_glo.p_script->RunScriptFunction(m_i_script, "ONCREATE");
			           g_glo.p_script->RunScriptFunction(m_i_script, "MAIN");
	 
                } else
                {
                    //not succesfull.  Kill the script we had created earlier.  Error message wil be
                    //handled by the CScriptEngine.
                    g_glo.p_script->UnloadScript(m_i_script);
                    m_i_script = C_SCRIPT_NONE;
                }
              
                
         return m_i_script;

}

online_struct::online_struct()
{
  memset(&status,0, sizeof(status));
  this->b_gave_logon_message = false;
  m_i_script = C_SCRIPT_NONE;
}

void online_struct::KillActiveScript()
{
    if (m_i_script != C_SCRIPT_NONE)
    {
      	g_glo.p_script->RunScriptFunction(m_i_script, "ONKILL");
   	    //now actually remove the script from memoty
		g_glo.p_script->UnloadScript(m_i_script);
		m_i_script = C_SCRIPT_NONE;
    }
}

void online_struct::KillActiveScriptWithoutRunningOnKill()
{
    if (m_i_script != C_SCRIPT_NONE)
    {
   	    //now actually remove the script from memoty
		g_glo.p_script->UnloadScript(m_i_script);
		m_i_script = C_SCRIPT_NONE;
    }
}


void online_struct::init(LPREQUEST lpReq_new)
{
  //We are now keeping track of a new guy online.  
  this->lpReq = lpReq_new; //changes every access, so we need to store his IP too
  
  SOCKADDR_IN * sock;
  sock = (SOCKADDR_IN*) lpReq->lpSockAddr; //Cast as SOCKADDR_IN

  this->ip.S_un.S_addr = sock->sin_addr.S_un.S_addr;
  this->b_logged_on = false;
  this->i_play = -1;
  this->t_last_packet = 0;
  this->i_hits = 0;
  this->i_status = ::C_STATUS_LOGON;
  //Set time logged in at
  time(&this->t_logon);
  time(&this->t_last_packet);

  //Let's add him to our list view
  g_dlg_main->user_list_add(lpReq_new);

  //if a script was active, kill it
  KillActiveScript();
 
}


bool online_struct::add_option(int i_new)
{
  a_options.Add(i_new);
  return true;
}


const CString online_struct::c_man()
{
	//returns a string description based on known information
	//of the customer, example" "girl"
	
	CString cst_man;

	int i_age_range = get_range_from_age(status.i_customer_age);

	if (status.b_customer_sex_male)
  {
   //male
	  if (i_age_range == ::c_age_range_kid) cst_man = "boy";
	  if (i_age_range == ::c_age_range_teen) cst_man = "boy";
	  if (i_age_range == ::c_age_range_adult) cst_man = "man";
	  if (i_age_range == ::c_age_range_old) cst_man = "man";
  } else
  {
   //female
	  if (i_age_range == ::c_age_range_kid) cst_man = "girl";
	  if (i_age_range == ::c_age_range_teen) cst_man = "girl";
	  if (i_age_range == ::c_age_range_adult) cst_man = "woman";
	  if (i_age_range == ::c_age_range_old) cst_man = "woman";
  }

return cst_man;
}
 

int C_MAX_KILL_TEXTS = 12;

const CString online_struct::d_fridge()
{
	//what killed the diseased
	CString cst_fridge = "Error";
 	int i_temp = this->status.e_death_object;
	
	if (i_temp == 0) cst_fridge = "car";
	if (i_temp == 1) cst_fridge = "tractor";
	if (i_temp == 2) cst_fridge = "fridge";
	if (i_temp == 3) cst_fridge = "piano";
	if (i_temp == 4) cst_fridge = "bull";
	if (i_temp == 5) cst_fridge = "elephant";
	if (i_temp == 6) cst_fridge = "truck";
	if (i_temp == 7) cst_fridge = "fork-lift";
	if (i_temp == 8) cst_fridge = "exercise machine";
	if (i_temp == 9) cst_fridge = "power boat";
	if (i_temp == 10) cst_fridge = "computer";
	if (i_temp == 11) cst_fridge = "house";

return cst_fridge;
}

int C_MAX_KILL_METHOD_TEXTS = 8;
const CString online_struct::d_smashed()
{
	//how diseased was killed
	CString cst_smashed = "Error";
 	int i_temp = this->status.e_death_type;
	
	if (i_temp == 0) cst_smashed = "hit by";
	if (i_temp == 1) cst_smashed = "smashed by";
	if (i_temp == 2) cst_smashed = "run over by";
	if (i_temp == 3) cst_smashed = "thrown by";
	if (i_temp == 4) cst_smashed = "crushed by";
	if (i_temp == 5) cst_smashed = "done in by";
	if (i_temp == 6) cst_smashed = "fatally stuck inside";
	if (i_temp == 7) cst_smashed = "smacked a good one by";


return cst_smashed;
}

  

const CString online_struct::c_a_young()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man = "Error";

	int i_age_range = get_range_from_age(status.i_customer_age);

	  if (i_age_range == ::c_age_range_kid) cst_man = "A little";
	  if (i_age_range == ::c_age_range_teen) cst_man = "A teenage";
	  if (i_age_range == ::c_age_range_adult) cst_man = "A middle aged";
	  if (i_age_range == ::c_age_range_old)  cst_man = "An old";
return cst_man;
}


const CString online_struct::c_income()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man = "Error";
	if (status.e_income == ::c_income_poor) cst_man = "Poor";
	if (status.e_income == ::c_income_average) cst_man = "Average";
	if (status.e_income == ::c_income_rich) cst_man = "Rich";
	if (status.e_income == ::c_income_super_rich) cst_man = "Super Rich";
return cst_man;
}


const CString online_struct::c_mood()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	CString cst_man;
	if (this->status.b_loved)
	{
		if (in_range_float(this->status.f_mood,0,1))
			cst_man = "In Shock"; else
			if (in_range_float(this->status.f_mood,1,2))
				cst_man = "Dazed"; else
				if (in_range_float(this->status.f_mood,2,3))
					cst_man = "Confused"; else
					if (in_range_float(this->status.f_mood,3,4))
						cst_man = "Crying"; else
						if (in_range_float(this->status.f_mood,4,5))
							cst_man = "Numb"; else
							if (in_range_float(this->status.f_mood,5,6))
								cst_man = "Disturbed"; else
								if (in_range_float(this->status.f_mood,6,7))
									cst_man = "Frustrated"; else
									if (in_range_float(this->status.f_mood,7,8))
										cst_man = "Irked"; else
										if (in_range_float(this->status.f_mood,8,9))
											cst_man = "Angry"; else
											if (in_range_float(this->status.f_mood,9,100))
												cst_man = "Pissed"; else
												
											{
												CString cst_man = "Error";
											}
	} else
	{
		
		if (in_range_float(this->status.f_mood,0,1))
			cst_man = "Chipper"; else
			if (in_range_float(this->status.f_mood,1,2))
				cst_man = "In a hurry"; else
				if (in_range_float(this->status.f_mood,2,3))
					cst_man = "Natural"; else
					if (in_range_float(this->status.f_mood,3,4))
						cst_man = "Tight Lipped"; else
						if (in_range_float(this->status.f_mood,4,5))
							cst_man = "Nervous"; else
							if (in_range_float(this->status.f_mood,5,6))
								cst_man = "Frustrated"; else
								if (in_range_float(this->status.f_mood,6,7))
									cst_man = "Annoyed"; else
									if (in_range_float(this->status.f_mood,7,8))
										cst_man = "Irked"; else
										if (in_range_float(this->status.f_mood,8,9))
											cst_man = "Angry"; else
											if (in_range_float(this->status.f_mood,9,100))
												cst_man = "Pissed"; else
												
											{
												CString cst_man = "Error";
											}
											
											
											
											
	}
	

return cst_man;
}


const CString online_struct::c_young()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man = "Error";

	int i_age_range = get_range_from_age(status.i_customer_age);

	  if (i_age_range == ::c_age_range_kid) cst_man = "little";
	  if (i_age_range == ::c_age_range_teen) cst_man = "teenage";
	  if (i_age_range == ::c_age_range_adult) cst_man = "middle aged";
	  if (i_age_range == ::c_age_range_old)  cst_man = "old";
return cst_man;
}


const CString online_struct::c_flash_url()
{
	//returns closest match of a flash URL to the customer's appearance
	
	CString cst_dir = "flash/people/";
	CString cst_url = "crap";
	int i_age_range = get_range_from_age(status.i_customer_age);

	if (status.b_customer_sex_male)
	{

	  if (i_age_range == ::c_age_range_kid) cst_url = "m_kid";
	  if (i_age_range == ::c_age_range_teen) 
	  {
		  //pick a random teen graphic
		   int i_random = random(2);
		 
		  if (i_random == 0) cst_url = "m_teen";
		  if (i_random == 1) cst_url = "m_teen_2";
	  }
	  
	  if (i_age_range == ::c_age_range_adult) 
	  {
		  //randomly pick one
		  int i_random = random(4);
		  if (i_random == 0) cst_url = "m_adult";
		  if (i_random == 1) cst_url = "m_adult_2";
		  if (i_random == 2) cst_url = "m_adult_3";
		  if (i_random == 3) cst_url = "m_adult_4";
	  }
	  if (i_age_range == ::c_age_range_old) 
	  {
		  int i_random = random(4);
		 
		  if (i_random == 0)  cst_url = "m_old";
		  if (i_random == 1)  cst_url = "m_old_2";
		  if (i_random == 2)  cst_url = "m_old_5";
		  if (i_random == 3)  cst_url = "m_old_6";
	  }
	} else
	{

	  if (i_age_range == ::c_age_range_kid) cst_url = "f_kid";
	  
	  if (i_age_range == ::c_age_range_teen) 
	  {
		  //pick a random teen graphic
		   int i_random = random(2);
		 
		  if (i_random == 0) cst_url = "f_teen";
		  if (i_random == 1) cst_url = "f_teen_2";
	  }

	  if (i_age_range == ::c_age_range_adult) 
	  {
		  int i_random = random(2);
		  
		  if (i_random == 0) cst_url = "f_adult";
		  if (i_random == 1) cst_url = "f_adult_2";
	  }
	  if (i_age_range == ::c_age_range_old)
	  {
			 int i_random = random(2);
		 
		  if (i_random == 0)  cst_url = "f_old";
		  if (i_random == 1)  cst_url = "f_old_2";
	  
	  }


	}
	  
	  return cst_dir + cst_url+".swf";
}

const CString online_struct::d_relation()
{
	//returns relationship with the deceased.
	
	CString cst_man = "Error";

	if (status.b_deceased_sex_male)
	{
	  //deceased was male
	  if (status.e_relation == c_relation_sibling) cst_man = "brother";
	  if (status.e_relation == c_relation_parent)
	  {
		  cst_man = "father";
		  if (get_range_from_age(status.i_customer_age) == c_age_range_kid)
			  cst_man = "dad"; //kids say dad, not father
		  if (get_range_from_age(status.i_customer_age) == c_age_range_teen)
			  cst_man = "dad"; //kids say dad, not father
	  }

	  if (status.e_relation == c_relation_child) cst_man = "son";
	  if (status.e_relation == c_relation_spouse) cst_man = "husband";
	  if (status.e_relation == c_relation_cousin) cst_man = "cousin";
	  if (status.e_relation == c_relation_grand_parent) cst_man = "grandfather";
	  if (status.e_relation == c_relation_lover) cst_man = "boyfriend";
	  if (status.e_relation == c_relation_grand_child) cst_man = "grandson";
	  if (status.e_relation == c_relation_friend) cst_man = "friend";

	} else
	{
	  //deceased was female
   	  if (status.e_relation == c_relation_sibling) cst_man = "sister";
	  if (status.e_relation == c_relation_parent)
	  {
		  cst_man = "mother";
		  if (get_range_from_age(status.i_customer_age) == c_age_range_kid)
			  cst_man = "mom"; //kids say dad, not father
		  if (get_range_from_age(status.i_customer_age) == c_age_range_teen)
			  cst_man = "mom"; //kids say dad, not father
	  }

	  if (status.e_relation == c_relation_child) cst_man = "daughter";
	  if (status.e_relation == c_relation_spouse) cst_man = "wife";
	  if (status.e_relation == c_relation_cousin) cst_man = "cousin";
	  if (status.e_relation == c_relation_grand_parent) cst_man = "grandmother";
	  if (status.e_relation == c_relation_lover) cst_man = "girlfriend";
	  if (status.e_relation == c_relation_grand_child) cst_man = "granddaughter";
	  if (status.e_relation == c_relation_friend) cst_man = "friend";

	}
	
	
return cst_man;
}

const CString online_struct::c_him()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_customer_sex_male) cst_man = "him"; else
	cst_man = "her";
    return cst_man;
}



const CString online_struct::c_he()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_customer_sex_male) cst_man = "he"; else
	cst_man = "she";
    return cst_man;
}

const CString online_struct::c_He()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_customer_sex_male) cst_man = "He"; else
	cst_man = "She";
    return cst_man;
}


const CString online_struct::d_He()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_deceased_sex_male) cst_man = "He"; else
	cst_man = "She";
    return cst_man;
}

const CString online_struct::d_he()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_deceased_sex_male) cst_man = "he"; else
	cst_man = "she";
    return cst_man;
}


const CString online_struct::c_his()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_customer_sex_male) cst_man = "his"; else
	cst_man = "her";
    return cst_man;
}

const CString online_struct::d_him()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man;
	if (status.b_deceased_sex_male) cst_man = "him"; else
	cst_man = "her";
    return cst_man;
}

const CString online_struct::c_Young_man()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_man = c_young()+" "+c_man();
	cst_man.SetAt(0,toupper(cst_man[0]));
    return cst_man;
}

const CString online_struct::c_Sir()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	if (status.b_customer_sex_male)
		return "Sir"; else return "Ma'am";
}

 const CString online_struct::c_sir()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	if (status.b_customer_sex_male)
		return "sir"; else return "ma'am";
}




const CString online_struct::get_price()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_price;
	int i_price = get_price_int();

	
	
	cst_price.Format("%s",int_to_cash(i_price));
    return cst_price;
}

const int online_struct::get_price_int()
{
	

	int i_price = 0;
	if (status.e_desired_casket == ::c_coffin_wood) i_price += C_COFFIN_WOOD_PRICE;
	if (status.e_desired_casket == ::c_coffin_silver) i_price += C_COFFIN_SILVER_PRICE;
	if (status.e_desired_casket == ::c_coffin_gold) i_price += C_COFFIN_GOLD_PRICE;

	int i;
	 //now add each option
	 for (i=0; i < a_options.GetSize(); i++)
	 {
		 i_price += ::a_option[this->a_options[i]].i_price;
	 }

	 for (i=0; i <= this->status.e_big_upgrade; i++)
	 {
		 i_price += a_upgrade[i].i_price;
	 }


    return i_price;
}



const CString online_struct::casket()
{
	//returns a string description based on known information
	//of the customer, example" "little"
	
	CString cst_casket;
	cst_casket = get_casket_name(status.e_desired_casket);
    return cst_casket;
}

int get_range_from_age(int i_age)
{
  
  if (i_age >= 60) return c_age_range_old;
  if (i_age >= 28) return c_age_range_adult;
  if (i_age >= 15) return c_age_range_teen;
  if (i_age >= 7) return c_age_range_kid;

  //default
  log_error("get_range_from_age: Error, age %d isn't valid.",i_age);
  return 0;
}



