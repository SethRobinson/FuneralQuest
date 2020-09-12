/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "CScriptEngine.h"
#include "CTextParse.h"
#include "CScriptPassVars.h"  
#include "CScriptParse.h" //decipher text into our custom language for quicker running
#include "uni.h"
#include "timer.h"
#include <cassert>


// #define _PROFILE_SCRIPT_ENGINE_  //uncomment this to show how many MS it takes to process scripts

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScriptEngine *g_p_script_engine;

void func_debug(CScriptPassVars *vars)
{
	//get text in WS form and convert it to regular char
	LogMsg(uni(vars->a_vars[0].GetWChar()).to_st());

}  


void func_log_error(CScriptPassVars *vars)
{
	//get text in WS form and convert it to regular char
	LogError(uni(vars->a_vars[0].GetWChar()).to_st());
}


void func_wait(CScriptPassVars *vars)
{
	vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, vars->a_vars[0].GetInt(), 0, NULL);
}



void func_random_range(CScriptPassVars *vars)
{
	//get text in WS form and convert it to regular char
	vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 
	random_range(vars->a_vars[0].GetInt(), vars->a_vars[1].GetInt()), 0, NULL);
}

void func_random(CScriptPassVars *vars)
{
	//get text in WS form and convert it to regular char
	vars->return_var.SetVar(::C_SCRIPT_VAR_INT, 
	random_range(0, vars->a_vars[0].GetInt()), 0, NULL);
}


void func_frandom_range(CScriptPassVars *vars)
{
	static float f_temp;
	
	f_temp = frandom_range(vars->a_vars[0].GetFloat(), vars->a_vars[1].GetFloat());
	
	//get text in WS form and convert it to regular char
	vars->return_var.SetVar(::C_SCRIPT_VAR_FLOAT, *(int*) &f_temp, 0, NULL);
}



void func_StringExpand (CScriptPassVars *vars)
{
	vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, vars->a_vars[0].GetWChar());
}

void func_GetStringPiece(CScriptPassVars *vars)
{
    
    //get the string, convert from unicode, note>> This should be converted to not drop the uncode
    //stuff at some point.
    
    uni u_string(vars->a_vars[0].GetWChar());
    uni u_sep(vars->a_vars[1].GetWChar());
    uni u_piece;

    char *p_st_return = new char[strlen(u_string.st_data)]; 

    seperate_string(u_string.st_data, vars->a_vars[2].GetInt(), u_sep.st_data[0], p_st_return);
    u_piece.set(p_st_return);
    vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, u_piece.us_data);

}

CScriptEngine::CScriptEngine()
{
	g_p_script_engine = this; //pointer, some of the script functions use this
#ifndef _NO_ZLIB
 m_p_file_lib = NULL;
#endif
	//do this later, for testing I don't want to reserve anything though
	//script_vec.reserve(100);
	//startup
    //let's register our globals script
   InitGlobalScript();

}
void CScriptEngine::InitGlobalScript()
{
     
    //kill the global script, if it existed

    if (this->IsScriptValidSilent(0))
    {
        this->UnloadScript(0);
    }
    
    CreateScript(); //this will be our main/globals script (always 0)
											   
	//add our global variables and functions
    int i_var_index = GetScript(0)->AddVar("true", ::C_SCRIPT_VAR_INT);
	GetScript(0)->GetVar(i_var_index)->SetInt(1);
    i_var_index = GetScript(0)->AddVar("false", ::C_SCRIPT_VAR_INT);
    GetScript(0)->GetVar(i_var_index)->SetInt(0);
    

  	

	CScriptPassVars * p_vars; 


	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);
	script_vec[0]->AddFunction("RandomRange", func_random_range, p_vars);
	
   	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);
	script_vec[0]->AddFunction("Random", func_random, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_FLOAT);
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_FLOAT);
	p_vars->return_var.SetType(C_SCRIPT_VAR_FLOAT);
	script_vec[0]->AddFunction("FRandomRange", func_frandom_range, p_vars);
	
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);
	script_vec[0]->AddFunction("Wait", func_wait, p_vars);

	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
	script_vec[0]->AddFunction("LogMsg", func_debug, p_vars);
	
	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
	script_vec[0]->AddFunction("LogError", func_log_error, p_vars);



	p_vars = new CScriptPassVars();
	p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
	script_vec[0]->AddFunction("StringExpand", func_StringExpand, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT);
    p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
    script_vec[0]->AddFunction("GetStringPiece", func_GetStringPiece, p_vars);
    

}

