#pragma once

#include <afxcoll.h>
#include <atlstr.h>
#define CStringEx CString


//Replacements for things that were in StringEx that (even modern) CString is missing
int ReverseFindString(const CString& s1, const CString& s2);
int FindNoCase(CString s1, CString text);
int ReplaceNoCase(CString& str, LPCTSTR pszOld, LPCTSTR pszNew);
CString GetField(const CString* s1, char separator, int index);
CString GetDelimitedField(CString s1, LPCTSTR delimStart, LPCTSTR delimEnd, int fieldnum = 0);