// dlg_simple_edit.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "dlg_simple_edit.h"
#include "mfc_all.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_simple_edit dialog


dlg_simple_edit::dlg_simple_edit(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_simple_edit::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_simple_edit)
	m_cst_string = _T("");
	//}}AFX_DATA_INIT
}


void dlg_simple_edit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_simple_edit)
	DDX_Text(pDX, IDC_STRING, m_cst_string);
	DDV_MaxChars(pDX, m_cst_string, 78);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_simple_edit, CDialog)
	//{{AFX_MSG_MAP(dlg_simple_edit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_simple_edit message handlers

bool dlg_simple_edit::GetString(CString *cst_temp)
{
	m_cst_string = *cst_temp;
	this->p_cst_return = cst_temp; //hold pointer to external string so we can
	//put the answer in it later
	
	this->DoModal();

	return true;
}

void dlg_simple_edit::OnOK() 
{
	//They must have hit enter, ok, save the modified string back into the
	//cstring pointer that we've been saving here
	UpdateData(d_to_var);
	this->p_cst_return->Format("%s", this->m_cst_string);
	CDialog::OnOK();
}

BOOL dlg_simple_edit::OnInitDialog() 
{
	CDialog::OnInitDialog();

			 //SWP_NOSIZE
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
