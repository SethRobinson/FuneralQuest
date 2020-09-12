#if !defined(AFX_SERVER_H__69B2B021_84CC_11D4_98B2_0050DA82179A__INCLUDED_)
#define AFX_SERVER_H__69B2B021_84CC_11D4_98B2_0050DA82179A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Server.h : header file
//
#include "server_connect.h"
#include "server_settings.h"
void ResetUpdateTimers();
/////////////////////////////////////////////////////////////////////////////
// CServer dialog

class CServer : public CDialog
{
// Construction
public:
	CServer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServer)
	enum { IDD = IDD_SERVER_SETUP };
	CString	m_port;
	BOOL	m_extra_debug;
	int		m_i_auto_save_min;
	int		m_i_anon_min;
	int		m_i_player_min;
	BOOL	m_b_log;
	BOOL	m_b_sound;
	BOOL	m_b_ip_check;
	BOOL	m_b_advertise;
	CString	m_cst_license_description;
	CString	m_cst_license_logon;
	CString	m_cst_license_password;
	CString	m_game_name;
	BOOL	m_b_maint_delete;
	int		m_i_max_players;
	int		m_i_inactive_days;
	BOOL	m_b_swear_filter;
	CString	m_cst_force_ip;
	BOOL	m_b_sapi_enabled;
	int		m_i_max_online;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServer)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H__69B2B021_84CC_11D4_98B2_0050DA82179A__INCLUDED_)
