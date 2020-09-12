////////////////////////////////////////////////////////////////////
//
//  CRobotInternet.cpp - CRobotInternet class implementation
//


#include <afx.h>        // MFC Automation classes
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>	
#include <afxinet.h>
#include "CRobot.h"
#include "CRobotInternet.h"
#pragma warning (disable:4996) //SETH added for VC8
// Constructor

CRobotInternet::CRobotInternet()
{
	m_bReadFromCache = false;
	m_bWriteToCache = false;
	m_nContext = 0;
	m_sLogonUsername = "";
	m_sLogonPassword = "";
	m_sProxyLogonMethod = "";
	m_sProxyLogonUsername = "";
	m_sProxyLogonPassword = "";
	m_sUserAgent = "Mozilla";
}


// Destructor

CRobotInternet::~CRobotInternet()
{
	m_sProxyLogonMethod.Empty();
	m_sProxyLogonUsername.Empty();
	m_sProxyLogonPassword.Empty();
}


////////////////////////////////////////////////////////////////////
//
// Support functions


// ********************** private
// *                    *
// *  EncodeTextBase64  *
// *                    *
// **********************
// Function: Returns the Base64-encoded version of a text string.

CString CRobotInternet::EncodeTextBase64(const CString& sText)
{
	unsigned char cChar[255];
	int nIndex1, nIndex2, nIndex3, nIndex4;
	int nChars;
	CString sBase64 = "";
	char cTable[64 + 1];
	CString sTemp;

	cTable[0] = 'A';
	cTable[1] = 'B';
	cTable[2] = 'C';
	cTable[3] = 'D';
	cTable[4] = 'E';
	cTable[5] = 'F';
	cTable[6] = 'G';
	cTable[7] = 'H';
	cTable[8] = 'I';
	cTable[9] = 'J';
	cTable[10] = 'K';
	cTable[11] = 'L';
	cTable[12] = 'M';
	cTable[13] = 'N';
	cTable[14] = 'O';
	cTable[15] = 'P';

	cTable[16] = 'Q';
	cTable[17] = 'R';
	cTable[18] = 'S';
	cTable[19] = 'T';
	cTable[20] = 'U';
	cTable[21] = 'V';
	cTable[22] = 'W';
	cTable[23] = 'X';
	cTable[24] = 'Y';
	cTable[25] = 'Z';
	cTable[26] = 'a';
	cTable[27] = 'b';
	cTable[28] = 'c';
	cTable[29] = 'd';
	cTable[30] = 'e';
	cTable[31] = 'f';

	cTable[32] = 'g';
	cTable[33] = 'h';
	cTable[34] = 'i';
	cTable[35] = 'j';
	cTable[36] = 'k';
	cTable[37] = 'l';
	cTable[38] = 'm';
	cTable[39] = 'n';
	cTable[40] = 'o';
	cTable[41] = 'p';
	cTable[42] = 'q';
	cTable[43] = 'r';
	cTable[44] = 's';
	cTable[45] = 't';
	cTable[46] = 'u';
	cTable[47] = 'v';

	cTable[48] = 'w';
	cTable[49] = 'x';
	cTable[50] = 'y';
	cTable[51] = 'z';
	cTable[52] = '0';
	cTable[53] = '1';
	cTable[54] = '2';
	cTable[55] = '3';
	cTable[56] = '4';
	cTable[57] = '5';
	cTable[58] = '6';
	cTable[59] = '7';
	cTable[60] = '8';
	cTable[61] = '9';
	cTable[62] = '+';
	cTable[63] = '/';

	cTable[64] = '=';

	nChars = sText.GetLength();
	
	int nPos;

	for (nPos = 0; nPos < nChars; nPos++) 
	{
		cChar[nPos] = sText.GetAt(nPos);
	} // End for

	//   cChar[nPos]    cChar[nPos+1]   cChar[nPos+2]
	//        |               |               |
	// -------+------- -------+------- -------+-------
	// 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
	// | | | | | | | | | | | | | | | | | | | | | | | |
	// x x x x x x x x x x x x x x x x x x x x x x x x
	// | | | | | | | | | | | | | | | | | | | | | | | |
	// 5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0
	// -----+----- -----+----- -----+----- -----+-----
	//      |           |           |           |
	//   nIndex1     nIndex2     nIndex3     nIndex4
	//
	for (nPos = 0; nPos < nChars; nPos += 3)
	{
		if (nPos + 1 >= nChars) cChar[nPos + 1] = '0';
		if (nPos + 2 >= nChars) cChar[nPos + 2] = '0';
		nIndex4 = ( cChar[nPos + 2] & 0x3F ) & 0x3F;
		nIndex3 = ( ((cChar[nPos + 1] & 0x0F) << 2) 
					| ((cChar[nPos + 2] & 0xC0) >> 6) ) & 0x3F;
		nIndex2 = ( ((cChar[nPos] & 3) << 4) 
					| ((cChar[nPos + 1] & 0xF0) >> 4) ) & 0x3F;
		nIndex1 = ( (cChar[nPos] & 0xFC) >> 2 ) & 0x3F;
		if (nPos + 1 >= nChars)
		{
			nIndex3 = 64;
			nIndex4 = 64;
		} // end if
		if (nPos + 2 >= nChars)
		{
			nIndex4 = 64;
		} // end if
		sTemp.Format("%c%c%c%c",
					 cTable[nIndex1],
					 cTable[nIndex2],
					 cTable[nIndex3],
					 cTable[nIndex4]);
		sBase64 += sTemp;
	} // End for

	return sBase64;
}


// ********************* private
// *                   *
// *  ResponseMessage  *
// *                   *
// *********************
// Function: Returns a textual message describing 
//           a TCP/IP response code.

CString CRobotInternet::ResponseMessage(const int nCode)
{
	CString sErrMsg = "";

	switch (nCode)
	{
	//---- 200 series (success) ----
	case 200:
		sErrMsg = "OK, request succeeded";
		break;
	case 201:
		sErrMsg = "OK, new resource created.";
		break;
	case 202:
		sErrMsg = "Request accepted but processing not completed.";
		break;
	case 204:
		sErrMsg = "OK, but no content to return.";
		break;
	//---- 300 series (redirection) ----
	case 301:
		sErrMsg = "Requested resource has been assigned a "
				  "new permanent URL.";
		break;
	case 302:
		sErrMsg = "Requested resource resides temporarily "
				  "under a different URL.";
		break;
	case 304:
		sErrMsg = "Document has not been modified.";
		break;
	//---- 400 series (client error) ----
	case 400:
		sErrMsg = "Bad request.";
		break;
	case 401:
		sErrMsg = "Unauthorized; request requires "
				  "user authentication.";
		break;
	case 403:
		sErrMsg = "Forbidden for unspecified reason.";
		break;
	case 404:
		sErrMsg = "Cannot find required data on website.  Try again later.";
		break;
	case 407:
		sErrMsg = "Unauthorized; reject by proxy server.";
		break;
	//---- 500 series (server error) ----
	case 500:
		sErrMsg = "Internal server error.";
		break;
	case 501:
		sErrMsg = "Not implemented.";
		break;
	case 502:
		sErrMsg = "Bad gateway; invalid response from "
				  "gateway or upstream server.";
		break;
	case 503:
		sErrMsg = "Service temporarily unavailable.";
		break;
	default:
		sErrMsg.Format("Error %d", nCode);
		break;
	} // End switch
	return sErrMsg;
}


// ****************** private
// *                *
// *  ErrorMessage  *
// *                *
// ******************
// Function: Returns a textual message describing a CRobot
//           error code.

