// dlg_game_settings.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "dlg_game_settings.h"
#include "mfc_all.h"
#include "maint.h"
#include "funeral.h"  
#include "reset_game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_game_settings dialog


dlg_game_settings::dlg_game_settings(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_game_settings::IDD, pParent)
	, m_name_change_cost(0)
{
	//{{AFX_DATA_INIT(dlg_game_settings)
	m_i_starting_gold = 0;
	m_i_starting_karma = 0;
	m_i_starting_silver = 0;
	m_i_starting_wood = 0;
	m_i_turns_per_day = 0;
	m_i_game_age = 0;
	m_i_starting_plots = 0;
	m_i_starting_luck = 0;
	m_news = _T("");
	m_f_luck_odds = 0.0f;
	m_i_yangs_per_day = 0;
	m_i_risk = 0;
	m_i_tourn_days = 0;
	m_b_tourn_active = FALSE;
	m_bonus_luck = 0;
	m_i_trade_bonus = 0;
	m_bonus_bp = 0;
	m_i_visits_per_day = 0;
	i_starting_strength = 0;
	i_starting_hp = 0;
	m_cst_admin_email = _T("");
	m_f_normal_odds = 0.0f;
	m_f_script_odds = 0.0f;
	m_i_tags_recievable_per_day = 0;
	m_name_change_cost = 0;
	//}}AFX_DATA_INIT
}


void dlg_game_settings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_game_settings)
	DDX_Text(pDX, IDC_STARTING_KARMA, m_i_starting_karma);
	DDX_Text(pDX, IDC_STARTING_SILVER, m_i_starting_silver);
	DDX_Text(pDX, IDC_STARTING_WOOD, m_i_starting_wood);
	DDX_Text(pDX, IDC_TURNS_PER_DAY, m_i_turns_per_day);
	DDX_Text(pDX, IDC_GAME_AGE, m_i_game_age);
	DDX_Text(pDX, IDC_STARTING_PLOTS, m_i_starting_plots);
	DDX_Text(pDX, IDC_STARTING_LUCK, m_i_starting_luck);
	DDV_MinMaxInt(pDX, m_i_starting_luck, 0, 100);
	DDX_Text(pDX, IDC_NEWS, m_news);
	DDX_Text(pDX, IDC_LUCK_ODDS, m_f_luck_odds);
	DDX_Text(pDX, IDC_YANGS_PER_DAY, m_i_yangs_per_day);
	DDX_Text(pDX, IDC_RISK, m_i_risk);
	DDX_Text(pDX, IDC_TOURN_DAYS, m_i_tourn_days);
	DDX_Check(pDX, IDC_TOURN_ACTIVE, m_b_tourn_active);
	DDX_Text(pDX, IDC_BONUS_LUCK, m_bonus_luck);
	DDX_Text(pDX, IDC_TRADE_BONUS, m_i_trade_bonus);
	DDX_Text(pDX, IDC_BONUS_EXP, m_bonus_bp);
	DDX_Text(pDX, IDC_I_VISITS_PER_DAY, m_i_visits_per_day);
	DDX_Text(pDX, IDC_STARTING_STRENGTH, i_starting_strength);
	DDX_Text(pDX, IDC_STARTING_HP, i_starting_hp);
	DDX_Text(pDX, IDC_ADMINS_EMAIL, m_cst_admin_email);
	DDX_Text(pDX, IDC_NORMAL_ODDS, m_f_normal_odds);
	DDX_Text(pDX, IDC_SCRIPT_ODDS, m_f_script_odds);
	DDX_Text(pDX, IDC_TAGS_RECIEVABLE_PER_DAY, m_i_tags_recievable_per_day);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_CHANGE_NAME_COST, m_name_change_cost);
	DDV_MinMaxInt(pDX, m_name_change_cost, -1, 9999999);
}


BEGIN_MESSAGE_MAP(dlg_game_settings, CDialog)
	//{{AFX_MSG_MAP(dlg_game_settings)
	ON_BN_CLICKED(IDC_NEW_DAY, OnNewDay)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_game_settings message handlers

