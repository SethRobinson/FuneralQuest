#include "command_script_vars.h"
#include "all.h"
#include "uni.h"

void func_GetTags(CScriptPassVars *p_vars);
void func_ModTags(CScriptPassVars *p_vars);
void func_IsPlayerOnNow(CScriptPassVars *p_vars);
void func_GetNameFromID(CScriptPassVars *p_vars);
void func_GetHomeNameFromID(CScriptPassVars *p_vars);
void func_GetTransportationFromID(CScriptPassVars *p_vars);
void func_GetCustomByID(CScriptPassVars *p_vars);
void func_ModCustomByID(CScriptPassVars *p_vars);

void func_SetCustomByID(CScriptPassVars *p_vars);

void func_GetTotalBurials(CScriptPassVars *p_vars);
void func_ModTotalBurials(CScriptPassVars *p_vars);
void func_GetTotalBuryPoints(CScriptPassVars *p_vars);
void func_ModTotalBuryPoints(CScriptPassVars *p_vars);
void func_GetTodaysBuryPoints(CScriptPassVars *p_vars);
void func_ModTodaysBuryPoints(CScriptPassVars *p_vars);
void func_GetLuck(CScriptPassVars *p_vars);
void func_ModLuck(CScriptPassVars *p_vars);
void func_GetTurns(CScriptPassVars *p_vars);
void func_ModTurns(CScriptPassVars *p_vars);
void func_GetStrength(CScriptPassVars *p_vars);
void func_ModStrength(CScriptPassVars *p_vars);

void func_GetMaxHP(CScriptPassVars *p_vars);
void func_ModMaxHP(CScriptPassVars *p_vars);
void func_GetRankPercent(CScriptPassVars *p_vars);
void func_GetTopPlayerID(CScriptPassVars *p_vars);
void func_GetDay(CScriptPassVars *p_vars);
void func_GetTourneyDaysLeft(CScriptPassVars *p_vars);



void func_SetTags(CScriptPassVars *p_vars);

void func_GetPsychUpgrades(CScriptPassVars *p_vars);
void func_ModPsychUpgrades(CScriptPassVars *p_vars);
void func_GetPoliceProtected(CScriptPassVars *p_vars);

void func_SetPoliceProtected(CScriptPassVars *p_vars);
void func_GetPlayerFights(CScriptPassVars *p_vars);
void func_ModPlayerFights(CScriptPassVars *p_vars);
void func_GetRandomPlayerID(CScriptPassVars *p_vars);


void InitScriptFunctionsVar()
{
  	CScriptPassVars *p_vars;
   
 
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //tag type
    g_glo.p_script->GetScript(0)->AddFunction("GetTags", func_GetTags, p_vars);
  
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //return new tag number just in case they want it
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //tag type
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT);  //number to mod by.
    g_glo.p_script->GetScript(0)->AddFunction("ModTags", func_ModTags, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //return new tag number just in case they want it
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
	p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //tag type
	p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT);  //number to mod by.
    g_glo.p_script->GetScript(0)->AddFunction("SetTags", func_SetTags, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //1 is yes
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("IsPlayerOnNow", func_IsPlayerOnNow, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetNameFromID", func_GetNameFromID, p_vars);

        p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetTransportationFromID", func_GetTransportationFromID, p_vars);

    
    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_WCHAR);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetHomeNameFromID", func_GetHomeNameFromID, p_vars);


    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //index of special
    g_glo.p_script->GetScript(0)->AddFunction("GetCustomByID", func_GetCustomByID, p_vars);

    p_vars = new CScriptPassVars();
	p_vars->return_var.SetType(C_SCRIPT_VAR_INT);
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //index of special
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT);  //what to mod it by
    g_glo.p_script->GetScript(0)->AddFunction("ModCustomByID", func_ModCustomByID, p_vars);

    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //index of special
    p_vars->a_vars[2].SetType(C_SCRIPT_VAR_INT);  //what to mod it by
    g_glo.p_script->GetScript(0)->AddFunction("SetCustomByID", func_SetCustomByID, p_vars);


    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetTotalBurials", func_GetTotalBurials, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModTotalBurials", func_ModTotalBurials, p_vars);


    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetTotalBPs", func_GetTotalBuryPoints, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModTotalBPs", func_ModTotalBuryPoints, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetTodaysBPs", func_GetTodaysBuryPoints, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModTodaysBPs", func_ModTodaysBuryPoints, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetLuck", func_GetLuck, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModLuck", func_ModLuck, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetTurns", func_GetTurns, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //burials
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModTurns", func_ModTurns, p_vars);



    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetStrength", func_GetStrength, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModStrength", func_ModStrength, p_vars);


    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetMaxHP", func_GetMaxHP, p_vars);


    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModMaxHP", func_ModMaxHP, p_vars);

     p_vars = new CScriptPassVars();
   	 p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
     p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
     g_glo.p_script->GetScript(0)->AddFunction("GetRankPercent", func_GetRankPercent, p_vars);

     p_vars = new CScriptPassVars();
   	 p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
     g_glo.p_script->GetScript(0)->AddFunction("GetTopPlayerID", func_GetTopPlayerID, p_vars);

     p_vars = new CScriptPassVars();
   	 p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
     g_glo.p_script->GetScript(0)->AddFunction("GetDay", func_GetDay, p_vars);

      p_vars = new CScriptPassVars();
   	 p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
     g_glo.p_script->GetScript(0)->AddFunction("GetTourneyDaysLeft", func_GetTourneyDaysLeft, p_vars);


       p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetPsychUpgrades", func_GetPsychUpgrades, p_vars);


    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModPsychUpgrades", func_ModPsychUpgrades, p_vars);

       p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetPolice", func_GetPoliceProtected, p_vars);


    p_vars = new CScriptPassVars();
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("SetPolice", func_SetPoliceProtected, p_vars);


    
    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    g_glo.p_script->GetScript(0)->AddFunction("GetPlayerFights", func_GetPlayerFights, p_vars);

    p_vars = new CScriptPassVars();
  	p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
    p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //id
    p_vars->a_vars[1].SetType(C_SCRIPT_VAR_INT);  //how many to change it by
    g_glo.p_script->GetScript(0)->AddFunction("ModPlayerFights", func_ModPlayerFights, p_vars);


         p_vars = new CScriptPassVars();
   	 p_vars->return_var.SetType(C_SCRIPT_VAR_INT); //var
     p_vars->a_vars[0].SetType(C_SCRIPT_VAR_INT);  //if not -1 will ignore this person
   g_glo.p_script->GetScript(0)->AddFunction("GetRandomPlayerID", func_GetRandomPlayerID, p_vars);



}


