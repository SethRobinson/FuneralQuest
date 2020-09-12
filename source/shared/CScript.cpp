/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CScript.h"
#include "Uni.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScript::CScript()
{
	Kill(); 
    SetName("?");
}

void CScript::Kill()
{
  	
	for (unsigned int i=0; i < m_function_vec.size(); i++)
	{
		SAFE_DELETE(m_function_vec[i]);
	}

	m_function_vec.clear();
    
    for (unsigned int v=0; v < m_var_vec.size(); v++)
	{
	   SAFE_DELETE(m_var_vec[v]);
	}

     m_var_vec.clear();

	//reset everything
	b_active = false;
	b_callback_active = false;

    m_map_function_index.clear();
    m_map_var_index.clear();

}	


CScript::~CScript()
{
	Kill();

}

void CScript::SetWait(int i_wait_time, int i_func_id, int i_op_id)
{

   b_callback_active = true;
   this->i_callback_function = i_func_id;
   this->i_callback_op = i_op_id;

   //make sure we aren't in any weird if statement position in the function
   this->m_function_vec[i_func_id]->ResetNestLock();

   timer_callback.set_interval(i_wait_time);
}

bool CScript::UpdateCallback()
{
	if (!b_callback_active) return false; //not waiting for anything

	if (timer_callback.interval_reached())
	{
		b_callback_active = false;
		this->m_function_vec[i_callback_function]->RunFunction(NULL, i_callback_op);
	}
	return true;  //true just indicates we were or are waiting on a callback.
}	


//this is like UpdateCallback only it forces the callback to happen now (for functions that need to
//wait different times depending on a keypress, etc)

bool CScript::StartUpAfterWait()
{
	if (!b_callback_active) return false; //not waiting for anything
		b_callback_active = false;
		this->m_function_vec[i_callback_function]->RunFunction(NULL, i_callback_op);

	return true; 
}


//you must pass this a valid p_vars that you have new()'d !! it will take the
//pointer and handle dallocating it.

int CScript::AddFunction(char st_function_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE],
				void (*func)(CScriptPassVars*), CScriptPassVars * p_vars)
{
	CScriptFunction *p_new_func = new CScriptFunction(p_vars);
	if (!p_new_func)
	{
		LogError("Not enough memory to add a function in script %s.", GetName());
		return -1;
	}
   	//let's upcase this, we don't want to be case sensitive
	
    char st_temp[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];

	//if I try to _strupr on st_var_name it crashes? what? Ok, I'll do it this way
	strcpy(st_temp, st_function_name);

	//let's upcase this, we don't want to be case sensitive
	_strupr(st_temp);



	m_function_vec.push_back(p_new_func);
	//copy vars to it

	int i_index = m_function_vec.size()-1;
  	//set some defaults
	p_new_func->SetName(st_temp);
	p_new_func->SetFunctionPointer(func);
	p_new_func->SetParentScript(this); //might need this later
	p_new_func->SetId(i_index); //they may want this later

//	LogMsg("adding function %s in slot %d.", st_function_name, i_index);
    FunctionIndexAdd(st_temp, i_index);
   
return i_index;
}


//this will returns the vars index in the var array or -1 for none found.
//Give this an UPPER CASE var only.
int CScript::FindVarByName(const char *st_search)
{
    //first let's try our fast way
    unsigned long ul_hash = ul_compute_checksum_from_string(st_search);
    int i_hash_index;
    if ( (i_hash_index=GetVarIndexByHash(ul_hash)) != -1)
    {
  //      LogMsg("HASHed var %d found", i_hash_index);
        return i_hash_index;
    }

    //hash lookup failed.  Do the other method, it's possible (although unlikely)
    //that two function names had the same hash.  We deal with that by not adding
    //the second one to the hash.  
    
    for (unsigned int i=0; i < m_var_vec.size(); i++)
		if (strcmp(st_search, m_var_vec[i]->GetName()) == 0) return i;
		
		//if we got here we failed.
		return -1;
}	

