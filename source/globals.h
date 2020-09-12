#pragma once
#include "stdafx.h"
//some globals

#pragma once
#include "all.h"
#include "CScriptEngine.h"

class CSpeech;

class CScriptEngine; //forward

class globals
{

public:
    char st_server_address[80];
    int i_server_port;
    CSpeech *p_speech; //for talking
    CScriptEngine * p_script; //our main script object, holds all the scripts
    
    
    globals()
    {
        memset(this,0, sizeof(globals));
    }
    
    ~globals()
    {
        //scripting system has no dependencies, so this is safe.
        SAFE_DELETE(p_script);
    }
    

};

extern globals g_glo;