void func_GetPlayerFights(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetPlayerFights - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_player_fights, 0, NULL);
}



void func_ModPlayerFights(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModPlayerFights - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].i_player_fights += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_player_fights  < 0) p_play[i_play].i_player_fights = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_player_fights, 0, NULL);
}



void func_GetPoliceProtected(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetPoliceProtected - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].b_police_protected, 0, NULL);
}



void func_SetPoliceProtected(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("SetPoliceProtected - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].b_police_protected = p_vars->a_vars[1].GetInt() != 0;

   
}


       

void func_GetPsychUpgrades(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetPsychUpgrades - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_psych, 0, NULL);
}



void func_ModPsychUpgrades(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModPsychUpgrades - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].i_psych += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_psych  < 0) p_play[i_play].i_psych = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_psych, 0, NULL);
}



void func_GetTourneyDaysLeft(CScriptPassVars *p_vars)
{
    if (!glo.b_tourn_active)
    {
     //no tourney active   
     p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, -2, 0, NULL);
     return;
    }

   p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, glo.i_tourn_days - glo.i_game_age, 0, NULL);
  
}


void func_GetDay(CScriptPassVars *p_vars)
{
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, glo.i_game_age, 0, NULL);
}


    
void func_GetTopPlayerID(CScriptPassVars *p_vars)
{
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, get_top_player(), 0, NULL);
}



void func_GetRandomPlayerID(CScriptPassVars *p_vars)
{
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, get_random_player_excluding(p_vars->a_vars[0].GetInt()), 0, NULL);
}



void func_GetRankPercent(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetRankPercent - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, get_percent_of_ranking_of_player(i_play), 0, NULL);
}


void func_GetStrength(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetStrength - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_strength, 0, NULL);
}



void func_ModStrength(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModStrength - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].i_strength += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_strength < 0) p_play[i_play].i_strength = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_strength, 0, NULL);
}


 void func_GetMaxHP(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetMaxHP - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_hp_max, 0, NULL);
}



void func_ModMaxHP(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModMaxHP - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].i_hp_max += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_hp_max < 0) p_play[i_play].i_hp_max = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_hp_max, 0, NULL);
}


void func_GetTags(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTags - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
        return;
    }

    int i_tag_type = p_vars->a_vars[1].GetInt();
    if (i_tag_type >= c_max_coffin_types)
    {
        LogError("GetTags - %d is not a valid coffin tag type.", i_tag_type);
        return;
    }

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].a_coffins[i_tag_type], 0, NULL);
}

