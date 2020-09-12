/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "windows.h"
#include "CScriptVarBase.h"
#include "CScriptEngine.h"
#include <CAssert>

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CScriptEngine *g_p_script_engine;

void log_msg(LPCSTR lpFormat, ...);
void LogError(LPCSTR lpFormat, ...);


 char * GetVarTypeString(int i_var_type)
 {
	 //special hacks to let me use this safely twice in the same log_msg.
	 
	 static char st_name[10], st_name2[10];
	 static bool b_first = true;
	 char * p_st_target;

	 if (b_first)
	 {
		p_st_target = (char*)&st_name;
		b_first = false;
	 } else
	 {
		 p_st_target = (char*)&st_name2;
		 b_first = true;

	 }

	 if (i_var_type == C_SCRIPT_VAR_INT) strcpy(p_st_target, "int"); else
	 if (i_var_type == C_SCRIPT_VAR_FLOAT)  strcpy(p_st_target, "float"); else
	 if (i_var_type == C_SCRIPT_VAR_NONE)  strcpy(p_st_target, "none"); else
	 if (i_var_type == C_SCRIPT_VAR_DYNAMIC)  strcpy(p_st_target, "dynamic"); else
	 if (i_var_type == C_SCRIPT_VAR_WCHAR)  strcpy(p_st_target, "wchar"); else
	 if (i_var_type == C_SCRIPT_VAR_VECTOR3)  strcpy(p_st_target, "vector3"); else
	 if (i_var_type == C_SCRIPT_VAR_FORCE_WAIT)  strcpy(p_st_target, "wait"); else
	 				   
	 {
		 //couldn't figure out what kind of var it is.
		 strcpy(p_st_target, "unknown");
	 }

   return  p_st_target;
 }

 

CScriptVarBase::CScriptVarBase()
{
	m_i_type = C_SCRIPT_VAR_NONE; //default is not used
    this->m_p_extra = NULL; //if null we know we don't have to free anything...
}

CScriptVarBase::~CScriptVarBase()
{
  SAFE_FREE(m_p_extra);

}

int CScriptVarBase::GetInt()
{
	if (GetType() == ::C_SCRIPT_VAR_FLOAT) return int(GetFloat());
	return m_i_parm1;
};


void CScriptVarBase::SetFloat(float f_new)
{ 
	if (GetType() == C_SCRIPT_VAR_INT) 
	{
		SetInt(int(f_new));
		return;
	}
	*(float*)&m_i_parm1 = f_new;
}
void CScriptVarBase::SetInt(int i_new)
{
	if (GetType() == C_SCRIPT_VAR_FLOAT)
	{
		SetFloat(float(i_new));
	}
	m_i_parm1 = i_new;
}

float CScriptVarBase::GetFloat() 
{
	if (GetType() == C_SCRIPT_VAR_INT)
	{
		return (float(GetInt()));
	}
	
	return  *(float*)&m_i_parm1;
}

	
void CScriptVarBase::SetType(int i_new_type)
 {
	 m_i_type = i_new_type; 
	 SAFE_FREE(m_p_extra); //either, make sure this is empty
 }

WCHAR * CScriptVarBase::GetWChar()
{
	return (WCHAR*)m_p_extra;
}

