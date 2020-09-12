//
// HTTPA.C	HTTP Server Functions
//
//			Asynchronous Model
//
//

//#include "httpa.h"

//
// Internal linkage


//

#include "shared\\winsock_util.h"

 CString GetNowDate();
 CString GetHttpDate( LPFILETIME pft );

static SOCKET listenSocket;
static SOCKET sock_stream_listen;

static char szWebRoot[_MAX_PATH];
static HWND	ghwnd;
static UINT guMsgAsy;
static UINT guMsgApp;
void request_process(LPREQUEST lpReq, CString cst_request);
void format_for_web(char *st_buff);
void format_for_web_cst(CString *cst_temp);
#include "security.h"
//funeral specific includes

////////////////////////////////////////////////////////////

  void print_winsock_error(CString cst_label, int i_error)
 {

	
	  CString cst_temp;
	  if (i_error == WSANOTINITIALISED)
   {
	cst_temp = "A successful AfxSocketInit must occur before using this API.";
   }

   if (i_error == WSAENETDOWN)
   {
	cst_temp = "The Windows Sockets implementation detected that the network subsystem failed.";
   }

   if (i_error == WSAEFAULT)
   {
	cst_temp = "The lpSockAddrLen argument is too small (less than the size of a SOCKADDR structure).";
   }										  

   if (i_error == WSAEINPROGRESS)
   {
	cst_temp = "A blocking Windows Sockets call is in progress.";
   }

   if (i_error == WSAEINVAL)
   {
	cst_temp = "Listen was not invoked prior to accept.";
   }
   if (i_error == WSAEMFILE)
   {
	cst_temp = "The queue is empty upon entry to accept and there are no descriptors available.";
   }
   if (i_error == WSAENOBUFS)
   {
	cst_temp = "No buffer space is available.";
   }
   if (i_error == WSAENOTSOCK)
   {
	cst_temp = "The descriptor is not a socket.";
   }
  if (i_error == WSAEOPNOTSUPP)
   {
	cst_temp = "The referenced socket is not a type that supports connection-oriented service.";
   }

  if (i_error == WSAEWOULDBLOCK)
   {
cst_temp = "The socket is marked as nonblocking and no connections are present to be accepted.";
   }
  
  if (i_error == WSASYSNOTREADY)
  {
   cst_temp = "Socket not ready";

  }

  if (cst_temp == "")
  {

   cst_temp = "Unknown socket error";
  }
  
  log_error((char*)(LPCSTR)cst_label +cst_temp);
 }



//Send a string to the port
void send_msg(LPREQUEST lpReq, CString cst_final)
{
	int nRet = 0;
	cst_final += "/r/n";
   //Now let's web-ize it.
//	char szBuf[12000];
//	strcpy(szBuf, cst_final);
	format_for_web_cst(&cst_final);
#ifdef _DEBUG
	if (sys.b_extra_debug)
	g_dlg_main->Log(cst_final);
#endif  
	nRet = send(lpReq->Socket,
				cst_final,
				cst_final.GetLength(),
				0);
	
   if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			LogWinSockError(ghwnd, 
							"sendmsg()@SendError()",
							WSAGetLastError());
			nRet = 0;
		}
	}
	lpReq->dwSend += nRet;

	lpReq->hFile = HFILE_ERROR; //so it won't try to close a random file handle later
	CloseConnection(lpReq);
	  
}

	
bool create_sock_stream_listen( int i_port)
{
  	SOCKADDR_IN		saServer;		
	char			szBuf[256];		
	char			szAddress[128];
    int				nRet;			


 	//
	// Create a TCP/IP stream socket
	//
	sock_stream_listen = socket(AF_INET, 
						  SOCK_STREAM, 
						  IPPROTO_TCP);
	if (sock_stream_listen == INVALID_SOCKET)
	{
		LogWinSockError(ghwnd, 
						"Could not create stream listen socket",
						WSAGetLastError());
		return FALSE;
	}

	//
	// Request async notification
	//
    nRet = WSAAsyncSelect(sock_stream_listen, 
						  ghwnd, 
						  guMsgAsy,
						  FD_ACCEPT		| 
						  FD_READ		| 
						  FD_WRITE		| 
						  FD_CLOSE);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "sock_stream_listen - WSAAsyncSelect() error",
						 WSAGetLastError());
		closesocket(sock_stream_listen);
		return FALSE;
	}

	//
	// If a specific port number was specified
	// then use it
	//
		saServer.sin_port = htons(i_port);

	// Fill in the rest of the address structure
	//
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;

	//
	// bind our name to the socket
	//
	nRet = bind(sock_stream_listen, 
				(LPSOCKADDR)&saServer, 
				sizeof(struct sockaddr));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "bind() error - sock_stream_port",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// Set the socket to listen
	//
	nRet = listen(sock_stream_listen, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "sock_stream_listen() error",
						 WSAGetLastError());
		closesocket(sock_stream_listen);
		return FALSE;
	}

	//
	// Display the host name and address
	//
	gethostname(szBuf, sizeof(szBuf));
	GetLocalAddress((char*)&szAddress);
	LogMsg( 
			 "Stream Listen Server Started: %s [%s] on port %d",
			 szBuf,
			 szAddress,
			 htons(saServer.sin_port));
		