//this will returns the functions index in the array or -1 for none found.
//Give this an UPPER CASE function name only.

int CScript::FindFunctionByName(const char *st_search)
{
 //first let's try our fast way
    unsigned long ul_hash = ul_compute_checksum_from_string(st_search);
    int i_hash_index;
    if ( (i_hash_index=GetFunctionIndexByHash(ul_hash)) != -1) 
    {
 //       LogMsg("HASHed func %d found", i_hash_index);
        return i_hash_index;
    }

    //hash lookup failed.  Do the other method, it's possible (although unlikely)
    //that two function names had the same hash.  We deal with that by not adding
    //the second one to the hash.  
  	for (unsigned int i=0; i < m_function_vec.size(); i++)
		if (strcmp(st_search, m_function_vec[i]->GetName()) == 0) return i;
		//if we got here we failed.
		return -1;
}	


//conver to ANSI and up case, then compare
int CScript::FindVarByNameWS(WCHAR *wst_search)
{
	uni uni_temp(wst_search);
	_strupr(uni_temp.to_st());
	return FindVarByName(uni_temp.st_data);

}



int CScript::AddVar(char st_var_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE], int i_type)
{
	int i_index = -1;
    char st_temp[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];

	//if I try to _strupr on st_var_name it crashes? what? Ok, I'll do it this way
	strcpy(st_temp, st_var_name);

	//let's upcase this, we don't want to be case sensitive
	_strupr(st_temp);


	if ( (i_index = FindVarByName(st_temp)) != -1)
	{
		LogMsg("Var %s already exists in slot %d. (%s)", st_temp, i_index, GetName());
		return i_index;
	}

	CScriptVar *p_var = new CScriptVar();
	
	if (!p_var)
	{
		LogError("Couldn't get memory for new variable.");
		return -1;
	}

	m_var_vec.push_back(p_var);
	i_index = m_var_vec.size()-1;
	m_var_vec[i_index]->SetName(st_temp);
	m_var_vec[i_index]->SetVar(i_type, 0, 0, NULL); //set it up with defaults
	
	//	LogMsg("Adding var %s in slot %d.", st_var_name, i_index);
    //add to our hash table map
    
    VarIndexAdd(st_temp, i_index);
    


        
	return i_index; //this is the index of the var we created
}

//returns var index or -1 for no var

int CScript::GetVarIndexByHash(unsigned long ul_hash)
 {
    itorHashIndex iitor = m_map_var_index.find(ul_hash);

    if (iitor == m_map_var_index.end())
    {
        //not in hash.
        return -1;
    }
    
    return iitor->second; //this is the index of the var
}

void CScript::VarIndexAdd(char *p_st_name, int i_index)
{
    unsigned long ul_hash = ul_compute_checksum_from_string(p_st_name);
    
    if (GetVarIndexByHash(ul_hash) != -1)
    {
        LogMsg("Var %s already hashed. (hashed parsing for this won't be available) (%lu)", p_st_name, ul_hash);
       
        //mark it as bad, so we won't rely on the has for this id
         m_map_var_index[ul_hash] = -1;

        return;
    }

 //LogMsg("Just added hash of %lu for index %d of var %s.",ul_hash, i_index, p_st_name);
    
    //else add it.
    m_map_var_index[ul_hash] = i_index;

}

int CScript::GetFunctionIndexByHash(unsigned long ul_hash)
 {
    itorHashIndex iitor = m_map_function_index.find(ul_hash);

    if (iitor == m_map_function_index.end())
    {
        //not in hash.
        return -1;
    }
    
    return iitor->second; //this is the index of the var
}

void CScript::FunctionIndexAdd(char *p_st_name, int i_index)
{
    unsigned long ul_hash = ul_compute_checksum_from_string(p_st_name);
    
    if (GetFunctionIndexByHash(ul_hash) != -1)
    {
        LogMsg("Func %s already hashed. (hashed parsing for this won't be available) (%lu)", p_st_name, ul_hash);
        m_map_function_index[ul_hash] = -1;
        return;
    }

//  LogMsg("Just added hash of %lu for index %d of function %s.",ul_hash, i_index, p_st_name);
    
    //else add it.
    m_map_function_index[ul_hash] = i_index;

}


