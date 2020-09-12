// alert_socket.cpp : implementation file
//
#include "alert_socket.h"
//#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// alert_socket




alert_socket::alert_socket()
{
b_connected =false;
}

alert_socket::~alert_socket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(alert_socket, CSocket)
	//{{AFX_MSG_MAP(alert_socket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// alert_socket member functions

void alert_socket::OnConnect(int nErrorCode) 
{
//	log_msg_thread_safe("Connected.");
	CSocket::OnConnect(nErrorCode);
	b_connected = true;
}

void alert_socket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
 static int i=0;

   i++;

   TCHAR buff[4096];
   int nRead;
   nRead = Receive(buff, 4096); 
   
   switch (nRead)
   {
   case 0:
      Close();
      break;
   case SOCKET_ERROR:
      if (GetLastError() != WSAEWOULDBLOCK) 
      {
         //AfxMessageBox ("Error occurred");
          log_msg_thread_safe("Got receive error %d.", GetLastError());
		  Close();
      }
      break;
   default:
      buff[nRead] = 0; //terminate the string
	  CString szTemp(buff);
#ifdef _DEBUG
//	  OutputDebugString((LPCTSTR)szTemp);
#endif
	  m_strRecv += szTemp;   // m_strRecv is a CString declared 
                           // in CMyAsyncSocket
   }
#ifdef _DEBUG
   if (sys.b_extra_debug) 
   {
	   log_msg_thread_safe("RTSOFT says: %s", m_strRecv);
   }
#endif
   split_messages();
  m_strRecv = ""; //clear buffer
  
   		b_connected = true;
   CSocket::OnReceive(nErrorCode);
}

void alert_socket::OnSend(int nErrorCode) 
{
		// TODO: Add your specialized code here and/or call the base class
	 while (m_nBytesSent < m_nBytesBufferSize)
   {
      int dwBytes;

      if ((dwBytes = Send((LPCTSTR)m_sendBuffer + m_nBytesSent, 
         m_nBytesBufferSize - m_nBytesSent)) == SOCKET_ERROR)
      {
         if (GetLastError() == WSAEWOULDBLOCK) break;
         else
         {
            TCHAR szError[256];
            wsprintf(szError, "Server Socket failed to send: %d - We're not connected!", 
               GetLastError());
            log_msg_thread_safe(szError);
			Close();
			return;
            
         }
      }
      else
      {
         m_nBytesSent += dwBytes;
      //	 CString szTemp;
		// szTemp.Format("Client just sent %d bytes.",dwBytes);
	//	 if (sys.b_extra_debug) log_msg_thread_safe(szTemp);
	  }
   }
   if (m_nBytesSent == m_nBytesBufferSize)
      {
         m_nBytesSent = m_nBytesBufferSize = 0;
         m_sendBuffer = "";
//      	 OutputDebugString("Finished sending the good stuff. (CLIENT)");
   }
   

	CAsyncSocket::OnSend(nErrorCode);

	CSocket::OnSend(nErrorCode);
}

void alert_socket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
//	MessageBox(g_hwnd_main, "Server has disconnected us.", "CM Pilot Alert",0);
//	log_msg_thread_safe("Lost connection with license server, will try to reconnect soon.");
//	SendMessage(g_hwnd_main, WM_CLOSE,0,0);
     b_connected = false;	
	
	 CSocket::Close();
	 CSocket::OnClose(nErrorCode);
}


void alert_socket::split_messages()
{
	  CString cst_temp;
	  int i_pos=0;
	  while (1)
	  {  
	  i_pos = m_strRecv.Find("&&&");
	  if (i_pos == -1) //couldn't find anything, done reading packets.
	  {
		  m_strRecv = "";
		  return;
	  }

	  //break off the part we want

	  cst_temp = m_strRecv.Left(i_pos);
	  m_strRecv.Delete(0, i_pos+3);
	  process_messages_from_license_server(&cst_temp);
 
	  //repeat
	  }

}
