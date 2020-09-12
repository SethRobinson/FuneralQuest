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
#pragma warning (disable:4786)

#include "all.h"
#include "CScript.h"
#include "CFileLib\\CFileLib.h"
#include <vector>

using namespace std;

typedef vector<CScript*> ScriptVec;
typedef ScriptVec::iterator ScriptItor;

const int C_SCRIPT_NONE = -1;

class CScriptEngine
{
public:
    bool IsScriptValid(int i_script);
    bool IsScriptValidSilent(int i_script); //doesn't give error messages

    CScriptEngine();
    virtual ~CScriptEngine();
    int CreateScript();
    CScript * GetScript(int i_script); //don't trust this pointer, if a new script is added it may be invalid
    bool RunScriptFunction(int i_script, const char * st_name); //send an UPCASED function name
    int GetFunctionParmCount(int i_script, int i_function_index);
    
    int load_and_compile_script(const char *st_script, int i_script);
    void UnloadAllScripts();
    void UnloadScript(int i_script);
    void UnloadAllScriptsExceptGlobals(); //don't unload our global functions and vars (in script 0)
    
    //these save and load all vars in a script.  Make sure you pass an already
    //opened file pointer to them.  It's up to you to close it.
    bool SaveScriptVars(int i_script, FILE *fp);
    bool LoadScriptVars(int i_script, FILE *fp);

#ifndef _NO_ZLIB
    CFileLib * GetFileLib(){return m_p_file_lib;}
    void SetFileLib(CFileLib *p_new) {m_p_file_lib = p_new;}
#endif

    int GetScriptsLoaded();
    void LogAllScriptsLoaded();
    void InitGlobalScript(); //script 0 is always for globals, this is called during the init

private:
    ScriptVec script_vec; //holds all our active scripts
    void ResetProcess();
//support zipped script files

#ifndef _NO_ZLIB
     CFileLib *m_p_file_lib;
#endif

};
