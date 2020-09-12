/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CScriptParse.h"
#include "CScript.h"
#include "uni.h"
#include "unicode_utils.h"
#include "CScriptEngine.h"

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CScriptParse::CScriptParse()
{
	m_i_nest = -1;  //none
	m_i_script = -1;
	m_i_cur_function = C_SCRIPT_NO_FUNCTION; //not in a function right now
	m_i_line_count = 0;
}


CScriptParse::~CScriptParse()
{

}
void CScriptParse::ParseLineWC(WCHAR *p_line)
{
	m_i_line_count++;

	//first strip beginning spaces
	p_line = strip_spaces_from_pointer_wc(p_line);
	
	//is this line a comment?
	if (p_line[0] == L'/') if (p_line[0] == L'/') return; //ignore this whole line
	if (p_line[0] == 0) return; //it's null

	if (m_i_cur_function == C_SCRIPT_NO_FUNCTION)
	{
		//not inside a function right now, so we can assume the thing will be a
		//function header

	if (wcsnicmp(p_line, L"void", 4) == 0)
	{
//		log_msg("Nesting at %d.", m_i_nest);
	  //isolate function name
	  p_line += 4; //pass the void part
	  p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
		
	  WCHAR *p_end = wcschr(p_line, L'(');
	  if (!p_end)
	  {
		  LogError("Couldn't read function name %s in %s at line %d.",uni(p_line).st_data, ScriptName(),
			  m_i_line_count);
		  return;
	  }

	  p_end[0] = 0; //null terminate this, we'll ignore the rest for now, but later
	  
	  //backup in case they had spaces after the function name
	  while (p_end[-1] == L' ') 
	  {
		  p_end--;
		  p_end[0] = 0;
	  }
	  
	  //we may add checking for vars to pass
	  CScriptPassVars *p_vars = new CScriptPassVars();
	  if (!p_vars)
	  {
		  LogError("Error getting memory for CScriptPassVars in CScriptParse.");
		  return;
	  }

	  this->m_i_cur_function = m_p_engine->GetScript(m_i_script)->AddFunction(uni(p_line).st_data, NULL, p_vars);
	  this->m_i_nest = -1; //hasn't started yet, will when we hit the first {
	  return;
	
	} else
	{
		//what is this???  var declaration not in a function?  Ok, might as well allow those.
		
		LogError("Expected to find a function header, not %s in script %s at line %d.",
		uni(p_line).st_data, ScriptName(), m_i_line_count);
		return;
	}
	}

	//we got here

	if (m_i_nest == -1)
	{
		//we're waiting for a { or nothing
		if (p_line[0] == L'{')
		{
			m_i_nest++;
//			log_msg("Nesting at %d.", m_i_nest);
			return;
		} 
			LogError("Expected a { %s.", ErrorInfo());
			return;
	}

	if (p_line[0] == L'}')
	{
		m_i_nest--;
		if (m_i_nest == -1)
		{
			//we just exitted a function it looks like.
			m_i_cur_function = -1;
		}
		return;
	} 
  		if (p_line[0] == L'{')
		{
			m_i_nest++;
//			log_msg("Nesting at %d.", m_i_nest);
			return;
		} 
		
	if (wcsnicmp(p_line, L"goto ", 5) == 0)
	{
		p_line += 5; //skip the if and first space
	    p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
		
	    WCHAR * p_test = wcspbrk_ignore_inside_strings(p_line, L" ;");
 		
		if (p_test)
		{
		
		p_test[0] = 0; //cut off just the name part
		}
		//log_msg("Adding goto %s command.", uni(p_line).to_st());
		
		int i_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->CreateOp();
		//get pointer to the opcode we just made
		CScriptOp *p_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->m_op[i_op];
		p_op->SetNesting(m_i_nest);
		p_op->SetOp(C_SCRIPT_OP_GOTO);
		p_op->SetControl(C_SCRIPT_CONTROL_GOTO_BY_WCHAR);
		p_op->parms[0].SetVar(::C_SCRIPT_VAR_WCHAR, -1, 0, p_line); 
		//remember this as a string, later we'll change it to a direct look up during the next phase.
		
		return;
	}

		if (wcsnicmp(p_line, L"return", 6) == 0)
	{
		p_line += 6; //skip the if and first space
	    p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
		
		if (p_line[0] == L';')
		{
			//this is definatly a command to just exit out of here
		 	int i_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->CreateOp();
		//get pointer to the opcode we just made
		CScriptOp *p_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->m_op[i_op];
		p_op->SetNesting(m_i_nest);
		p_op->SetOp(C_SCRIPT_OP_SPECIAL);
		p_op->SetControl(C_SCRIPT_CONTROL_RETURN);
		return;
		}

	 	
	
	}

	

	 //ok, let's assume this is an actual line of code we want to add
	if (wcsnicmp(p_line, L"if ", 3) == 0)
	{
		//for an if statement, we evaluate two vars.  If false, we set the "ignore next {" flag for the function
		//itself.
		p_line += 3; //skip the if and first space
	    p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist

//		log_msg("Found IF statement");
		ParseIfStatement(p_line);
		return;
	}

	//hod, hod it!  Is this a label declaration?

   WCHAR * p_test = wcspbrk_ignore_inside_strings(p_line, L":");
   if (p_test)
   {
	   //the only possible way there would be a : not in quotes is if this is a goto 
     p_test = wcspbrk_ignore_inside_strings(p_line, L" :");
	 p_test[0] = 0; //ok, we've isolated the anchor name itself.
//	 log_msg("Found goto anchor %s.", uni(p_line).to_st());

   	int i_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->CreateOp();
		//get pointer to the opcode we just made
		CScriptOp *p_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->m_op[i_op];
		p_op->SetNesting(m_i_nest);
		p_op->SetOp(C_SCRIPT_OP_NONE); //not actually run
		p_op->SetControl(C_SCRIPT_CONTROL_GOTO_ANCHOR);
		p_op->parms[0].SetVar(::C_SCRIPT_VAR_WCHAR, -1, 0, p_line); 
		//remember this as a string, later we'll change it to a direct look up during the next phase.
		return;
   }


	if (TestAndCreateVars(p_line))
	{
		return;
	}


	//let's assume it's simply calling a function
	ParseOp(p_line);

} 

