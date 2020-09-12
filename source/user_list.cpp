//Stuff for our listview that we see current online people on

//Defines to make switching stuff later easier on the brain

#include "user_list.h"
#include "dlg_player_edit.h"

#define d_list_handle 0
#define d_list_logon 1
#define d_list_status 2
#define d_list_sex 3
#define d_list_hits 4
#define d_list_time_on 5
#define d_list_last_ping 6
#define d_list_ip_address 7

void CFqDlg::reinit_user_list()
{
  m_list_on.DeleteAllItems();
}


void CFqDlg::init_user_list()
{
	 //Set up our tables
		m_list_on.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	 m_list_on.InsertColumn(d_list_handle, "Handle", LVCFMT_LEFT, 100);
	 m_list_on.InsertColumn(d_list_logon, "Logon", LVCFMT_LEFT, 100);
	 m_list_on.InsertColumn(d_list_status, "Status", LVCFMT_LEFT, 70);
	 m_list_on.InsertColumn(d_list_sex, "Sex", LVCFMT_LEFT, 30);
	 m_list_on.InsertColumn(d_list_hits, "Hits", LVCFMT_LEFT, 40);
	 m_list_on.InsertColumn(d_list_time_on, "Session Time", LVCFMT_LEFT, 80);
	 m_list_on.InsertColumn(d_list_last_ping, "Since last hit", LVCFMT_LEFT, 80);
	 m_list_on.InsertColumn(d_list_ip_address, "IP Address", LVCFMT_LEFT, 90);
}


bool CFqDlg::user_list_add(LPREQUEST lpReq)
{
	//New blood logged on. Create him on our table
	int i_desired_index, i_index;
	i_desired_index = m_list_on.GetItemCount();  
	
	i_index = m_list_on.InsertItem(i_desired_index, "Temp"); //blank item created
	
	if (i_index == -1)
	 {
	   log_error("Error creating new list view control for new guy at IP %s.", my_ip(lpReq));
	   return false;
	 }
  	  m_list_on.SetItemData(i_index,lpReq->i_on_index);  //Used later for sorting
//	  Msg("Setting index %d",i_index);
	  p_on[lpReq->i_on_index]->i_list_view = i_index;

	  user_list_update(lpReq); //Fill out everything
	 
	 	int i_max = m_list_on.GetItemCount();
	
	for (int i = 0; i < i_max; i++)
	{
		//Update online player index so it knows where to find the newly sorted version of itself
		//for updating
	
		if (p_on [m_list_on.GetItemData(i)] != NULL)
		p_on[m_list_on.GetItemData(i)]->i_list_view = i; else
		{
		  //error
			log_error("Error in user_list_remove, p_on %d doesn't exist event through its on the listview",m_list_on.GetItemData(i));

		}
	}

 

	 return true;
}

void CFqDlg::user_list_remove(int i_guy)
{
  if (!m_list_on.DeleteItem(i_guy))  //bye bye
  {
	//couldn't delete the guy
	  log_error("Couldn't remove visual guy %d from the local people on view",i_guy);
	  
  }
//and of course, resort everything so that p_on can still locate it's visual counterpart
  	int i_max = m_list_on.GetItemCount();
	
	for (int i = 0; i < i_max; i++)
	{
		//Update online player index so it knows where to find the newly sorted version of itself
		//for updating
	
		if (p_on [m_list_on.GetItemData(i)] != NULL)
		p_on[m_list_on.GetItemData(i)]->i_list_view = i; else
		{
		  //error
			log_error("Error in user_list_remove, p_on %d doesn't exist event through its on the listview",m_list_on.GetItemData(i));

		}
	}





}

bool CFqDlg::user_list_update(LPREQUEST lpReq)
{
	CString cst_temp;
	
	//I don't want to type this too many times, so let's just get the index for the list view
	
	int i_index = p_on[lpReq->i_on_index]->i_list_view; //index off listview id
	
	if (p_on[lpReq->i_on_index]->b_logged_on)
	{						   
	 	int i_play = p_on[lpReq->i_on_index]->i_play; //Player record index

		m_list_on.SetItemText(i_index, d_list_handle, p_play[i_play].st_name); //Set name field
		m_list_on.SetItemText(i_index, d_list_logon, p_play[i_play].st_logon); 
		m_list_on.SetItemText(i_index, d_list_status, p_on[lpReq->i_on_index]->cst_location);
	
		
		if (p_play[i_play].b_male)	m_list_on.SetItemText(i_index, d_list_sex, "M");
		else m_list_on.SetItemText(i_index, d_list_sex, "F");

	} else
	{
	 //Not logged on
		m_list_on.SetItemText(i_index, d_list_handle, my_ip(lpReq)); //Set name field
		m_list_on.SetItemText(i_index, d_list_logon, "(Not logged in)"); 
	   	m_list_on.SetItemText(i_index, d_list_status, "Logon");
		m_list_on.SetItemText(i_index, d_list_sex, "?");
			  
	}

	//Applicable to both
	cst_temp.Format("%d", p_on[lpReq->i_on_index]->i_hits);
	m_list_on.SetItemText(i_index, d_list_hits, cst_temp); //Set name field
	m_list_on.SetItemText(i_index, d_list_ip_address, my_ip(lpReq)); //Set name field
	

	user_list_update_time(p_on[lpReq->i_on_index]);
	return true;
}

