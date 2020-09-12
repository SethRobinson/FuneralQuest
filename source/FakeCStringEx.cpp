#include "FakeCStringEx.h"

//https://www.codeproject.com/Articles/260/Case-Insensitive-String-Replace?msg=5580408#xx5580408xx



int ReplaceNoCase(CString& str, LPCTSTR pszOld, LPCTSTR pszNew)
{
	CString	sDest(str);
	CString	sOld(pszOld);
	sDest.MakeLower();	// convert destination and old string to lower case
	sOld.MakeLower();
	// first pass: find instances of old string and store their positions
	int	nOldLen = sOld.GetLength();
	CDWordArray	arrSub;
	int	iPos = 0;
	while ((iPos = sDest.Find(sOld, iPos)) >= 0) {	// while old string is found
		arrSub.Add(iPos);	// add found string's position to substitution array
		iPos += nOldLen;	// advance position past found string
	}
	// second pass: do the substitions, in reverse order for stability
	int	nSubs = static_cast<int>(arrSub.GetSize());	// string indices are 32-bit
	for (int iSub = nSubs - 1; iSub >= 0; iSub--) {	// for each substitution
		str.Delete(arrSub[iSub], nOldLen);	// delete old string
		str.Insert(arrSub[iSub], pszNew);	// insert new string
	}
	return nSubs;	// return number of substitutions
}

int FindNoCase(CString s1, CString text)
{
	s1.MakeLower();
	text.MakeLower();
	return s1.Find(text);
}

int ReverseFindString(const CString& s1, const CString& s2)
{
	int start = s1.Find(s2, 0);

	if (start >= 0)
	{
		while (start < s1.GetLength())
		{
			int idx = s1.Find(s2, start + 1);
			if (idx >= 0)
				start = idx;
			else
				break;
		}
	}

	return start;
}


CString GetField(const CString * s1, char separator, int index)
{
	int iStart = 0;

	int curIndex = 0;
	CString token;

	while(iStart < s1->GetLength()-1)
	{
		token = s1->Tokenize(CString(separator), iStart);
		if (iStart == -1) return ""; // fail
		if (curIndex == index) return token;
		curIndex++;
	}

	return "";
}


CString GetDelimitedField(CString s1, LPCTSTR delimStart, LPCTSTR delimEnd, int fieldnum /*= 0*/)
{
	LPTSTR lpsz, lpszEnd, lpszRet;
	 LPTSTR lpszRemainder = s1.GetBuffer();
	int lenDelimStart = lstrlen(delimStart), lenDelimEnd = lstrlen(delimEnd);

	while (fieldnum-- >= 0)
	{
		lpsz = _tcsstr(lpszRemainder, delimStart);
		if (lpsz)
		{
			// We did find the Start delim
			lpszRet = lpszRemainder = lpsz + lenDelimStart;
			lpszEnd = _tcsstr(lpszRemainder, delimEnd);
			if (lpszEnd == NULL) return"";
			lpszRemainder = lpsz + lenDelimEnd;
		}
		else return "";
	}
	return s1.Mid(lpszRet - s1.GetBuffer(), lpszEnd - lpszRet);
}