bool CScriptParse::VarDefinationExists(WCHAR **p_line, WCHAR *p_type)
{
	if (wcsnicmp(*p_line, p_type, wcslen(p_type)) == 0)
	{
	   *p_line += wcslen(p_type);
	   return true; //found it and move the  p_line pointer past it
	}

 return false; //didn't find that var type here. go fish
}



//look for int/varname and do some stuff with it

bool CScriptParse::TestAndCreateVars(WCHAR *p_line)
{
   	
	int i_var_type = C_SCRIPT_VAR_NONE;
	int i_temp_script = m_i_script;

	if (wcsnicmp(p_line, L"global ",7) == 0)
	{
		//omg!  This should be defined in the script 0, so all scripts can see it.
	   i_temp_script = 0; 
	   p_line = &p_line[7]; //skip ahead
	}
	
	
	if (VarDefinationExists(&p_line, L"int ")) i_var_type = C_SCRIPT_VAR_INT;
	if (VarDefinationExists(&p_line, L"float ")) i_var_type = C_SCRIPT_VAR_FLOAT;
	if (VarDefinationExists(&p_line, L"string ")) i_var_type = C_SCRIPT_VAR_WCHAR;
	if (VarDefinationExists(&p_line, L"vector3 ")) i_var_type = C_SCRIPT_VAR_VECTOR3;


	if (i_var_type != ::C_SCRIPT_VAR_NONE)
	{
		//we did find a var defination here.

	   p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
	
		//let's go ahead and declare it for them.
		
	  WCHAR *p_end = wcspbrk(p_line, L" =;");
	  if (!p_end)
	  {
		  LogError("Missing ; or = while declaring a variable. (%s) %s", uni(p_line).st_data, ErrorInfo());
		  return true; //signal error
	  }
	 
 	    //isolate just the variable name
	   unsigned short wchar_temp = p_end[0]; //save this
	   p_end[0] = 0; //NULL it

	   //make the var
	   
//	   log_msg("Just added var %s.", uni(p_line).to_st());
	   m_p_engine->GetScript(i_temp_script)->AddVar(uni(p_line).to_st(), i_var_type);
	   
	   //put back the temp char we moved to null it
	   p_end[0] = wchar_temp;
	   //did they also want to assign it a default value?
	 
	   if (wcspbrk(p_line, L"="))  //all ops must have an equal in them for this to work!!! bad design? yes!!
	   {
		   //there is an equals in there, let's run this through our parser now that the var is taken care
		   //of and we chopped the int part off.
	   p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
	   ParseOp(p_line);
	   }

	   return true; //we made a var.
	}

   return false; //we didn't do anything
}