void CFqDlg::user_list_update_time(online_struct *p_online)
{
	CString cst_temp;
	//I don't want to type this too many times, so let's just get the index for the list view
	int i_index = p_online->i_list_view;

	get_time_difference_in_text(&cst_temp,  p_online->t_logon);
	m_list_on.SetItemText(i_index, d_list_time_on, cst_temp); //Set name field
	
	
	double dbl_elapsed_seconds = get_time_difference_in_text(&cst_temp, p_online->t_last_packet);
	 if (p_online->b_logged_on)
	 {
	 
	 if ( (dbl_elapsed_seconds / 60) >= sys.i_player_kick_minutes)
	{
		//he's been a bad boy.  Boot his ass.
		 broadcast("%s got kicked for inactivity",p_play[p_online->i_play].st_name);
		 g_on.online_delete_guy(p_online->i_index);
		return;
	}
	 } else
	 {
	  //different timeout period for people who are not logged on
	 if ( (dbl_elapsed_seconds / 60) >= sys.i_anon_kick_minutes)
	{
		//he's been a bad boy.  Boot his ass.
		g_on.online_delete_guy(p_online->i_index);
		return;
	}

	 }
	 
	m_list_on.SetItemText(i_index, d_list_last_ping, cst_temp); //Set name field
	m_list_on.SetItemText(i_index, d_list_status, p_online->cst_location);
	
}

static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM i_which_col)
{
	CString sItem[2];
  
	CListCtrl * list_on = &g_dlg_main->m_list_on;
 	
	sItem[0] = list_on->GetItemText((int)lParam1, (int) i_which_col);
	sItem[1] = list_on->GetItemText((int)lParam2, (int) i_which_col);

//	Msg("Sorting item %d.",i_which_col);

		bool bDescending = false;
    if(bDescending /* defined elsewhere */)
      return sItem[1].CompareNoCase(sItem[0]);
    else
      return sItem[0].CompareNoCase(sItem[1]);
}



void CFqDlg::OnColumnclickOn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* p = (NM_LISTVIEW*)pNMHDR;

    //Before we sort, let's associate which row each one is on with it's itemdata.
	
	this->m_list_on.SortItems(CompareItems, (LPARAM) p->iSubItem);
	//Sorted.  Now let's update our online index.
	int i_max = m_list_on.GetItemCount();
	
	for (int i = 0; i < i_max; i++)
	{
		//Update online player index so it knows where to find the newly sorted version of itself
		//for updating
		p_on[m_list_on.GetItemData(i)]->i_list_view = i; 
		//update the actual list so it will be ready for next sort
	//	m_list_on.SetItemData(i, i);

	}



	*pResult = 0;
}

void CFqDlg::OnRclickOn(NMHDR* pNMHDR, LRESULT* pResult) 
{

	 POINT pt;
   GetCursorPos(&pt);
	
	CMenu menu;
	menu.LoadMenu(IDR_P_MENU);
	CMenu* popup = menu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);
	*pResult = 0;

	
}

  void CFqDlg::OnOptionsEdit() 
{
	// TODO: Add your command handler code here
		POSITION pos = m_list_on.GetFirstSelectedItemPosition();
	if (pos)
	{
	 //something is selected
	 int i_index = m_list_on.GetNextSelectedItem(pos);

	 //get name
	 CString cst_name = m_list_on.GetItemText(i_index, 1);
	 
	 int i_play = get_play_from_logon_name(cst_name);
	 
	 if (i_play == -1)
	 {
	  MessageBox("Player has not logged on yet, can't edit.");
	 } else
	 {
		 //Edit player dialog box
		 dlg_player_edit dlg;
		 dlg.m_i_play = i_play;
		 dlg.DoModal();
		 }
	} else
	{
	   //nothing is highlighted, do nothing.
	}
	
}

void CFqDlg::OnDblclkOn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOptionsEdit();
	*pResult = 0;
}
 int find_online_guy_from_list_index(int i_list_index)
{
  
	for (int i=0; i <g_on.i_cur_people; i++)
	{
		if (p_on[i] != NULL)
		{
		  //actually exists
			if (p_on[i]->i_list_view == i_list_index)
			{
				//bingo
				return i;

			}

		}
	}


//couldn't find 'em
	return -1;
}

void CFqDlg::OnOptionsKick() 
{
		POSITION pos = m_list_on.GetFirstSelectedItemPosition();
	if (pos)
	{
	 //something is selected
	 int i_index = m_list_on.GetNextSelectedItem(pos);

	 //get name
	 int i_on_index = m_list_on.GetItemData(i_index);
	 if (i_on_index != -1)
	 {
	   g_on.online_delete_guy(i_on_index);

	 } else
	 {

	  log_error("Problem, can't find p_on account to match this thingie.  Deleting it.");
	  this->user_list_remove(i_index);
	 }

	} else
	{

	 LogMsg("Nothing here to kick.");

	}
	
}
