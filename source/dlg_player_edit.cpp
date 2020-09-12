// dlg_player_edit.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "dlg_player_edit.h"

/////////////////////////////////////////////////////////////////////////////
// dlg_player_edit dialog


dlg_player_edit::dlg_player_edit(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_player_edit::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_player_edit)
	m_cst_logon = _T("");
	m_cst_name = _T("");
	m_cst_password = _T("");
	m_cst_parlor = _T("");
	m_cst_email = _T("");
	m_i_money = 0;
	m_i_exp = 0;
	m_i_turns = 0;
	m_b_active = FALSE;
	m_int_rating = 0;
	m_i_casket_silver = 0;
	m_int_casket_wood = 0;
	m_i_casket_gold = 0;
	m_i_karma = 0;
	m_i_last_logged = 0;
	m_i_plots = 0;
	m_todays_bps = 0;
	m_i_luck = 0;
	m_b_always = FALSE;
	m_i_yangs_sent = 0;
	m_b_male = FALSE;
	m_b_can_attack = FALSE;
	m_i_hp = 0;
	m_i_hp_max = 0;
	m_i_psych = 0;
	m_i_strength = 0;
	m_i_attacks = 0;
	m_b_police = FALSE;
	m_i_custom = 0;
	m_i_tags_recieved = 0;
	//}}AFX_DATA_INIT
}