CString CRobotInternet::ErrorMessage(const int nError)
{
	CString sErrMsg = "";

	switch (nError) 
	{
	case CROBOT_ERR_SUCCESS:
		sErrMsg = "Successful";
		break;
	case CROBOT_ERR_INVALID_URL:
		sErrMsg = "Invalid URL";
		break;
	case CROBOT_ERR_INVALID_PARAMETER:
		sErrMsg = "Invalid parameter";
		break;
	case CROBOT_ERR_CONNECTION_FAILED:
		sErrMsg = "Connection failed";
		break;
	case CROBOT_ERR_TIMED_OUT:
		sErrMsg = "Timed out.. try again later, website could be down.";
		break;
	case CROBOT_ERR_NOT_FOUND:
		sErrMsg = "Cannot find required data.  Try again later.";
		break;
	case CROBOT_ERR_NOT_AUTHORIZED:
		sErrMsg = "Not authorized";
		break;
	case CROBOT_ERR_DISK_FILE_ERROR:
		sErrMsg = "Disk/file error";
		break;
	default:
		sErrMsg.Format("CRobotInternet error %d", nError);
		break;
	} // End switch
	return sErrMsg;
}


// **************************************************************
// *															*
// *															*
// *				 H T T P  F u n c t i o n s					*
// *															*
// *															*
// **************************************************************


// ---------------------------------------------------------------
// ************************** private
// *                        *
// *  CreateStandardHeader  *
// *                        *
// **************************
// Function: Return a standard header to use with OpenURL calls.
//           If a call has been made to set proxy logon information,
//           the authentication string is included in the header
//			 that is returned.
//
// This is a private function called by various public functions.

CString CRobotInternet::CreateStandardHeader()
{
	CString sHeader;

	sHeader = "Accept: */*\r\n";

	if (m_sProxyLogonMethod=="basic" && m_sProxyLogonUsername!="")
	{
		sHeader += "Proxy-authorization: Basic "
				   + EncodeTextBase64(m_sProxyLogonUsername 
									  + ":"
									  + m_sProxyLogonPassword)
				   + "\r\n";
	} // End if

	if (m_sLogonUsername != "") 
	{
/*		sHeader += "Authorization: "
				   + m_sLogonUsername
				   + ":"
				   + m_sLogonPassword
				   + "\r\n";
*/
		sHeader += "Authorization: Basic "
				   + EncodeTextBase64(m_sLogonUsername
				                      + ":"
									  + m_sLogonPassword)
				   + "\r\n";
	} // End if

	sHeader += "\r\n";
	return sHeader;
}


// --------------------------------------------------------------
// ************* public
// *           *
// *  httpGet  *
// *           *
// *************
// Function: Retrieves a URL and returns it in CString form.
//
// Inputs:	sURL              - The URL to access
//								 (example: "www.mysite.com")
//
// Outputs:	<function_result> - True if data was successfully
//								 retrieved, false otherwise
//			sResponse         - The HTML retrieved.
//			nResult           - Completion code. 0 = success,
//								 n = error (defined in CRobot.h)
//			sErrMsg           - The error message, if nResult != 0

