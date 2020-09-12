#if !defined(AFX_DLG_SIMPLE_EDIT_H__1964C713_A027_49B3_BC4D_75390E14F0AE__INCLUDED_)
#define AFX_DLG_SIMPLE_EDIT_H__1964C713_A027_49B3_BC4D_75390E14F0AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_simple_edit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_simple_edit dialog

class dlg_simple_edit : public CDialog
{
private:
	CString * p_cst_return; //hold address of string we were sent in the start
		// Construction
public:
	bool GetString(CString * cst_temp);
	dlg_simple_edit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_simple_edit)
	enum { IDD = IDD_SIMPLE_EDIT };
	CString	m_cst_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_simple_edit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_simple_edit)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SIMPLE_EDIT_H__1964C713_A027_49B3_BC4D_75390E14F0AE__INCLUDED_)
