// fqDlg.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include <direct.h>
#include "file_system.h"
#include "rankings.h"
#include "all.h"
#include "CProtection.h"
#include "globals.h"  
#include "server_connect.h"
#include <assert.h>

			  
//#include "vars.h"

UINT g_timer = 0;
static BOOL  fStarted = FALSE;

const int d_max_log_lines=80;
#define C_UPDATE_LOG_LIMIT_MS 500 //only update the log every half a second max


#include "create_customer.h"
#include "fq.h"
#include "fqDlg.h"

#include "all.h"
#include "mfc_all.h"
#include "reqlist.c"

#include "player_struct.h"
#include "online_struct.h"


online_struct **p_on;
online_global g_on;
CFqDlg *g_dlg_main = NULL;
HWND g_hwnd_main = NULL;


#include "funeral_file.h"
#include "httpa.c"
#include "dlg_player_edit.h"
#include "misc_utils.h"
#include "user_list.h"
#include "maint.h"

#include "funeral.h"
#include "dlg_player_edit.cpp"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "CTimer.h"
#include "server.cpp"
#include "dlg_edit_choose.cpp"
#include "dlg_karma.h"
#include "dlg_ratings_text.h"
#include "dlg_game_settings.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "window_timer.cpp"

void UpdateEverything();

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFqDlg dialog

