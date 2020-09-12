#include "CAutoSave.h"

//construction

CAutoSave::CAutoSave()
{
   //set last save time to now
	time(&t_last_save);
}

CAutoSave::~CAutoSave()
{
   //kill anything we started
}

void CAutoSave::TriggerNext()
{
   t_last_save = 0;
}


bool CAutoSave::seconds_elapsed(int i_seconds)
{
	if (i_seconds == 0) return false; //disabled
	
	//have this many  minutes elapsed yet?  If so, return true and reset the
	//timer.
	time_t t_now;
	time(&t_now);
	double dbl_elapsed_seconds;
	dbl_elapsed_seconds = difftime(t_now, this->t_last_save);
	if (dbl_elapsed_seconds > i_seconds)
	{
		//yep, that much time has passed.  Reset timer.
		this->t_last_save = t_now;
		return true;
	} 


	//nope, not time yet.
	return false;

}

bool CAutoSave::minutes_elapsed(int i_minutes)
{
	//have this many  minutes elapsed yet?  If so, return true and reset the
	//timer.
	return (seconds_elapsed(i_minutes * 60));
}