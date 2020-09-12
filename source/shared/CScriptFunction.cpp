/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CScriptFunction.h"
#include "CScriptEngine.h"
#include "uni.h"
#include "CScript.h"

extern CScriptEngine *g_p_script_engine;

CScriptFunction::~CScriptFunction()
{
	//delete all our ops properly
	for (unsigned int i=0; i < m_op.size(); i++)
	{
		SAFE_DELETE(m_op[i]);
	}

	SAFE_DELETE(p_m_vars);
}


//p_var_is can be null if you're sure this is not a virtual function
//that actually links to C++ code.
//starting op should be 0, unless you want to skip some part of
//the function. (used in the wait() type statements)

//returns true if they should quit out
bool CScriptFunction::RunFunction(CScriptPassVars *p_var_in, int i_starting_op)
{

	if (m_p_func)
	{
		//here we should really check our p_var in types against our p_var
		//and make sure they are compatible

		m_p_func(p_var_in);
		//all done
		

		return false;
	}


	//if we got here we must have our internal code

	for (int i=i_starting_op; i < int(m_op.size()); i++)
	{
		//run this op code
		if (m_op[i]->GetOp() == C_SCRIPT_OP_NONE)
		{
			//log_msg("Running no op. %d",i);
			continue; //ignore this
		}

		if (GetNestLock() != -1)
		{
			if (GetNestLock() < m_op[i]->GetNesting())
			{
				//we're not supposed to show this
						continue; //ignore this

			} else
			{
				if (GetNestLock() >= m_op[i]->GetNesting())
				{
					//reached the end of the loop, we can go back to normal.
					SetNestLock(-1); //no lock active
				}

			}

		}

		if (m_op[i]->GetOp() == C_SCRIPT_OP_GOTO)
		{
			int i_goto_target = -1;
			if (m_op[i]->GetControl() == ::C_SCRIPT_CONTROL_GOTO_BY_WCHAR)
			{
				//the slow way of using goto.
				i_goto_target = GetGotoAnchorIdByName(&m_op[i]->parms[0]);
				//in the future let's use a direct goto instead of this string searching crap
				m_op[i]->SetControl(C_SCRIPT_CONTROL_GOTO_BY_INT);
				m_op[i]->parms[0].SetVar(C_SCRIPT_VAR_INT, i_goto_target, 0, NULL);
			
			} else
			{
				//assume Control is goto by int
				i_goto_target = m_op[i]->parms[0].GetInt();
			}



			if (i_goto_target == -1)
			{
				LogError("Goto command missing target.");
				continue;
			}


			    //actually move there
				this->m_i_nest_lock = -1; //reset any nesting thing
				i = i_goto_target;
				continue;
		}



		if (m_op[i]->GetControl() == ::C_SCRIPT_CONTROL_IF)
		{
			//we're going to do an if comparison.
   				if (!m_op[i]->parms[0].If(&m_op[i]->parms[1], m_op[i]->GetOp(), this->GetName()))
				{
					//failed the IF test.  lose next level
					SetNestLock(m_op[i]->GetNesting());
				
				}
		//either way we're done
			continue; //ignore this


		}



		if (m_op[i]->GetOp() != C_SCRIPT_OP_FUNCTION)
		{
			CScriptVarBase * p_mod_var = &m_op[i]->parms[0]; //default thing to use 
			
			if (m_op[i]->GetFunctionScript() != ::C_SCRIPT_NO_FUNCTION)
			{
				//the main goal here is to change a variable.  But we will set the this  variable to the
				//output of a function.  So let's run the function first, then copy it's output to parm0, which
				//is what the set/mod commands always use.
				
					//we're just supposed to run a function plain and simple.  Return value not important.
			CScriptPassVars vars; //build a custom var list for it
           	vars.i_script = this->m_p_parent_script->GetId();
		
			for (int v=0; (v < C_SCRIPT_MAX_PASS) && (m_op[i]->parms[v].GetType() != C_SCRIPT_VAR_NONE); v++)
			{
				vars.a_vars[v].SetType(m_op[i]->parms[v].GetBaseType()); //duplicate the type of var
				vars.a_vars[v].Set(&m_op[i]->parms[v]); //actually copy the vars over
				if (vars.a_vars[v].GetType() == ::C_SCRIPT_VAR_WCHAR)
				{
					ExpandWS(&vars.a_vars[v]); //convert variables into what they are supposed to be
				}
			}

			//run the function
			//log_msg("Running op func %s",g_p_script_engine->GetScript(m_op[i]->GetFunctionScript())->GetFunction(
			//	m_op[i]->GetFunctionId())->GetName() );
			g_p_script_engine->GetScript(m_op[i]->GetFunctionScript())->GetFunction(
				m_op[i]->GetFunctionId())->RunFunction(&vars, 0);
			
			//function has been run.  let's force the following MOD commands to use vars.return instead
			//of parm0.
		   	p_mod_var = &vars.return_var;
			 //now let it continue with the set/mod command using the new value.
		
			 	if (m_op[i]->GetOp() == C_SCRIPT_OP_ADD)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_ADD);
				continue;
			}
		
			if (m_op[i]->GetOp() == C_SCRIPT_OP_SUBTRACT)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_SUBTRACT);
				continue;
			}
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_MULTIPLY)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_MULTIPLY);
				continue;
			}
			if (m_op[i]->GetOp() == C_SCRIPT_OP_MODULUS)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_MODULUS);
				continue;
			}

	
			if (m_op[i]->GetOp() == C_SCRIPT_OP_DIVIDE)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_DIVIDE);
				continue;
			}
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_SET)
			{
				
				m_op[i]->return_var.Set(p_mod_var);
				
				//if it's a string let's go ahead and de token it, might change this later to
				//be a certain feature that does it
				continue;
			}

			
			
			} else
			{
			
			
			
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_ADD)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_ADD);
				continue;
			}
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_SUBTRACT)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_SUBTRACT);
				continue;
			}
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_MULTIPLY)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_MULTIPLY);
				continue;
			}
			
				if (m_op[i]->GetOp() == C_SCRIPT_OP_MODULUS)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_MODULUS);
				continue;
			}
			
			if (m_op[i]->GetOp() == C_SCRIPT_OP_DIVIDE)
			{
				//the function is smart enough to figure out if they are compatible
				//data types or not and do conversions
				m_op[i]->return_var.Modify(p_mod_var, C_SCRIPT_OP_DIVIDE);
				continue;
			}
		
			if (m_op[i]->GetOp() == C_SCRIPT_OP_SET)
			{
				
				m_op[i]->return_var.Set(p_mod_var);
				
				//if it's a string let's go ahead and de token it, might change this later to
				//be a certain feature that does it
				continue;
			}
			}
		}
		
	   if (m_op[i]->GetOp() == C_SCRIPT_OP_FUNCTION)
		{
			//we're just supposed to run a function plain and simple.  Return value not important.
			CScriptPassVars vars; //build a custom var list for it

			for (int v=0; (v < C_SCRIPT_MAX_PASS) && (m_op[i]->parms[v].GetType() != C_SCRIPT_VAR_NONE); v++)
			{
				vars.a_vars[v].SetType(m_op[i]->parms[v].GetBaseType()); //duplicate the type of var
				vars.a_vars[v].Set(&m_op[i]->parms[v]); //actually copy the vars over
				if (vars.a_vars[v].GetType() == ::C_SCRIPT_VAR_WCHAR)
				{
					ExpandWS(&vars.a_vars[v]); //convert variables into what they are supposed to be
				}
			}

			//let's let them know who called them too.
				
			if (m_op[i]->GetFunctionScript() == this->m_p_parent_script->GetId())
			{
			//	log_msg("telling it to come back to %s..", GetName());
				//we're calling a local function so let's save some info in it.
				this->m_p_parent_script->GetFunction(m_op[i]->GetFunctionId())->SetSavedParentFunction(m_i_id, i+1);
			}
			vars.i_script = this->m_p_parent_script->GetId();
			//run the function
			bool b_wait = false;
			if (g_p_script_engine->GetScript(m_op[i]->GetFunctionScript())->GetFunction(
				m_op[i]->GetFunctionId())->RunFunction(&vars, 0))
			{
				//we were told to quit directly out, from a wait() statement of some sort
				//probably..
			   return true;
			} else
			
			//one special case to check for - are we supposed to force a wait about now?

			if (vars.return_var.GetType() == ::C_SCRIPT_VAR_FORCE_WAIT)
			{
				this->m_p_parent_script->SetWait(vars.return_var.GetInt(), this->m_i_id, i+1);
				return true;
			}

		
			continue;
		}

	   
	   if (m_op[i]->GetControl() == C_SCRIPT_CONTROL_RETURN)
	   {
		   break; //exit this function right now
	   }
	   
	   //if we got here it's an unhandled op
		LogError("Unhandled OP of %d.",m_op[i]->GetOp());

	}

	//ok we're all done.  But were we supposed to remember to call someone else back
	//about now?
	 
	if (i_starting_op != 0)
	{
		//entry was iregular. Let's go to a saved position if we have one.
		CallSavedParentFunction();
		return false;
	}
	
	return false;
}