CFqDlg::CFqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFqDlg)
	m_log = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFqDlg)
	DDX_Control(pDX, IDC_ON, m_list_on);
	DDX_Text(pDX, IDC_LOG, m_log);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFqDlg, CDialog)
	//{{AFX_MSG_MAP(CFqDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_SETTINGS_SERVERSETUP, OnSettingsServersetup)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_ON, OnColumnclickOn)
	ON_COMMAND(ID_SETTINGS_PLAYER_EDIT, OnSettingsPlayerEdit)
	ON_NOTIFY(NM_RCLICK, IDC_ON, OnRclickOn)
	ON_COMMAND(ID_OPTIONS_EDIT, OnOptionsEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_ON, OnDblclkOn)
	ON_COMMAND(ID_ABOUT_FQ, OnAboutFq)
	ON_COMMAND(ID_GAME_KARMA_TEXT, OnGameKarmaText)
	ON_WM_CLOSE()
	ON_COMMAND(ID_KILLSERVER, OnKillserver)
	ON_COMMAND(ID_RATING_TEXT, OnRatingText)
	ON_COMMAND(ID_GAME_SETTINGS, OnGameSettings)
	ON_BN_CLICKED(IDC_LOGON_1, OnLogon1)
	ON_BN_CLICKED(IDC_LOGON_2, OnLogon2)
	ON_COMMAND(ID_OPTIONS_KICK, OnOptionsKick)
	ON_COMMAND(ID_GAME_YANGS, OnGameYangs)
	ON_COMMAND(ID_GAME_NO_CUSTOMER, OnGameNoCustomer)
	ON_COMMAND(ID_GAME_NEWS, OnGameNews)
	ON_COMMAND(ID_GAME_EVENTS, OnGameEvents)
	ON_COMMAND(ID_GAME_EDIT_RISKS, OnGameEditRisks)
	ON_BN_CLICKED(IDC_VIEW_LOG, OnViewLog)
	ON_BN_CLICKED(IDC_VIEW_LOG_DELETE, OnViewLogDelete)
	ON_BN_CLICKED(IDC_VIEW_ERROR, OnViewError)
	ON_BN_CLICKED(IDC_VIEW_ERROR_DELETE, OnViewErrorDelete)
	ON_COMMAND(IDC_NEWSPAPER, OnNewspaper)
	ON_COMMAND(IDC_RELOAD_TEXT, OnReloadText)
	ON_BN_CLICKED(IDC_VIEW_STATS, OnViewStats)
	ON_COMMAND(IDC_HELP, OnHelp)
	ON_COMMAND(IDC_HTML_HELP, OnHtmlHelp)
	ON_COMMAND(ID_SWEAR_FILTER, OnSwearFilter)
	ON_COMMAND(ID_BAN_LIST, OnBanList)
	ON_COMMAND(ID_OPTIONS_BAN, OnOptionsBan)
	ON_COMMAND(IDD_HARD_SELL, OnHardSell)
	ON_COMMAND(IDD_NEWS_CRIME, OnNewsCrime)
	ON_COMMAND(IDD_SOFT_SELL, OnSoftSell)
	ON_COMMAND(ID_GAME_YANGS_NICE, OnGameYangsNice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFqDlg message handlers

BOOL CFqDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
  //  ODS("Initting dialog\r\n");

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
  	
	// TODO: Add extra initialization here
	g_hwnd_main = this->m_hWnd;
	g_dlg_main = this;
	
    m_timer_update_log.set_interval(C_UPDATE_LOG_LIMIT_MS);
    init_user_list(); //initialize the user list view
	server_init();
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFqDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFqDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFqDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFqDlg::Log(CString cst_message)
{
    string temp = cst_message;
    temp += "\r\n";

    m_deq_log.push_front(temp);

    if (m_deq_log.size() > d_max_log_lines)
    {
        m_deq_log.pop_back();
    }
    if (sys.b_log)
    {
        //we're instructed to log info to disk
        add_text((char *)(LPCSTR)temp.c_str(), "log.txt");
    }
  m_log.Empty();

    for (deque<string>::reverse_iterator itor =  m_deq_log.rbegin(); itor !=  m_deq_log.rend(); ++itor)
    {
        m_log += itor->c_str();
    }
 
    if (m_timer_update_log.interval_reached())
    {
    this->UpdateData(d_to_window); 
    CEdit* cedit_status = (CEdit*)GetDlgItem(IDC_LOG);  
    cedit_status->LineScroll(1000000, 0);

    } else
    {
        m_b_update_log = true;
        return;
    }

    m_b_update_log = false;
    return;
    m_log += cst_message;
 
  CEdit* cedit_status = (CEdit*)GetDlgItem(IDC_LOG);  
 // OutputDebugString(cst_message+"\r\n");
	if (sys.b_log)
	{
	  //we're instructed to log info to disk
		add_text((char *)(LPCSTR)cst_message, "log.txt");
	}
 this->UpdateData(d_to_window);

  int i_lines = 0;
  while (i_lines = cedit_status->GetLineCount() > d_max_log_lines)
  {
	 int i_line_size = cedit_status->LineLength(1);
	 m_log.Delete(0, i_line_size+2); //plus 2 is to get the linefeed
	 this->UpdateData(d_to_window);
  } 
	

  this->UpdateData(d_to_window);
  cedit_status->LineScroll(1000000, 0);

}

LRESULT CFqDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
 UINT uID; 
    UINT uMouseMsg; 
 

    uID = (UINT) wParam; 
    uMouseMsg = (UINT) lParam; 
 
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_ADD_TO_LOG)
	{
    
      char *temp = (char *) lParam;
	  this->Log(temp);
     
	  //free the memory that was created to pass this string
	  SAFE_FREE(temp);
    }

    if (message == WM_GOT_SERVER_INFO)
    {

#ifdef _DEBUG
        LogMsg("Connecting to server: %s:%d",g_glo.st_server_address, g_glo.i_server_port);
#endif
        AfxBeginThread(listener_init, NULL);	
    }

    
    if (message == WM_TIMER)
    {
        //is this timer 1?  If so, let's update everything.
        if (wParam == 1)
        {
            if (m_b_update_log)
            {
                if (m_timer_update_log.interval_reached())
                {
                  UpdateData(D_TO_WINDOW);
                  m_b_update_log = false;
               CEdit* cedit_status = (CEdit*)GetDlgItem(IDC_LOG);  
                 cedit_status->LineScroll(1000000, 0);
                  
                }
            }
            
            UpdateEverything();
         }


    }
    
    if (message == WM_SAY_HELLO_TO_SERVER)
	{
	  //finally ready to send hello message to the license server

		say_hello();
           //Let's setup a maint timer
	 timer_init();


	}
	
	 	if (message == WM_GET_SERVER_INFO)
	{
  
            if (!cls_pro.is_validated())
            {
                
                
                LogMsg("Locating master server, please wait...");
            }
        AfxBeginThread(get_server_dat, NULL);
  	}
	 if (message == WM_SYSCOMMAND)
	{

         if (wParam == SC_MINIMIZE)
	 {
	 tray_hide_window();
	 return true;
	 }
	}
	  if (uMouseMsg == WM_LBUTTONDOWN) 
	{ 
   
          if (uID == WM_MY_TRAY_NOTIFICATION)
	   {
		   //left clicked our icon
		   left_clicked_icon();
		  return true;
 
	   }
	 }
	   if (uMouseMsg == WM_RBUTTONDOWN) 
	{ 
   
           if (uID == WM_MY_TRAY_NOTIFICATION)
	   {
		   //show a menu
		   tray_icon_menu();
		  return true;
 
	   }
	 }


	if (message == UM_ASYNC)
	{
	
        //We've got a sockets thing happening, check it out
			HandleAsyncMsg(this->m_hWnd, wParam, lParam);
			return TRUE;	
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


void CFqDlg::OnSettingsServersetup() 
{
	CServer dlg;
	dlg.DoModal();
	
}

void CFqDlg::OnDestroy() 
{
	//close down everything
	shutdown_server();
	file_sys_save();
	this->remove_from_tray();
   CDialog::OnDestroy();
  Sleep(400); //stops a crash on exit that happened when I stress tested, don't know why	
   // TODO: Add your message handler code here

}
/*
 void CFqDlg::OnHide() 
{
		//hide window
	tray_hide_window();
}
  */


void CFqDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (::g_b_server_started)
	{
	  //update a few controls to match the new window size

	 //resize log	
	 CEdit* cedit_status = (CEdit*)GetDlgItem(IDC_LOG);  
  	
     if (cedit_status)
     {
     
     CRect rect_cur;
	 CRect rect_box;
	 cedit_status->GetWindowRect(&rect_cur);
	 this->GetWindowRect(&rect_box);

	 rect_cur.OffsetRect(-rect_box.left, -rect_box.top); //get current local X/Y of our control
	 rect_box.OffsetRect(-rect_box.left, -rect_box.top); //get current local X/Y of our window
	
	 cedit_status->SetWindowPos(&wndTop, 0,0,rect_box.right - rect_cur.left -20,rect_box.bottom-rect_cur.top-30, SWP_NOMOVE);
     }
	 
	}
	
	// TODO: Add your message handler code here
	
}