BOOL dlg_game_settings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//put the defaults in
  	m_i_starting_karma = glo.i_starting_karma;
	this->m_i_turns_per_day = glo.i_turns_per_day;
	
	this->m_i_starting_gold = glo.a_casket_sp[::c_coffin_gold];

    this->m_i_starting_wood = glo.a_casket_sp[::c_coffin_wood];

    this->m_i_starting_silver = glo.a_casket_sp[::c_coffin_silver];
	this->m_i_game_age = glo.i_game_age;
	this->m_i_starting_plots = glo.i_plot_sp;
	this->m_i_starting_luck = glo.i_starting_luck;
	this->m_news.Format("%s", glo.st_news);
	this->m_f_luck_odds = glo.f_luck_odds;
	
	this->m_f_normal_odds = glo.f_normal_odds;
    this->m_f_script_odds = glo.f_script_odds;


    this->m_i_yangs_per_day = glo.i_yangs_per_day;
	this->m_i_risk = glo.i_risk;
	this->m_b_tourn_active = glo.b_tourn_active;
	this->m_i_tourn_days = glo.i_tourn_days;
	this->m_bonus_luck = glo.i_bonus_luck;
	this->m_i_trade_bonus = glo.i_trade_bonus;
	this->m_bonus_bp = glo.i_bonus_exp;
	this->m_i_visits_per_day = glo.i_visits_per_day;
	this->i_starting_hp = glo.i_starting_hp;
	this->i_starting_strength = glo.i_starting_strength;
    this->m_i_tags_recievable_per_day = glo.i_tags_recievable_per_day;
    this->m_name_change_cost = glo.i_name_change_cost;

    m_cst_admin_email.Format("%s",glo.st_admin_email);
	//tm today = *localtime( &ltime );
//	long time_now = (long)today.tm_mday + 256L*(long)today.tm_mon + 65536L*(long)today.tm_year;


	
	UpdateData(d_to_window);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_game_settings::OnOK() 
{
	// TODO: Add extra validation here
	LogMsg("Updating game data.");
	UpdateData(d_to_var);

	glo.i_starting_money = this->m_i_starting_money;
	glo.i_starting_karma = this->m_i_starting_karma;
	glo.i_starting_rating = this->m_i_starting_rating;
	glo.i_turns_per_day = this->m_i_turns_per_day;
	glo.i_game_age = this->m_i_game_age;
	glo.a_casket_sp[::c_coffin_wood]  = m_i_starting_wood;
	glo.a_casket_sp[::c_coffin_silver] = m_i_starting_silver;
	glo.a_casket_sp[::c_coffin_gold] = m_i_starting_gold;
	glo.i_plot_sp = m_i_starting_plots;
	glo.i_starting_luck = m_i_starting_luck;
	glo.f_luck_odds = m_f_luck_odds;
	glo.i_yangs_per_day = this->m_i_yangs_per_day;
	glo.i_risk = m_i_risk;
	glo.b_tourn_active = m_b_tourn_active != false;
	glo.i_tourn_days = m_i_tourn_days;
	glo.i_bonus_exp = m_bonus_bp;
	glo.i_bonus_luck = m_bonus_luck;
	glo.i_trade_bonus = m_i_trade_bonus;
	glo.i_visits_per_day = m_i_visits_per_day;
	strcpy(glo.st_news, m_news);
	glo.i_starting_hp = this->i_starting_hp;
	glo.i_starting_strength = this->i_starting_strength;
	strcpy(glo.st_admin_email, this->m_cst_admin_email);

	glo.f_normal_odds = m_f_normal_odds;
    glo.f_script_odds = m_f_script_odds;
	
    glo.i_tags_recievable_per_day = m_i_tags_recievable_per_day;
	glo.i_name_change_cost = m_name_change_cost;

	build_server_settings_htm(); //build the html we show people

	file_config_save();
	CDialog::OnOK();
}

void dlg_game_settings::OnNewDay() 
{
	::maint_new_day();
	this->OnInitDialog();
}

void dlg_game_settings::OnReset() 
{
	
	
	// TODO: Add your control notification handler code here
  //let's reset the game
   	if (MessageBox("Are you sure you want to delete all players and completely erase the game?", "RESET GAME?", MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
	 	if (MessageBox("Ok, are you really, really, REALLY sure?  You can't undo this.", "RESET GAME?", MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{

		shutdown_server();
		LogMsg("Resetting game...");
		reset_game();
		server_init();
	    SendMessage(WM_CLOSE);
		}
	}
			

}

