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
#include "all.h"
#include "CScriptPassVars.h" //just to get the define we need
//fowards for speed
class CScriptEngine;
class CScriptOp;
class CScriptVarBase;

class CScriptParse
{
public:
	
	CScriptParse();
	~CScriptParse();
	
	void SetScript(int i_new_script, CScriptEngine *p_engine)
	{
		m_i_script = i_new_script;
	    m_p_engine = p_engine;
	}


	void ParseLineWC(WCHAR *p_line);

private:
	char * CScriptParse::ScriptName();
	char * CScriptParse::ErrorInfo();
	void CScriptParse::ParseOp(WCHAR *p_line);
	void CScriptParse::SetupSetSource(CScriptOp *p_op, WCHAR * p_line);
	void CScriptParse::SetupSetOp(CScriptOp * p_op, WCHAR *p_line, WCHAR *p_wst_op, int i_op_type);
	bool CScriptParse::ParseParmsIntoOp(CScriptOp *p_op, WCHAR *p_line);
	bool CScriptParse::ConvertWCStringToBaseVar(CScriptVarBase *p_var, WCHAR *p_word);
	bool CScriptParse::TestAndCreateVars(WCHAR *p_line);
	bool CScriptParse::VarDefinationExists(WCHAR **p_line, WCHAR *p_type);
	void CScriptParse::ParseIfStatement(WCHAR *p_line);

	WCHAR wst_temp[C_SCRIPT_MAX_WCHAR_SIZE];
	int m_i_line_count; //helps us report which line an error happens on
	int m_i_script;
	CScriptEngine * m_p_engine;
	int m_i_cur_function;
	int m_i_nest; //controls nesting level
};


WCHAR * wcspbrk_ignore_inside_strings( WCHAR* p_search, WCHAR *p_chars);
