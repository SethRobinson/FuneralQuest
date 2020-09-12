// dlg_ratings_text.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "resource.h"
#include "funeral_file.h"
#include "StrListCtrl.h"
#include "dlg_ratings_text.h"


/////////////////////////////////////////////////////////////////////////////
// dlg_ratings_text dialog


dlg_ratings_text::dlg_ratings_text(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_ratings_text::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_ratings_text)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_ratings_text::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_ratings_text)
	DDX_Control(pDX, IDC_LIST_RATINGS, m_list_ratings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_ratings_text, CDialog)
	//{{AFX_MSG_MAP(dlg_ratings_text)
	ON_BN_CLICKED(ID_IMPORT, OnImport)
	ON_BN_CLICKED(ID_EXPORT, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_ratings_text message handlers

void dlg_ratings_text::OnImport() 
{
	m_list_ratings.import_text("ratings.txt");
	
}

void dlg_ratings_text::OnExport() 
{
 this->m_list_ratings.export_text("ratings.txt");	
}

void dlg_ratings_text::OnOK() 
{
	// TODO: Add extra validation here
		for (int i=0; i < ::c_max_rating_strings; i++)
	{
		strcpy(glo.a_st_rating[i], m_list_ratings.GetLine(i));
	}

	CDialog::OnOK();
}

BOOL dlg_ratings_text::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//let's fill in the stuff with info
	m_list_ratings.init_normal();
	for (int i=0; i < ::c_max_karma_strings; i++)
	{
		this->m_list_ratings.add_and_auto_number(glo.a_st_rating[i]);

	}

										    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
