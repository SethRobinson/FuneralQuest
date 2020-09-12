#include "server_connect.h"
#include "globals.h"
#include "license_vars.h"     
#include "CProtection.h"
#include "server_settings.h"
const int C_TIME_TO_WAIT_BETWEEN_LICENSE_CONNECTS = 30000;

alert_socket main_socket;

 void print_winsock_error(CString cst_label, int i_error);


void force_reconnect()
{
	if (main_socket.m_hSocket != NULL)
	main_socket.OnClose(0);
}

void convert_and_send_to_server(CString *cst_new)
{
	main_socket.m_sendBuffer += *cst_new;
	format_for_web_cst(&main_socket.m_sendBuffer);
	main_socket.m_sendBuffer += "%s&&&\n";
	main_socket.m_nBytesBufferSize = main_socket.m_sendBuffer.GetLength();
	main_socket.OnSend(0);
}


UINT listener_init(LPVOID pCrap)
{
	PostMessage(g_dlg_main->m_hWnd, WM_SAY_HELLO_TO_SERVER,0,0);
	log_msg_thread_safe("Waiting for welcome message...");
	return 0;
}

UINT say_hello()
{
   //removed DRM
	return 0;
}

bool get_server_address(char st_server[255])
{
#ifdef _DEBUG
LogMsg("Getting server address..");
#endif
 	
	DWORD dwConnectionTypes = INTERNET_CONNECTION_LAN |
                           INTERNET_CONNECTION_MODEM |
                           INTERNET_CONNECTION_PROXY;
 if (!InternetGetConnectedState(&dwConnectionTypes, 0))
 {
     InternetAutodial(INTERNET_AUTODIAL_FORCE_UNATTENDED,
                    0);
 } 
		
	CRobotInternet crobot_hold;
  int int_result;
  CString st_url, st_html, st_error;
  st_url = st_server;
  if (crobot_hold.httpGet(st_url, st_html,int_result, st_error))
{
    //got the file, let's tokenize it.
	  REQUEST Req;
	  Req.lpSockAddr = NULL;
	  Req.Socket = NULL;
	  LPREQUEST lpReq = &Req;
	  if (!tokenize_input(&Req, st_html))
	  {
	    LogMsg("Unable to understand response from master server.");
		return false;
	  }
 	//save the important stuff
	  CString cst_server; 
	  get_token_st(lpReq, "server", &cst_server);
	  get_token_int(lpReq, "port", &g_glo.i_server_port);
	  
	  //cls_pro.set_server_info(cst_server, i_port);
	  strcpy(g_glo.st_server_address, cst_server);

	  //log_msg_thread_safe("Got %s:%s",cst_server, cst_port);
	//got information, ready to connect


  } else
  {
    //error!
   
     return false;
  } 


return true;
 }
        

UINT get_server_dat(LPVOID pCrap)
{

	//DRM removed, so just setting whatever we want
    cls_pro.i_max_on = 500;
    cls_pro.i_max_players = 5000;
    cls_pro.b_validated = true;
  	PostMessage(g_dlg_main->m_hWnd, WM_SAY_HELLO_TO_SERVER,0,0);
	return 0;
}