void CFqDlg::OnSettingsPlayerEdit() 
{
	// TODO: Add your command handler code here
	dlg_edit_choose dlg;
	dlg.DoModal();
}





void CFqDlg::OnAboutFq() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CFqDlg::OnGameKarmaText() 
{
	//let's let them change the karma text
//	dlg_karma dlg;
//	dlg.DoModal();
  	  open_file(this->m_hWnd, "karma.txt");	


	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString cst_temp;
	cst_temp.Format("Version %d.%d - Compiled %s at %s.",

		::i_max_version, ::i_min_version, __DATE__, __TIME__);
	//set static text
	GetDlgItem(IDC_COMPILED)->SetWindowText(cst_temp);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFqDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
   OnKillserver();


}

void CFqDlg::OnKillserver() 
{
//while debugging I don't want to hassle with this check all the time

#ifndef _DEBUG
	if (IDOK == MessageBox("Are you sure you want to shut down?", "Shutdown request", MB_ICONQUESTION|MB_OKCANCEL))
	{
		this->DestroyWindow();
	}
#else
		this->DestroyWindow();
#endif

}

void CFqDlg::OnRatingText() 
{
//	dlg_ratings_text dlg;
//	dlg.DoModal();
	  open_file(this->m_hWnd, "ratings.txt");	
}

void CFqDlg::OnGameSettings() 
{
   dlg_game_settings dlg;
   dlg.DoModal();

	
}

void CFqDlg::OnLogon1() 
{
  	//perform a local logon
  char st_hold[255];
 sprintf(st_hold, "127.0.0.1:%d",sys.i_port);
 winall_create_url_file(st_hold);
 int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
 ProcessError(this->m_hWnd, result, "temp.url",st_hold); 	
	
}

void CFqDlg::OnLogon2() 
{
	// TODO: Add your control notification handler code here
  char st_hold[255];
  sprintf(st_hold, "%s:%d",sys.st_local_ip, sys.i_port);
  winall_create_url_file(st_hold);
  int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
  ProcessError(this->m_hWnd, result, "temp.url",st_hold); 	
}




