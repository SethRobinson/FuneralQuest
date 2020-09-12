#if !defined(AFX_ALERT_SOCKET_H__FBED7B94_FA03_4A67_A7F8_5DEA8E1B4535__INCLUDED_)
#define AFX_ALERT_SOCKET_H__FBED7B94_FA03_4A67_A7F8_5DEA8E1B4535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// alert_socket.h : header file
//
#include "stdafx.h"
#include "resource.h"
#include "fqdlg.h"
#include "funeral_util.h"
#include "license_process.h"
/////////////////////////////////////////////////////////////////////////////
// alert_socket command target

class alert_socket : public CSocket
{
// Attributes
public:

// Operations
public:
	CString      m_sendBuffer;   //for async send
    int      m_nBytesSent;
    int      m_nBytesBufferSize;
	CString m_strRecv;
	bool b_connected;
	alert_socket();
	virtual ~alert_socket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(alert_socket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(alert_socket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	void alert_socket::split_messages();


// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERT_SOCKET_H__FBED7B94_FA03_4A67_A7F8_5DEA8E1B4535__INCLUDED_)
