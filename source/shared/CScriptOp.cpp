/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CScriptOp.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScriptOp::CScriptOp()
{
	m_i_function_script = -1; //signal no function
	SetControl(C_SCRIPT_CONTROL_NONE);
}

void CScriptOp::SetFunction(int i_function_script, int i_function_id)
{
	m_i_function_script = i_function_script;	
	m_i_function_id = i_function_id;
}