BOOL CRobotInternet::httpGet(const CString& sURL,
							 CString& sResponse,
							 int& nResult,
							 CString& sErrMsg)
{
	// Variable declarations
	CInternetSession* pSession;
	CHttpFile* pHttpFile;
	CString sHeader;

	int nRead;
	LPSTR pBuffer = NULL;
	CString sResult;
	CString sWorkingUrl;
	CString sMsg;
	sErrMsg = "";
	nResult = CROBOT_ERR_SUCCESS;
	DWORD dwHttpStatus;

	try 
	{
		// Initialize variables
		pSession = NULL;
		pHttpFile = NULL;
		sHeader = CreateStandardHeader();
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sURL;
		
		/* Trim URL and add http:// if it contains no 
		   protocol identifier */

		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		DWORD dwFlags;

		// Check for invalid parameters
		if (!sURL.IsEmpty()) 
		{
			// URL is not empty
			/* Check the URL - must be valid and of the 'http:'
			   service type */
			DWORD dwServiceType;
			CString sServer, sObject;
			unsigned short nPort;
			if (AfxParseURL(sWorkingUrl,
							dwServiceType,
							sServer,
							sObject,
							nPort))
			{
				// URL is valid
				if (dwServiceType == AFX_INET_SERVICE_HTTP)
				{
					//URL is the correct service type (HTTP).
					pSession = new CInternetSession(
									m_sUserAgent,
									++m_nContext,
									INTERNET_OPEN_TYPE_PRECONFIG);

					dwFlags = INTERNET_FLAG_TRANSFER_BINARY 
							  | INTERNET_FLAG_EXISTING_CONNECT;
					if (!m_bReadFromCache) 
						dwFlags = dwFlags | INTERNET_FLAG_RELOAD;
					if (!m_bWriteToCache)
						dwFlags = dwFlags | INTERNET_FLAG_DONT_CACHE;
					   OutputDebugString("Opening internet file..");
					pHttpFile = (CHttpFile*) 
									pSession->OpenURL(sWorkingUrl,
													  1,
													  dwFlags,
													  sHeader,
													  -1L);
					if (pHttpFile) /* OpenURL worked */
					{
						// Check the HTTP return code
						if (!pHttpFile->QueryInfoStatusCode(dwHttpStatus))
							dwHttpStatus = 200;

						if (dwHttpStatus >= 400)
						{
							switch(dwHttpStatus)
							{
							case 404:
								nResult = CROBOT_ERR_NOT_FOUND;
								break;
							case 403:
							case 407:
								nResult = CROBOT_ERR_NOT_AUTHORIZED;
								break;
							default:
								nResult = CROBOT_ERR_CONNECTION_FAILED;
								break;
							} // End switch
						} // End if dwHttpStatus
						else /* No error - read response data */
						{
							nResult = CROBOT_ERR_SUCCESS;
							// Read the data
							do 
							{
								nRead = pHttpFile->Read(pBuffer, 1023);
								if (nRead != 0) 
								{
									pBuffer[nRead] = 0;
									sResult += pBuffer;
								} // End if
							} while (nRead != 0);
							sResponse = sResult;
						} // End else
					} // End if pHttpFile
					else /* OpenURL failed */
						nResult = CROBOT_ERR_CONNECTION_FAILED;

				} // End if
				else
					// Wrong service type
					nResult = CROBOT_ERR_INVALID_URL;
			}
			else
				// Invalid URL
				nResult = CROBOT_ERR_INVALID_URL;
		} // End if
		else
			// Empty URL
			nResult = CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch (CInternetException* e) 
	{
		e->Delete();
		sResponse = sResult;
		
		// Internet exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch
	catch (...) 
	{
		sResponse = sResult;
		
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pBuffer != NULL) 
	{
		delete pBuffer;
		pBuffer = NULL;
	} // End if

	if (pHttpFile != NULL) 
	{
		pHttpFile->Close();
		delete pHttpFile;
	} // End if

	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession;
	} // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}


// --------------------------------------------------------------
// ***************** public
// *               *
// *  httpGetFile  *
// *               *
// *****************
// Function: Retrieves a URL and outputs it to a local file
//
// Inputs:	sURL              - The URL to access
//                               (example: "www.mysite.com")
//			sFile             - File to output to
//                               (example: "c:\temp\file1.gif")
//
// Outputs:	<function_result> - True if data was successfully
//                               retrieved, false otherwise
//			nResult           - Completion code. 0 = success,
//                               n = error (defined in CRobot.h)
//			sErrMsg           - The error message, if nResult != 0

BOOL CRobotInternet::httpGetFile(const CString& sURL,
								 const CString& sOutputFilespec,
								 int& nResult,
								 CString& sErrMsg)
{
	CInternetSession* pSession;
	CHttpFile* pHttpFile;
	CFile* pLocalFile;
	CFileException exFile;
	CString sHeader;
	int nRead;
	LPSTR pBuffer = NULL;
	CString sResult;
	CString sWorkingUrl;
	CString sTemp;
	sErrMsg = "";
	DWORD dwHttpStatus;
	nResult = CROBOT_ERR_SUCCESS;
	CString sMsg;

	try 
	{
		pSession = NULL;
		pHttpFile = NULL;
		pLocalFile = NULL;
		sHeader = CreateStandardHeader();
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sURL;

		/* Trim URL and add http:// if it contains no 
		   protocol identifier */

		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		// Check for invalid parameters
		if (!(sURL.IsEmpty()) && !(sOutputFilespec.IsEmpty()))
		{
			// URL is not empty and output file spec is not empty
			/* Check the URL - must be valid and 
			   of the 'http:' service type */
			DWORD dwServiceType;
			CString sServer, sObject;
			unsigned short nPort;
			if (AfxParseURL(sWorkingUrl,
							dwServiceType,
							sServer,
							sObject,
							nPort)) 
			{
				if (dwServiceType == AFX_INET_SERVICE_HTTP)
				{
					pSession = new CInternetSession(
											m_sUserAgent,
											++m_nContext,
											INTERNET_OPEN_TYPE_PRECONFIG);

					pHttpFile = (CHttpFile*) 
						pSession->OpenURL(
									sWorkingUrl,
									1,
									INTERNET_FLAG_RELOAD
									    | INTERNET_FLAG_DONT_CACHE
										| INTERNET_FLAG_TRANSFER_BINARY
										| INTERNET_FLAG_EXISTING_CONNECT,
								    sHeader,
									-1L);
					if (pHttpFile) /* OpenURL worked */
					{
						// Check the http return code
						if (!pHttpFile->QueryInfoStatusCode(dwHttpStatus))
							dwHttpStatus = 200;

						if (dwHttpStatus >= 400)
						{
							switch(dwHttpStatus)
							{
							case 404:
								nResult = CROBOT_ERR_NOT_FOUND;
								break;
							case 403:
							case 407:
								nResult = CROBOT_ERR_NOT_AUTHORIZED;
								break;
							default:
								nResult = CROBOT_ERR_CONNECTION_FAILED;
								break;
							} // End switch
						} // End if dwHttpStatus
						else /* No error - read response data */
						{
							nResult = CROBOT_ERR_SUCCESS;
							// Open local file for output
							pLocalFile = new CFile;
							pLocalFile->Open(sOutputFilespec,
											 CFile::modeWrite 
											     | CFile::modeCreate);
							// Read the data
							do 
							{
								nRead = pHttpFile->Read(pBuffer, 1023);
								if (nRead != 0)
								{
									pBuffer[nRead] = 0;
									pLocalFile->Write(pBuffer, nRead);
									if (sResult == "") sResult = pBuffer;
								} // End if
							} while (nRead != 0);
						} // End else
						// Check for error embedded in return page
					} // End if pHttpFile
					else /* OpenURL failed */
					{
						nResult = CROBOT_ERR_CONNECTION_FAILED;
					} // End else
				} // End if
				else
					// Wrong service
					nResult = CROBOT_ERR_INVALID_URL;
			} // End if
			else
				// Invalid URL
				nResult = CROBOT_ERR_INVALID_URL;
		} // End if
		else
			// Empty URL or empty filespec
			nResult = CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch(CFileException* e) 
	{
		int nErr = e->m_cause;
		e->Delete();
		delete pLocalFile;
		pLocalFile = NULL;
		// File exception occurred
		nResult = CROBOT_ERR_FILE + nErr;
	} // End catch

	catch (CInternetException* e)
	{
		e->Delete();
		
		// Internet exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

	catch (...) 
	{
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pBuffer != NULL) 
	{
		delete pBuffer;
		pBuffer = NULL;
	} // End if

	if (pHttpFile != NULL) 
	{
		pHttpFile->Close();
		delete pHttpFile;
	} // End if

	if (pLocalFile != NULL) 
	{
		pLocalFile->Close();
		delete pLocalFile;
	} // End if

	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession;
	} // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}


// --------------------------------------------------------------
// **************** public
// *              *
// *  httpHeader  *
// *              *
// ****************
// Function: Retrieves a server header for a URL and returns it
//           in CString form.
//			 See also: Similar function httpHeaderFields.
//
// Inputs:	sURL              - The target URL
//								 (example: "www.mysite.com")
//
// Outputs:	<function_result> - True if header was successfully 
//                               retrieved, false otherwise.
//			sResponse         - The header returned by the server
//			nResult           - completion code; 0 = success,
//								 n = error (defined in CRobot.h)
//			sErrMsg           - the error message, if nResult != 0

BOOL CRobotInternet::httpHeader(const CString& sUrl,
								CString& sResponse,
								int& nResult,
								CString& sErrMsg) 
{
	CInternetSession* pSession;
	CHttpConnection* pConnection;
	CHttpFile* pHttpFile;
	CString sHeader;
	int nRead;
	LPSTR pBuffer;
	CString sResult;
	CString sWorkingUrl;
	CString sMsg;
	sErrMsg = "";
	nResult = CROBOT_ERR_CONNECTION_FAILED;

	try 
	{
		pSession = NULL;
		pConnection = NULL;
		pHttpFile = NULL;
		pHttpFile = NULL;
		sHeader = CreateStandardHeader();
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sUrl;

		/* Trim URL and add http:// if it contains no 
		   protocol identifier */
		
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		// Check for invalid parameters
		if (!sUrl.IsEmpty()) 
		{
			// URL is not empty
			/* Check the URL - must be valid and of the 'http:'
			   service type */
			DWORD dwServiceType;
			CString sServer, sObject;
			unsigned short nPort;
			if (AfxParseURL(sWorkingUrl,
							dwServiceType,
							sServer,
							sObject,
							nPort))
			{
				//URL is valid. Now check service type.
				if (dwServiceType == AFX_INET_SERVICE_HTTP) 
				{
					//Service type is valid. Make connection.
					pSession = new CInternetSession(
										m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);
					pConnection = pSession->GetHttpConnection(sServer,
															  nPort,
															  NULL,
															  NULL);
					pHttpFile = pConnection->OpenRequest(
									CHttpConnection::HTTP_VERB_HEAD,
									sObject,
									NULL,
									1,
									NULL,
									NULL,
									INTERNET_FLAG_EXISTING_CONNECT
										| INTERNET_FLAG_RELOAD
										| INTERNET_FLAG_DONT_CACHE);
					if (pHttpFile->SendRequest()) /* SendRequest worked */
					{
						DWORD dwHttpStatus;
						// Check the http return code
						if(!pHttpFile->QueryInfoStatusCode(dwHttpStatus))
							dwHttpStatus = 400;

						if (dwHttpStatus >= 400)
						{
							switch(dwHttpStatus)
							{
							case 404:
								nResult = CROBOT_ERR_NOT_FOUND;
								break;
							case 403:
							case 407:
								nResult = CROBOT_ERR_NOT_AUTHORIZED;
								break;
							default:
								nResult = CROBOT_ERR_CONNECTION_FAILED;
								break;
							} // End switch
							unsigned long nFlag = 1; // HTTP_QUERY_CUSTOM;
							unsigned long nChars = 1023;
							DWORD nFlag2 = HTTP_QUERY_RAW_HEADERS_CRLF;
							pHttpFile->QueryInfo(nFlag2, sResult, NULL);
							sResponse = sResult;
						} // End if dwHttpStatus
						else /* No error - read response data */
						{
							nResult = CROBOT_ERR_SUCCESS;
							// Read the data
							do 
							{
								nRead = pHttpFile->Read(pBuffer, 1023);
								if (nRead != 0)
								{
									pBuffer[nRead] = 0;
									sResult += pBuffer;
								} // End if
							} while (nRead != 0);
							sResponse = sResult;
						} // End else
					} // End if pHttpFile
					else /* SendRequest failed */
						nResult = CROBOT_ERR_CONNECTION_FAILED;
					
				} // End if
				else
					// Wrong service
					nResult = CROBOT_ERR_INVALID_URL;
			} // End if
			else
				// Invalid URL
				nResult = CROBOT_ERR_INVALID_URL;
		} // End if
		else
			// Empty URL
			nResult =  CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch (CInternetException* e) 
	{
		e->Delete();
		sResponse = sResult;

		// Internet exception occurred
		nResult =  CROBOT_ERR_CONNECTION_FAILED;
	} // End catch
	catch (...) 
	{
		sResponse = sResult;
		
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pBuffer != NULL) 
	{
		delete pBuffer;
		pBuffer = NULL;
	} // End if

	if (pHttpFile != NULL) 
	{
		pHttpFile->Close();
		delete pHttpFile; 
	} // End if

	if (pConnection != NULL) 
	{
		pConnection->Close();
		delete pConnection; 
	} // End if

	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession; 
	} // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}


// --------------------------------------------------------------
// ********************** public
// *                    *
// *  httpHeaderFields  *
// *                    *
// **********************
// Function: Retrieves a server header for a URL and returns it in
//           CString form. Like httpHeader, but also parses header
//           into individual field names and values.
//
// Inputs:   sURL              - The target URL
//								 (example: "www.mysite.com")
//
// Outputs:	 <function_result> - True if header was successfully 
//								 retrieved, false otherwise
//			 sResponse         - The header returned by the server
//			 nResult           - Completion code. 0 = success,
//								 n = error (defined in CRobot.h)
//			 sErrMsg           - The error message, if nResult != 0
//			 m_sHeader		   - Copy of the header string
//			 m_nHeaderFields   - Set to number of header fields
//			 m_sHeadName[]     - Contains individual header field names
//			 m_sHeadValue[]    - Contains individual header field values

BOOL CRobotInternet::httpHeaderFields(const CString& sUrl,
									  CString& sResponse,
									  int& nResult,
									  CString& sErrMsg) 
{
	CInternetSession* pSession;
	CHttpConnection* pConnection;
	CHttpFile* pHttpFile;
	CString sHeader;
	int nRead;
	LPSTR pBuffer;
	CString sResult;
	CString sWorkingUrl;
	CString sMsg;
	CString sHTML, sTemp;
	int nPos;
	sErrMsg = "";
	nResult = CROBOT_ERR_SUCCESS;
	m_nHeaderFields = 0;

	try 
	{
		pSession = NULL;
		pConnection = NULL;
		pHttpFile = NULL;
		sHeader = CreateStandardHeader();
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sUrl;

		/* Trim URL and add http:// if it contains no 
		   protocol identifier */
		
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		// Check for invalid parameters
		if (!sUrl.IsEmpty()) 
		{
			// URL is not empty
			/* Check the URL - must be valid and of the 'http:'
			   service type */
			DWORD dwServiceType;
			CString sServer, sObject;
			unsigned short nPort;
			if (AfxParseURL(sWorkingUrl,
							dwServiceType,
							sServer,
							sObject,
							nPort)) 
			{
				// URL is valid. Now check service type.
				if (dwServiceType == AFX_INET_SERVICE_HTTP) 
				{
					//Service type is valid. Make connection.
					pSession = new CInternetSession(
										m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);
					pConnection = pSession->GetHttpConnection(sServer,
															  nPort,
															  NULL,
															  NULL);

					pHttpFile = pConnection->OpenRequest(
									CHttpConnection::HTTP_VERB_HEAD,
									sObject,
									NULL,
									1,
									NULL,
									NULL,
									INTERNET_FLAG_EXISTING_CONNECT
										| INTERNET_FLAG_RELOAD
										| INTERNET_FLAG_DONT_CACHE);
					pHttpFile->SendRequest();
					
					unsigned long nFlag = 1;	// HTTP_QUERY_CUSTOM;
					unsigned long nChars = 1023;
					DWORD nFlag2 = HTTP_QUERY_RAW_HEADERS_CRLF;
					pHttpFile->QueryInfo(nFlag2, sResult, NULL);
					sResponse = sResult;
					nResult =  CROBOT_ERR_SUCCESS;
				} // End if
				else
					// Wrong service
					nResult = CROBOT_ERR_INVALID_URL;
			} // End if
			else
				// Invalid URL
				nResult = CROBOT_ERR_INVALID_URL;
		} // End if
		else
			// Empty URL
			nResult =  CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch (CInternetException* e) 
	{
		e->Delete();
		sResponse = sResult;
		
		// Internet exception occurred
		nResult =  CROBOT_ERR_CONNECTION_FAILED;
	} // End catch
	catch (...) 
	{
		sResponse = sResult;
		
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pBuffer != NULL) 
	{
		delete pBuffer;
		pBuffer = NULL;
	} // End if

	if (pHttpFile != NULL)
	{
		pHttpFile->Close();
		delete pHttpFile; 
	} // End if

	if (pConnection != NULL)
	{
		pConnection->Close();
		delete pConnection; 
	} // End if

	if (pSession != NULL)
	{
		pSession->Close();
		delete pSession; 
	} // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
	{
		// Parse header into individual fields
		m_sHeader = sResponse;
		m_nHeaderFields = 0;
		sHTML = sResponse;
		if (sHTML.Right(1) != "\n")
			sHTML += "\n";
		nPos = sHTML.Find("\n");
		while (nPos != -1)
		{
			sTemp = sHTML.Left(nPos);
			sHTML = sHTML.Mid(nPos + 1);
			nPos = sTemp.Find(":");
			if (nPos != -1)
			{
				m_sHeadName[m_nHeaderFields] = sTemp.Left(nPos);
				m_sHeadValue[m_nHeaderFields] = sTemp.Mid(nPos+1);
				m_sHeadValue[m_nHeaderFields].TrimLeft();
				m_sHeadValue[m_nHeaderFields].TrimRight();
				m_nHeaderFields++;
			} // End if
			nPos = sHTML.Find("\n");
		} // End while
		return true;
	} // End if
	else
		return false;
}


// --------------------------------------------------------------
// ************************ public
// *                      *
// *  httpGetHeaderField  *
// *                      *
// ************************
// Function: Returns the value for the specified header field.
//			 Call httpHeaderFields before calling this function.
//
// Inputs:	 sFieldName		  - The header field name desired 
//								 (example: "Last-modified").
//			 m_sHeader		  - Set from previous call to 
//								 httpHeaderFields
//			 m_nHeaderFields  - Set from previous call to 
//                               httpHeaderFields
//			 m_sHeaderName[]  - Set from previous call to
//								 httpHeaderFields
//			 m_sHeaderValue[] - Set from previous call to
//								 httpHeaderFields
//
// Outputs:	<function_result> - True if header field was found
//			sValue            - Value for the field

BOOL CRobotInternet::httpGetHeaderField(const CString& sName,
										CString& sValue)
{
	CString sTemp1;
	CString sTemp2;
	sValue = "";
	sTemp1 = sName;
	sTemp1.MakeLower();
	if (sTemp1.Right(1) == ":")
		sTemp1 = sTemp1.Left(sTemp1.GetLength() - 1);
	
	for (int n = 0; n < m_nHeaderFields; n++)
	{
		sTemp2 = m_sHeadName[n];
		sTemp2.MakeLower();
		if (sTemp1 == sTemp2)
		{
			sValue = m_sHeadValue[n];
			n = m_nHeaderFields;
			return true;
			} // End if
	} // End for
	return false;
}


// --------------------------------------------------------------
// ************** public
// *            *
// *  httpPost  *
// *            *
// **************
// Function: Submits a URL and form data/parameters using the POST
//           method. Retrieves a response returns it in CString form.
//
// Inputs:	sURL              - The URL to access
//								 (example: "www.mysite.com")
//			sData			  - The parameters (or "form data")
//								 to submit
//
// Outputs:	<function_result> - True if data was successfully
//								 retrieved, false otherwise
//			sResponse         - The HTML retrieved
//			nResult           - Completion code. 0 = success,
//								 n = error (defined in CRobot.h)
//			sErrMsg           - The error message, if nResult != 0

BOOL CRobotInternet::httpPost(const CString& sUrl,
							  const CString& sData,
							  CString& sResponse,
							  int& nResult, CString& sErrMsg)
{
	CInternetSession* pSession;
	CHttpConnection* pConnection;
	CHttpFile* pHttpFile;
	int nRead;
	LPSTR pBuffer = NULL;
	CString sResult;
	CString sWorkingUrl;
	CString sHeaders = "";
	CString sMsg;
	sErrMsg = "";
	DWORD dwHttpStatus;
	nResult = CROBOT_ERR_SUCCESS;
	TCHAR sTemp[1024];

	try 
	{
		pSession = NULL;
		pConnection = NULL;
		pHttpFile = NULL;
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sUrl;
		sHeaders = _T("Content-Type: "
					  "application/x-www-form-urlencoded\r\n")
				   + CreateStandardHeader();

		/* Trim URL and add http:// if it contains no 
		   protocol identifier */
		
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		/* Check the URL - must be valid and of the 'http:'
		   service type */
		DWORD dwServiceType;
		CString sServer, sObject;
		unsigned short nPort;
		if (AfxParseURL(sWorkingUrl,
						dwServiceType,
						sServer,
						sObject,
						nPort))
		{
			// URL is valid. Now check service type.
			if (dwServiceType == AFX_INET_SERVICE_HTTP) 
			{
				// Service type is valid (HTTP). Now make connection.
				pSession = new CInternetSession(
										m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);
				pConnection = pSession->GetHttpConnection(sServer,
														  nPort,
														  NULL,
														  NULL);

				pHttpFile = pConnection->OpenRequest(
										CHttpConnection::HTTP_VERB_POST,
										sObject,
										sServer,
										1,
										NULL,
										NULL,
										INTERNET_FLAG_EXISTING_CONNECT
											| INTERNET_FLAG_RELOAD
											| INTERNET_FLAG_DONT_CACHE);
				strcpy (sTemp, sData);
				pHttpFile->SendRequest(sHeaders,
									   sTemp,
									   sData.GetLength());
				if (pHttpFile) /* SendRequest worked */
				{
					// Check the http return code
					if (!pHttpFile->QueryInfoStatusCode(dwHttpStatus))
						dwHttpStatus = 200;

					if (dwHttpStatus >= 400)
					{
						switch(dwHttpStatus)
						{
						case 404:
							nResult = CROBOT_ERR_NOT_FOUND;
							break;
						case 403:
						case 407:
							nResult = CROBOT_ERR_NOT_AUTHORIZED;
							break;
						default:
							nResult = CROBOT_ERR_CONNECTION_FAILED;
							break;
						} // End switch
					} // End if dwHttpStatus
					else /* No error - read response data */
					{
						nResult = CROBOT_ERR_SUCCESS;
						do 
						{
							nRead = pHttpFile->Read(pBuffer, 1023);
							if (nRead != 0) 
							{
								pBuffer[nRead] = 0;
								sResult += pBuffer;
							} // End if
						} while (nRead != 0);
						sResponse = sResult;
					} // End else
				} // End if pHttpFile
				else /* SendRequest failed */
				{
					nResult = CROBOT_ERR_CONNECTION_FAILED;
				} // End else
			} // End if
			else
				// Wrong service
				nResult = CROBOT_ERR_INVALID_URL;
		} // End if
		else
			// Invalid URL
			nResult = CROBOT_ERR_INVALID_URL;
	} // End try

	catch (CInternetException* e) 
	{
		e->Delete();
		sResponse = sResult;
		
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch
	catch (...) 
	{
		sResponse = sResult;
		
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pBuffer != NULL)
	{
		delete pBuffer;
		pBuffer = NULL;
	} // End if
	
	if (pHttpFile != NULL) 
	{
		pHttpFile->Close();
		delete pHttpFile; 
	} // End if

	if (pConnection != NULL) 
	{
		pConnection->Close();
		delete pConnection; 
	} // End if

	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession; 
	} // End if
	
	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}


// --------------------------------------------------------------
// ****************** public
// *                *
// *  httpPostFile  *
// *                *
// ******************
// Function: Submits a URL and form data/parameters using
//           the POST method. Retrieves response and outputs
//           it to a file.
//
// Inputs:	sURL              - The URL to access
//								 (example: "www.mysite.com")
//			sData			  - The parameters (or "form data")
//								 to submit
//			sOutputFilespec   - File specification of file to 
//								 create/overwrite
//
// Outputs:	<function_result> - True if data was successfully
//								 retrieved, false otherwise
//			nResult           - Completion code. 0 = success,
//								 n = error (defined in CRobot.h)
//			sErrMsg           - The error message, if nResult != 0

BOOL CRobotInternet::httpPostFile(const CString& sUrl,
								  const CString& sData,
								  const CString& sOutputFilespec,
								  int& nResult,
								  CString& sErrMsg)
{
	CInternetSession* pSession;
	CHttpConnection* pConnection;
	CHttpFile* pHttpFile;
	CString sHeader;
	int nRead;
	LPSTR pBuffer = NULL;
	CString sResult;
	CString sWorkingUrl;
	CString sHeaders = "";
	CFile* pLocalFile;
	char sTemp[1024];
	DWORD dwHttpStatus;
	nResult = CROBOT_ERR_SUCCESS;
	CString sMsg;

	try 
	{
   		pSession = NULL;
		pConnection = NULL;
		pHttpFile = NULL;
		pLocalFile = NULL;
		sHeader = CreateStandardHeader();
		nRead = 0;
		pBuffer = new char[1024];
		sResult = "";
		sWorkingUrl = sUrl;
		sHeaders = _T("Content-Type: "
					  "application/x-www-form-urlencoded\r\n")
				   + CreateStandardHeader();

		/* Trim URL and add http:// if it contains no 
		   protocol identifier */
		
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();
		if (sWorkingUrl.Find(":") == -1) 
		{
			if (sWorkingUrl.Left(1) == "/")
				sWorkingUrl = "http:" + sWorkingUrl;
			else
				sWorkingUrl = "http://" + sWorkingUrl;
		} // End if

		/* Check the URL - must be valid and of the 'http:'
		   service type */
		DWORD dwServiceType;
		CString sServer, sObject;
		unsigned short nPort;
		if (AfxParseURL(sWorkingUrl,
						dwServiceType,
						sServer,
						sObject,
						nPort)) 
		{
			// URL is valid. Now check service type.
			if (dwServiceType == AFX_INET_SERVICE_HTTP)
			{
				/* Service type is correct (HTTP). 
				   Now make the connection. */
				pSession = new CInternetSession(
										m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);
				pConnection = pSession->GetHttpConnection(sServer,
														  nPort,
														  NULL,
														  NULL);
				
				pHttpFile = pConnection->OpenRequest(
									CHttpConnection::HTTP_VERB_POST,
									sObject,
									NULL,
									1,
									NULL,
									NULL,
									INTERNET_FLAG_EXISTING_CONNECT
										| INTERNET_FLAG_RELOAD
										| INTERNET_FLAG_DONT_CACHE);

				strcpy (sTemp, sData);
				pHttpFile->SendRequest(sHeaders,
									   &sTemp,
									   sData.GetLength());
				if (pHttpFile) /* SendRequest worked */
				{
					// Check the http return code
					if (!pHttpFile->QueryInfoStatusCode(dwHttpStatus))
						dwHttpStatus = 200;

					if (dwHttpStatus >= 400)
					{
						switch(dwHttpStatus)
						{
						case 404:
							nResult = CROBOT_ERR_NOT_FOUND;
							break;
						case 403:
						case 407:
							nResult = CROBOT_ERR_NOT_AUTHORIZED;
							break;
						default:
							nResult = CROBOT_ERR_CONNECTION_FAILED;
							break;
						} // End switch
					} // End if dwHttpStatus
					else /* No error - read response data */
					{
						nResult = CROBOT_ERR_SUCCESS;
						// Open local file for output
						pLocalFile = new CFile;

						pLocalFile->Open(sOutputFilespec,
										 CFile::modeWrite 
											| CFile::modeCreate);

						do 
						{
							nRead = pHttpFile->Read(pBuffer, 1023);
							if (nRead != 0)
							{
								pBuffer[nRead] = 0;
								pLocalFile->Write(pBuffer, nRead);
							} // End if
						} while (nRead != 0);
					} // End else
				} // End if pHttpFile
				else /* SendRequest failed */
				{
					nResult = CROBOT_ERR_CONNECTION_FAILED;
				}
			} // End if
			else
				// Wrong service
				nResult = CROBOT_ERR_CONNECTION_FAILED;
		} // End if
		else
			// Invalid URL
			nResult = CROBOT_ERR_INVALID_URL;
	} // End try

	catch (CInternetException* e) 
	{
		e->Delete();
		
		// Internet exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

	catch (...) 
	{
		// Exception occurred
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function
	
	if (pBuffer != NULL) 
	{
		delete pBuffer; 
		pBuffer = NULL;
	} // End if

	if (pHttpFile != NULL) 
	{
		pHttpFile->Close();
		delete pHttpFile; 
	} // End if
	
	if (pLocalFile != NULL) 
	{
		pLocalFile->Close();
		delete pLocalFile; 
	} // End if

	if (pConnection != NULL) 
	{
		pConnection->Close();
		delete pConnection; 
	} // End if
	
	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession;
	}  // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}



// --------------------------------------------------------------
// *************** public
// *             *
// *  httpError  *
// *             *
// ***************
// Function: Scans an HTML response page for HTTP error codes.
//
// Inputs:	sHTML             - HTML response of a prior Internet
//								 access
//
// Outputs:	<function_result> - True if an error code was detected;
//								 false if no errors were detected in
//								 the response
//			nErrorCode        - HTTP error code, such as 404
//			sErrMsg           - Error message text for nErrorCode,
//								 such as "object not found"

BOOL CRobotInternet::httpError(const CString& sHTML,
							   int& nErrorCode,
							   CString& sErrMsg)
{
	CString sTemp = sHTML;
	int nPos;

	nPos = sTemp.Find("HTTP/1.0 ");
	if (nPos != -1)
		sTemp = sTemp.Mid(nPos + 9, 3);

	if (nPos == -1) 
	{
		nPos = sTemp.Find("HTTP Error ");
		if (nPos != -1)
			sTemp = sTemp.Mid(nPos + 11, 3);
	} // End if

	if (nPos != -1) 
	{
		nErrorCode = atoi(sTemp);
		sErrMsg = ResponseMessage(nErrorCode);
		return true;
	} // End if
	else 
	{
		nErrorCode = 0;
		sErrMsg = "";
		return false;
	} // End else
}


// --------------------------------------------------------------
// ************************ public
// *                      *
// *  ParseServerFromURL  *
// *                      *
// ************************
// Function: Scans a URL and returns the server name portion
//           of the URL
//
// Inputs:	sURL              - A fully qualified URL
//
// Outputs:	<function_result> - The server portion of the URL

CString CRobotInternet::ParseServerFromURL(const CString& sURL)
{
	DWORD dwService;
	INTERNET_PORT nPort;
	CString sServer;
	CString sObject;
	CString sPath;
	CString sTempURL;

	sTempURL = sURL;

	if (sTempURL.Find(":") == -1) 
	{
		if (sTempURL.Left(1) != "/")
			sTempURL = "//" + sTempURL;
		sTempURL = "http:" + sTempURL;
	} // End if

	AfxParseURL(sTempURL,
				dwService,
				sServer,
				sObject,
				nPort);

	return sServer;
}


// --------------------------------------------------------------
// ********************** public
// *                    *
// *  ParsePathFromURL  *
// *                    *
// **********************
// Function: Scans a URL and returns the directory path portion of
//           the URL.
//
// Inputs:	sURL              - a fully qualified URL.
//
// Outputs:	<function_result> - the directory path portion of the URL.

CString CRobotInternet::ParsePathFromURL(const CString& sURL)
{
	DWORD dwService;
	INTERNET_PORT nPort;
	CString sServer;
	CString sObject;
	CString sPath;
	int nPos;
	CString sTempURL;

	sTempURL = sURL;

	if (sTempURL.Find(":") == -1) 
	{
		if (sTempURL.Left(1) != "/")
			sTempURL = "//" + sTempURL;
		sTempURL = "http:" + sTempURL;
	} // End if

	AfxParseURL(sTempURL, dwService, sServer, sObject, nPort);

	sPath = sObject;

	if (sPath=="")
		sPath = "/";

	if (sPath.Find(".") != -1) 
	{
		nPos = sPath.ReverseFind('/');
		if (nPos != -1)
			sPath = sPath.Left(nPos + 1);
	} // End if

	if (sPath.Right(1) != "/")
		sPath += "/";

	return sPath;
}


// --------------------------------------------------------------
// ******************* public
// *                 *
// *  RobotExcluded  *
// *                 *
// *******************
// Function: Scans a robot policy file (previous acquired from a site),
//			 and reports  whether  a  particular  URL  is  considered
//			 accessible. This function expects the  m_sUserAgent field
//			 to have been set in advance to identify the agent.
//
// Inputs:	sRobotPolicy      - A site's robots.txt file (httpGet may
//								 be used to acquire this file)
//			sUrlToAccess	  - A fully qualified URL
//
// Outputs:	<function_result> - True if access is excluded (denied);
//								 false if access is not explicitly 
//								 prohibited

BOOL CRobotInternet::RobotExcluded(const CString& sRobotPolicy,
								   const CString& sUrlToAccess)
{
	CString sPath = ParsePathFromURL(sUrlToAccess);
	BOOL bIsExcluded;

	/* Unless the user agent value is blank, first check for an
	   explicit reference to this agent */
	
	if (m_sUserAgent != "")
		if (CheckExclusion(sRobotPolicy,
						   m_sUserAgent,
						   sPath,
						   bIsExcluded))
			/* There is an entry for the user agent */
			return bIsExcluded;
	
	/* If there is no entry for the user agent, look for an entry
	   that applies to * (all other robots) */

	if (CheckExclusion(sRobotPolicy, "*", sPath, bIsExcluded))
		/* there is an entry for "*" */
		return bIsExcluded;

	/* Robots.txt contains neither specific nor default prohibitions.
	   Return false to indicate no exclusion. */

	return false;
}

// --------------------------------------------------------------
// ******************** private
// *                  *
// *  CheckExclusion  *
// *                  *
// ********************
// This is a private function called by the public 
//  function RobotExcluded
//
// Function: Scans a URL and returns the server portion of the URL
//
// Inputs:	sRobotPolicy      - Text of robots.txt file previously
//								 acquired from a site.
//			sAgent            - User agent entry to search for. Can 
//								 be a specific agent name, or "*".
//			sPath	          - The path name to be visited on
//								 the site.
//
// Outputs:	<function_result> - True if a section of robots.txt was
//								 found for sAgent
//			bIsExcluded       - Set to true if the access is
//								 explicitly denied; set to true
//								 otherwise

BOOL CRobotInternet::CheckExclusion(const CString& sRobotPolicy,
									const CString& sAgent,
									const CString& sPath,
									BOOL& bIsExcluded)
{
	int nPos;
	int nAgentLen, nExclusionLen;

	CString sRobotLower = sRobotPolicy;
	sRobotLower.MakeLower();

	CString sAgentLower = sAgent;
	sAgentLower.MakeLower();

	nAgentLen = sAgentLower.GetLength();

	CString sPathLower = sPath;
	sPathLower.MakeLower();

	CString sEntryLower = "";

	CString sExclusion = "";

	BOOL bScanningUserAgent, bScanningDisallow;

	/* Find each user-agent: entry and compare it to the
	   agent name specified */

	bScanningUserAgent = true;
	while (bScanningUserAgent) 
	{
		nPos = sRobotLower.Find("user-agent:");
		if (nPos != -1) 
		{
			sRobotLower = sRobotLower.Mid(nPos + 11);
			sRobotLower.TrimLeft();
			if (sRobotLower.Left(nAgentLen) == sAgentLower) 
			{
				// Found entry for this agent
				nPos = sRobotLower.Find("user-agent:");
				if (nPos == -1)
					sEntryLower = sRobotLower;
				else
					sEntryLower = sRobotLower.Left(nPos);
				
				// Find each disallow: statement in the entry

				bScanningDisallow = true;
				while (bScanningDisallow) 
				{
					nPos = sEntryLower.Find("disallow:");
					if (nPos != -1) 
					{
						sEntryLower = sEntryLower.Mid(nPos + 9);
						sEntryLower.TrimLeft();
						nPos = sEntryLower.Find("disallow:");
						if (nPos == -1)
							sExclusion = sEntryLower;
						else
							sExclusion = sEntryLower.Left(nPos);
						sExclusion.TrimRight();
						if (sExclusion.Right(1) != "/")
							sExclusion += "/";
						nExclusionLen = sExclusion.GetLength();

						/* Compare exclusion to target path to see 
						   if there is a match */
						if (sPath.Left(nExclusionLen) == sExclusion)
						{
							bIsExcluded = true;
							return true;
						} // End if sPath
						
					} // End if nPos
					else
						bScanningDisallow = false;
				} // End while bScanningDisallow

				/* No exclusion encountered or function would have 
				   already returned */
				bIsExcluded = false;
				
				// There is an entry for this agent
				return true;
			} // End if
		} // end if nPos
		else
			bScanningUserAgent = false;
	} // End while bScanningUserAgent

	bIsExcluded = false;
	return false;	/* Agent entry not found */
}


// **************************************************************
// *															*
// *															*
// *					F T P  F u n c t i o n s				*
// *															*
// *															*
// **************************************************************


// --------------------------------------------------------------
// ****************
// *              *
// *  ftpGetFile  *
// *              *
// ****************
// Function: Retrieves a file from an FTP site
//
// Inputs:	sURL              - URL to access, incouding user id
//								 and password info if required
//			sDir              - Directory to move to (optional;
//								 leave blank or NULL to access
//								 default directory).
//			sRemoteFilespec	  - Name of file on remote FTP server
//								 to be retrieved.
//			sLocalFilespec	  - Name to give retrieved file on
//								 local system.
//
// Outputs:	<function-result> - True if successful, false if an
//								 error occurred
//			nResult           - Error code (0 = no error,
//								 n = error code)
//			sErrMsg           - Error message text of error (if any)

BOOL CRobotInternet::ftpGetFile(const CString& sUrl,
								const CString& sDir,
								const CString& sRemoteFilespec,
								const CString& sLocalFilespec,
								int& nResult, CString& sErrMsg)
{
	TCHAR sz[1024];
	CInternetSession* pSession = NULL;
	CStdioFile* pFile = NULL;
	DWORD nRead = 0;
	CFile* pMyFile = NULL;
	CString sId, sPassword;
	CString sTemp;
	CString sWorkingUrl, sWorkingDir;
	nResult = CROBOT_ERR_SUCCESS;
	CString sMsg;
	CString sHeader = "Accept: */*\r\n\r\n";

	try 
	{
		pSession = new CInternetSession(m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);

		sWorkingUrl = sUrl;
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();

		// Check for invalid parameters
		if (!(sUrl.IsEmpty()) 
			&& !(sLocalFilespec.IsEmpty()) 
			&& !(sRemoteFilespec.IsEmpty())) 
		{
			sId = m_sLogonUsername;
			sPassword = m_sLogonPassword;
			
			sTemp = sWorkingUrl.Left(4);
			sTemp.MakeLower();;
			if (sTemp == "ftp:") sWorkingUrl = sWorkingUrl.Mid(4);

			if (sWorkingUrl.Left(2) == "//")
				sWorkingUrl = sWorkingUrl.Mid(2);

			int nPos1 = sWorkingUrl.Find(":");
			int nPos2 = sWorkingUrl.Find("@");
			if (nPos1 > 0 && nPos2 > nPos1) 
			{
				sId = sWorkingUrl.Left(nPos1);
				sPassword = sWorkingUrl.Mid(nPos1 + 1,
											nPos2 - nPos1 - 1);
				sWorkingUrl = sWorkingUrl.Mid(nPos2 + 1);
			} // End if

			if (sId == "")
				sWorkingUrl = "ftp://" + sWorkingUrl;
			else
				sWorkingUrl = "ftp://"
							  + sId
							  + ":"
							  + sPassword
							  + "@"
							  + sWorkingUrl;

			if (sWorkingUrl.Right(1) != "/") sWorkingUrl += "/";

			sWorkingDir = sDir;
			if (sWorkingDir != "") 
			{
				if (sWorkingDir.Left(1) == "/")
					sWorkingDir = sWorkingDir.Mid(1);
				sWorkingUrl += sWorkingDir;
				if (sWorkingUrl.Right(1) != "/") sWorkingUrl += "/";
			} // End if

			sWorkingUrl += sRemoteFilespec;

			pMyFile = new CFile;
			if (pMyFile->Open(sLocalFilespec,
							  CFile::modeCreate
							      | CFile::modeReadWrite)) 
			{
				pFile = pSession->OpenURL (
									sWorkingUrl,
									1,
									INTERNET_FLAG_RELOAD
									    | INTERNET_FLAG_TRANSFER_BINARY,
									sHeader, // szHead
									-1L);
				if (pFile) /* OpenURL worked */
				{
					nResult = CROBOT_ERR_SUCCESS;
					// Get data
					do 
					{
					   nRead = pFile->Read(sz, 1023);
					   if (nRead != 0) 
					   {
							sz[nRead] = 0;
							pMyFile->Write (sz, nRead);
					   } // End if
					} while (nRead != 0);	// End do ... while
					nResult = CROBOT_ERR_SUCCESS;
				} // End if pFile
				else /* OpenURL failed */
				{
					nResult = CROBOT_ERR_CONNECTION_FAILED;
				} // End else
			} // End if
			else 
			{
				nResult = CROBOT_ERR_DISK_FILE_ERROR;
			} // End else
		} // End if
		else
			nResult = CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch (CInternetException *pEx) 
	{
		switch(pEx->m_dwError) 
		{
		case ERROR_INTERNET_TIMEOUT:
			nResult = CROBOT_ERR_TIMED_OUT;
			break;
		case ERROR_INTERNET_INVALID_URL:
			nResult = CROBOT_ERR_INVALID_URL;
			break;
		case ERROR_INTERNET_EXTENDED_ERROR:
			// Invalid or non-existing filename
			nResult = CROBOT_ERR_NOT_FOUND;
			break;
		case ERROR_INTERNET_INCORRECT_USER_NAME:
		case ERROR_INTERNET_INCORRECT_PASSWORD:
		case ERROR_INTERNET_LOGIN_FAILURE:
			nResult = CROBOT_ERR_NOT_AUTHORIZED;
			break;
		default:
			nResult = CROBOT_ERR_CONNECTION_FAILED;
			break;
		} // End switch
		pEx->Delete();
	} // End catch

	catch (CFileException *pEx) 
	{
		int nErr = pEx->m_cause;
		pEx->Delete();
		nResult = CROBOT_ERR_FILE+nErr;
	} // End catch

	catch (...) 
	{
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pFile != NULL) 
	{
		pFile->Close();
		delete pFile; 
	} // End if
	
	if (pMyFile != NULL) 
	{ 
		pMyFile->Close();
		delete pMyFile; 
	} // End if
	
	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession; 
	} // End if
	
	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
}

// --------------------------------------------------------------
// ****************
// *              *
// *  PutFtpFile  *
// *              *
// ****************
//
// Outputs:	<functionresult> - 0 for success, or error code

BOOL CRobotInternet::ftpPutFile(const CString& sUrl,
								const CString& sDir,
								const CString& sLocalFilespec,
								const CString& sRemoteFilespec,
								int& nResult, CString& sErrMsg)
{
	#define	FTP_UNKNOWN	 0
	#define	FTP_BLANK	 1
	#define	FTP_OPEN	 2
	#define	FTP_CLOSE	 3
	#define	FTP_CD		 4
	#define	FTP_LCD		 5
	#define	FTP_GET		 6
	#define	FTP_PUT		 7
	#define	FTP_BYE		 8
	#define	FTP_ASCII	 9
	#define	FTP_BINARY	10

	CInternetSession* pSession = NULL;
	CFtpConnection* pFTPConnection = NULL;
	CString LocalFile, RemoteFile;
	CString sWorkingUrl;
	CString sId, sPassword;
	CString sTemp;
	nResult = CROBOT_ERR_SUCCESS;
	CString sMsg;

	try 
	{
		sWorkingUrl = sUrl;
		sWorkingUrl.TrimLeft();
		sWorkingUrl.TrimRight();

		// Check for invalid parameters

		if (!(sWorkingUrl.IsEmpty())
			&& !(sLocalFilespec.IsEmpty())
			&& !(sRemoteFilespec.IsEmpty())) 
		{

			// See if the file to send exists and is available
			
			CFileStatus fs;

			if (CFile::GetStatus(sLocalFilespec, fs)) 
			{

				/* If user:password@ specified in URL, extract to 
				   sID and sPassword and shorten sWorkingUrl */

				sId = m_sLogonUsername;
				sPassword = m_sLogonPassword;
				
				sTemp = sWorkingUrl.Left(4);
				sTemp.MakeLower();
				if (sTemp == "ftp:")
					sWorkingUrl = sWorkingUrl.Mid(4);

				if (sWorkingUrl.Left(2) == "//")
					sWorkingUrl = sWorkingUrl.Mid(2);

				int nPos1 = sWorkingUrl.Find(":");
				int nPos2 = sWorkingUrl.Find("@");
				if (nPos1 > 0 && nPos2 > nPos1) 
				{
					sId = sWorkingUrl.Left(nPos1);
					sPassword = sWorkingUrl.Mid(nPos1 + 1,
												nPos2 - nPos1 - 1);
					sWorkingUrl = sWorkingUrl.Mid(nPos2 + 1);
				} // End if
				
				// Establish Internet connection

				pSession = new CInternetSession(
										m_sUserAgent,
										1,
										INTERNET_OPEN_TYPE_PRECONFIG);

				if (pSession) 
				{
					
					// Session established. Now open connection.

					pFTPConnection = pSession->GetFtpConnection(
														sWorkingUrl,
														sId,
														sPassword);
					
					if (pFTPConnection) 
					{
						
						/* Established FTP connection.
						   Set the directory (unless blank sDir
						   passed to function). */

						if (sDir != "" 
							&& !pFTPConnection->SetCurrentDirectory(sDir))
							// Set directory failed
							nResult = CROBOT_ERR_CONNECTION_FAILED;
						else 
						{
							// Send the file
							if (!pFTPConnection->PutFile(
											sLocalFilespec,
											sRemoteFilespec,
											FTP_TRANSFER_TYPE_BINARY,
											1))
								// File transfer failed
								nResult = CROBOT_ERR_CONNECTION_FAILED;
							else
								nResult = CROBOT_ERR_SUCCESS;
						} // End else

					} // End if
					else
						// FTP connection failed
						nResult = CROBOT_ERR_CONNECTION_FAILED;
				} // End if
				else
					// Unable to create session
					nResult = CROBOT_ERR_CONNECTION_FAILED;
			} // End if
			else
				// File to send does not exist
				nResult = CROBOT_ERR_NOT_FOUND;
		} // End if
		else
			// Empty parameter
			nResult = CROBOT_ERR_INVALID_PARAMETER;
	} // End try

	catch(CInternetException* pEx) 
	{
		switch(pEx->m_dwError) 
		{
		case ERROR_INTERNET_TIMEOUT:
			nResult = CROBOT_ERR_TIMED_OUT;
			break;
		case ERROR_INTERNET_INVALID_URL:
			nResult = CROBOT_ERR_INVALID_URL;
			break;
		case ERROR_INTERNET_EXTENDED_ERROR:
			// Invalid or non-existing filename
			nResult = CROBOT_ERR_NOT_FOUND;
			break;
		case ERROR_INTERNET_INCORRECT_USER_NAME:
		case ERROR_INTERNET_INCORRECT_PASSWORD:
		case ERROR_INTERNET_LOGIN_FAILURE:
			nResult = CROBOT_ERR_NOT_AUTHORIZED;
			break;
		default:
			nResult = CROBOT_ERR_CONNECTION_FAILED;
			break;
		} // End switch
		pEx->Delete();
	} // End catch

	catch(...) 
	{
		nResult = CROBOT_ERR_CONNECTION_FAILED;
	} // End catch

// Clean up and exit function

	if (pFTPConnection != NULL) 
	{
		pFTPConnection->Close();
		delete pFTPConnection; 
	} // End if
	
	if (pSession != NULL) 
	{
		pSession->Close();
		delete pSession;
	} // End if

	sErrMsg = ErrorMessage(nResult);
	if (nResult == CROBOT_ERR_SUCCESS)
		return true;
	else
		return false;
};