//	strcpy(sys.st_local_ip, szAddress);
	return TRUE;
}



BOOL StartHTTP(LPHTTPSERVINFO lpInfo)
{
	SOCKADDR_IN		saServer;		
	LPSERVENT		lpServEnt;		
	char			szBuf[256];		
	char			szAddress[128];
    int				nRet;			

	//
	// Save the Window handle and message
	// ID's for further use
	//
	ghwnd    = lpInfo->hwnd;
	guMsgAsy = lpInfo->uMsgAsy;
	if (lpInfo->lpRootDir != NULL)
		strcpy(szWebRoot, lpInfo->lpRootDir);
	else
		strcpy(szWebRoot, "/WebPages");

 	//
	// Create a TCP/IP stream socket
	//
	listenSocket = socket(AF_INET, 
						  SOCK_STREAM, 
						  IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		LogWinSockError(ghwnd, 
						"Could not create listen socket",
						WSAGetLastError());
		return FALSE;
	}

	//
	// Request async notification
	//
    nRet = WSAAsyncSelect(listenSocket, 
						  ghwnd, 
						  guMsgAsy,
						  FD_ACCEPT		| 
						  FD_READ		| 
						  FD_WRITE		| 
						  FD_CLOSE);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "WSAAsyncSelect() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// If a specific port number was specified
	// then use it
	//
	if (lpInfo->nPort != 0)
		saServer.sin_port = htons(lpInfo->nPort);
	else
	{
		//
		// Find a port number
		//
		lpServEnt = getservbyname("http", "tcp");
		if (lpServEnt != NULL)
		
			saServer.sin_port = lpServEnt->s_port;
		else
		{
		//	LogMsg("Forcing port..");
			saServer.sin_port = htons(HTTPPORT);
		}
	}

	//
	// Fill in the rest of the address structure
	//
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;

	//
	// bind our name to the socket
	//
	nRet = bind(listenSocket, 
				(LPSOCKADDR)&saServer, 
				sizeof(struct sockaddr));
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "bind() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}

	//
	// Set the socket to listen
	//
	nRet = listen(listenSocket, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
	{
		LogWinSockError(ghwnd,
						 "listen() error",
						 WSAGetLastError());
		closesocket(listenSocket);
		return FALSE;
	}
                            
	//
	// Display the host name and address
	//
	gethostname(szBuf, sizeof(szBuf));
//	dwAddrStrLen = sizeof(szAddress);
	GetLocalAddress((char*)&szAddress);
	LogMsg( 
			 "HTTP Server Started: %s [%s] on port %d",
			 szBuf,
			 szAddress,
			 htons(saServer.sin_port));

	strcpy(sys.st_local_ip, szAddress);

    if (htons(saServer.sin_port) == 80)
    {
        LogMsg("Browser address to reach server: http://%s (Might be wrong IP, go to www.myipaddress.com to be sure)", szAddress);
    }   else
    {
         LogMsg("Browser address to reach server: http://%s:%d (Might be wrong IP, go to www.myipaddress.com to be sure)", szAddress, htons(saServer.sin_port));
  
    }

	//great, we can serve webpages.  But now let's start the stream listen socket
//	create_sock_stream_listen(1024); //port
	
	return TRUE;
}

////////////////////////////////////////////////////////////

void StopHTTP()
{
	LPREQUEST lpReq;

	//
	// Close the listening socket
	//
	closesocket(listenSocket);

	//
	// Close all open connections
	//
	lpReq = GetFirstRequest();
	while(lpReq)
	{
		closesocket(lpReq->Socket);
		if (lpReq->hFile != HFILE_ERROR)
			_lclose(lpReq->hFile);
		lpReq = GetNextRequest(lpReq);
	}

//	closesocket(sock_stream_listen);
	//
	// And clean up the linked list
	// of connections
	//
	DelAllRequests();
	log_msg_not_threadsafe("Server Stopped.");
}

////////////////////////////////////////////////////////////

void HandleAsyncMsg(HWND hwnd, 
					WPARAM wParam, 
					LPARAM lParam)
{
	int nErrorCode = WSAGETSELECTERROR(lParam);
	
	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_ACCEPT:
			OnAccept(hwnd, (SOCKET)wParam, nErrorCode);
			break;

		case FD_READ:
			OnRead((SOCKET)wParam, nErrorCode);
			break;

		case FD_WRITE:
			OnWrite((SOCKET)wParam, nErrorCode);
			break;

		case FD_CLOSE:
			OnClose((SOCKET)wParam, nErrorCode);
			break;
	}
}