void CScriptFunction::SetSavedParentFunction(int i_func, int i_op)
{
	m_i_callback_function = i_func;
	m_i_callback_op = i_op;
}


void CScriptFunction::CallSavedParentFunction()
{
	if (m_i_callback_function == -1){return;} //no callback set
	int i_temp = m_i_callback_function;
	m_i_callback_function = -1; //clear it
	this->m_p_parent_script->GetFunction(i_temp)->RunFunction(NULL, m_i_callback_op);
	
}
int CScriptFunction::CreateOp()
{
  
   CScriptOp *p_op = new CScriptOp();
   if (!p_op)
   {
	   LogError("Error allocating memory for op code in CScriptFunction.");
	   return -1;
   }

   m_op.push_back(p_op);
   return m_op.size()-1;
}

int CScriptFunction::CountRequiredParms()
{
	//how many parms do we require?
	for (int i=0; i < C_SCRIPT_MAX_PASS; i++)
	{
		if (this->p_m_vars->a_vars[i].GetType() == ::C_SCRIPT_VAR_NONE)
		{
			//done counting
			return i;
		}

	}
  //if we got here they are all full
	return C_SCRIPT_MAX_PASS;
}  

bool CScriptFunction::ExpandWS(CScriptVarBase * p_var)
{
    //cycle through and replace any variables, the problem is some variables may be bigger than
   //the ascii variable name so we also have to resize the string.
   static WCHAR ws_work[C_SCRIPT_MAX_WCHAR_SIZE];
   
   memset(ws_work, 0, C_SCRIPT_MAX_PASS*2);

   //will hold the vars we find temporarily so we can look them up. static for speed.

   static WCHAR ws_var[C_SCRIPT_MAX_FUNCTION_NAME_SIZE];

   
   WCHAR *p_orig = p_var->GetWChar(); //the original string.
   int i_work_cur = 0;
  
   if (p_orig == 0)
   {
	   //um.. the thing is empty.  Not much we can do here.
	   return true;
   }
   
   //go through it line by line
   for (int i_cur=0; p_orig[i_cur] != 0; i_cur++ )
   {
	   /*
	   if ( (p_orig[i_cur] == L'\"') && (p_orig[i_cur+1] == L'\"'))
		{
		  //change double quotes to quotes
		  ws_work[i_work_cur] = p_orig[i_cur];
		  i_cur++;
		  i_work_cur++;
		  continue;
		}
		 */
	      if ( (p_orig[i_cur] == L'\\') && (p_orig[i_cur+1] == L'\\'))
		{
		  // change \\ to just
		  ws_work[i_work_cur] = L'\\';
		  i_cur++;
		  i_work_cur++;
		  continue;
		}

	   	  
	      if ( (p_orig[i_cur] == L'\\') && (p_orig[i_cur+1] == L'n'))
		{
		  //change \n to a real line feed
		  ws_work[i_work_cur] = L'\n';
		  i_cur++;
		  i_work_cur++;
		  continue;
		}

	      if ( (p_orig[i_cur] == L'\\') && (p_orig[i_cur+1] == L'\"'))
		{
		  //change a blackslash quote to a quote
		  ws_work[i_work_cur] = L'\"';
		  i_cur++;
		  i_work_cur++;
		  continue;
		}


	   if (p_orig[i_cur] == L'$')
	  {
		  //looks like we found a var of some type.
	      //advance by one
		  i_cur++;
		  if (p_orig[i_cur] == L'$')
		  {
			  //no, there are two $'s in a row, convert it to a regular money symbol.
			  ws_work[i_work_cur] = p_orig[i_cur];
			  i_work_cur++;
			  continue;
		  }

		//it's definately a var of some kind.  Let's grab it.
		  int i_var_cur = 0; //add chars to our variable name we're finding
		
		while ( (p_orig[i_cur] != L'$') && (p_orig[i_cur] != 0))
		{
			ws_var[i_var_cur] = p_orig[i_cur];
			i_var_cur++;
			i_cur++;
		}

		//add ending NULL to the var
		ws_var[i_var_cur] = 0;
		//ok, got var. (hopefully) Can we find it though?
	//	 log_msg("Found Var %s.", uni(ws_var).to_st());
  
		//first check vars from this function:
		 int i_var_script = m_p_parent_script->GetId(); //get the script id this function is in
		 int i_var_index = g_p_script_engine->GetScript(i_var_script)->FindVarByNameWS(ws_var);
		 if (i_var_index == -1)
		 {
			 //couldn't find it there, let's check the globals.
			 i_var_script = 0; 
			 i_var_index = g_p_script_engine->GetScript(i_var_script)->FindVarByNameWS(ws_var);
		 }
		 if (i_var_index == -1)
		 {
			 //still couldn't match up the var. give up.
			 LogError("Unable to find string token %s in script %s.", uni(ws_var).to_st(),
				 this->m_p_parent_script->GetName());
		 } else
		 {
			 //located the var!  Let's add it.
			// log_msg("Located var as index %d.", i_var_index);
			 
			 //ok, here we send our work pointer at the current point over to get the variable overwritten
			 //into it.  It will return the size it wrote.  This way we can fast forward to the end of it
			 //and keep adding.
			  i_work_cur += g_p_script_engine->GetScript(i_var_script)->GetVar(i_var_index)->AppendToWChar(&ws_work[i_work_cur]);
			
			 //finished adding it.  Continue with regular programming.
			  continue;
		 }


	  } else
	  {
		  //no var here, just copy whatever it is.
		  ws_work[i_work_cur] = p_orig[i_cur];
		  i_work_cur++;
	  }

   }
   
   //add final null to our work string.
   ws_work[i_work_cur] = 0;

   //set our var to it
   p_var->SetVar(p_var->GetType(), 0, 0, ws_work);

   return true; //success

}


int CScriptFunction::GetGotoAnchorIdByName(CScriptVarBase * p_var)
{
	//cycle through all ops and find the goto target
	for (unsigned int i=0; i < this->m_op.size(); i++)
	{
		if (m_op[i]->GetControl() == ::C_SCRIPT_CONTROL_GOTO_ANCHOR)
		{
			//this is a goto anchor.  The right one?
			if (p_var->If(&m_op[i]->parms[0], C_SCRIPT_OP_IF_EQUALS, GetName()))
			{
				//match made.
				return i;
			}

		}


	}

	//couldn't find it. 
	return -1;

}