////////////////////////////////////////////////////////////////////
//
//  CRobotInternet.h - CRobotInternet class declarations
//
//  Source: "Programming Robots, Spiders and Intelligent Agents
//           Using Visual C++"
//
//	Copyright (C) 1999 David Pallmann. All Rights Reserved.

class CRobotInternet
{
public:
	BOOL m_bReadFromCache, m_bWriteToCache;
	int m_nContext;
	CString m_sProxyLogonMethod, m_sProxyLogonUsername, m_sProxyLogonPassword;
	CString m_sLogonUsername, m_sLogonPassword;
	CString m_sUserAgent;
	
	// Header (httpHeader, httpHeaderFields)
	CString m_sHeader;
	
	// Number of header fields (httpHeaderFields)
	int m_nHeaderFields;
	
	// Header field names (httpHeaderFields)
	CString m_sHeadName[100];

	// Header field values (httpHeaderFields)
	CString m_sHeadValue[100];

//---- Public functions ----

public:
	CRobotInternet();
	~CRobotInternet();
	
	//---- HTTP functions ----
	BOOL httpGet(const CString& sURL,
				 CString& sHTML,
				 int& nResult,
				 CString& sErrMsg);
	BOOL httpGetFile(const CString& sURL,
					 const CString& sFile,
					 int& nResult,
					 CString& sErrMsg);
	BOOL httpHeader(const CString& sUrl,
					CString& sResponse,
					int& nResult,
					CString& sErrMsg);
	BOOL httpHeaderFields(const CString& sUrl,
						  CString& sResponse,
						  int& nResult,
						  CString& sErrMsg);
	BOOL httpGetHeaderField(const CString& sName, CString& sValue);
	BOOL httpPost(const CString& sUrl,
				  const CString& sData,
				  CString& sResponse,
				  int& nResult,
				  CString& sErrMsg);
	BOOL httpPostFile(const CString& sUrl,
					  const CString& sData,
					  const CString& sOutputFilespec,
					  int& nResult,
					  CString& sErrMsg);
	BOOL httpError(const CString& sHTML,
				   int& nResult,
				   CString& sErrMsg);

	//---- FTP functions ----
	BOOL ftpGetFile(const CString& sUrl,
					const CString& sDir,
					const CString& sRemoteFilespec,
					const CString& sLocalFilespec,
					int& nResult,
					CString& sErrMsg);
	BOOL ftpPutFile(const CString& sUrl,
					const CString& sDir,
					const CString& sLocalFilespec,
					const CString& sRemoteFilespec,
					int& nResult,
					CString& sErrMsg);
	
	//---- Generalized functions ----
	CString ParseServerFromURL(const CString& sURL);
	CString ParsePathFromURL(const CString& sURL);
	BOOL RobotExcluded(const CString& sRobotPolicy,
					   const CString& sUrlToAccess);

//---- Private functions ----

private:
	CString EncodeTextBase64(const CString& sText);
	CString ErrorMessage(const int nError);
	CString ResponseMessage(const int nCode);
	CString CreateStandardHeader();
	BOOL CheckExclusion(const CString& sRobotPolicy,
						const CString& sAgent,
						const CString& sPath,
						BOOL& bIsExcluded);

};

////////////////////////////////////////////////////////////////////