////////////////////////////////////////////////////////////

void OnAccept(HWND hwnd, SOCKET socket, int nErrorCode)
{
	SOCKADDR_IN	SockAddr;
	LPREQUEST	lpReq;
	SOCKET		peerSocket;
	int			nRet;
	int			nLen;

	//
	// accept the new socket descriptor
	//
	nLen = sizeof(SOCKADDR_IN);
	peerSocket = accept(listenSocket,
					    (LPSOCKADDR)&SockAddr,
						&nLen);
	if (peerSocket == SOCKET_ERROR)
	{
		nRet = WSAGetLastError();
		if (nRet != WSAEWOULDBLOCK)
		{
			//
			// Just log the error and return
			//
			LogWinSockError(ghwnd, 
							"accept() error",
							WSAGetLastError());
			return;
		}
	}

	//
	// Make sure we get async notices for this socket
	//
    nRet = WSAAsyncSelect(peerSocket,
						  hwnd,
						  guMsgAsy, 
						  FD_READ | FD_WRITE | FD_CLOSE);
	if (peerSocket == SOCKET_ERROR)
	{
		nRet = WSAGetLastError();
		//
		// Just log the error and return
		//
		LogWinSockError(ghwnd, 
						"accept() error",
						WSAGetLastError());
		return;
	}

	// 
	// Add the connection to the linked list
	//
	lpReq = AddRequest(peerSocket, 
					   (LPSOCKADDR)&SockAddr, 
					   nLen);
	if (lpReq == NULL)
	{
		// We're probably out of memory
		log_error("ERROR:  No memory to create another socket request structure?");
		closesocket(peerSocket);
	}
		  /*
	      LogMsg( 
			 "%s: Connection accepted on port %d",
			 inet_ntoa(SockAddr.sin_addr), peerSocket);
			 */
}

////////////////////////////////////////////////////////////

void OnRead(SOCKET socket, int nErrorCode)
{
	static BYTE		buf[4096];
	LPREQUEST		lpReq;
	int				nRet;

	//
	// Zero the buffer so the recv is null-terminated
	//
	memset(buf, 0, sizeof(buf));

	// 
	// Find this socket in the linked list
	//
	lpReq = GetRequest(socket);
	if (lpReq == NULL)
	{
		//
		// Not in the list. Log the error,
		// read the data to clear the buffers
		// and close the connection
		//
		nRet = 0;
		while(nRet != SOCKET_ERROR)
			nRet = recv(socket, (char *)buf, sizeof(buf)-1, 0);
	
		if (nRet == SOCKET_ERROR)
		{
		//	print_winsock_error("OnRead:recv - ",WSAGetLastError());
		}
			closesocket(socket);
		return;
	}

	//
	// Recv the data
	//
	nRet = recv(socket, (char *)buf, sizeof(buf)-1, 0);
	buf[nRet+1] = 0; //force an ending so our string functions won't choke
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return;
		LogWinSockError(ghwnd,
						"recv()",
						WSAGetLastError());
		CloseConnection(lpReq);
		return;
	}

	//
	// Keep statistics
	//
	lpReq->dwRecv += nRet;
	sys.i_hits_today += 1;
	sys.i_hits_total += 1;
	//
	// Parse the HTTP request
	//
	ParseRequest(lpReq, buf);
}

