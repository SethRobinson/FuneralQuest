// dlg_edit_string.cpp : implementation file
//

#include "stdafx.h"
#include "dlg_edit_string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// dlg_edit_string dialog


dlg_edit_string::dlg_edit_string(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_edit_string::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_edit_string)
	//}}AFX_DATA_INIT
}


void dlg_edit_string::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_edit_string)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_edit_string, CDialog)
	//{{AFX_MSG_MAP(dlg_edit_string)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_string message handlers
