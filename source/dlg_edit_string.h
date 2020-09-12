#if !defined(AFX_DLG_EDIT_STRING_H__5C726262_CEBE_4587_A9BC_63BB59571BA7__INCLUDED_)
#define AFX_DLG_EDIT_STRING_H__5C726262_CEBE_4587_A9BC_63BB59571BA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_edit_string.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_string dialog

class dlg_edit_string : public CDialog
{
// Construction
public:
	dlg_edit_string(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_edit_string)
	enum { IDD = IDD_EDIT_STRING };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_edit_string)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_edit_string)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_EDIT_STRING_H__5C726262_CEBE_4587_A9BC_63BB59571BA7__INCLUDED_)