void CScriptVarBase::SetVar(int i_type, int i_parm1, int i_parm2, void *p_extra)
{
   SetType(i_type);

   if (i_type == ::C_SCRIPT_VAR_INT)
   {
	   this->m_i_parm1 = i_parm1;
	   return;
   }
  
    if (i_type == ::C_SCRIPT_VAR_FLOAT)
   {
	   
	  *((float*)&m_i_parm1) = *((float*)&i_parm1);
	   return;
   }



   if (i_type == ::C_SCRIPT_VAR_DYNAMIC)
   {
	   this->m_i_parm1 = i_parm1; //script
	   this->m_i_parm2 = i_parm2; //var index
	   return;
   }

   if (i_type == ::C_SCRIPT_VAR_WCHAR)
   {
	   //making a string resource
	   if (p_extra)
	   {
		//also set a default
		int i_length = wcslen((wchar_t *)p_extra);

	   m_p_extra = realloc(m_p_extra, (i_length+1)*2); //alloc or realloc whatever was there if it's not NULL
	   wcscpy((WCHAR*)m_p_extra, (WCHAR*)p_extra); //copy the string over, including the null at the end
	   } else
	   {
		   //it was set to null.  Let's creat it anyway
		int i_length = 0;

	   m_p_extra = realloc(m_p_extra, (i_length+1)*2); //alloc or realloc whatever was there if it's not NULL
	   memset(m_p_extra, 0, 2); //copy two blank chars to be the null	   
 
	   }
   }

   
   //used for causing delays in excecution.  Done this way so outside sources can
   //control it.  Another method is set it to 9999999999999 and force a continue
   //yourself later from an outside source.
   
   if (i_type == ::C_SCRIPT_VAR_FORCE_WAIT)
   {
	   this->m_i_parm1 = i_parm1;
	   return;
   }


} 


bool CScriptVarBase::Modify(CScriptVarBase *p_new, int i_mod_type)
{

 	if (GetType() == C_SCRIPT_VAR_DYNAMIC)
	{
		//let's actually change the var we should be pointing too
		return g_p_script_engine->GetScript(this->m_i_parm1)->GetVar(m_i_parm2)->Modify(p_new, i_mod_type);
	}
	
   if (p_new->GetType() == C_SCRIPT_VAR_DYNAMIC)
   {
	   //call us with the real var, not this crap
	   return Modify(g_p_script_engine->GetScript(p_new->m_i_parm1)->GetVar(p_new->m_i_parm2), i_mod_type);
   }

   	if (this->GetType() == p_new->GetType())
	{
	
		//no conversion required, just copy the var over
		if (m_i_type == C_SCRIPT_VAR_VECTOR3)
		{
			if (i_mod_type == ::C_SCRIPT_OP_ADD)
			{
				assert(!"Removed");
	//			SetVector3(&(*GetVector3() + *p_new->GetVector3()));
			}	else
		
				
				if (i_mod_type == ::C_SCRIPT_OP_SUBTRACT)
				{
					assert(!"Removed");
//					SetVector3(&((*GetVector3()) - (*p_new->GetVector3()))); else
				} else
//			if (i_mod_type == ::C_SCRIPT_OP_MULTIPLY)  SetVector3( &(   (*GetVector3())  * ((*p_new->GetVector3())))); else
//			if (i_mod_type == ::C_SCRIPT_OP_DIVIDE)  SetVector3( &(*GetVector3() / *p_new->GetVector3()));	else
			 
			
			{
				LogError("Error with op %d dealing with vector3.", i_mod_type);
				return false;
			}
			
			return true; 
		}

	
		
		
		
		
		//no conversion required, just copy the var over
		if (m_i_type == C_SCRIPT_VAR_INT)
		{
			if (i_mod_type == ::C_SCRIPT_OP_ADD) m_i_parm1 += p_new->m_i_parm1; else
			if (i_mod_type == ::C_SCRIPT_OP_SUBTRACT) m_i_parm1 -= p_new->m_i_parm1; else
			if (i_mod_type == ::C_SCRIPT_OP_MULTIPLY) m_i_parm1 *= p_new->m_i_parm1; else
			if (i_mod_type == ::C_SCRIPT_OP_DIVIDE) m_i_parm1 /= p_new->m_i_parm1;	else
			if (i_mod_type == ::C_SCRIPT_OP_MODULUS) m_i_parm1 = altmod(m_i_parm1, p_new->m_i_parm1); else
			{
				LogError("Error with op %d dealing with ints.", i_mod_type);
				return false;
			}
			
			return true; 
		}

		if (m_i_type == C_SCRIPT_VAR_FLOAT)
		{
			if (i_mod_type == ::C_SCRIPT_OP_ADD) SetFloat(GetFloat() + p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_SUBTRACT) SetFloat(GetFloat() - p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_MULTIPLY) SetFloat(GetFloat() * p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_DIVIDE) SetFloat(GetFloat() / p_new->GetFloat()); else
			{
				LogError("Error with op %d dealing with floats.", i_mod_type);
				return false;
			}
			
			return true; 
		}
	
        //deal with string/string ops

        if (m_i_type == C_SCRIPT_VAR_WCHAR)
        {
            if (i_mod_type == C_SCRIPT_OP_ADD) this->WCharAdd(p_new->GetWChar()); //concat the string in new to ours
            return true;
        }


	}

   	if (this->GetType() == C_SCRIPT_VAR_FLOAT)
	{
	
		if (p_new->GetType() == C_SCRIPT_VAR_INT)
		{
			if (i_mod_type == ::C_SCRIPT_OP_ADD) SetFloat(GetFloat() + p_new->GetInt()); else
			if (i_mod_type == ::C_SCRIPT_OP_SUBTRACT) SetFloat(GetFloat() - p_new->GetInt()); else
			if (i_mod_type == ::C_SCRIPT_OP_MULTIPLY) SetFloat(GetFloat() * p_new->GetInt()); else
			if (i_mod_type == ::C_SCRIPT_OP_DIVIDE) SetFloat(GetFloat() / p_new->GetInt()); else
			{
				LogError("Error with op %d dealing with floats and ints", i_mod_type);
				return false;
			}
			
			return true; 
		}
	
	}

   	if (this->GetType() == C_SCRIPT_VAR_INT)
	{
	
		//from float to int
		if (p_new->GetType() == C_SCRIPT_VAR_FLOAT)
		{
			if (i_mod_type == ::C_SCRIPT_OP_ADD) m_i_parm1 += int(p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_SUBTRACT)  m_i_parm1 -= int(p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_MULTIPLY)  m_i_parm1 *= int(p_new->GetFloat()); else
			if (i_mod_type == ::C_SCRIPT_OP_DIVIDE)  m_i_parm1 /= int(p_new->GetFloat()); else
            if (i_mod_type == ::C_SCRIPT_OP_MODULUS)  m_i_parm1 %= int(p_new->GetFloat()); else


			{
				LogError("Error with op %d dealing with ints and floats", i_mod_type);
				return false;
			}
			
			return true; 
		}
	
	}



	//if we got here there is an error
	LogError("Don't know how to modify %s with %s.", GetVarTypeString(GetType()), GetVarTypeString(p_new->GetType()));
	return true; //no errors in the conversion/set


}