////////////////////////////////////////////////////////////

void OnWrite(SOCKET socket, int nErrorCode)
{
	LPREQUEST lpReq;
	BYTE buf[1024];
	int nRet;

	lpReq = GetRequest(socket);
	if (lpReq == NULL)
	{
		//
		// Not in our list!?
		//
		nRet = 0;
		while(nRet != SOCKET_ERROR)
		{
			nRet = recv(socket, (char *)buf, sizeof(buf)-1, 0);
		    if (nRet == SOCKET_ERROR)
			{
			 //print_winsock_error("OnWrite:recv - ", WSAGetLastError());

			}
		}
		if (closesocket(socket) == SOCKET_ERROR)
		{
		//	 print_winsock_error("OnWrite:closesocket - ",WSAGetLastError());
		}
		return;		
	}

	//
	// Is this the first FD_WRITE 
	// or did we fill the protocol
	// stack buffers?
	//
	if (lpReq->hFile == HFILE_ERROR)
		return;

	//
	// Continue sending a file
	//
	SendFileContents(lpReq);
}

////////////////////////////////////////////////////////////

void OnClose(SOCKET socket, int nErrorCode)
{
	LPREQUEST lpReq;

	//
	// Have we already deleted this entry?
	//
	lpReq = GetRequest(socket);
	if (lpReq == NULL)
		return;		

	// 
	// It's still in our list
	// The client must have reset the connection.
	// Clean up.
	//
	CloseConnection(lpReq);
}

////////////////////////////////////////////////////////////

void ParseRequest(LPREQUEST lpReq, byte lpBuf[4096])
{
	char szFileName[_MAX_PATH];
	char szSeps[] = " \n";
	char *cpToken;

	
	if (is_banned(lpReq))
	{
		SendError(lpReq, HTTP_STATUS_UNAUTHORIZED1);
		CloseConnection(lpReq);
	    return;
	}
	
#ifdef _DEBUG
	if (sys.b_extra_debug)
	LogMsg("%s",lpBuf);
#endif
	//
	// Don't let requests include ".." characters
	// in requests
	//
	if (strstr((const char*)lpBuf, "..") != NULL)
	{
		// Send "bad request" error
		SendError(lpReq, HTTP_STATUS_BADREQUEST1);
		CloseConnection(lpReq);
		return;
	}


	//
	// Determine request method
	//
	cpToken = strtok((char *)lpBuf, szSeps);
	if (!_stricmp(cpToken, "GET"))
		lpReq->nMethod = METHOD_GET;
	else
	{
		// Send "not implemented" error
		SendError(lpReq, HTTP_STATUS_NOTIMPLEMENTED1);
		CloseConnection(lpReq);
		return;
	}

	//
	// Get the file name
	//
	//replace %5C with \ symbol, some browsers send this, not sure why.


	cpToken = strtok(NULL, szSeps);
	if (cpToken == NULL)
	{
		// Send "bad request" error
		SendError(lpReq, HTTP_STATUS_BADREQUEST1);
		CloseConnection(lpReq);
		return;
	}
//	LogMsg("Socket is %d.",lpReq->Socket);
		replace("%5C", "/", (char*)cpToken);


	if (strstr(cpToken,"request.xp?") != NULL)
	{
	  //this is a server specific request, not for a regular file. Let's party.
	  request_process(lpReq, cpToken);
	  return;
	}
	
	//at this point they want a file.  Let's give them our header too...



 int i_length = strlen(cpToken);

	strcpy(szFileName, szWebRoot);
	
    
    if ( i_length > 1)
    {
        if (i_length > 220)
        {
          log_error("Um, filename too long? Ignoring request.");

        }  else
        {
        
        
        strcat(szFileName, cpToken);
        }
    }
	else
		strcat(szFileName, "/home.html");


    
    
	SendFile(lpReq, szFileName);
   


}

