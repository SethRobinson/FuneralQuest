//Seth's Microsoft Speech wrapper
#pragma once

#include "all.h"
#include "../SharedLib/SAPI/include/sapi.h"

class CSpeech
{
public:

	CSpeech();
	~CSpeech();

#ifdef _UNICODE
    void say(const TCHAR p_tchar[]); //unicode
#endif
   void set_mute(bool b_new);
void say(const LPCSTR lpFormat, ...);
void say(const WCHAR p_tchar[], DWORD dw_options);

private:

	ISpVoice * pVoice;
	bool b_muted;


};


