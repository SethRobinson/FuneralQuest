#include "command_script.h"
#include "all.h"
#include "uni.h"
#include "command_script_vars.h"
#include "command_script_misc.h"

void func_AddCustom(CScriptPassVars *p_vars);
void func_SetLocation(CScriptPassVars *p_vars);
void func_SendPacketAndWait(CScriptPassVars *p_vars);
void func_AddButton(CScriptPassVars *p_vars);
void func_AddButtonTownMap(CScriptPassVars *p_vars);
void func_AddButtonHome(CScriptPassVars *p_vars);
void func_RunScriptNoReturn(CScriptPassVars *p_vars);
void func_SendPacketAndWaitOverlay(CScriptPassVars *p_vars);
void func_AddUpdateTags(CScriptPassVars *p_vars);
void func_AddButtonStore(CScriptPassVars *p_vars);
void func_SendPacketAndWaitStats(CScriptPassVars *p_vars);
void func_AddUpdateStats(CScriptPassVars *p_vars);
void func_AddButtonChangeName(CScriptPassVars *p_vars);
                       
void func_AddUpdateBPs(CScriptPassVars *p_vars);
void func_AddUpdateTurns(CScriptPassVars *p_vars);
void func_AddUpdateLuck(CScriptPassVars *p_vars);




bool RunSinglePassScript(char *p_fname)
{
  int i_script = g_glo.p_script->CreateScript();
  int i_result = g_glo.p_script->load_and_compile_script(p_fname, i_script);
  
  if (i_script != C_SCRIPT_NONE)
  {
      
      g_glo.p_script->RunScriptFunction(i_script, "MAIN");
  } else
  {
      return false; //error
  }

  return true; //success

}

bool RunSinglePassScriptAndSetPlayerID(char *p_fname, int i_play)
{
  int i_script = g_glo.p_script->CreateScript();
  
   int i_temp_var = g_glo.p_script->GetScript(i_script)->AddVar("i_my_id", ::C_SCRIPT_VAR_INT);
   g_glo.p_script->GetScript(i_script)->GetVar(i_temp_var)->SetInt(i_play);
            

   int i_result = g_glo.p_script->load_and_compile_script(p_fname, i_script);
 
  if (i_script != C_SCRIPT_NONE)
  {
      
      g_glo.p_script->RunScriptFunction(i_script, "MAIN");
  } else
  {
      return false; //error
  }

  return true; //success


}



void CommandScript(LPREQUEST lpReq)
{
  	//received request from client, they want to start a script.
    //let's setup the script callback default.
    init_location_info(lpReq, c_request_script_callback, "Script");

    d_get_token_string(cst_script_fname, "st_extra_text"); //get name they want

    //load this script
    online_struct *p = p_on[lpReq->i_on_index];
    cst_script_fname = CString("script\\public\\")+cst_script_fname;
    p->InitScript((LPCSTR)cst_script_fname);

}

void CommandScriptRunScript(LPREQUEST lpReq, CString st_location, CString st_script)
{
  	//received request from client, they want to start a script.
    //let's setup the script callback default.
    init_location_info(lpReq, c_request_script_callback, st_location);

    //load this script
    online_struct *p = p_on[lpReq->i_on_index];
    p->InitScript((LPCSTR)st_script);

}


void CommandScriptCallback(LPREQUEST lpReq)
{

    
    //d_get_token_string(cst_script_fname, "st_extra_text"); //get name they want
    online_struct *p = p_on[lpReq->i_on_index];
  	d_get_token_int(i_command_sub, "i_command_sub");

    int i_his_id;
    if (get_token_int(lpReq, "i_his_id", &i_his_id))
    {
      g_glo.p_script->GetScript(p->m_i_script)->GetVar(1)->SetVar(C_SCRIPT_VAR_INT, i_his_id, 0,NULL);

    }   else
    {
      g_glo.p_script->GetScript(p->m_i_script)->GetVar(1)->SetVar(C_SCRIPT_VAR_INT, i_command_sub, 0,NULL);
    }
    g_glo.p_script->GetScript(p->m_i_script)->StartUpAfterWait();
      
}


