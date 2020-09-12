/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



#include "unicode_utils.h"

void log_msg(const char *lpFormat, ...);
void LogError(const char *lpFormat, ...);

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//send this a null terminated unicode pointer and it will skip it ahead
//past the spaces.

WCHAR * strip_spaces_from_pointer_wc(WCHAR *p_line)
{
	while(p_line[0] == L' ')
	{
		p_line = &p_line[1];
	}
return p_line;
}


bool WCharIsANumber(WCHAR w_char)
{
	
	if (w_char == L'-') return true;
	if (w_char >= L'0') if (w_char <= L'9') return true;

	//not a number if we got here
	return false;

}
