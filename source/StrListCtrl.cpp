// StrListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "dlg_simple_edit.h"
#include "mfc_all.h"
#include "all.h"
#include "StrListCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStrListCtrl




CStrListCtrl::CStrListCtrl()
{
  
}

CStrListCtrl::~CStrListCtrl()
{
}


BEGIN_MESSAGE_MAP(CStrListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CStrListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStrListCtrl message handlers

bool CStrListCtrl::add_and_auto_number(CString cst_item)
{
  	//first add one
	
	int i_desired_index = GetItemCount();  
	int i_index = InsertItem(i_desired_index, "Temp"); //blank item created
 	 	if (i_index == -1)
	 {
	   OutputDebugString("StrListCtrl-add_and_auto_number failed to get #");
	   return false;
	 }

	 //increment our line counter
		this->m_i_lines++;

	 //now just fill in the info
	  CString cst_temp;
	 cst_temp.Format("%d",m_i_lines);
	 SetItemText(i_index, 0, cst_temp ); //Set name field
 	 SetItemText(i_index, 1, cst_item); //Set name field
	 return true;
	
}

bool CStrListCtrl::init_normal()
{
 //initializing 
   SetExtendedStyle(LVS_EX_FULLROWSELECT);
   	//Set up our tables
	InsertColumn(0, "#", LVCFMT_LEFT, 60);
	InsertColumn(1, "Data", LVCFMT_LEFT, 500);

	//set starting lines to 0
	this->m_i_lines = 0;

   return true;
}


void CStrListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos)
	{
	 
		//something is selected, let's edit it
	 int i_index = GetNextSelectedItem(pos);
	
	 //this pops up an edit box and lets the user change any string
	 //I call DoModel in GetString btw
	 dlg_simple_edit dlg;
	 CString cst_temp = GetItemText(i_index, 1);
	 dlg.GetString(&cst_temp);
	 //let's update the changed string in our list
//	 OutputDebugString(cst_temp);
	 SetItemText(i_index, 1, cst_temp);

	} else
	{
	   //nothing is highlighted, do nothing.
	}

	*pResult = 0;
	
	*pResult = 0;
}

CString CStrListCtrl::GetLine(int i_line)
{
   return GetItemText(i_line, 1);

}

bool CStrListCtrl::export_text(CString cst_fname)
{
	CString cst_filename = save_txt_file_dialog(cst_fname);
	if (cst_filename == "") return false; //no file was selected

	//save out text data
	LogMsg("Saving to %s..", cst_filename);
    unlink(cst_filename); //delete old file
	CString cst_line = "";
	for (int i=0; i < this->m_i_lines; i++)
	{
	    //save each line out to the text file
	 	cst_line.Format("%d|%s|\r\n",i+1,GetItemText(i, 1));
		add_text((char *)(LPCSTR)cst_line, (char *)(LPCSTR)cst_filename);
	}

	return true;

}

bool CStrListCtrl::import_text(CString cst_fname)
{
   	CString cst_filename = load_txt_file_dialog(cst_fname);
	if (cst_filename == "") return false; //no file was selected
	LogMsg("Loading %s..", cst_filename);
 
	//read every line from this text file

   FILE *fp = open_file_cst(cst_filename);
   CString cst_line;
   
   while (get_line_from_file_cst(fp, &cst_line))
   {
     //read a line.
	
	   CString cst_num; //index
	   CString cst_data; //actual data
	   
	   separate_string_mfc(cst_line, 1, '|', &cst_num);
	   separate_string_mfc(cst_line, 2, '|', &cst_data);

	   int i_index = atol((LPCSTR)cst_num);

	   if (in_range(i_index, 1, m_i_lines))
	   {
		 //set data
	   	 SetItemText(i_index-1, 1, cst_data);
	   } else
	   {
//	   log_error("Can't add entry %s, not in range. (it said %s).",cst_num, cst_data);
	   }


   }
   fclose(fp);

   return true;
}
