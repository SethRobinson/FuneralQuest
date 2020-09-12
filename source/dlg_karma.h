#if !defined(AFX_DLG_KARMA_H__B658C07B_0DE2_491E_A471_C6B6A78062D9__INCLUDED_)
#define AFX_DLG_KARMA_H__B658C07B_0DE2_491E_A471_C6B6A78062D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_karma.h : header file
//
#include "strlistctrl.h" 
/////////////////////////////////////////////////////////////////////////////
// dlg_karma dialog

class dlg_karma : public CDialog
{
// Construction
public:
	dlg_karma(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_karma)
	enum { IDD = IDD_KARMA_TEXT };
	CStrListCtrl	m_list_karma;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_karma)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_karma)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnExport();
	afx_msg void OnImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_KARMA_H__B658C07B_0DE2_491E_A471_C6B6A78062D9__INCLUDED_)