char * CScriptParse::ScriptName()
{
	return m_p_engine->GetScript(m_i_script)->GetName();
}

char * CScriptParse::ErrorInfo()
{
	static char st_temp[256];

	sprintf(st_temp, "(File: %s - Line %d)", ScriptName(),m_i_line_count);
	return (char*)&st_temp; //safe because it's static
}


void CScriptParse::ParseOp(WCHAR *p_line)
{
	//make op
	int i_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->CreateOp();
	//get pointer to the opcode we just made
	CScriptOp *p_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->m_op[i_op];
	p_op->SetNesting(m_i_nest);

	WCHAR *p_search = p_line; //our temp pointer for our search

	bool b_in_string = false;

	//log_msg("Searching %s", uni(p_line).to_st());

	//let's search for a predefined OP but let's ignore anything we find inside a string
	
	for (int i_cur = 0; p_search[i_cur] != 0; i_cur++)
	{
		
		//before processing.. wait.  Is this the start of a string?
		if (p_search[i_cur] == L'\"')
		{
			//we found a quote. if there isn't another quote following it right away, it's a real
			//string start or stop.
			if (p_search[i_cur+1] == L'\"')
			{
				//false alarm, there are two quotes here, ignore.
				i_cur++; //jump past both of them
			} else
			{
				//this is a real start or ending of a string.
				if (!b_in_string)
				{
					b_in_string = true;
				} else
				{
					b_in_string = false;  
					continue;

				}
			}
		}
		
		//only do these checks if we are NOT in the middle of a string
		if (!b_in_string)
		{
			if (wcsnicmp(&p_search[i_cur], L"+=", 2) == 0) 
			{
				SetupSetOp(p_op, p_line, L"+=", C_SCRIPT_OP_ADD);
				return;
			}
			
			if (wcsnicmp(&p_search[i_cur], L"-=", 2) == 0) 
			{
				SetupSetOp(p_op, p_line, L"-=", C_SCRIPT_OP_SUBTRACT);
				return;
			}
			
			if (wcsnicmp(&p_search[i_cur], L"*=", 2) == 0) 
			{
				SetupSetOp(p_op, p_line, L"*=", C_SCRIPT_OP_MULTIPLY);
				return;
			}
			
			if (wcsnicmp(&p_search[i_cur], L"/=", 2) == 0) 
			{
				SetupSetOp(p_op, p_line, L"/=", C_SCRIPT_OP_DIVIDE);
				return;
			}
			if (wcsnicmp(&p_search[i_cur], L"%=", 2) == 0) 
			{
				SetupSetOp(p_op, p_line, L"%=", C_SCRIPT_OP_MODULUS);
				return;
			}
			
			//is this a special opcode?
			if (wcsnicmp(&p_search[i_cur], L"=", 1) == 0) 
			{
				SetupSetOp(p_op, p_line, L"=", C_SCRIPT_OP_SET);
				return;
			}
		}
		
		
		
	}
	
	
	  
		//must be a function, looks like we don't care about the return value though.
	 p_op->SetOp(C_SCRIPT_OP_FUNCTION);
	 
	 SetupSetSource(p_op, p_line);
	//uni test(p_line);
	//log_msg("%s",test.st_data);

}


void CScriptParse::SetupSetOp(CScriptOp * p_op, WCHAR *p_line, WCHAR *p_wst_op, int i_op_type)
{
		
		WCHAR *p_search;
		WCHAR * p_start = p_line;  //remember this for later
	//	log_msg("Setting op with %s. Op is %s witha length of %d.", uni(p_line).to_st(), uni(p_wst_op).to_st(), 
	//		wcslen(p_wst_op));

		p_search = wcsstr(p_line, p_wst_op);
		
		//it's something = something.
		p_op->SetOp(i_op_type);
	 	
		p_line = p_search+wcslen(p_wst_op); //the original line should now hold "what to put in there" for later
	
		
		p_search[0] = 0;  //this truncates wst_temp at the = mark, but we still need to kill the
		//trailing spaces

		for (int i=-1; p_search[i] == L' '; i--) p_search[i] = 0;
		
		
		int i_var; //the id of the var, it has to pre exist.
		int i_var_script; //script this var can be found in
		//find the var, search our local vars first
		if ( (i_var = m_p_engine->GetScript(m_i_script)->FindVarByNameWS(p_start)) != -1)
		{
		   //found it.
			i_var_script = m_i_script;
		}
		
		if (i_var == -1)
		{
			//didn't find the var.
			//let's look in our globals too since we didn't find it (we assume script 0 is globals)
		    i_var = m_p_engine->GetScript(0)->FindVarByNameWS(p_start);
		    i_var_script = 0; 
		}
	

		 if (i_var == -1)
		 {
			 //hmm.  Still couldn't find it.
			 p_op->SetOp(C_SCRIPT_OP_NONE); //disable this op
			 LogError("%s is an undeclared variable. %s", uni(p_start).to_st(), ErrorInfo());
			 return;
		 }
	 
		 //found it!
		 
		 //set target
		 p_op->return_var.SetVar(C_SCRIPT_VAR_DYNAMIC, i_var_script, i_var, NULL);
		 p_line = strip_spaces_from_pointer_wc(p_line);
		 

		 //ok, we know what to change but what should we change it to? A function or var?
		 SetupSetSource(p_op, p_line);
}


