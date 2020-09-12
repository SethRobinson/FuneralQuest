#if !defined(AFX_DLG_PLAYER_EDIT_H__34557C2A_101F_4F73_9396_934A6F0E25CA__INCLUDED_)
#define AFX_DLG_PLAYER_EDIT_H__34557C2A_101F_4F73_9396_934A6F0E25CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_player_edit.h : header file
//

#include "player_struct.h"

/////////////////////////////////////////////////////////////////////////////
// dlg_player_edit dialog

class dlg_player_edit : public CDialog
{

public:
	int m_i_play;
	dlg_player_edit(CWnd* pParent = NULL);   // standard constructor
    void dlg_player_edit::SetupCustomCombo();
    void dlg_player_edit::SetCustom(int i_index);

    int m_a_custom[C_CUSTOM_SIZE];

// Dialog Data
	//{{AFX_DATA(dlg_player_edit)
	enum { IDD = IDD_PLAYER_EDIT };
	CComboBox	m_combo_custom;
	CString	m_cst_logon;
	CString	m_cst_name;
	CString	m_cst_password;
	CString	m_cst_parlor;
	CString	m_cst_email;
	int		m_i_money;
	int		m_i_exp;
	int		m_i_turns;
	BOOL	m_b_active;
	int		m_int_rating;
	int		m_i_casket_silver;
	int		m_int_casket_wood;
	int		m_i_casket_gold;
	int		m_i_karma;
	int		m_i_last_logged;
	int		m_i_plots;
	int		m_todays_bps;
	int		m_i_luck;
	BOOL	m_b_always;
	int		m_i_yangs_sent;
	BOOL	m_b_male;
	BOOL	m_b_can_attack;
	int		m_i_hp;
	int		m_i_hp_max;
	int		m_i_psych;
	int		m_i_strength;
	int		m_i_attacks;
	BOOL	m_b_police;
	int		m_i_custom;
	int		m_i_tags_recieved;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_player_edit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_player_edit)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboCustom();
	afx_msg void OnUpdateCustomEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PLAYER_EDIT_H__34557C2A_101F_4F73_9396_934A6F0E25CA__INCLUDED_)
