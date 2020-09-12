const int c_maint_check_time = 5000; //about five seconds
const int C_HEARTBEAT_TIME = 1000*60*15; //every fifteen minutes

CTimer timer_update;
CTimer timer_heartbeat;


void ResetUpdateTimers()
{
    timer_update.TriggerInterval();
    timer_heartbeat.TriggerInterval();
    c_reconnect.TriggerNext();
}

class ctimer_init
{
public:
	ctimer_init()
	{
      timer_update.set_interval(c_maint_check_time);
	  timer_heartbeat.set_interval(C_HEARTBEAT_TIME);
	}

} ctimer_junk;

		  

void UpdateEverything()
{
  

#ifdef _DEBUG
   if (timer_update.interval_reached())

#else

    if (timer_update.interval_reached())
#endif
	{
	//Time to run maint?
	maint_time_check();
	//Update our boys!
  	g_on.update_list_view_times();
	}

	//always update this part
   	CString cst_temp;

	MEMORYSTATUS mem;
	GlobalMemoryStatus(&mem);

	cst_temp.Format("%.2f megs", float((float(mem.dwAvailPhys)/1024)/1024));


	//set static text
	g_dlg_main->GetDlgItem(IDC_MEM_FREE)->SetWindowText(cst_temp);

	cst_temp.Format("%d",sys.i_logons_today);
	g_dlg_main->GetDlgItem(IDC_LOGONS)->SetWindowText(cst_temp);

	cst_temp.Format("%d",sys.i_logons_total);
	g_dlg_main->GetDlgItem(IDC_LOGONS_TOTAL)->SetWindowText(cst_temp);

	cst_temp.Format("%d",sys.i_hits_today);
	g_dlg_main->GetDlgItem(IDC_HITS)->SetWindowText(cst_temp);

	cst_temp.Format("%d",sys.i_hits_total);
	g_dlg_main->GetDlgItem(IDC_HITS_TOTAL)->SetWindowText(cst_temp);

}
bool CFqDlg::timer_init()
{
  
   timer_kill();  
    
#ifdef _DEBUG
   g_timer = SetTimer(1, 1000, NULL); 
   
#else
   g_timer = SetTimer(1, 1000, NULL); 

#endif
  if (g_timer == 0) 
  {
	  log_error("Windows reports that no timer object is available. (realtime updating of list view disabled)");
	  return false; //no timer available
  }
  return true; 
}


void CFqDlg::timer_kill()
{
    if (g_timer)
    {
    
    if (!this->KillTimer(1)) log_error("Windows couldn't find timer object to kill.");
    g_timer = 0;
    }
}
