#pragma once

#include "stdafx.h"

#define UM_ASYNC WM_USER+1
#define WM_ADD_TO_LOG WM_USER+2
#define WM_GOT_SERVER_INFO WM_USER+3
#define WM_GET_SERVER_INFO WM_USER+4
#define WM_MY_TRAY_NOTIFICATION WM_USER+5
#define WM_SAY_HELLO_TO_SERVER WM_USER+6
#define HTTPPORT 80
#define METHOD_GET 0
#define d_max_tokens 10
#define d_max_token_size 2000

// HTTPSERVINFO structure
//
typedef struct tagHTTPSERVINFO
{
	HWND		hwnd;
	UINT		uMsgAsy;
	short		nPort;
	LPCSTR		lpRootDir;
}HTTPSERVINFO, *LPHTTPSERVINFO;

//
// CONNECTION structure
//

struct seth_string
{
  char data[d_max_token_size];
};
class online_struct;

typedef struct tagREQUEST
{
  SOCKET		Socket;
  LPSOCKADDR	lpSockAddr;
  int			nAddrLen;
  int			nMethod;
  BOOL			fFullResponse;
  DWORD			dwConnectTime;
  DWORD			dwRecv;
  DWORD			dwSend;
  HFILE			hFile;
  DWORD			dwFilePtr;
  struct		tagREQUEST *lpNext;
  seth_string token[d_max_tokens];
  seth_string token_amount[d_max_tokens];
  seth_string command;
  int i_on_index;

}REQUEST, *LPREQUEST;

//
// Statistics structure
//
typedef struct tagHTTPSTATS
{
	DWORD	dwElapsedTime;
	DWORD	dwRecv;
	DWORD	dwSend;
}HTTPSTATS, *LPHTTPSTATS;

//
// These numbers relate to their
// sequence in an array in
// SendError()
//


#define HTTP_STATUS_OK1				0
#define HTTP_STATUS_CREATED1			1
#define HTTP_STATUS_ACCEPTED1		2
#define HTTP_STATUS_NOCONTENT1		3
#define HTTP_STATUS_MOVEDPERM1		4
#define HTTP_STATUS_MOVEDTEMP1		5
#define HTTP_STATUS_NOTMODIFIED1		6
#define HTTP_STATUS_BADREQUEST1		7
#define HTTP_STATUS_UNAUTHORIZED1	8
#define HTTP_STATUS_FORBIDDEN1		9
#define HTTP_STATUS_NOTFOUND1		10
#define HTTP_STATUS_SERVERERROR1		11
#define HTTP_STATUS_NOTIMPLEMENTED1	12
#define HTTP_STATUS_BADGATEWAY1		13
#define HTTP_STATUS_UNAVAILABLE1		14

//
// HTTPSERV.C
//
BOOL StartHTTP(LPHTTPSERVINFO lpInfo);
void StopHTTP(void);
void HandleAsyncMsg(HWND hwnd, WPARAM wParam, 
				    LPARAM lParam);
void OnAccept(HWND hwnd, SOCKET socket, int nErrorCode);
void OnRead(SOCKET socket, int nErrorCode);
void OnWrite(SOCKET socket, int nErrorCode);
void OnClose(SOCKET socket, int nErrorCode);
void ParseRequest(LPREQUEST lpReq, LPBYTE lpBuf);
void SendFile(LPREQUEST lpReq, LPCSTR lpFileName);
void CloseConnection(LPREQUEST lpReq);
void SendError(LPREQUEST lpReq, UINT uError);
void SendFileContents(LPREQUEST lpReq);
void LogWinSockError(HWND hwnd, LPCSTR lpText, int nErrorCode);

//int GetLocalAddress(LPSTR lpStr, LPDWORD lpdwStrLen);

//
// REQLIST.C
//
LPREQUEST	AddRequest(SOCKET Socket, 
					   LPSOCKADDR lpSockAddr,
					   int nAddrLen);
LPREQUEST	GetRequest(SOCKET Socket);
LPREQUEST	GetFirstRequest(void);
LPREQUEST	GetNextRequest(LPREQUEST lpThis);
void		DelRequest(LPREQUEST lpThis);
void		DelAllRequests(void);
char * my_ip(LPREQUEST lpReq);

