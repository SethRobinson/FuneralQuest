// dlg_edit_choose.cpp : implementation file
//

#include "stdafx.h"
#include "fq.h"
#include "dlg_edit_choose.h"
#include "std_all.h"

#define d_pchoose_list_account 0
#define d_pchoose_list_logon 1
#define d_pchoose_list_handle 2
#define d_pchoose_list_status 3
#define d_pchoose_list_ip_address 4

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_choose dialog


dlg_edit_choose::dlg_edit_choose(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_edit_choose::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_edit_choose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlg_edit_choose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_edit_choose)
	DDX_Control(pDX, IDC_PLAYERS, m_list_choose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_edit_choose, CDialog)
	//{{AFX_MSG_MAP(dlg_edit_choose)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYERS, OnRclickPlayers)
	ON_COMMAND(ID_OPTIONS_EDIT, OnOptionsEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYERS, OnDblclkPlayers)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PLAYERS, OnColumnclickPlayers)
	ON_COMMAND(ID_OPTIONS_BAN, OnOptionsBan)
	ON_COMMAND(ID_OPTIONS_KICK, OnOptionsKick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_edit_choose message handlers

BOOL dlg_edit_choose::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//setup list of players to choose from
	setup_listbox();
	UpdateData(d_to_window);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_edit_choose::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void dlg_edit_choose::setup_listbox()
{
	 m_list_choose.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//Set up our tables
	 m_list_choose.InsertColumn(d_pchoose_list_account, "Account", LVCFMT_LEFT, 60);
	 m_list_choose.InsertColumn(d_pchoose_list_logon, "Logon", LVCFMT_LEFT, 100);
	 m_list_choose.InsertColumn(d_pchoose_list_handle, "Handle", LVCFMT_LEFT, 100);
	 m_list_choose.InsertColumn(d_pchoose_list_status, "Status", LVCFMT_LEFT, 70);
	 m_list_choose.InsertColumn(d_pchoose_list_ip_address, "Last IP", LVCFMT_LEFT, 100);

	 //now fill in the data
	 this->fill_in_listbox();
}



void dlg_edit_choose::fill_in_listbox()
{
	//get all players and throw them in this

	for (int i=0; i < g_on.i_cur_players; i++)
	{
	  //add guy
	 int i_desired_index = m_list_choose.GetItemCount();  
	 int i_index = m_list_choose.InsertItem(i_desired_index, "Temp"); //blank item created
 	 	if (i_index == -1)
	 {
	   log_error("Error creating choose player list");
	   return;
	 }

	 m_list_choose.SetItemData(i_index,i_index);  //Used later for sorting
	 
	 update_player(i_index, i_index);
	 //now fill in the details
	
	}

}

void dlg_edit_choose::OnRclickPlayers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
   POINT pt;
   GetCursorPos(&pt);
	
	CMenu menu;
	menu.LoadMenu(IDR_P_MENU);
	CMenu* popup = menu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);
	*pResult = 0;
//	List Control
}

void dlg_edit_choose::OnOptionsEdit() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_list_choose.GetFirstSelectedItemPosition();
	if (pos)
	{
	 //something is selected
	 int i_index = m_list_choose.GetNextSelectedItem(pos);
	 dlg_player_edit dlg;
	 dlg.m_i_play = atol(m_list_choose.GetItemText(i_index, 0));
	 dlg.DoModal();
	
	 //update our list in case they changed something
	 update_player(i_index, dlg.m_i_play);
	} else
	{
	   //nothing is highlighted, do nothing.
	}
}

void dlg_edit_choose::OnDblclkPlayers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnOptionsEdit();

	*pResult = 0;
}

