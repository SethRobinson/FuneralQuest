
bool g_b_in_tray = false;

void CFqDlg::add_to_tray()
{
    	NOTIFYICONDATA notify_struct;

	//set the info for the struct

	notify_struct.cbSize = sizeof(NOTIFYICONDATA);
	notify_struct.hWnd = this->m_hWnd;
	notify_struct.uID = WM_MY_TRAY_NOTIFICATION;
	notify_struct.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
	notify_struct.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
	notify_struct.hIcon = m_hIcon;
	sprintf(notify_struct.szTip, "Funeral Quest Server");

	Shell_NotifyIcon(NIM_ADD, &notify_struct);	
    
	this->UpdateWindow();
	g_b_in_tray = true;
}

void CFqDlg::remove_from_tray()
{

   	NOTIFYICONDATA notify_struct;

	//set the info for the struct

	notify_struct.cbSize = sizeof(NOTIFYICONDATA);
	notify_struct.hWnd = this->m_hWnd;
	notify_struct.uID = WM_MY_TRAY_NOTIFICATION;
	notify_struct.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_DELETE, &notify_struct);	
    //ShowWindow(SW_SHOW);
	this->UpdateWindow();
	g_b_in_tray = false;

}

void CFqDlg::left_clicked_icon()
{
	if (g_b_in_tray)
		remove_from_tray();
	//show window
   ShowWindow(SW_SHOW);
   UpdateWindow();
   SetFocus();
   SetForegroundWindow();
}
void CFqDlg::tray_hide_window()
{
	if (!g_b_in_tray) 
		add_to_tray();
	ShowWindow(SW_HIDE);
}



void CFqDlg::tray_icon_menu()
{
    POINT pt;
   GetCursorPos(&pt);
	
	CMenu menu;
	menu.LoadMenu(IDR_MAIN);
	ASSERT(menu);

	CMenu* popup = menu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);

}