////////////////////////////////////////////////////////////

void CloseConnection(LPREQUEST lpReq)
{
	/*
	LogMsg(
			 "Closing socket: %d",
			 lpReq->Socket);
	  */
	  //
	// Log the event and close the socket
	//
	if (closesocket(lpReq->Socket) == SOCKET_ERROR)
		print_winsock_error("CloseConnection:closesocket - ", WSAGetLastError());
	//
	// If the file is still open,
	// then close it
	//
	if (lpReq->hFile != HFILE_ERROR)
		_lclose(lpReq->hFile);

	// Give the user interface the stats
	//
	DelRequest(lpReq);
}

////////////////////////////////////////////////////////////


CString FindContentType( CString m_strFullPath )
{
	// get the extension....
	   CString strExt = m_strFullPath.Mid(
		m_strFullPath.ReverseFind('.') );
	// find it in the registry....

//	   LogMsg("Finding %s..",strExt);
	   HKEY hKey = NULL;
	if ( RegOpenKeyEx( HKEY_CLASSES_ROOT, strExt,
		0, KEY_READ, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwSize = 0;
		// see how long the data is....
		if ( RegQueryValueEx( hKey, "Content Type", NULL, NULL,
			NULL, &dwSize ) == ERROR_SUCCESS )
		{
			CString strType;
			LONG lRet = RegQueryValueEx( hKey, "Content Type", NULL, NULL,
				(LPBYTE)strType.GetBuffer( dwSize ), &dwSize );
			strType.ReleaseBuffer();
			if ( lRet == ERROR_SUCCESS )
			{
				RegCloseKey( hKey );

				return( strType );
			}
		}
		RegCloseKey( hKey );
	}
 return ("Text/html"); //default
}

void SendFile(LPREQUEST lpReq, LPCSTR lpFileName)
{
	 
	// Open the file for reading
	//
	lpReq->hFile = _lopen(lpFileName, 
						  OF_READ|OF_SHARE_COMPAT);
	if (lpReq->hFile == HFILE_ERROR)
	{
		LogMsg("File %s not found.",lpFileName);
	
		// Send "404 Not Found" error
		SendError(lpReq, HTTP_STATUS_NOTFOUND1);
		CloseConnection(lpReq);
		return;
	}

	//
	// Tell the user interface about the file hit
	// (Sending just the request portion -- without
	// the root web directory portion)
	//
	//
	// Send as much of the file as we can
	//

#ifdef _DEBUG
	if (sys.b_extra_debug)
	LogMsg("Sending \"%s\".",lpFileName);
#endif	
	//send 1.0 header

     //open with CreateFile to get the date in the format we want..

   	HANDLE hFile = CreateFile( lpFileName,
			GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
		{
	
		 LogMsg("error opening %s.",lpFileName);
	      return;
		}

	FILETIME ft_temp;
	GetFileTime(hFile, NULL, NULL, &ft_temp);
	DWORD dw_filesize = GetFileSize(hFile, NULL);
   	CloseHandle(hFile);

      	CString cst_header;
    cst_header.Format("HTTP/1.0 200 OK\r\nServer: FQ/1.0\r\nDate: %s\r\n"
	"Content-length: %ld\r\nLast-Modified: %s\r\nContent-type: %s\r\n\r\n",  GetHttpDate(NULL), dw_filesize, GetHttpDate(&ft_temp),
	FindContentType(lpFileName));
	//	fclose(fp);

	int nBytesSent = send(lpReq->Socket,
						  (char *)(LPCSTR)cst_header,
						  cst_header.GetLength(),
						  0);
#ifdef _DEBUG
	if (sys.b_extra_debug)
	LogMsg(cst_header);
#endif
	lpReq->dwFilePtr = 0L;
	SendFileContents(lpReq);
}

 
CString GetHttpDate( LPFILETIME pft )
{
	SYSTEMTIME st;
	if ( pft )
		FileTimeToSystemTime( pft, &st );
	else
		GetSystemTime( &st );

	CTime timeHttp( st );
	return timeHttp.Format( "%a, %d %b %Y %H:%M:%S GMT" );
}


/*
BOOL CRequestSocket::IfModSince( const CTime& timeIfMod )
{
	// assume it has been modified....
	BOOL bOk = TRUE;
	FILETIME ft;
	if ( GetFileTime( m_hFile, NULL, NULL, &ft ) )
	{
		SYSTEMTIME st;
		if ( FileTimeToSystemTime( &ft, &st ) )
		{
			CTime timeFile( st );
			if ( timeFile <= timeIfMod )
				bOk = FALSE;
		}
	}
	return bOk;
}

static int IntVal( CString strVal )
{
	int nVal = 0;
	strVal.TrimLeft();
	for( int ndx = 0; ndx < strVal.GetLength(); ++ndx )
		nVal = nVal*10 + strVal.GetAt(ndx) - '0';

	return nVal;
}

static int MonthFromStr( const CString& str )
{
	LPSTR aMonths[] = {
		"xxx", "jan", "feb", "mar", "apr", "may", "jun",
		"jul", "aug", "sep", "oct", "nov", "dec" };
	for( int nMonth=1; nMonth <= 12; ++nMonth )
	{
		if ( str.CompareNoCase( aMonths[nMonth] ) == 0 )
			break;
	}

	return nMonth;
}

// Dow, dd Mon year hh:mm:ss GMT
BOOL CRequestSocket::FromHttpTime( const CString& strHttp, CTime& timeHttp )
{
	// assume we couldn't get a good time conversion....
	BOOL bOk = FALSE;
	SYSTEMTIME st = {0};
	int ndx;
	switch( strHttp.GetAt(3) )
	{
	case ',':
		// read RFC-1123 (preferred)....
		st.wDay = IntVal( strHttp.Mid(5,2) );
		st.wMonth = MonthFromStr( strHttp.Mid(8,3) );
		st.wYear = IntVal( strHttp.Mid(12,4) );
		st.wHour = IntVal( strHttp.Mid(17,2) );
		st.wMinute = IntVal( strHttp.Mid(20,2) );
		st.wSecond = IntVal( strHttp.Mid(23,2) );
		break;
	case ' ':
		// read ANSI-C time format....
		st.wDay = IntVal( strHttp.Mid(8,2) );
		st.wMonth = MonthFromStr( strHttp.Mid(4,3) );
		st.wYear = IntVal( strHttp.Mid(20,4) );
		st.wHour = IntVal( strHttp.Mid(11,2) );
		st.wMinute = IntVal( strHttp.Mid(14,2) );
		st.wSecond = IntVal( strHttp.Mid(17,2) );
		break;
	default:
		if ( (ndx = strHttp.Find( ", " )) != -1 )
		{
			st.wDay = IntVal( strHttp.Mid(ndx+2,2) );
			st.wMonth = MonthFromStr( strHttp.Mid(ndx+5,3) );
			st.wYear = IntVal( strHttp.Mid(ndx+9,2) );
			st.wHour = IntVal( strHttp.Mid(ndx+12,2) );
			st.wMinute = IntVal( strHttp.Mid(ndx+15,2) );
			st.wSecond = IntVal( strHttp.Mid(ndx+18,2) );
			// add the correct century....
			st.wYear += (st.wYear > 50)?1900:2000;
		}
		break;
	}
	// if year not zero, we pulled the info out of the string....
	if ( st.wYear != 0 )
	{
		// assume GMT....
		CTime strTime( st );
		// check to see if the minutes are the same....
		if ( strTime.GetMinute() == st.wMinute )
		{
			// assume it worked....
			timeHttp = strTime;
			bOk = TRUE;
		}
	}
	return bOk;
}


*/

////////////////////////////////////////////////////////////

void SendError(LPREQUEST lpReq, UINT uError)
{
	int nRet;
	static char szMsg[512];
	static char *szStatMsgs [] = {
								"200 OK",
								"201 Created",
								"202 Accepted",
								"204 No Content",
								"301 Moved Permanently",
								"302 Moved Temporarily",
								"304 Not Modified",
								"400 Bad Request",
								"401 Unauthorized",
								"403 Forbidden",
								"404 Not Found",
								"500 Internal Server Error",
								"501 Not Implemented",
								"502 Bad Gateway",
								"503 Service Unavailable"
								};
	#define NUMSTATMSGS sizeof(szStatMsgs) / sizeof(szStatMsgs[0])

	if (uError < 0 || uError > NUMSTATMSGS)
		return;

	wsprintf(szMsg, "<body><h1>%s</h1></body>",
			 szStatMsgs[uError]);
	nRet = send(lpReq->Socket,
				szMsg,
				strlen(szMsg),
				0);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			LogWinSockError(ghwnd, 
							"send()@SendError()",
							WSAGetLastError());
			nRet = 0;
		}
	}
	lpReq->dwSend += nRet;
}

