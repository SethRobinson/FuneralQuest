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

const int C_SCRIPT_VAR_NONE = 0;
const int C_SCRIPT_VAR_INT = 1;
const int C_SCRIPT_VAR_FLOAT = 2;
const int C_SCRIPT_VAR_WCHAR = 3;
const int C_SCRIPT_VAR_DYNAMIC = 4;
const int C_SCRIPT_VAR_FORCE_WAIT = 5;
const int C_SCRIPT_VAR_VECTOR3 = 6;


#include <wtypes.h>

class CScriptVarBase
{

public:

	CScriptVarBase();
	virtual ~CScriptVarBase();
	
	void SetVar(int i_type, int i_parm1, int i_parm2, void *p_extra); //sets type with default values
    bool Set(CScriptVarBase *p_new);
    int GetType(){return m_i_type;}
	void SetType(int i_new_type);

	
	WCHAR * CScriptVarBase::GetWChar();

	//convert whatever out var type is to a string
	int CScriptVarBase::AppendToWChar(WCHAR *p_dest);
	bool CScriptVarBase::Modify(CScriptVarBase *p_new, int i_mod_type);
	

    
    void CScriptVarBase::WCharAdd(WCHAR *p_new); //add a string to the string we have
	float CScriptVarBase::GetFloat();
    int CScriptVarBase::GetInt();
	void CScriptVarBase::SetInt(int i_new);
	void CScriptVarBase::SetFloat(float f_new);
	
	int GetBaseType();
    bool If(CScriptVarBase *p_new, int i_mod_type, const char *p_optional_text_for_errors);

protected:
		//only one of these will actually be active
	int m_i_type;
	int m_i_parm1; //we'll cast this into anything we want actually
	int m_i_parm2;
	void *m_p_extra;
};	  

//utility function
char * GetVarTypeString(int i_var_type);