void CScript::RunFunction(const char *st_name, CScriptPassVars *vars)
{
   //first find the function by name
	int i_function = FindFunctionByName(st_name);
	if (i_function == -1)
	{
		//we couldn't find it in here.
		LogError("Unable to locate function %s in script %s.", st_name, this->GetName());
		return ;
	}

    //let's run the script
	m_function_vec[i_function]->RunFunction(vars, 0);

}

CScriptVar * CScript::GetVar(int i_var_to_get)
{
   return m_var_vec[i_var_to_get];
}


bool CScript::LoadVars(FILE *fp)
{
	bool b_force_vars_to_exist = false;  //if true, will ignore vars not already initted
	//in memory
	int i_type;
	char st_var_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];
	
	while(1)
	{
	  //first load what kind of var it is
	  if (fread(&i_type, sizeof(int), 1, fp) == 0)
      {
          //something is wrong, the file is over?
          break;
      }

	  if (i_type == 1000)
	  {
		  //all done
		  break;
	  }
	  
	  //else load the var name
	  	fread(&st_var_name, C_SCRIPT_MAX_FUNCTION_NAME_SIZE, 1, fp);

//		LogMsg("Loading %s...", st_var_name);
		//ok, we now know what kind of var it is and what the name of it is.
		//first let's find out if the var already exists
		int i_var_index = FindVarByName(st_var_name);

		if (i_var_index == -1)
		{
			//well, the var doesn't exist.  We can ignore it or add the var.
		    if (!b_force_vars_to_exist)
			{
				//add it.
				this->AddVar(st_var_name, i_type);
			}
		}

		//load the var's actual data, this depends on the var type
		
		if (i_type == C_SCRIPT_VAR_INT)
		{
			int i_value;
			fread(&i_value, sizeof(int), 1, fp);
			if (i_var_index != -1)
			{
				//set it.
				m_var_vec[i_var_index]->SetInt(i_value);
			}

		}

		if (i_type == C_SCRIPT_VAR_FLOAT)
		{
			float f_value;
			fread(&f_value, sizeof(float), 1, fp);
			if (i_var_index != -1)
			{
				//set it.
				m_var_vec[i_var_index]->SetFloat(f_value);
			}

		}


	  	if (i_type == C_SCRIPT_VAR_WCHAR)
		{
			//now the last and most tricky.
			
			
			//how many chars is it? (these are wchars, so 2 bytes each)
			int i_string_size;
			fread(&i_string_size, sizeof(int), 1, fp);
		
			WCHAR *p_wchar = NULL;
			if (i_string_size > 0)
			{
			
			p_wchar = new WCHAR[i_string_size];
			//load it into our buffer
			fread(p_wchar, i_string_size*2, 1, fp);
			}
			

			//and finally copy it to our vars buffer.  We could save an extra copy if
			//we really wanted to.. but it's cleaner this way and speedwise there is
			//no problem.  Yet.

			if (i_var_index != -1)
			{
				//set it.
				m_var_vec[i_var_index]->SetVar(i_type, 0, 0, p_wchar);
			}

			//either way, delete the memory we had created.
			SAFE_DELETE(p_wchar);

		}


	}

	return true; //success
}



bool CScript::SaveVars(FILE *fp)
{

	//loop through and write each var, the Save() function actually decides if it should save the
	//var or not.

	for (unsigned int i=0; i < m_var_vec.size(); i++)
	{
	   m_var_vec[i]->Save(fp);
	}

   	//save a final "we're done" message

	int i_type = 1000; //we're done, no vartype is beyond 10 right now
	fwrite(&i_type, sizeof(int), 1, fp);


	return true; //success
}
