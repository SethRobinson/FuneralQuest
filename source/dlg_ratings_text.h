#if !defined(AFX_DLG_RATINGS_TEXT_H__04E69CB9_5681_4DEC_BBE0_BF8157B27AA2__INCLUDED_)
#define AFX_DLG_RATINGS_TEXT_H__04E69CB9_5681_4DEC_BBE0_BF8157B27AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_ratings_text.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_ratings_text dialog

class dlg_ratings_text : public CDialog
{
// Construction
public:
	dlg_ratings_text(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_ratings_text)
	enum { IDD = IDD_RATINGS_TEXT };
	CStrListCtrl	m_list_ratings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_ratings_text)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_ratings_text)
	afx_msg void OnImport();
	afx_msg void OnExport();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_RATINGS_TEXT_H__04E69CB9_5681_4DEC_BBE0_BF8157B27AA2__INCLUDED_)