//return the true type of var we are, even if we are are just a reference.
int CScriptVarBase::GetBaseType()
{
  if (GetType() != C_SCRIPT_VAR_DYNAMIC)
  {
	  return GetType();
  }

  //we are a reference. 
  return g_p_script_engine->GetScript(this->m_i_parm1)->GetVar(m_i_parm2)->GetType();

}


bool CScriptVarBase::Set(CScriptVarBase *p_new)
{

	if (GetType() == C_SCRIPT_VAR_DYNAMIC)
	{
		//let's actually change the var we should be pointing too
		return g_p_script_engine->GetScript(this->m_i_parm1)->GetVar(m_i_parm2)->Set(p_new);
	}
	
   if (p_new->GetType() == C_SCRIPT_VAR_DYNAMIC)
   {
	   //call us with the real var, not this crap
	   return Set(g_p_script_engine->GetScript(p_new->m_i_parm1)->GetVar(p_new->m_i_parm2));
   }

	if (this->GetType() == p_new->GetType())
	{
		//no conversion required, just copy the var over
		if (m_i_type == C_SCRIPT_VAR_INT)
		{
			m_i_parm1 = p_new->m_i_parm1;
			return true; 
		}
	
		if (m_i_type == C_SCRIPT_VAR_FLOAT)
		{
			memcpy(&m_i_parm1, &p_new->m_i_parm1, 4); //we use m_i_parm1 as an int or a float, confusing, neh?
			return true; 
		}
		
		if (m_i_type == C_SCRIPT_VAR_WCHAR)
		{
			SetVar(m_i_type, 0, 0, p_new->GetWChar());
			return true;
		}
	}


	//well, if we got here we have to do a conversion of some sort.
	if (this->GetType() == C_SCRIPT_VAR_FLOAT)
	{
		if (p_new->GetType() == C_SCRIPT_VAR_INT)
		{
			SetFloat(float(p_new->GetInt()));
			return true;
		}

	}

	//if we are an int...
	if (this->GetType() == C_SCRIPT_VAR_INT)
	{
		if (p_new->GetType() == C_SCRIPT_VAR_FLOAT)
		{
			m_i_parm1 = int(p_new->GetFloat());
			return true;
		}

	}

		if (this->GetType() == C_SCRIPT_VAR_VECTOR3)
	{
		SetVar(m_i_type, 0, 0, p_new->m_p_extra);
		return true;
	}

	//if we got here there is an error
	LogError("Don't know how to SET %s to a %s.", GetVarTypeString(GetType()), GetVarTypeString(p_new->GetType()));
	return false; //no errors in the conversion/set
}

