// fqDlg.h : header file
//

#if !defined(AFX_FQDLG_H__A776B807_84A6_11D4_98B2_0050DA82179A__INCLUDED_)
#define AFX_FQDLG_H__A776B807_84A6_11D4_98B2_0050DA82179A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFqDlg dialog

#include "vars.h"
#include "CTimer.h"
#include <string>
#include <deque>
using namespace std;

class CFqDlg : public CDialog
{
// Construction
public:
	void reinit_user_list();
	void user_list_remove(int i_guy);
	void user_list_update_time(online_struct *p_online);

	bool user_list_update(LPREQUEST lpReq);
	bool user_list_add(LPREQUEST lpReq);
	void timer_kill();
	bool timer_init();
	void init_user_list();
	void Log(CString cst_message);

	//tray stuff
	void add_to_tray();
	void remove_from_tray();
	void left_clicked_icon();
	void tray_hide_window();
	void tray_icon_menu();


	CFqDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFqDlg)
	enum { IDD = IDD_FQ_DIALOG };
	CListCtrl	m_list_on;
	CString	m_log;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFqDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    deque<string> m_deq_log;
    bool m_b_update_log;
    CTimer m_timer_update_log;
	// Generated message map functions
	//{{AFX_MSG(CFqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSettingsServersetup();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnColumnclickOn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSettingsPlayerEdit();
	afx_msg void OnRclickOn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionsEdit();
	afx_msg void OnDblclkOn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAboutFq();
	afx_msg void OnGameKarmaText();
	afx_msg void OnClose();
	afx_msg void OnKillserver();
	afx_msg void OnRatingText();
	afx_msg void OnGameSettings();
	afx_msg void OnLogon1();
	afx_msg void OnLogon2();
	afx_msg void OnOptionsKick();
	afx_msg void OnGameYangs();
	afx_msg void OnGameNoCustomer();
	afx_msg void OnGameNews();
	afx_msg void OnGameEvents();
	afx_msg void OnGameEditRisks();
	afx_msg void OnViewLog();
	afx_msg void OnViewLogDelete();
	afx_msg void OnViewError();
	afx_msg void OnViewErrorDelete();
	afx_msg void OnNewspaper();
	afx_msg void OnReloadText();
	afx_msg void OnViewStats();
	afx_msg void OnHelp();
	afx_msg void OnHtmlHelp();
	afx_msg void OnSwearFilter();
	afx_msg void OnBanList();
	afx_msg void OnOptionsBan();
	afx_msg void OnHardSell();
	afx_msg void OnNewsCrime();
	afx_msg void OnSoftSell();
	afx_msg void OnGameYangsNice();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FQDLG_H__A776B807_84A6_11D4_98B2_0050DA82179A__INCLUDED_)


extern CFqDlg *g_dlg_main ;
extern HWND g_hwnd_main;
