// Server.cpp : implementation file
//

//#include "stdafx.h"
//#include "fq.h"
#include "Server.h"
#include "CSpeech.h"

/////////////////////////////////////////////////////////////////////////////
// CServer dialog


CServer::CServer(CWnd* pParent /*=NULL*/)
	: CDialog(CServer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServer)
	m_port = _T("");
	m_extra_debug = FALSE;
	m_i_auto_save_min = 0;
	m_i_anon_min = 0;
	m_i_player_min = 0;
	m_b_log = FALSE;
	m_b_sound = FALSE;
	m_b_ip_check = FALSE;
	m_b_advertise = FALSE;
	m_cst_license_description = _T("");
	m_cst_license_logon = _T("");
	m_cst_license_password = _T("");
	m_game_name = _T("");
	m_b_maint_delete = FALSE;
	m_i_max_players = 0;
	m_i_inactive_days = 0;
	m_b_swear_filter = FALSE;
	m_cst_force_ip = _T("");
	m_b_sapi_enabled = FALSE;
	m_i_max_online = 0;
	//}}AFX_DATA_INIT
}


void CServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServer)
	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Check(pDX, IDC_DEBUG, m_extra_debug);
	DDX_Text(pDX, IDC_AUTO_SAVE_MIN, m_i_auto_save_min);
	DDX_Text(pDX, IDC_ANON_MIN, m_i_anon_min);
	DDX_Text(pDX, IDC_PLAYER_MIN, m_i_player_min);
	DDX_Check(pDX, IDC_LOG, m_b_log);
	DDX_Check(pDX, IDC_SOUND, m_b_sound);
	DDX_Check(pDX, IDC_IP_CHECK, m_b_ip_check);
	DDX_Check(pDX, IDC_B_ADVERTISE, m_b_advertise);
	DDX_Text(pDX, IDC_LICENSE_DESCRIPTION, m_cst_license_description);
	DDV_MaxChars(pDX, m_cst_license_description, 254);
	DDX_Text(pDX, IDC_LICENSE_LOGON, m_cst_license_logon);
	DDV_MaxChars(pDX, m_cst_license_logon, 59);
	DDX_Text(pDX, IDC_LICENSE_PASSWORD, m_cst_license_password);
	DDV_MaxChars(pDX, m_cst_license_password, 59);
	DDX_Text(pDX, IDC_LICENSE_GAME_NAME, m_game_name);
	DDV_MaxChars(pDX, m_game_name, 29);
	DDX_Check(pDX, IDC_B_MAINT_DELETE, m_b_maint_delete);
	DDX_Text(pDX, IDC_MAX_PLAYERS, m_i_max_players);
	DDX_Text(pDX, IDC_INACTIVE_DAYS, m_i_inactive_days);
	DDX_Check(pDX, IDC_SWEAR_FILTER, m_b_swear_filter);
	DDX_Text(pDX, IDC_LICENSE_FORCE_IP, m_cst_force_ip);
	DDX_Check(pDX, IDC_SAPI_ENABLED, m_b_sapi_enabled);
	DDX_Text(pDX, IDC_MAX_ONLINE, m_i_max_online);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServer, CDialog)
	//{{AFX_MSG_MAP(CServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServer message handlers

BOOL CServer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Give all the crap values and such
	m_port.Format("%d", sys.i_port);
	m_extra_debug = sys.b_extra_debug;
	m_b_log = sys.b_log;
	this->m_i_player_min = sys.i_player_kick_minutes;
	this->m_i_anon_min = sys.i_anon_kick_minutes;
	m_i_auto_save_min = sys.i_auto_save_minutes;
    m_b_advertise = sys.b_advertise;
	this->m_cst_license_description = sys.st_license_description;
	m_cst_license_logon = sys.st_license_logon;
	m_cst_license_password = sys.st_license_password;
	this->m_game_name = sys.st_license_game_name;
	this->m_cst_force_ip = sys.st_force_ip;
	m_b_sound= sys.b_sound;
	m_b_ip_check = sys.b_ip_check;
	m_b_maint_delete = sys.b_maint_delete;
	m_i_max_players= sys.i_max_players;
	m_i_inactive_days = sys.i_inactive_days;
	m_i_max_online = sys.i_max_online;

    this->m_b_swear_filter = sys.b_swear_filter;
	this->m_b_sapi_enabled = sys.b_sapi_enabled;

	UpdateData(d_to_window);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CServer::OnOK() 
{
	//it's a go.  Save all values
	UpdateData(d_to_var);

    if (sys.i_port != atoi((LPCSTR)m_port))
    {
        MessageBox("You have changed the port address of the server.  You must restart the FQ server for this to take effect.", "Warning", MB_ICONEXCLAMATION);
    }
    
    sys.i_port = atoi((LPCSTR)m_port);
	sys.b_extra_debug = m_extra_debug;
	sys.b_log = this->m_b_log;
	sys.i_auto_save_minutes = this->m_i_auto_save_min;
	sys.i_player_kick_minutes = this->m_i_player_min;
	sys.i_anon_kick_minutes = this->m_i_anon_min;
	
    //remove illegal characters
    m_cst_license_description.Replace("&", "");
    m_cst_license_description.Replace("\\", "");

    m_game_name.Replace("&", "");
    m_game_name.Replace("\\", "");
    
    strcpy(sys.st_license_description, m_cst_license_description);
	strcpy(sys.st_license_logon, m_cst_license_logon);
	strcpy(sys.st_license_password, m_cst_license_password);
	strcpy(sys.st_license_game_name, m_game_name);
	
	(bool)sys.b_advertise = (bool)this->m_b_advertise;
	(bool)sys.b_maint_delete = (bool)this->m_b_maint_delete;
	sys.i_max_players = m_i_max_players;
	sys.i_inactive_days = m_i_inactive_days;

	
	sys.b_sound = this->m_b_sound;
	sys.b_ip_check = this->m_b_ip_check;
	sys.b_swear_filter = this->m_b_swear_filter;
	sys.b_sapi_enabled = this->m_b_sapi_enabled;
    sys.i_max_online = m_i_max_online;

	strcpy(sys.st_force_ip, m_cst_force_ip);
	file_sys_save();
	build_server_settings_htm(); //build the html we show people
   	if (g_glo.p_speech)
	g_glo.p_speech->set_mute(!sys.b_sapi_enabled);
    cls_pro.Reset();
    ResetUpdateTimers();

    //force_reconnect();
	CDialog::OnOK();
}
