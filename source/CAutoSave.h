#include "stdafx.h"

//timing device, create the object then check to see if so much time has passed yet.

//When it has, it resets itself to the current time.

class CAutoSave
{

private:
	time_t t_last_save;

public:
	CAutoSave(); //contruction
    ~CAutoSave(); //deconstruction
	bool seconds_elapsed(int i_seconds);
	bool CAutoSave::minutes_elapsed(int i_minutes);
    void TriggerNext(); //force the next elapsed question to return true
};


