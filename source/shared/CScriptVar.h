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

#include "CScriptVarBase.h"
#include "CScriptPassVars.h"
//holds a single variable of some sort

class CScriptVar : public CScriptVarBase
{
public:

	void SetName(char st_new_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE])
	{
		strcpy(m_st_name, st_new_name);
	}
  	char * GetName(){return (char*)&m_st_name;}
	bool Save(FILE *fp);
	

private:
	char m_st_name[::C_SCRIPT_MAX_FUNCTION_NAME_SIZE];

};
