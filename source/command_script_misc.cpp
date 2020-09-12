#include "command_script_vars.h"
#include "all.h"
#include "uni.h"


void func_AddToNews(CScriptPassVars *p_vars);
void func_BroadCastTV(CScriptPassVars *p_vars);
void func_MailText(CScriptPassVars *p_vars);
void func_MailPicture(CScriptPassVars *p_vars);
void func_MailContinueButton(CScriptPassVars *p_vars);


void InitScriptFunctionsMisc()
{
  	CScriptPassVars *p_vars;
   
 
    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //id
    g_glo.p_script->GetScript(0)->AddFunction("AddToNews", func_AddToNews, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_WCHAR);  //id
    g_glo.p_script->GetScript(0)->AddFunction("BroadCastTV", func_BroadCastTV, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR);  //message
    g_glo.p_script->GetScript(0)->AddFunction("MailText", func_MailText, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_WCHAR);  //message
    g_glo.p_script->GetScript(0)->AddFunction("MailPicture", func_MailPicture, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("MailContinueButton", func_MailContinueButton, p_vars);

}


void func_AddToNews(CScriptPassVars *p_vars)
{
  add_to_news(uni(p_vars->a_vars[0].GetWChar()).to_st());
}

void func_BroadCastTV(CScriptPassVars *p_vars)
{
  broadcast_tv(uni(p_vars->a_vars[0].GetWChar()).to_st());
}

void func_MailText(CScriptPassVars *p_vars)
{
   int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("MailText - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

 send_packet(i_play, uni(p_vars->a_vars[1].GetWChar()).to_st(), ::c_que_importance_high, ::c_que_type_text, ::c_que_nobody);
}

void func_MailPicture(CScriptPassVars *p_vars)
{
   int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("MailPicture - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

	send_packet_picture(i_play, uni(p_vars->a_vars[1].GetWChar()).to_st());
}

void func_MailContinueButton(CScriptPassVars *p_vars)
{
   int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("MailContinueButton - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

	send_packet_continue_button(i_play);
}