CScriptEngine::~CScriptEngine()
{
	//kill all scripts
	UnloadAllScripts();
}


int CScriptEngine::GetScriptsLoaded()
{
     //count the scripts we're actually using.  Forget about the placeholders.
    int i_count = 0;

    for (unsigned int i=0; i < this->script_vec.size(); i++)
    {
        if (script_vec[i]) i_count++;
    }

    return i_count;
}
   
void CScriptEngine::LogAllScriptsLoaded()
{
     //count the scripts we're actually using.  Forget about the placeholders.
    int i_count = 0;
    for (unsigned int i=0; i < this->script_vec.size(); i++)
    {
        if (script_vec[i])
        {
            i_count++; 
           LogMsg("Script %d is %s.", i_count, script_vec[i]->GetName());
        }

    }
}



int CScriptEngine::CreateScript()
{

	//add a script
	//first find an empty slot
	int i_script = C_SCRIPT_NONE;

	for (unsigned int i=0; i < script_vec.size(); i++)
	{
		if (!script_vec[i])
		{
			//we can use this slot
			i_script = i;
			break;
		}
	}

	CScript *p_script = new CScript();
	if (!p_script)
	{
		LogError("Unable to allocate new script.");
		return -1;
	}


	if (i_script == C_SCRIPT_NONE)
	{
		//couldn't replace an old one, let's allocate a new one.
 		script_vec.push_back(p_script);
		i_script = script_vec.size()-1; //stl thing, size is always 1 extra
        assert(i_script > -1);
	} else
	{
		//found another script we can write over.
		//let's just delete it and reinit
		SAFE_DELETE(script_vec[i_script]);
		script_vec[i_script] = p_script;

	}
	
	
	p_script->SetActive(true); //nobody else will overwrite this slot now
	p_script->SetId(i_script); //it may want to know where it is located later
//	LogMsg("Creating script %d.", i_script);
	return i_script;
}

//give a filename, it returns the scripts id
int CScriptEngine::load_and_compile_script(const char *st_script, int i_script)
{
    
#ifdef _DEBUG
if (strchr(st_script, '/'))
{
    LogMsg("There is a forward slash in the filename of %s.  This wont work with the CFL, change to backslash.",
        st_script);
    assert(0);
}

#endif

#ifdef _PROFILE_SCRIPT_ENGINE_
    int i_time_start = get_time();
#endif   
    
    CTextParse parse;
    bool b_found = false;

#ifndef _NO_ZLIB

    assert(m_p_file_lib);
    int i_size;
    char *p_file = m_p_file_lib->GetCFL()->getFile(st_script, i_size);
    if (p_file)
    {
        parse.AddText(p_file, i_size);
        parse.AddText("\0\0", 2); //add null
        b_found = true;
        parse.CheckForUnicode();

    }
    
    SAFE_DELETE_ARRAY(p_file);
     
#else
   //non CFL way for FQ 
    
    char st_temp[256];
    sprintf(st_temp, "%s%s", GetExeDir(), st_script);
    
    if (exist(st_temp))
    {
        if (!parse.open(st_temp))
        {
            LogError("Unable to parse file %s.", st_temp);
            return -1;
        }
        b_found = true;    
    }  else
    {
        //yet another failure
        LogError("Unable to find file %s. in even the main dir resource zip", st_script);
        return -1;
    }
 
#endif
    
    //ok, the script has been completely loaded and is now in memory.
//	for (int i=0; i < 50; i++) //speed test


	if (!parse.IsUnicode())
	{
         LogError("Error in %s. File not found or file is not unicode.", st_script);
         assert(0);		
 		return -1;
	}
   // char st_temp[256];

    
   this->script_vec[i_script]->SetName(st_script);
	CScriptParse script_parse; //use this to parse it
	script_parse.SetScript(i_script, this);
	
	WCHAR * p_line;
	
	WCHAR *wc_mark;
	while (parse.get_next_line_wide(&p_line))
	{
		//let's truncate the line so comments don't confuse things

			//add a fake end maybe
		   wc_mark = wcspbrk_ignore_inside_strings(p_line, L";:");

		   if (wc_mark)
		   {
			   wc_mark[1] = 0; //truncate
		   }

			//actually process what we've built
		   	script_parse.ParseLineWC(p_line);


	//	LogMsg("%s",uni(p_line).to_st());
	}
  
#ifdef _PROFILE_SCRIPT_ENGINE_

   LogMsg("Script %s time: %d", st_script, get_time()-i_time_start);
#endif
    return i_script; //handle to our new script
}