void dlg_player_edit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_player_edit)
	DDX_Control(pDX, IDC_COMBO_CUSTOM, m_combo_custom);
	DDX_Text(pDX, IDC_LOGON, m_cst_logon);
	DDV_MaxChars(pDX, m_cst_logon, 29);
	DDX_Text(pDX, IDC_NAME, m_cst_name);
	DDV_MaxChars(pDX, m_cst_name, 29);
	DDX_Text(pDX, IDC_PASSWORD, m_cst_password);
	DDV_MaxChars(pDX, m_cst_password, 29);
	DDX_Text(pDX, IDC_PARLOR, m_cst_parlor);
	DDV_MaxChars(pDX, m_cst_parlor, 29);
	DDX_Text(pDX, IDC_EMAIL, m_cst_email);
	DDV_MaxChars(pDX, m_cst_email, 49);
	DDX_Text(pDX, IDC_MONEY, m_i_money);
	DDX_Text(pDX, IDC_EXP, m_i_exp);
	DDX_Text(pDX, IDC_TURNS_LEFT, m_i_turns);
	DDX_Check(pDX, IDC_ACTIVE, m_b_active);
	DDX_Text(pDX, IDC_RATING, m_int_rating);
	DDX_Text(pDX, IDC_SILVER, m_i_casket_silver);
	DDX_Text(pDX, IDC_WOOD, m_int_casket_wood);
	DDX_Text(pDX, IDC_GOLD, m_i_casket_gold);
	DDX_Text(pDX, IDC_KARMA, m_i_karma);
	DDX_Text(pDX, IDC_LAST_LOGGED, m_i_last_logged);
	DDV_MinMaxInt(pDX, m_i_last_logged, -99999999, 2147483647);
	DDX_Text(pDX, IDC_PLOTS, m_i_plots);
	DDX_Text(pDX, IDC_TODAYS_BPS, m_todays_bps);
	DDX_Text(pDX, IDC_LUCK, m_i_luck);
	DDV_MinMaxInt(pDX, m_i_luck, 1, 100);
	DDX_Check(pDX, IDC_ALWAYS, m_b_always);
	DDX_Text(pDX, IDC_YANGS_SENT, m_i_yangs_sent);
	DDX_Check(pDX, IDC_B_MALE, m_b_male);
	DDX_Check(pDX, IDC_B_ATTACK, m_b_can_attack);
	DDX_Text(pDX, IDC_I_HP, m_i_hp);
	DDX_Text(pDX, IDC_I_HP_MAX, m_i_hp_max);
	DDX_Text(pDX, IDC_I_PSYCH, m_i_psych);
	DDX_Text(pDX, IDC_I_STRENGTH, m_i_strength);
	DDX_Text(pDX, IDC_I_VISITS, m_i_attacks);
	DDX_Check(pDX, IDC_B_POLICE, m_b_police);
	DDX_Text(pDX, IDC_CUSTOM_EDIT, m_i_custom);
	DDX_Text(pDX, IDC_TAGS_RECIEVED, m_i_tags_recieved);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_player_edit, CDialog)
	//{{AFX_MSG_MAP(dlg_player_edit)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM, OnSelchangeComboCustom)
	ON_EN_UPDATE(IDC_CUSTOM_EDIT, OnUpdateCustomEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_player_edit message handlers

BOOL dlg_player_edit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString cst_temp;

	if (!(in_range(m_i_play, 0, g_on.i_cur_players)))
	{
	 log_error("Can't edit player %d",m_i_play);
	 return true;
	}

	
	if (p_play[m_i_play].b_active == false)
	{
	  //player is deleted
 	cst_temp.Format("Account %d - Deleted", m_i_play);
	} else
	{
	 if (p_play[m_i_play].p_on == NULL)
	 {
		 //not online
		 cst_temp.Format("Account %d - Offline", m_i_play);
	 } else
	 {
	   //online now
		 cst_temp.Format("Account %d - Online", m_i_play);

	 }


	}
   GetDlgItem(IDC_STATUS)->SetWindowText(cst_temp);

	m_cst_logon = p_play[m_i_play].st_logon;
	m_cst_password = p_play[m_i_play].st_password;
	m_cst_name = p_play[m_i_play].st_name;
	m_cst_parlor = p_play[m_i_play].st_funeral_name;
 	m_cst_email = p_play[m_i_play].st_email;
	this->m_i_exp = p_play[m_i_play].i_exp;
	this->m_todays_bps = p_play[m_i_play].i_sp;
	this->m_i_money = p_play[m_i_play].i_money;
	this->m_i_turns = p_play[m_i_play].i_turns;
	this->m_b_active = p_play[m_i_play].b_active;
	this->m_b_always = p_play[m_i_play].b_always;
	this->m_b_male = p_play[m_i_play].b_male;
	
	this->m_i_karma = p_play[m_i_play].i_karma;
	this->m_int_rating = p_play[m_i_play].i_rating;
	this->m_int_casket_wood = p_play[m_i_play].a_coffins[::c_coffin_wood];
	this->m_i_casket_silver = p_play[m_i_play].a_coffins[::c_coffin_silver];
	this->m_i_casket_gold = p_play[m_i_play].a_coffins[::c_coffin_gold];
	this->m_i_last_logged = p_play[m_i_play].i_day_last_logged_on;
	this->m_i_plots = p_play[m_i_play].i_plots;
	this->m_i_luck = p_play[m_i_play].i_luck;
	this->m_i_yangs_sent = p_play[m_i_play].i_yangs_sent;

	
	this->m_b_can_attack = p_play[m_i_play].b_can_be_attacked;
	this->m_i_strength = p_play[m_i_play].i_strength;
	this->m_i_hp = p_play[m_i_play].i_hp;
	this->m_i_hp_max = p_play[m_i_play].i_hp_max;
 	this->m_i_attacks = p_play[m_i_play].i_player_fights;
   	this->m_b_police = p_play[m_i_play].b_police_protected;
	this->m_i_psych = p_play[m_i_play].i_psych;
     this->m_i_tags_recieved = p_play[m_i_play].i_tags_recieved_today;

	if (m_i_last_logged == glo.i_game_age)
	{
		//last logon was today
	     cst_temp = "(today)";
	} else
	{
	  //some days ago?
	   cst_temp.Format("(%d days ago)", glo.i_game_age - m_i_last_logged);
	}
    GetDlgItem(IDC_LAST_TEXT)->SetWindowText(cst_temp);
 
	
	//fill in info about IP addresses
   cst_temp.Format("%s", inet_ntoa(p_play[m_i_play].last_ip));
   GetDlgItem(IDC_LAST_IP)->SetWindowText(cst_temp);
   cst_temp.Format("%s", inet_ntoa(p_play[m_i_play].old_ip));
   GetDlgItem(IDC_OLD_IP)->SetWindowText(cst_temp);

   //copy all custom values to our temp array
   memcpy(&m_a_custom, &p_play[m_i_play].a_custom, sizeof(int)*C_CUSTOM_SIZE);

   m_i_custom = 0;
   //populate our combo box
   int i_default_custom = 0;
   SetupCustomCombo();
   m_combo_custom.SetCurSel(i_default_custom);
   m_a_custom[m_combo_custom.GetCurSel()] = m_a_custom[i_default_custom];
   m_i_custom = m_a_custom[i_default_custom];
 



   UpdateData(d_to_window);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_player_edit::SetupCustomCombo()
{

    CString cst_temp;
    for (int i=0; i < C_CUSTOM_SIZE; i++)
    {
        
        if (i == C_CUSTOM_FIRE_ALARM) cst_temp = "Fire Alarm Installed"; else
        if (i == C_CUSTOM_SAFE) cst_temp = "Safe Installed"; else
        if (i == C_CUSTOM_PADLOCK) cst_temp = "Heavy Padlock Installed"; else
        if (i == C_CUSTOM_LOCKPICK) cst_temp = "Lockpicks on hand"; else
        if (i == C_CUSTOM_PADLOCK_STRENGTH) cst_temp = "Padlock Health (100 is max)"; else

        
        cst_temp.Format("Custom Var %d",i);
        
        m_combo_custom.AddString(cst_temp);

    }

}


void dlg_player_edit::OnOK() 
{
    UpdateData(d_to_var);
	
	strcpy(p_play[m_i_play].st_logon,(LPCSTR)m_cst_logon);
	
	strcpy(p_play[m_i_play].st_password ,(LPCSTR) m_cst_password);
	strcpy(p_play[m_i_play].st_name ,(LPCSTR) m_cst_name) ;
	strcpy(p_play[m_i_play].st_funeral_name ,(LPCSTR) m_cst_parlor);
 	strcpy(p_play[m_i_play].st_email ,(LPCSTR) m_cst_email);
	
	if (p_play[m_i_play].b_active != m_b_active)
		if (p_play[m_i_play].p_on != NULL)
		{
		  //they are trying to delete someone who is online.. they can't do that!
		  m_b_active = true; //turn them back on!
		  MessageBox("Sorry, cannot delete the guy, he's online.  Kick him first.","Woah nelly!");
		}


	p_play[m_i_play].b_always = m_b_always;
	p_play[m_i_play].b_active = m_b_active == true;
	p_play[m_i_play].b_male = m_b_male;
	p_play[m_i_play].i_exp = m_i_exp;
	p_play[m_i_play].i_sp = m_todays_bps;
    p_play[m_i_play].i_money = m_i_money;
    p_play[m_i_play].i_turns = m_i_turns;
	
	p_play[m_i_play].i_karma = this->m_i_karma;
	p_play[m_i_play].i_rating = this->m_int_rating;

	p_play[m_i_play].a_coffins[::c_coffin_wood] = this->m_int_casket_wood;
	p_play[m_i_play].a_coffins[::c_coffin_silver] = this->m_i_casket_silver;
	p_play[m_i_play].a_coffins[::c_coffin_gold] = this->m_i_casket_gold;
	p_play[m_i_play].i_psych = this->m_i_psych;
	p_play[m_i_play].i_plots = m_i_plots;
	p_play[m_i_play].i_luck = m_i_luck;
	p_play[m_i_play].i_yangs_sent = m_i_yangs_sent;
	p_play[m_i_play].i_day_last_logged_on = this->m_i_last_logged;

	p_play[m_i_play].b_can_be_attacked  = m_b_can_attack;

	p_play[m_i_play].i_hp = m_i_hp;
	p_play[m_i_play].i_hp_max = m_i_hp_max;
	p_play[m_i_play].i_strength = m_i_strength;
	p_play[m_i_play].i_player_fights = m_i_attacks;
	p_play[m_i_play].b_police_protected  = m_b_police;
    p_play[m_i_play].i_tags_recieved_today = m_i_tags_recieved;

    //copy all custom values from our temp array
    memcpy(&p_play[m_i_play].a_custom, &m_a_custom, sizeof(int)*C_CUSTOM_SIZE);

	CDialog::OnOK();
}



void dlg_player_edit::OnSelchangeComboCustom() 
{
    int i_new_index = m_combo_custom.GetCurSel();
    //LogMsg("Selected %d.", i_new_index);
    SetCustom(i_new_index);
} 


void dlg_player_edit::SetCustom(int i_index)
{
   m_combo_custom.SetCurSel(i_index);

   //setup our data box with the default
   UpdateData(D_TO_VAR);
   m_i_custom = m_a_custom[i_index];
   UpdateData(D_TO_WINDOW);

}

void dlg_player_edit::OnUpdateCustomEdit() 
{
   UpdateData(D_TO_VAR);
   m_a_custom[m_combo_custom.GetCurSel()] = m_i_custom;
 	
}