void InitScriptFunctions()
{
   	if (!g_glo.p_script)
	{
		log_error("Scripting engine not initted, can't add FQ specific functions.");
	    return;
    }
	
	CScriptPassVars *p_vars;

    
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add it to
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR);  //name of object
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //value of object
    g_glo.p_script->GetScript(0)->AddFunction("AddCustom", func_AddCustom, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add it to
    g_glo.p_script->GetScript(0)->AddFunction("SetLocation", func_SetLocation, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to send
    g_glo.p_script->GetScript(0)->AddFunction("SendPacketAndWait", func_SendPacketAndWait, p_vars);
 
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to send
    g_glo.p_script->GetScript(0)->AddFunction("SendPacketAndWaitOverlay", func_SendPacketAndWaitOverlay, p_vars);
  

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //button number on screen (1 to 5)
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //button label
    p_vars->a_vars[3].SetType(C_SCRIPT_VAR_INT);  //return code
    g_glo.p_script->GetScript(0)->AddFunction("AddButton", func_AddButton, p_vars);

   
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //button number on screen (1 to 5)
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //button label
    g_glo.p_script->GetScript(0)->AddFunction("AddButtonTownMap", func_AddButtonTownMap, p_vars);

 
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //button number on screen (1 to 5)
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //button label
    g_glo.p_script->GetScript(0)->AddFunction("AddButtonStore", func_AddButtonStore, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //button number on screen (1 to 5)
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //button label
    g_glo.p_script->GetScript(0)->AddFunction("AddButtonChangeName", func_AddButtonChangeName, p_vars);

    
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //button number on screen (1 to 5)
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_WCHAR);  //button label
    g_glo.p_script->GetScript(0)->AddFunction("AddButtonHome", func_AddButtonHome, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //script to run
    g_glo.p_script->GetScript(0)->AddFunction("RunScriptNoReturn", func_RunScriptNoReturn, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    g_glo.p_script->GetScript(0)->AddFunction("AddUpdateTags", func_AddUpdateTags, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    g_glo.p_script->GetScript(0)->AddFunction("AddUpdateStats", func_AddUpdateStats, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    g_glo.p_script->GetScript(0)->AddFunction("AddUpdateBPs", func_AddUpdateBPs, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    g_glo.p_script->GetScript(0)->AddFunction("AddUpdateLuck", func_AddUpdateLuck, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);  //return packet
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to add to
    g_glo.p_script->GetScript(0)->AddFunction("AddUpdateTurns", func_AddUpdateTurns, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_FORCE_WAIT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //packet to send
    g_glo.p_script->GetScript(0)->AddFunction("SendPacketAndWaitStats", func_SendPacketAndWaitStats, p_vars);
  
    //add all our var manipulation functions
    InitScriptFunctionsVar();
    InitScriptFunctionsMisc();
}

void func_AddButton(CScriptPassVars *p_vars)
{
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    
    add_button(&cst_final, p_vars->a_vars[1].GetInt(), 
  p_vars->a_vars[2].GetWChar(), c_request_script_callback, p_vars->a_vars[3].GetInt());

    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}

void func_AddButtonTownMap(CScriptPassVars *p_vars)
{
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    add_button(&cst_final, p_vars->a_vars[1].GetInt(), 
    p_vars->a_vars[2].GetWChar(), c_request_map, C_SUB_NONE);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_AddButtonStore(CScriptPassVars *p_vars)
{
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    add_button(&cst_final, p_vars->a_vars[1].GetInt(), 
    p_vars->a_vars[2].GetWChar(), c_request_store, C_SUB_NONE);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_AddButtonChangeName(CScriptPassVars *p_vars)
{
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    add_button(&cst_final, p_vars->a_vars[1].GetInt(), 
    p_vars->a_vars[2].GetWChar(), c_request_options, C_SUB_NONE);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}

void func_AddButtonHome(CScriptPassVars *p_vars)
{
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    add_button(&cst_final, p_vars->a_vars[1].GetInt(), 
    p_vars->a_vars[2].GetWChar(), c_request_main, C_SUB_NONE);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_AddUpdateTags(CScriptPassVars *p_vars)
{
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
  
    CString cst_final(p_vars->a_vars[0].GetWChar());
    add_tags(&cst_final, i_play);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_AddUpdateBPs(CScriptPassVars *p_vars)
{
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
   LPREQUEST lpReq = p_play[i_play].p_on->lpReq;

 
    CString cst_final(p_vars->a_vars[0].GetWChar());
  	add_exp(&cst_final, lpReq);
  	add_sp(&cst_final, lpReq);
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}

void func_AddUpdateLuck(CScriptPassVars *p_vars)
{
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    LPREQUEST lpReq = p_play[i_play].p_on->lpReq;


    CString cst_final(p_vars->a_vars[0].GetWChar());
  	add_luck(&cst_final, lpReq);
   p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


 void func_AddUpdateTurns(CScriptPassVars *p_vars)
{
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
      LPREQUEST lpReq = p_play[i_play].p_on->lpReq;


    CString cst_final(p_vars->a_vars[0].GetWChar());
  	add_turns(&cst_final, lpReq);
   p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}

void func_AddUpdateStats(CScriptPassVars *p_vars)
{
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    LPREQUEST lpReq = p_play[i_play].p_on->lpReq;

    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    add_turns(&cst_final, lpReq);
	add_exp(&cst_final, lpReq);
	add_money(&cst_final, lpReq);
	add_tags(&cst_final, get_play(lpReq));
	add_var(&cst_final, "i_plots", int_to_cash(p_play[get_play(lpReq)].i_plots));
	add_var(&cst_final, "i_sp", int_to_cash(p_play[get_play(lpReq)].i_sp));
	add_var(&cst_final, "i_psych", "Level "+(int_to_cash(p_play[get_play(lpReq)].i_psych+1)));
//	add_var(&cst_final, "st_karma", get_karma_string(lpReq));
	CString cst_temp;
	cst_temp.Format("#%d - %s", get_player_rank(get_play(lpReq)), get_rating_string(get_percent_of_ranking_of_player(get_play(lpReq))));
	add_var(&cst_final, "st_rating", cst_temp);
  	add_var(&cst_final, "i_yangs", int_to_cash(glo.i_yangs_per_day - p_play[get_play(lpReq)].i_yangs_sent));
  	add_var(&cst_final, "i_visits", int_to_cash(glo.i_visits_per_day - p_play[get_play(lpReq)].i_player_fights));
  	add_var(&cst_final, "i_hit_max", int_to_cash(p_play[get_play(lpReq)].i_hp_max));
  	add_var(&cst_final, "i_strength", int_to_cash(p_play[get_play(lpReq)].i_strength));
  	add_var(&cst_final, "i_lockpick", int_to_cash(p_play[get_play(lpReq)].a_custom[C_CUSTOM_LOCKPICK]));

    add_var(&cst_final, "st_ride", get_transportation(get_play(lpReq)));




    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_AddCustom(CScriptPassVars *p_vars)
{
    //these string operations are pretty slow.  This is an area ripe for optimization, cutting down on the
    //dynamic allocations and conversions from WCHAR/CHAR.  All of FQ should be converted to WCHAR to properly
    //support foreign languages, really.
    
    CString cst_final(p_vars->a_vars[0].GetWChar());
    
    cst_final += p_vars->a_vars[1].GetWChar();
    cst_final += "=";
    cst_final += p_vars->a_vars[2].GetWChar();
    cst_final += "&";
  
    p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(cst_final).us_data);
}


void func_SetLocation(CScriptPassVars *p_vars)
{
    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    //figure out our LPREQ
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    //because we created it right after the script was created.
    //get lpreq from play
    init_location_info(p_play[i_play].p_on->lpReq, c_request_script_callback, cst_final);
}

 void func_RunScriptNoReturn(CScriptPassVars *p_vars)
{
    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    //init the new script
      int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
      
      p_play[i_play].p_on->m_i_script = C_SCRIPT_NONE; //lose interest in the script but don't auto delete
      //it because we're still in it.
      p_play[i_play].p_on->InitScript(cst_final);
     
      //mark the script we're in now as "inactive" so it will get overwritten ASAP.  Can't just delete
      //it now because we're still nested inside it.
      g_glo.p_script->GetScript(p_vars->i_script)->SetActive(false);

}

void func_SendPacketAndWait(CScriptPassVars *p_vars)
{
    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    //figure out our LPREQ
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    //because we created it right after the script was created.
    //get lpreq from play 
    bool b_handled = false;
   	if (cst_final.Find("st_object") != -1 )
    {
        //they are creating a new screen.  Do it this way.
        add_var(&cst_final, "i_sent",c_command_object);
      
        b_handled = true;
    }
    

    if (!b_handled)
    {
        //default method
          add_var(&cst_final, "i_sent",c_command_generic);
    }

    

    seth_ansi(&cst_final); //convert color codes/etc
    if (p_play[i_play].p_on)
    send_msg(p_play[i_play].p_on->lpReq, cst_final);

   	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL); //wait forever

}


void func_SendPacketAndWaitOverlay(CScriptPassVars *p_vars)
{
    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    //figure out our LPREQ
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    //because we created it right after the script was created.
    //get lpreq from play 
    bool b_handled = false;
   	if (cst_final.Find("st_object") != -1 )
    {
      //first add a thing so it knows who to call when it's done choosing the
        //name.  This is highly convoluted
        add_var(&cst_final, "i_return_command",c_request_script_callback);
          //they are creating a new screen.  Do it this way.
        add_var(&cst_final, "i_sent",c_command_object_add);
        b_handled = true;
    }
    

    if (!b_handled)
    {
        //default method
          add_var(&cst_final, "i_sent",c_command_generic);
    }
    
    seth_ansi(&cst_final); //convert color codes/etc
    if (p_play[i_play].p_on)
	send_msg(p_play[i_play].p_on->lpReq, cst_final);

   	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL); //wait forever




}



void func_SendPacketAndWaitStats(CScriptPassVars *p_vars)
{
    CString cst_final(p_vars->a_vars[0].GetWChar());
   
    //figure out our LPREQ
    int i_play = g_glo.p_script->GetScript(p_vars->i_script)->GetVar(0)->GetInt(); //we know var 0 is i_my_id
    //because we created it right after the script was created.
    //get lpreq from play 
    bool b_handled = false;
   	if (cst_final.Find("st_object") != -1 )
    {
        //they are creating a new screen.  Do it this way.
        add_var(&cst_final, "i_sent",c_command_object);
      
        b_handled = true;
    }
    

    if (!b_handled)
    {
        //default method
          add_var(&cst_final, "i_sent",c_command_stats);
    }
    
    seth_ansi(&cst_final); //convert color codes/etc
    if (p_play[i_play].p_on)
	send_msg(p_play[i_play].p_on->lpReq, cst_final);

   	p_vars->return_var.SetVar(C_SCRIPT_VAR_FORCE_WAIT, 200000000, 0, NULL); //wait forever

}