//search for any of the chars including while ignoring anything inside of quotes.
//returns location of match or NULL if none.

WCHAR * wcspbrk_ignore_inside_strings( WCHAR* p_search, WCHAR *p_chars)
{
    int i_chars_to_check = wcslen(p_chars);

	bool b_in_string = false;

	//let's search for a predefined OP but let's ignore anything we find inside a string
	
	for (int i_cur = 0; p_search[i_cur] != 0; i_cur++)
	{
		
		//before processing.. wait.  Is this the start of a string?
		if (p_search[i_cur] == L'\"')
		{
			//we found a quote. 
			if ( (i_cur > 0) && (p_search[i_cur-1] == L'\\'))
			{
				//false alarm, there are two quotes here, ignore.
				//i_cur++; //jump past both of them
			    continue;
			} else
			{
				//this is a real start or ending of a string.
				if (!b_in_string)
				{
					b_in_string = true;
				} else
				{
					b_in_string = false;
				   continue; //otherwise the quote will get checked.
				}
			}
		}
		
		//only do these checks if we are NOT in the middle of a string
		if (!b_in_string)
		{
			for (int u=0; u < i_chars_to_check; u++)
			{
				if (p_search[i_cur] == p_chars[u])
				{
					//found a match.
					return &p_search[i_cur];
				}
			}
			
		}
		
		
		
	}
	

	return NULL; //didn't find it
}

//this expects a string with a variable or function, it will add the function call/var to
//the op code.

void CScriptParse::SetupSetSource(CScriptOp *p_op, WCHAR * p_line)
{

	WCHAR *p_search;

	//if a ( exists and it's NOT in a string, this is a function not a var or constant.

	 
	 p_search = wcspbrk_ignore_inside_strings((WCHAR*)p_line, L"(");
	
	 if (!p_search)
	 {
		 
		 //it's not a function, so it's a variable or constant declaration.  Let's let our boys
		 //handle it.
		 
		 //truncate to cut off extra spaces or ;
		 p_search = wcspbrk_ignore_inside_strings((WCHAR*)p_line, L" ;\"");
		 
		 
		 if (p_search)  p_search[0] = 0;
		 
		 if (!ConvertWCStringToBaseVar(&p_op->parms[0], p_line))
		 {
			p_op->SetOp(C_SCRIPT_OP_NONE); //disable this op
			return; //error
		 }

		 //success
		 return;
	 }

	 //ok, if we got here then it is pretty obvious this is a function.

     //we need to get the function name then break it down into pieces
	 
	
     p_search = wcspbrk(p_line, L" (");

     
     //find the cutoff the funtion name
     if (!p_search)
     {
       	 LogError("Error reading function name. %s",  ErrorInfo());
	    return;
     }
     
     p_search[0] = 0; //truncate the line, don't worry, we'll fix it in a sec
     _wcsupr(p_line); //upcase it
     uni un_func(p_line);

     //advance pline to pas this point..
     p_line = p_search+1;

	 //get this functions script and index
	 int i_function_script = m_i_script;
	 int i_function_id = m_p_engine->GetScript(i_function_script)->FindFunctionByName(un_func.st_data);

	 if (i_function_id == -1)
	 {
		 //also check the global functions
	 i_function_script = 0;
	 i_function_id = m_p_engine->GetScript(i_function_script)->FindFunctionByName(un_func.st_data);
	 }


	 if (i_function_id == -1)
	 {
		 //still couldn't locate the function, a pox on us.
		 p_op->SetOp(C_SCRIPT_OP_NONE); //disable this op
		 LogError("%s is an undeclared function. %s", un_func.st_data, ErrorInfo());
		 return;
	 }



// log_msg("Adding call to function %s, script %d id %d.", uni_func.to_st(), i_function_script, i_function_id);

	 p_op->SetFunction(i_function_script, i_function_id);

	 //remove any leading spaces
	  p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
	
	  //are there any functions at all?
	  if (p_line[0] == L')')
	  {
		  //no,  no parms given.
		  if (m_p_engine->GetFunctionParmCount(i_function_script, i_function_id) > 0)
		  {
	  	    p_op->SetOp(C_SCRIPT_OP_NONE); //disable this op
			LogError("%s requires %d parms, not 0. %s", un_func.st_data, m_p_engine->GetFunctionParmCount(i_function_script, i_function_id),
				ErrorInfo());
			return;

		  }
		  
		  //success
		  return;
	  }
	 
	  //now let's add all the parms, or leave now if it has none.
	 if (!ParseParmsIntoOp(p_op, p_line))
	 {
		 //error occured, better to ignore this function call all-together 
		 p_op->SetOp(C_SCRIPT_OP_NONE); 
		 return;
	 }

}

