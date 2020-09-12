#if !defined(AFX_DLG_EDIT_CHOOSE_H__3AAD0341_C39B_4843_86C5_D56808FF85EA__INCLUDED_)
#define AFX_DLG_EDIT_CHOOSE_H__3AAD0341_C39B_4843_86C5_D56808FF85EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_edit_choose.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_choose dialog

class dlg_edit_choose : public CDialog
{
// Construction
public:
	void dlg_edit_choose::update_player(int i_index, int i_play);

	void fill_in_listbox();
	void setup_listbox();
	dlg_edit_choose(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_edit_choose)
	enum { IDD = IDD_EDIT_CHOOSE };
	CListCtrl	m_list_choose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_edit_choose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_edit_choose)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRclickPlayers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionsEdit();
	afx_msg void OnDblclkPlayers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclickPlayers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionsBan();
	afx_msg void OnOptionsKick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_CHOOSE_H__3AAD0341_C39B_4843_86C5_D56808FF85EA__INCLUDED_)
