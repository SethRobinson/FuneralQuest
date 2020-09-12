#if !defined(AFX_DLG_GAME_SETTINGS_H__EA0E1E3D_9DE4_4C40_AB91_7F063A5B5ED1__INCLUDED_)
#define AFX_DLG_GAME_SETTINGS_H__EA0E1E3D_9DE4_4C40_AB91_7F063A5B5ED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_game_settings.h : header file
//
#include "resource.h"
#include "funeral_file.h"	
#include "server_settings.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// dlg_game_settings dialog

class dlg_game_settings : public CDialog
{
// Construction
public:
	dlg_game_settings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_game_settings)
	enum { IDD = IDD_GAME_SETTINGS };
	int		m_i_starting_gold;
	int		m_i_starting_karma;
	int		m_i_starting_money;
	int		m_i_starting_rating;
	int		m_i_starting_silver;
	int		m_i_starting_wood;
	int		m_i_turns_per_day;
	int		m_i_game_age;
	int		m_i_starting_plots;
	int		m_i_starting_luck;
	CString	m_news;
	float	m_f_luck_odds;
	int		m_i_yangs_per_day;
	int		m_i_risk;
	int		m_i_tourn_days;
	BOOL	m_b_tourn_active;
	int		m_bonus_luck;
	int		m_i_trade_bonus;
	int		m_bonus_bp;
	int		m_i_visits_per_day;
	int		i_starting_strength;
	int		i_starting_hp;
	CString	m_cst_admin_email;
	float	m_f_normal_odds;
	float	m_f_script_odds;
	int		m_i_tags_recievable_per_day;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_game_settings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_game_settings)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnNewDay();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	int m_name_change_cost;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_GAME_SETTINGS_H__EA0E1E3D_9DE4_4C40_AB91_7F063A5B5ED1__INCLUDED_)