void func_ModTags(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModTags - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
        return;
    }

    int i_tag_type = p_vars->a_vars[1].GetInt();
    if (i_tag_type >= c_max_coffin_types)
    {
        LogError("ModTags - %d is not a valid coffin tag type.", i_tag_type);
        return;
    }

    p_play[i_play].a_coffins[i_tag_type] += p_vars->a_vars[2].GetInt();

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].a_coffins[i_tag_type], 0, NULL);
}


void func_SetTags(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("SetTags - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
        return;
    }

    int i_tag_type = p_vars->a_vars[1].GetInt();
    if (i_tag_type >= c_max_coffin_types)
    {
        LogError("SetTags - %d is not a valid coffin tag type.", i_tag_type);
        return;
    }

    p_play[i_play].a_coffins[i_tag_type] = p_vars->a_vars[2].GetInt();

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].a_coffins[i_tag_type], 0, NULL);
}


void func_IsPlayerOnNow(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("IsPlayerOnNow - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    if (p_play[i_play].p_on)  
    {
      //he's playing now
       p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 1, 0, NULL);
    } else
    {
        //he's not online
      p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);

    }
    
}
    

void func_GetNameFromID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetNameFromID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

        //he's not online
      p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(p_play[i_play].st_name).us_data);

}

void func_GetTransportationFromID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTransportationFromID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

       
      p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni( get_transportation(i_play)).us_data);

}


void func_GetHomeNameFromID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetNameFromID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

        //he's not online
      p_vars->return_var.SetVar(C_SCRIPT_VAR_WCHAR, 0, 0, uni(p_play[i_play].st_funeral_name).us_data);

}

void func_GetCustomByID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetCustomByID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    //is the ID in range?
    int i_custom_index = p_vars->a_vars[1].GetInt();
    if (i_custom_index > C_CUSTOM_SIZE)
    {
        LogError("GetCustomByID - Custom index %d not valid.", i_custom_index);
        return;
    }

        //grab the var they want
      p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].a_custom[i_custom_index], 0, NULL);
}


void func_ModCustomByID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModCustomByID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    //is the ID in range?
    int i_custom_index = p_vars->a_vars[1].GetInt();
    if (i_custom_index > C_CUSTOM_SIZE)
    {
        LogError("ModCustomByID - Custom index %d not valid.", i_custom_index);
        return;
    }

    //modify it as desired
     
       p_play[i_play].a_custom[i_custom_index] += p_vars->a_vars[2].GetInt();

        //grab the var they want
      p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].a_custom[i_custom_index], 0, NULL);
}

void func_SetCustomByID(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModCustomByID - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    //is the ID in range?
    int i_custom_index = p_vars->a_vars[1].GetInt();
    if (i_custom_index > C_CUSTOM_SIZE)
    {
        LogError("ModCustomByID - Custom index %d not valid.", i_custom_index);
        return;
    }

    //modify it as desired
     
       p_play[i_play].a_custom[i_custom_index] = p_vars->a_vars[2].GetInt();

}

void func_GetTotalBurials(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTotalBurials - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_plots, 0, NULL);
}



void func_ModTotalBurials(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModTotalBurials - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }

    p_play[i_play].i_plots += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_plots < 0) p_play[i_play].i_plots = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_plots, 0, NULL);
}


void func_GetTotalBuryPoints(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTotalBuryPoints - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_exp, 0, NULL);
}



void func_ModTotalBuryPoints(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModTotalBuryPoints - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    p_play[i_play].i_exp += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_exp < 0) p_play[i_play].i_exp = 0;

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_exp, 0, NULL);
}


void func_GetTodaysBuryPoints(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTodaysBuryPoints - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
    
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_sp, 0, NULL);
}



void func_ModTodaysBuryPoints(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModTodaysBuryPoints - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    exp_mod(i_play, p_vars->a_vars[1].GetInt());

    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_sp, 0, NULL);
}

void func_GetLuck(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetLuck - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_luck, 0, NULL);
}



void func_ModLuck(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModLuck - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    luck_mod(i_play, p_vars->a_vars[1].GetInt());

 
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_luck, 0, NULL);
}


void func_GetTurns(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("GetTurns - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_turns, 0, NULL);
}



void func_ModTurns(CScriptPassVars *p_vars)
{
    int i_play = GetValidPlayerID(p_vars->a_vars[0].GetInt()); //the player we should operate on

    if (i_play == -1)
    {
        LogError("ModTurns - Player ID %d is not valid.", p_vars->a_vars[0].GetInt());
         p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, 0, 0, NULL);
       return;
    }
   
     p_play[i_play].i_turns += p_vars->a_vars[1].GetInt();

    if (p_play[i_play].i_turns < 0) p_play[i_play].i_turns = 0;

    
 
    p_vars->return_var.SetVar(C_SCRIPT_VAR_INT, p_play[i_play].i_turns, 0, NULL);
}
