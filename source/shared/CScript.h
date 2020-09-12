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
#include "CScriptPassVars.h"
#include "CScriptFunction.h"
#include "CScriptVar.h"
#include "CTimer.h"

#include <vector>
#include <map>

using namespace std;

typedef vector<CScriptFunction*> FunctionVec;
typedef FunctionVec::iterator FunctionItor;

typedef vector<CScriptVar*> VarVec;
typedef VarVec::iterator VarItor;

typedef map<unsigned long, int> mapHashIndex;   //index to find our var names quick
typedef mapHashIndex::iterator itorHashIndex;


class CScript
{
public:


	CScript();
	~CScript();

    void Kill();
    void SetActive(bool b_new){b_active = b_new;}
    bool IsActive(){if (this == NULL) return false; return b_active;};
    int AddFunction(char st_function_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE], 
        void (*func)(CScriptPassVars*), CScriptPassVars * p_vars);
    int AddVar(char st_var_name[C_SCRIPT_MAX_FUNCTION_NAME_SIZE], int i_type);
    void SetName(const char *st_name){strcpy(m_st_name, st_name);}
    char * GetName(){return m_st_name;}
    CScriptFunction * GetFunction(int i_func){return m_function_vec[i_func];}
    int FindVarByName(const char *st_search);
    int FindVarByNameWS(WCHAR *wst_search); //wide char version
    void RunFunction(const char *st_name, CScriptPassVars *vars);
    int FindFunctionByName(const char *st_search);
    CScriptVar * GetVar(int i_var_to_get);
    void SetId(int i_id){m_i_id = i_id;}
    int GetId(){return m_i_id;}
    void SetWait(int i_wait_time, int i_func_id, int i_op_id);
    
    bool UpdateCallback();
    bool StartUpAfterWait();
    
    bool SaveVars(FILE *fp);
    bool LoadVars(FILE *fp);
    
    //stuff to make string lookups fast.  Strings are only compared this way during
    //the initial parse, after that it's O(1) lookups all the way.
    
    void VarIndexAdd(char *p_st_name, int i_index);
    int GetVarIndexByHash(unsigned long ul_hash);
    int GetFunctionIndexByHash(unsigned long ul_hash);
    void FunctionIndexAdd(char *p_st_name, int i_index);


private:

    bool b_active;
    FunctionVec m_function_vec; //holds all the functions in this scripts
	VarVec m_var_vec; //holds all the variables allocated
	char m_st_name[256]; //script's full path and name	
	int m_i_id; //remember what our id is in the main script vector
	CTimer timer_callback;
	bool b_callback_active;
	int i_callback_function;
	int i_callback_op;
    mapHashIndex m_map_var_index; //for fast lookups of vars, added 8-4-02
    mapHashIndex m_map_function_index; //for fast lookups of function names, added 8-4-02

};