CScript * CScriptEngine::GetScript(int i_script)
{
	if (in_range(i_script, 0, this->script_vec.size()-1))
	{
		if (!script_vec[i_script])
		{
			LogError("GetScript returning inactive script %d.",i_script);
		}
		return script_vec[i_script];
	} else
	{
	LogError("GetScript: Script %d is out of range.", i_script);
	return NULL;
	}
}


bool CScriptEngine::IsScriptValid(int i_script)
{

	if (in_range(i_script, 0, this->script_vec.size()-1))
	{
		if (!script_vec[i_script])
		{
			LogError("Script %d is not active.", i_script);
			return false;
		}
		return true;
	} else
	{
	LogError("Script %d does not exist.", i_script);
	  return false;
	}

}

//same as above but doesn't give the errors
bool CScriptEngine::IsScriptValidSilent(int i_script)
{

	if (in_range(i_script, 0, this->script_vec.size()-1))
	{
		if (!script_vec[i_script])
		{
			return false;
		}
		return true;
	} else
	{
	  return false;
	}

}


//you MUST send this as all CAPS!!!
bool CScriptEngine::RunScriptFunction(int i_script, const char *st_name)
{
    
#ifdef _DEBUG
        char st_upcased_version[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];
        strcpy(st_upcased_version, st_name);
        strupr(st_upcased_version);
        assert(strcmp(st_upcased_version, st_name) == 0 && "Error, only send this an all upper case function name.");
#endif

        
    if (IsScriptValid(i_script))
	{
		
		CScriptPassVars vars;
		script_vec[i_script]->RunFunction(st_name, &vars);
	    return true;
	}

	return false; //couldn't find it
	  
}


//does not check to see if the script is valid, so check that before using this
int CScriptEngine::GetFunctionParmCount(int i_script, int i_function_index)
{
  return script_vec[i_script]->GetFunction(i_function_index)->CountRequiredParms();
}

void CScriptEngine::UnloadAllScripts()
{
		for (unsigned int i=0; i < this->script_vec.size(); i++)
	{
			UnloadScript(i);
	}

}

void CScriptEngine::UnloadAllScriptsExceptGlobals()
{
  	for (unsigned int i=1; i < this->script_vec.size(); i++)
	{
			UnloadScript(i);
	}

}

void CScriptEngine::UnloadScript(int i_script)
{
	SAFE_DELETE(script_vec[i_script]);
}


bool CScriptEngine::SaveScriptVars(int i_script, FILE *fp)
{
	//save all our globals to this file
	
	//first count them
	return script_vec[i_script]->SaveVars(fp);
}

bool CScriptEngine::LoadScriptVars(int i_script, FILE *fp)
{
	//save all our globals to this file
	
	//first count them
	return script_vec[i_script]->LoadVars(fp);
}