//this function assumes it's getting WCHAR format string of function headers 
//must not have spaces before, example is "( crap, "stuf")" is ok.
bool CScriptParse::ParseParmsIntoOp(CScriptOp *p_op, WCHAR *p_line)
{
	
	//remove the first ( if there is one
	if (p_line[0] == L'(')
	{
		p_line++;
		p_line = strip_spaces_from_pointer_wc(p_line); //strip more spaces if they exist
	}
	
	
//  log_msg("Processing var %s.", uni(p_line).to_st());
	 
	 WCHAR * p_word;
	 int i_parm_count = 0;
	 while (p_line[0]) //haha, I hope this doesn't bite me in the ass later
	 {
		 //kill any spaces in the beginning
		 while (p_line[0] == L' ')
		 {
			 p_line++;
		 }
		 
		 //if the next thingie is a commas, get rid of it,
		 if (p_line[0] == L',')
		 {
			 p_line++;
			 //strip spaces again just in case
			 while (p_line[0] == L' ')
			 {
				 p_line++;
			 }
		 }
		 
		 //ok, copy this parm or whatever it is into p_word.
		
		 p_word = &p_line[0];
		 //first find the end
		 WCHAR *p_end = wcspbrk_ignore_inside_strings(p_line, L", );");

		 if (!p_end)
		 {
			 p_line[0] = 0;
			 continue;
		 }
		  
		 p_end[0] = 0; //ok, p_word is truncated now.
		 p_line = &p_end[1];

		 if (p_word[0] == 0)
		 {
			 //we're all done it appears.
			 continue;
		 }
		if (!ConvertWCStringToBaseVar(&p_op->parms[i_parm_count], p_word))
		{
			//error extracting var
		 return false; //error

		}
	   
		//move our pointer to the next word
		
		i_parm_count++;
	 }

	 //ok, we read all the parms without incident. So do the parms match or what?
	 if (m_p_engine->GetScript(p_op->GetFunctionScript())->GetFunction(p_op->GetFunctionId()
		 )->CountRequiredParms() != i_parm_count)
	 {
	 	 //parm count doens't match.
		 LogError("Function %s requires %d parms, not %d. %s", 
			 m_p_engine->GetScript(p_op->GetFunctionScript())->GetFunction(p_op->GetFunctionId())->GetName(),
			m_p_engine->GetScript(p_op->GetFunctionScript())->GetFunction(p_op->GetFunctionId()
		 )->CountRequiredParms(), i_parm_count, ErrorInfo());
	    return false; //error
	 }
    

  //how about we compare the var types and make sure they are compatible?

	 for (int i=0; i <i_parm_count; i++ )
	 {
		 int i_rec_type = m_p_engine->GetScript(p_op->GetFunctionScript()
			 )->GetFunction(p_op->GetFunctionId())->GetVars()->a_vars[i].GetBaseType();
		 
		 int i_call_type = p_op->parms[i].GetBaseType();
		 
		 if ( i_rec_type != i_call_type)
		 {
			 //ops don't really match.  
		
             if (
                 (i_rec_type == C_SCRIPT_VAR_INT || C_SCRIPT_VAR_FLOAT)
                 &&     (i_call_type == C_SCRIPT_VAR_INT || C_SCRIPT_VAR_FLOAT)
                 )
             {
                 //however, if it's a float to int deal, don't worry about it too much
                 
                 
             }   else
             {
                 
 
			 LogError("Parm %d expecting a %s, not a %s in function %s. %s",
				 i, GetVarTypeString(i_rec_type), GetVarTypeString(i_call_type),
			m_p_engine->GetScript(p_op->GetFunctionScript()
			 )->GetFunction(p_op->GetFunctionId())->GetName(),	 ErrorInfo());
             }
		 }
	 }
	 
	 //also, let's check the return value for compatibilty.

			 int i_rec_type = m_p_engine->GetScript(p_op->GetFunctionScript()
			 )->GetFunction(p_op->GetFunctionId())->GetVars()->return_var.GetBaseType();
		 
		    int i_call_type = p_op->return_var.GetBaseType();

			/*
			
		 if ( i_rec_type != i_call_type)
		 {
			 
			 
			 //ops don't really match.  
		 LogError("Returning a %s, not a %s in function %s. %s",
				GetVarTypeString(i_rec_type), GetVarTypeString(i_call_type),
			m_p_engine->GetScript(p_op->GetFunctionScript()
			 )->GetFunction(p_op->GetFunctionId())->GetName(),	 ErrorInfo());
			 }

			  */

  return true; //success
}


