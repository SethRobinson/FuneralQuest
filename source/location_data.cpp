#include "location_data.h"

 

bool location_callback_ok(LPREQUEST lpReq, int i_command)
{
 online_struct *p = p_on[lpReq->i_on_index];

 if (p->status.i_location_callback == i_command) return true;

 
 LogMsg("Eh?  Callback %d not accepted, should be %d.",i_command,
	 p->status.i_location_callback);
 return false;
}

void init_location_info(LPREQUEST lpReq, int i_callback, CString cst_location_name)
{
 online_struct *p = p_on[lpReq->i_on_index];

 memset(&p->status,0,sizeof(communication_status));
 p->status.i_location_callback = i_callback;
 p->cst_location = cst_location_name;
}