////////////////////////////////////////////////////////////

void SendFileContents(LPREQUEST lpReq)
{
	static BYTE buf[1024];
	UINT uBytes;
	BOOL fEof;
	int nBytesSent;

	fEof = FALSE;

	//
	// We may be continuing, so seek to where
	// we left off last time
	//
	if (lpReq->dwFilePtr > 0)
		_llseek(lpReq->hFile, lpReq->dwFilePtr, FILE_BEGIN);

	//
	// Send as much of the file as we can
	//
	while(1)
	{
		//
		// Read a buffer full from the file
		//
		uBytes = _lread(lpReq->hFile,
						buf,
						sizeof(buf));
		if (uBytes == HFILE_ERROR)
		{
			LogMsg(
					 "Read file error: %d", 
					 GetLastError());
			// Send "500 Internal server" error
			SendError(lpReq, HTTP_STATUS_SERVERERROR1);
			_lclose(lpReq->hFile);
			lpReq->hFile = HFILE_ERROR;
			CloseConnection(lpReq);
			return;
		}

		//
		// Are we at End of File?
		//
		if (uBytes < sizeof(buf))
			fEof = TRUE;

		//
		// Send this buffer to the client
		//
		nBytesSent = send(lpReq->Socket,
						  (char *)buf,
						  uBytes,
						  0);
		if (nBytesSent == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
			//	LogWinSockError(ghwnd, 
			//					"sendfilecontents()",
			//					WSAGetLastError());
				_lclose(lpReq->hFile);
				lpReq->hFile = HFILE_ERROR;
				CloseConnection(lpReq);
				return;
			}
			nBytesSent = 0;
		}

		//
		// Keep track of what has actually been sent
		// from the file
		//
		lpReq->dwFilePtr += nBytesSent;

		//
		// And for statistics
		//
		lpReq->dwSend += nBytesSent;

		//
		// Are the protocol stack buffers full?
		//
		if (nBytesSent < (int)uBytes)
		{
			// We'll have to finish later
			return;
		}

		//
		// Are we finished?
		//
		if (fEof)
		{
			/*
			if (lpReq->hFile != HFILE_ERROR)
			_lclose(lpReq->hFile);
			lpReq->hFile = HFILE_ERROR;
			*/
			CloseConnection(lpReq);
			return;
		}
	}
}					  