//send this a constant or variable name, must be space trimmed already
bool CScriptParse::ConvertWCStringToBaseVar(CScriptVarBase *p_var, WCHAR *p_word)
{
	   
//log_msg("Figuring out what %s is.", uni(p_word).to_st());

		 if (WCharIsANumber(p_word[0]))
		 {
			 //it's just a number
			 
			 //cycle through and look for a period
			 bool b_is_a_float = false;
			 
			 for (int i=0;(p_word[i]) != 0; i++ )
			 {
				 if (!WCharIsANumber(p_word[i]))
				 {
					 if (p_word[i] == L'.')
					 {
						 //woah.  a period.  it's a float!
						 b_is_a_float = true;
						 break;
					 }	 else
					 {
						 //not a number and not a period.  let's abort this goosechase.
						 break;
					 }
				 }
			 }
			 
			 
			if (b_is_a_float) 
			{
			  //convert to float. can't find a wide char to float function so let's just
				//do this.
				p_var->SetVar(C_SCRIPT_VAR_FLOAT, 0, 0, NULL);
				p_var->SetFloat(float(atof(uni(p_word).to_st())));
				return true;
			}	else
			{
			 //handling for an int
			 p_var->SetVar(C_SCRIPT_VAR_INT, _wtoi(p_word), 0, NULL);
			 return true;
			}
		 }
	 
	   //if we got here that means it's a variable name or function (fuctions in a function not
		 //supported yet..but could be added...)

		//well.. it doesn't start with a number.  Maybe it starts with a string?

		 //first let's check for a special case - when they set a string to nothing.  We don't want the "" to be
		 //counted as a reak quotes character.
		 
		 if (p_word[0] == L'\"')
		 {
			 p_word++; //pass the first quote
			 //let's remove the quotes and plug it into the var
			 //is the last thing also a quote?  if not something is wrong
			 
			 //let's find the end
			 int i_size = 0;
			 for (; p_word[i_size] != 0; i_size++)
			 {
			
				 
				 if (p_word[i_size] == L'\\')
				 {
				   //special character we can ignore
				   i_size++;
				   continue;
				 }
				 
				 if (p_word[i_size] == L'\"')
				 {
					 //this might be the end...
						 break;
				 }
			 }

				 
			 if (p_word[i_size] != '\"')
			 {
				 //uh.. string isn't closed?
				 LogError("String %s missing closing quotes %s", uni(p_word).to_st(), ErrorInfo());
				return false;
			 }
			 
 			 //let's also remove the trailing quote
			 p_word[i_size] = 0;
	 	 //log_msg("Found parm %s.", uni(p_word).to_st());
	
			 //the var will copy it into its own malloc'ed buffer
			 p_var->SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, p_word);
			 return true; //success
		 }
		 

		 
		 int i_var; //the id of the var, it has to pre exist.
		int i_var_script; //script this var can be found in

		//find the var, search our local vars first
		if ( (i_var = m_p_engine->GetScript(m_i_script)->FindVarByNameWS(p_word)) != -1)
		{
		   //found it.
			i_var_script = m_i_script;
		}
		
		if (i_var == -1)
		{
			//didn't find the var.
			//let's look in our globals too since we didn't find it (we assume script 0 is globals)
		    i_var = m_p_engine->GetScript(0)->FindVarByNameWS(p_word);
			i_var_script = 0;
		
		}
	
		 if (i_var == -1)
		 {
			 //hmm.  Still couldn't find it.
			 LogError("%s is an undeclared variable. %s", uni(p_word).to_st(), ErrorInfo());
			 return false;
		 }

		 //at this point we can set the reference.
		 //we assume the source is always parm 0 for a simple statement like this. 
		 p_var->SetVar(C_SCRIPT_VAR_DYNAMIC, i_var_script, i_var, NULL);
	    
