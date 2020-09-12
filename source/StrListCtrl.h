#if !defined(AFX_STRLISTCTRL_H__CD05EFD6_D207_4B2B_8C28_BD4545B65503__INCLUDED_)
#define AFX_STRLISTCTRL_H__CD05EFD6_D207_4B2B_8C28_BD4545B65503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStrListCtrl window

class CStrListCtrl : public CListCtrl
{

private:
	int m_i_lines;

	// Construction
public:
	CStrListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool import_text(CString cst_fname);
	bool export_text(CString cst_fname);
	CString GetLine(int i_line);
	bool init_normal();
	bool add_and_auto_number(CString cst_item);
	virtual ~CStrListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStrListCtrl)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRLISTCTRL_H__CD05EFD6_D207_4B2B_8C28_BD4545B65503__INCLUDED_)