////////////////////////////////////////////////////////////

void LogWinSockError(HWND hwnd, LPCSTR lpText, int nErrorCode)
{
	log_error("Winsock error %d: %s", nErrorCode, lpText);
}



 /*
int GetLocalAddress(LPSTR lpStr, LPDWORD lpdwStrLen)
{
    struct in_addr *pinAddr;
    LPHOSTENT	lpHostEnt;
	int			nRet;
	int			nLen;


	// Get our local name
	//
    nRet = gethostname(lpStr, *lpdwStrLen);
	if (nRet == SOCKET_ERROR)
	{
		lpStr[0] = '\0';
		return SOCKET_ERROR;
	}

	//
	// "Lookup" the local name
	//
	lpHostEnt = gethostbyname(lpStr);
    if (lpHostEnt == NULL)
	{
		lpStr[0] = '\0';
		return SOCKET_ERROR;
	}

	//
    // Format first address in the list
	//
	pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr);
	nLen = strlen(inet_ntoa(*pinAddr));
	if ((DWORD)nLen > *lpdwStrLen)
	{
		*lpdwStrLen = nLen;
		WSASetLastError(WSAEINVAL);
		return SOCKET_ERROR;
	}

	*lpdwStrLen = nLen;
	strcpy(lpStr, inet_ntoa(*pinAddr));
    return 0;
}
*/