//	log_msg("Found parm %s.", uni(p_word).to_st());
		return true; //success
}


//this gets sent something like (i_crap == 2); and we have to decipher it and turn it into an op.

void CScriptParse::ParseIfStatement(WCHAR *p_line)
{

	if (p_line[0] != L'(')
	{
		LogError("Missing left parenthesis in if statement. (%s) %s.", uni(p_line).to_st(), ErrorInfo());
		return;
	}

	p_line++; //skip first parenthesis.
  	p_line = strip_spaces_from_pointer_wc(p_line);


	int i_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->CreateOp();
	//get pointer to the opcode we just made
	CScriptOp *p_op = m_p_engine->GetScript(m_i_script)->GetFunction(m_i_cur_function)->m_op[i_op];
	p_op->SetControl(C_SCRIPT_CONTROL_IF);
	p_op->SetNesting(m_i_nest);


	//first isolate first var

	//put it where it should go
	WCHAR * p_var_end;

	p_var_end = wcspbrk_ignore_inside_strings( p_line, L" ,=><!");

	if (p_var_end)
	{

	}	else
	{
		p_op->SetOp(C_SCRIPT_OP_NONE);
		LogError("Error in if statement. %s", ErrorInfo());
		return;
	}

	WCHAR char_temp = p_var_end[0];
	p_var_end[0] = 0; //detach the first var without losing what was there when we overwrite with the NULL

	ConvertWCStringToBaseVar(&p_op->parms[0], p_line);

	//put it back
	p_var_end[0] = char_temp;
	p_line = p_var_end;
	
	p_line = strip_spaces_from_pointer_wc(p_line);

	if (wcsnicmp(p_line, L"==", 2) == 0)
	{
		p_line += 2;
		p_op->SetOp(C_SCRIPT_OP_IF_EQUALS);
	} else
	if (wcsnicmp(p_line, L"!=", 2) == 0)
	{
		p_line += 2;
		p_op->SetOp(C_SCRIPT_OP_IF_NOT_EQUALS);
	} else
	if (wcsnicmp(p_line, L">=", 2) == 0)
	{
		p_line += 2;
		p_op->SetOp(C_SCRIPT_OP_IF_SAME_OR_MORE);
	} else
	if (wcsnicmp(p_line, L"<=", 2) == 0)
	{
		p_line += 2;
		p_op->SetOp(C_SCRIPT_OP_IF_SAME_OR_LESS);
	} else
	if (wcsnicmp(p_line, L"<", 1) == 0)
	{
		p_line += 1;
		p_op->SetOp(C_SCRIPT_OP_IF_LESS);
	} else
	if (wcsnicmp(p_line, L">", 1) == 0)
	{
		p_line += 1;
		p_op->SetOp(C_SCRIPT_OP_IF_MORE);
	} else
	
	{
 		p_op->SetOp(C_SCRIPT_OP_NONE);
		LogError("Unknown op in if statement. %s", ErrorInfo());
		return;
	}

  	p_line = strip_spaces_from_pointer_wc(p_line);

	p_var_end = wcspbrk_ignore_inside_strings( p_line, L" )");

	if (p_var_end)
	{

	}	else
	{
		p_op->SetOp(C_SCRIPT_OP_NONE);
		LogError("Error in if statement. %s", ErrorInfo());
		return;
	}

	p_var_end[0] = 0;
	
	//do another var check for the end
	ConvertWCStringToBaseVar(&p_op->parms[1], p_line);
	//log_msg("Made if statement with %s.", uni(p_line).to_st());

}
