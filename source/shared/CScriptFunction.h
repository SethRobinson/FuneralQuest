/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#pragma once
#include "CScriptPassVars.h"
#include "CScriptFunction.h"
#include "CScriptVar.h"
#include "CScriptOp.h"
#include <vector>

class CScript; 

using namespace std;

typedef vector<CScriptOp*> OpVec;
typedef OpVec::iterator OpItor;


class CScriptFunction
{
public:
	
	//input vars must be pre newed()!!! we're keeping them and will handle deallocation

	CScriptFunction(CScriptPassVars *p_new_vars)
	{
		m_p_func = NULL;
		m_st_name[0] = 0;
		p_m_vars = p_new_vars;
		SetNestLock(-1); //no lock
		m_i_callback_function = -1;
    }



	virtual ~CScriptFunction();
	
	void SetName(const char st_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE]){strcpy(m_st_name, st_name);
	_strupr((char*)&m_st_name);};
	char * GetName(){return (char*)&m_st_name;};
	int CreateOp();
	CScriptPassVars * GetVars(){return p_m_vars;};

	bool CScriptFunction::ExpandWS(CScriptVarBase * p_var);
	void SetParentScript(CScript *p_parent_script){m_p_parent_script = p_parent_script;}
	void SetId(int i_id){m_i_id = i_id;}
	int CScriptFunction::GetGotoAnchorIdByName(CScriptVarBase * p_var);
	void SetFunctionPointer(void (*func)(CScriptPassVars*))
	{
		m_p_func = func;
	}

   bool CScriptFunction::RunFunction(CScriptPassVars *p_var_in, int i_starting_op);
   OpVec m_op; //operation code this script has, if any
   int CScriptFunction::CountRequiredParms(); 
   void ResetNestLock() {m_i_nest_lock = -1;}
private:
	void SetNestLock(int i_nest_lock){m_i_nest_lock = i_nest_lock;}
	int GetNestLock(){return m_i_nest_lock;};
void CScriptFunction::SetSavedParentFunction(int i_func, int i_op);  
	void CScriptFunction::CallSavedParentFunction();
   CScript *m_p_parent_script; //if applicable
   int m_i_id; //what our index is
   CScriptPassVars *p_m_vars;
   void (*m_p_func)(CScriptPassVars*);
   char m_st_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];
   int m_i_nest_lock; //how many {'s in we are
   int m_i_callback_function; //used for wait() type statements
   int m_i_callback_op;  //used for wait() type statements
};