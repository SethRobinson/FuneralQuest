// dlg_karma.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "dlg_karma.h"
#include "funeral_file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_karma dialog


dlg_karma::dlg_karma(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_karma::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_karma)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_karma::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_karma)
	DDX_Control(pDX, IDC_LIST_KARMA, m_list_karma);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_karma, CDialog)
	//{{AFX_MSG_MAP(dlg_karma)
	ON_BN_CLICKED(ID_EXPORT, OnExport)
	ON_BN_CLICKED(ID_IMPORT, OnImport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_karma message handlers

BOOL dlg_karma::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//let's fill in the stuff with info
	m_list_karma.init_normal();
	for (int i=0; i < ::c_max_karma_strings; i++)
	{
		this->m_list_karma.add_and_auto_number(glo.a_st_karma[i]);

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_karma::OnOK() 
{
    //Save all changes
	 
		for (int i=0; i < ::c_max_karma_strings; i++)
	{
		strcpy(glo.a_st_karma[i], m_list_karma.GetLine(i));

	}



	CDialog::OnOK();
}

void dlg_karma::OnExport() 
{
this->m_list_karma.export_text("karma.txt");
}

void dlg_karma::OnImport() 
{
	// TODO: Add your control notification handler code here
m_list_karma.import_text("karma.txt");
	
}