void CFqDlg::OnGameYangs() 
{
  open_file(this->m_hWnd, "yangs.txt");	
}

void CFqDlg::OnGameNoCustomer() 
{
  open_file(this->m_hWnd, "no_customers.txt");	
	
}

void CFqDlg::OnGameNews() 
{
  open_file(this->m_hWnd, "news_blurbs.txt");	
	
}

void CFqDlg::OnGameEvents() 
{
  open_file(this->m_hWnd, "events.txt");	
	
}

void CFqDlg::OnGameEditRisks() 
{
	// TODO: Add your command handler code here
 open_file(this->m_hWnd, "risks.txt");	
	
}

void CFqDlg::OnViewLog() 
{
	// TODO: Add your control notification handler code here
 open_file(this->m_hWnd, "log.txt");	
	
}

void CFqDlg::OnViewLogDelete() 
{
	// TODO: Add your control notification handler code here
  unlink("log.txt");
  MessageBox("Log file cleared.");
}

void CFqDlg::OnViewError() 
{
	// TODO: Add your control notification handler code here
 open_file(this->m_hWnd, "errorlog.txt");	
	
}

void CFqDlg::OnViewErrorDelete() 
{
	// TODO: Add your control notification handler code here
  unlink("errorlog.txt");
  MessageBox("Error log file cleared.");
	
}

void CFqDlg::OnNewspaper() 
{
	// TODO: Add your command handler code here
   	char st_crap[255];
	sprintf(st_crap, "%snews_day_%d.txt", st_data_path, glo.i_game_age);	
	open_file(this->m_hWnd, st_crap);
}

void CFqDlg::OnReloadText() 
{
	//this can possible crash if you load less text than existed 
    //before and someone is playing.. hmm
    
    init_options();
    load_strings("ratings.txt", true);
	load_strings("karma.txt", false);
	load_swear_filter_from_file();
	load_ban_filter_from_file();
	load_transport_from_file();
	LogMsg("Text cleared and reloaded.");
}

void CFqDlg::OnViewStats() 
{
	// TODO: Add your control notification handler code here
	if (!exist("stats.txt"))
	{
	 MessageBox("No stats exist yet, wait a day.");

	} else
	open_file(this->m_hWnd, "stats.txt");	
	
	
}

void CFqDlg::OnHelp() 
{
	open_file(this->m_hWnd, "fqhelp\\help.htm");	
	
}

void CFqDlg::OnHtmlHelp() 
{
	open_file(this->m_hWnd, "fqhelp\\help.htm");	
	
}

void CFqDlg::OnSwearFilter() 
{
	open_file(this->m_hWnd, "swear_words.txt");	
	
}

void CFqDlg::OnBanList() 
{
	open_file(this->m_hWnd, "ban.txt");	
	
}

void CFqDlg::OnOptionsBan() 
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
	


	   static char st_ip[15];
  sprintf(st_ip, "%s",inet_ntoa( p_on[m_list_on.GetItemData(i_index)]->ip));


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
#include "dlg_tray.cpp"


BOOL CFqDlg::PreTranslateMessage(MSG* pMsg) 
{

  

    if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
{
//process enter key here... 
  //we're going to ignore this..
		return (1); //tell them we handled it
	
}

	return CDialog::PreTranslateMessage(pMsg);
}

void CFqDlg::OnHardSell() 
{
	// TODO: Add your command handler code here
	open_file(this->m_hWnd, "hard_sell.txt");	
	
}

void CFqDlg::OnNewsCrime() 
{
	// TODO: Add your command handler code here
 	char st_crap[255];
	sprintf(st_crap, "%snews_crime_day_%d.txt", st_data_path, glo.i_game_age);	
	open_file(this->m_hWnd, st_crap);
	
}

void CFqDlg::OnSoftSell() 
{
	open_file(this->m_hWnd, "soft_sell.txt");	
	
}

void CFqDlg::OnGameYangsNice() 
{
	// TODO: Add your command handler code here
	open_file(this->m_hWnd, "yangs_nice.txt");	
	
}
