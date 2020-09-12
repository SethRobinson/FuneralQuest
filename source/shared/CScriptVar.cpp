/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CScriptVar.h"
#include "all.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CScriptVar::Save(FILE *fp)
{

 if (GetType() == C_SCRIPT_VAR_NONE) return false;
 if (GetType() == C_SCRIPT_VAR_DYNAMIC) return false;
 if (GetType() == C_SCRIPT_VAR_FORCE_WAIT) return false;


//    log_msg("Saving %s.", GetName());
	fwrite(&m_i_type, sizeof(int), 1, fp);

	 //write var name
	fwrite(&m_st_name, C_SCRIPT_MAX_FUNCTION_NAME_SIZE, 1, fp);


	//now write the value
	if (m_i_type == C_SCRIPT_VAR_INT) fwrite(&m_i_parm1, sizeof(int), 1, fp);
	if (m_i_type == C_SCRIPT_VAR_FLOAT) fwrite(&m_i_parm1, sizeof(int), 1, fp);

	if (m_i_type == C_SCRIPT_VAR_WCHAR)
	{
		
		int i_length = 0;
		if (m_p_extra)
		{
		
		i_length = wcslen((WCHAR*)m_p_extra)+1;
		} else
		{
			//it's null, blank.
		}
		
		//write how long it is so we know how much to read later
		fwrite(&i_length, sizeof(int), 1, fp);
		
		//write it out.  Don't forget this is unicode, so double byte for each char.
		fwrite(m_p_extra, i_length*2, 1, fp);
	}

 

 return true;
}	


