//
// REQLIST.C -- Linked list of active requests
//

//#include "httpa.h"

////////////////////////////////////////////////////////////

//
// Head of linked list
// Internal linkage
//
void log_msg(const LPCSTR lpFormat, ...);

static LPREQUEST lpHead = NULL;

//
// Internal function
//
static void FreeRequest(LPREQUEST lpReq);

////////////////////////////////////////////////////////////


//
// AddRequest()
//
// Allocates memory for a new REQUEST structure,
// adds it to the linked list,
// assigns the SOCKET to the REQUEST,
// fills in default field values,
// and returns a pointer to the new REQUEST.
//
LPREQUEST	AddRequest(SOCKET Socket, 
					   LPSOCKADDR lpSockAddr,
					   int nAddrLen)
{
	LPREQUEST lpReq = NULL;
	LPREQUEST lpThis = NULL;

	//
	// Allocate memory from the heap for the new 
	// REQUEST structure
	//

	lpThis = (LPREQUEST)malloc(sizeof(REQUEST));

	if (lpThis == NULL)
		return(NULL);

	//
	// Allocate memory for a copy of the
	// SOCKADDR
	lpThis->lpSockAddr = (LPSOCKADDR)malloc(nAddrLen);
	if (lpThis->lpSockAddr == NULL)
	{
		free(lpThis);
		lpThis = NULL;
		return(NULL);
	}

	// If the list is empty
	if (lpHead == NULL) 
	{
		// Put this one at the head
		lpHead = lpThis;
    }
	else 
	{
		// Walk to the end of the list
		lpReq = lpHead;
		while(lpReq->lpNext)
			lpReq = lpReq->lpNext;

		// And append this one to the end
		lpReq->lpNext = lpThis;
    }
  
	//
	// Fill in the REQUEST structure
	//
	lpThis->dwConnectTime = GetTickCount();
    lpThis->Socket = Socket;
	lpThis->hFile = HFILE_ERROR;
	lpThis->lpNext = NULL;
	lpThis->fFullResponse = FALSE;
	lpThis->dwRecv = 0;
	lpThis->dwSend = 0;
	lpThis->i_on_index = -1; //disable
	memcpy(lpThis->lpSockAddr,
		   lpSockAddr, 
		   nAddrLen);
	// And return a pointer to it    
	return (lpThis);
}

////////////////////////////////////////////////////////////

//
// GetRequest()
//
// Returns the REQUEST that Socket belongs to
//
LPREQUEST GetRequest(SOCKET Socket) 
{
	LPREQUEST lpReq;
  
	//
	// Walk through the list looking
	// for this socket
	//
	lpReq = lpHead;
	while(lpReq != NULL)
	{
		if (lpReq->Socket == Socket)
			break;
		lpReq = lpReq->lpNext;
	}

	return(lpReq);
}

////////////////////////////////////////////////////////////

//
// DelRequest()
//
// Delete the REQUEST from the list
// and free associated memory
//
void DelRequest(LPREQUEST lpThis) 
{
	LPREQUEST lpReq;
	BOOL bRet = FALSE;

	//	
	// Search for "this"
	//

	// If This is the first member
	if (lpThis == lpHead) 
	{
		// This->Next becomes the head
		lpHead = lpThis->lpNext;
	}
	else 
	{  
		// Search the list
		for (lpReq = lpHead; lpReq; lpReq = lpReq->lpNext) 
		{
			// If This is next...
			if (lpReq->lpNext == lpThis)
			{
				// Set the pointer to skip "this"
				lpReq->lpNext = lpThis->lpNext;
				break;
			}
		}
	}

//	OutputDebugString("Freeing memory.");
	// free the memory
	FreeRequest(lpThis);
}

////////////////////////////////////////////////////////////

//
// GetFirstRequest()
//
// Return the head of the list
// even if it is NULL
//
LPREQUEST	GetFirstRequest(void)
{
	return(lpHead);
}

////////////////////////////////////////////////////////////

//
// GetNextRequest()
//
// Return the next REQUEST after This
//
LPREQUEST	GetNextRequest(LPREQUEST lpThis)
{
	return(lpThis->lpNext);
}

////////////////////////////////////////////////////////////

//
// DelAllRequests()
//
// Delete all of the REQUESTs in the list
// and free all memory
//
void DelAllRequests(void)
{
	LPREQUEST lpReq;
	LPREQUEST lpNext;

	//
	// Walk through the list
	// freeing memory
	//
	for (lpReq = lpHead; lpReq; ) 
	{
		lpNext = lpReq->lpNext;
		FreeRequest(lpReq);
		lpReq = lpNext;
	}
	lpHead = NULL;
}

////////////////////////////////////////////////////////////

static void FreeRequest(LPREQUEST lpReq)
{
	free(lpReq->lpSockAddr);
	lpReq->lpSockAddr = NULL;
	free(lpReq);
	lpReq = NULL;
}
