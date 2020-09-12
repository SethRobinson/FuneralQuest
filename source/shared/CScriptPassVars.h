#pragma once

#include "all.h"
#include "CScriptVarBase.h"

const int C_SCRIPT_MAX_PASS = 9;

const int C_SCRIPT_NO_FUNCTION = -1;

const int C_SCRIPT_MAX_FUNCTION_NAME_SIZE = 64;

const int C_SCRIPT_MAX_WCHAR_SIZE = 2024; //the "work" buffer is changed with it.  Vars only
//waste the actual size of the string generally.


class CScriptPassVars
{
public:

CScriptVarBase a_vars[C_SCRIPT_MAX_PASS];
CScriptVarBase return_var;
int i_script;
public:

	void Clear()
	{
	  for (int i=0; i < C_SCRIPT_MAX_PASS; i++)  
	  {
		  a_vars[i].SetType(C_SCRIPT_VAR_NONE);
	  }
	 return_var.SetType(C_SCRIPT_VAR_NONE);
	 i_script = -1;
	}

	CScriptPassVars()
	{
		Clear();
	}

};
