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

const int C_SCRIPT_OP_NONE = 0;
const int C_SCRIPT_OP_SET = 1;
const int C_SCRIPT_OP_FUNCTION = 2;
const int C_SCRIPT_OP_IF = 3;
const int C_SCRIPT_OP_ADD = 4;
const int C_SCRIPT_OP_SUBTRACT = 5;
const int C_SCRIPT_OP_DIVIDE = 6;
const int C_SCRIPT_OP_MULTIPLY = 7;
const int C_SCRIPT_OP_IF_EQUALS = 8;
const int C_SCRIPT_OP_IF_NOT_EQUALS = 9;
const int C_SCRIPT_OP_IF_MORE = 10;
const int C_SCRIPT_OP_IF_LESS = 11;
const int C_SCRIPT_OP_IF_SAME_OR_LESS = 12;
const int C_SCRIPT_OP_IF_SAME_OR_MORE = 13;
const int C_SCRIPT_OP_GOTO = 14;
const int C_SCRIPT_OP_SPECIAL = 15; //generally it's no op, but we don't want to get ignored
const int C_SCRIPT_OP_MODULUS = 16;


const int C_SCRIPT_CONTROL_NONE = 0;
const int C_SCRIPT_CONTROL_IF = 1;
const int C_SCRIPT_CONTROL_GOTO_BY_WCHAR = 2;
const int C_SCRIPT_CONTROL_GOTO_BY_INT = 3;
const int C_SCRIPT_CONTROL_GOTO_ANCHOR = 4;
const int C_SCRIPT_CONTROL_RETURN = 5;



class CScriptOp
{
public:
   CScriptOp();
	//this is where we get really complicated.  A function holds an opcode which holds
	//a function.
  void SetOp(int i_new_op){m_i_op = i_new_op;}
  int GetOp(){return m_i_op;}
  void SetFunction(int i_function_script, int i_function_id);
  CScriptVarBase return_var; 
  CScriptVarBase parms[C_SCRIPT_MAX_PASS];
  int GetFunctionScript(){return m_i_function_script;}
  int GetFunctionId(){return m_i_function_id;}
  void SetNesting(int i_nest){m_i_nest = i_nest;}
  int GetNesting(){return m_i_nest;};
  int GetControl(){return m_i_control;}
  void SetControl(int i_control){m_i_control = i_control;}

private:
    int m_i_op; //tells us the general command we need
	int m_i_function_script;
	int m_i_function_id; //if not -1, let's us assume we will run a function
	int m_i_nest; //current nesting, helps with if() statements.
	int m_i_control; //indicates a special control feature of this op
};