int CScriptVarBase::AppendToWChar(WCHAR *p_dest)
{
   //let's convert whatever our var type is into string format.
	
	if (GetType() == ::C_SCRIPT_VAR_INT)
	{
		return (swprintf(p_dest, L"%d", this->m_i_parm1));
	}

	if (GetType() == ::C_SCRIPT_VAR_FLOAT)
	{
		return (swprintf(p_dest, L"%f", *(float*)&m_i_parm1));
	}

	if (GetType() == ::C_SCRIPT_VAR_WCHAR)
	{
		
		return (swprintf(p_dest, L"%s", (WCHAR*)m_p_extra));
	
	}
	
	LogError("Don't know how to convert a token of var type %s into a string.",GetVarTypeString(GetType()));
	return 0; //error
}


bool CScriptVarBase::If(CScriptVarBase *p_new, int i_mod_type, const char *p_optional_text_for_errors)

{

 	if (GetType() == C_SCRIPT_VAR_DYNAMIC)
	{
		//let's actually change the var we should be pointing too
		return g_p_script_engine->GetScript(this->m_i_parm1)->GetVar(m_i_parm2)->If(p_new, i_mod_type, p_optional_text_for_errors);
	}
	
   if (p_new->GetType() == C_SCRIPT_VAR_DYNAMIC)
   {
	   //call us with the real var, not this crap
	   return If(g_p_script_engine->GetScript(p_new->m_i_parm1)->GetVar(p_new->m_i_parm2), i_mod_type, p_optional_text_for_errors);
   }

   //ok, let's compare these vars in the requested way.


   	if (this->GetType() == p_new->GetType())
	{
		//no conversion required, just compare them
		if (m_i_type == C_SCRIPT_VAR_INT)
		{
			//let's compare ints
			if (i_mod_type == C_SCRIPT_OP_IF_EQUALS) return (this->m_i_parm1 == p_new->m_i_parm1);
			if (i_mod_type == C_SCRIPT_OP_IF_LESS) return (this->m_i_parm1 < p_new->m_i_parm1);
			if (i_mod_type == C_SCRIPT_OP_IF_MORE) return (this->m_i_parm1 > p_new->m_i_parm1);
			if (i_mod_type == C_SCRIPT_OP_IF_NOT_EQUALS) return (this->m_i_parm1 != p_new->m_i_parm1);
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_LESS) return (this->m_i_parm1 <= p_new->m_i_parm1);
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_MORE) return (this->m_i_parm1 >= p_new->m_i_parm1);
			
			//if we get here it's an error
			return 0; 
		}

		
		if (m_i_type == C_SCRIPT_VAR_FLOAT)
		{
			//let's compare floats
			if (i_mod_type == C_SCRIPT_OP_IF_EQUALS) return (this->GetFloat() == p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_LESS) return (this->GetFloat() < p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_MORE) return (this->GetFloat() > p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_NOT_EQUALS) return (this->GetFloat() != p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_LESS) return (this->GetFloat() <= p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_MORE) return (this->GetFloat() >= p_new->GetFloat());
			
			//if we get here it's an error
			return 0; 
		}

		if (m_i_type == C_SCRIPT_VAR_WCHAR)
		{
			//let's compare ints
			if (i_mod_type == C_SCRIPT_OP_IF_EQUALS) 
			{
				if ( wcscmp(GetWChar(), p_new->GetWChar()) == 0) return true; else return false;
			}

			if (i_mod_type == C_SCRIPT_OP_IF_NOT_EQUALS) 
			{
				if (wcscmp(GetWChar(), p_new->GetWChar()) != 0) return true; else return false;
			}
			
			if (i_mod_type == C_SCRIPT_OP_IF_LESS) return (wcslen(GetWChar()) < wcslen(p_new->GetWChar()));
			if (i_mod_type == C_SCRIPT_OP_IF_MORE) return (wcslen(GetWChar()) > wcslen(p_new->GetWChar()));
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_MORE) return (wcslen(GetWChar()) >= wcslen(p_new->GetWChar()));
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_LESS) return (wcslen(GetWChar()) <= wcslen(p_new->GetWChar()));



			//if we get here it's an error
			return 0; 
		}


	}

   //ok, the data types actually don't match.  

	//well, if we got here we have to do a conversion of some sort.
	if (this->GetType() == C_SCRIPT_VAR_FLOAT)
	{
		if (p_new->GetType() == C_SCRIPT_VAR_INT)
		{
		
			
			if (i_mod_type == C_SCRIPT_OP_IF_EQUALS) return (this->GetFloat() == p_new->GetInt());
			if (i_mod_type == C_SCRIPT_OP_IF_LESS) return (this->GetFloat() < p_new->GetInt());
			if (i_mod_type == C_SCRIPT_OP_IF_MORE) return (this->GetFloat() > p_new->GetInt());
			if (i_mod_type == C_SCRIPT_OP_IF_NOT_EQUALS) return (this->GetFloat() != p_new->GetInt());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_LESS) return (this->GetFloat() <= p_new->GetInt());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_MORE) return (this->GetFloat() >= p_new->GetInt());
			return 0;
		}

	}

	//if we are an int...
	if (this->GetType() == C_SCRIPT_VAR_INT)
	{
		if (p_new->GetType() == C_SCRIPT_VAR_FLOAT)
		{
			if (i_mod_type == C_SCRIPT_OP_IF_EQUALS) return (this->GetInt() == p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_LESS) return (this->GetInt() < p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_MORE) return (this->GetInt() > p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_NOT_EQUALS) return (this->GetInt() != p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_LESS) return (this->GetInt() <= p_new->GetFloat());
			if (i_mod_type == C_SCRIPT_OP_IF_SAME_OR_MORE) return (this->GetInt() >= p_new->GetFloat());
		
			//error
			return 0;
		}
	}

   // assert(0);
    //if we got here there is an error
	LogError("Don't know how to compare %s to a %s in IF statement in %s.", GetVarTypeString(GetType()), GetVarTypeString(p_new->GetType()),
        p_optional_text_for_errors);
	return false; //big error


}


void CScriptVarBase::WCharAdd(WCHAR *p_new)
{
    //append to the char we have
    WCHAR *p_original = GetWChar();

    int i_size = wcslen(p_original);
    int i_added_size = wcslen(p_new);

    m_p_extra = realloc(m_p_extra, (i_size+i_added_size+1)*2); //add more memory as needed.  the +1 is for a NULL
	wcscpy((WCHAR*)m_p_extra+(i_size), p_new); //copy the string over, including the null at the end
}
