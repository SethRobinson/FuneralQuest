// fq.h : main header file for the FQ application
//

#if !defined(AFX_FQ_H__A776B805_84A6_11D4_98B2_0050DA82179A__INCLUDED_)
#define AFX_FQ_H__A776B805_84A6_11D4_98B2_0050DA82179A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFqApp:
// See fq.cpp for the implementation of this class
//

class CFqApp : public CWinApp
{
public:
	CFqApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFqApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFqApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FQ_H__A776B805_84A6_11D4_98B2_0050DA82179A__INCLUDED_)