void dlg_edit_choose::update_player(int i_index, int i_play)
{
     
	 int i = i_index;
	 CString cst_temp;
	 cst_temp.Format("%d",i_play);
	 m_list_choose.SetItemText(i, d_pchoose_list_account, cst_temp ); //Set name field
 	 m_list_choose.SetItemText(i, d_pchoose_list_logon, p_play[i_play].st_logon); //Set name field
 	 m_list_choose.SetItemText(i, d_pchoose_list_handle, p_play[i_play].st_name); //Set name field

	 if (p_play[i_play].p_on == NULL) cst_temp = ""; else cst_temp = "On Now";
	 if (!p_play[i_play].b_active) cst_temp = "Deleted";
	 
	 m_list_choose.SetItemText(i, d_pchoose_list_status, cst_temp); //Set name field
	 cst_temp = inet_ntoa(p_play[i_play].last_ip);
	 m_list_choose.SetItemText(i, d_pchoose_list_ip_address, cst_temp); //Set name field

}

CListCtrl	*p_g_list_choose = NULL;


  static int CALLBACK 
ComparePlay(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
   int i_type = (int) lParamSort;
   CListCtrl* pListCtrl = p_g_list_choose;
  
   if (  (i_type == 0) || (i_type == 4))
   {
	 //numerical sort
	 int i_a  = atol(pListCtrl->GetItemText(lParam2, i_type));
     int i_b =  atol(pListCtrl->GetItemText(lParam1, i_type));
 
	 if (i_a < i_b) return 1;
	 if (i_a > i_b) return -1;
	 return 0; //they are the same
   }
   
   
   
   CString    strItem1 = pListCtrl->GetItemText(lParam1, i_type);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, i_type);
  // LogMsg("Comparing %s to %s..",strItem1, strItem2);
   return strItem1.CompareNoCase(strItem2);
}

void dlg_edit_choose::OnColumnclickPlayers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//	pNMListView->SortItems(CompareItems, (LPARAM) p->iSubItem);
	 //	    LogMsg("Click collumn %d!", pNMListView->iSubItem);
			p_g_list_choose = &m_list_choose;
			m_list_choose.SortItems(ComparePlay, (LPARAM) pNMListView->iSubItem);
			
				int i_max = m_list_choose.GetItemCount();
    
  
    //update list	
	for (int i = 0; i < i_max; i++)
	{
			m_list_choose.SetItemData(i, i); 
	}
	
			*pResult = 0;
}

void dlg_edit_choose::OnOptionsBan() 
{
	POSITION pos = m_list_choose.GetFirstSelectedItemPosition();
	if (pos)
	{
	 //something is selected
	 int i_index = m_list_choose.GetNextSelectedItem(pos);
	
	 int i_play = atol(m_list_choose.GetItemText(i_index, 0));

	 char st_ip[15];
    sprintf(st_ip, "%s",inet_ntoa( p_play[i_play].last_ip));


	 //Ban player
	 CString cst_temp;
	 
	 if (i_play != -1)
	 {
	 	 cst_temp.Format("Name: %s, Parlor: %s",p_play[i_play].st_name, p_play[i_play].st_funeral_name);
	 }

	  CString cst_final;
	  cst_final.Format("ban|%s|Banned %s:%s. %s\r\n",st_ip, 
		  show_small_date(),show_time(), cst_temp);
	  add_text((char*)(LPCSTR)cst_final, "ban.txt"); 
	  LogMsg("Added command to ban.txt: %s", cst_final);
	  load_ban_filter_from_file();
	} else
	{
	   //nothing is highlighted, do nothing.
	}

}

void dlg_edit_choose::OnOptionsKick() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_list_choose.GetFirstSelectedItemPosition();
	if (pos)
	{
	 //something is selected
	 int i_index = m_list_choose.GetNextSelectedItem(pos);
	
	 int i_play = atol(m_list_choose.GetItemText(i_index, 0));

     if (p_play[i_play].p_on)
	 {
	 g_on.online_delete_guy(p_play[i_play].p_on->i_index);
	 } else
	 {

	  LogMsg("Can't kick %s, not online now.",p_play[i_play].st_name);
	 }



	}	
}
