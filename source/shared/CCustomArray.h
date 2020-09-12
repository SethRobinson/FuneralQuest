//////////////////////////////////////////////////////////////////////////
// Kishore:-
// include this pice of code in .h file.
// function for putting values to VARIANT->SAFEARRAY
// function for getting values from VARIANT->SAFEARRAY
// 
// @1 Type of Array 
// @2 Actual type contained in the variant
// @3 Variant to read or write 
// @4 Actual array to read or write
// @5 VT_BSTR or VT_DATE ...  (FillVariant)
// Usage:- 
// BSTRArray someThing; someThing.Add(bstr);
// FillVariant<BSTRArray,BSTR>(&toVariant,someThing,VT_BSTR);
//
// _BSTR_TArray someThing;
// GetVariant<_BSTR_TArray,BSTR>(fromVariant,someThing);
// _bstr_t _bstr=someThing.GetSize(),GetAt(),[n]
//
// GetVariant<DATEArray,DATE>(fromVariant,dt);
// GetVariant<OLEDATETIMEArray,DATE>(fromVariant,dt);
// OleDateTime x=dt.GetAt(n) ...GetSize(),[],.....
////////////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

typedef CArray<BSTR,BSTR> BSTRArray;
typedef CArray<DATE,DATE> DATEArray;
typedef CArray<COleDateTime,COleDateTime> OLEDATETIMEArray;
typedef CArray<DWORD,DWORD> DWORDArray;

template <class T,class T1> 
void FillVariant(VARIANT* pVariant, T& arrySrc,int iType)
{
 ASSERT(NULL!=pVariant);
 VariantInit(pVariant);
 int iMax = arrySrc.GetSize();

 SAFEARRAY * pSafeArray; 
 SAFEARRAYBOUND aDim[1]; 
 aDim[0].lLbound = 0; 
 aDim[0].cElements = iMax;

 pVariant->vt = VT_ARRAY | iType;

 pSafeArray = SafeArrayCreate(iType, 1, aDim);

 T1* dwArray = NULL;
 SafeArrayAccessData(pSafeArray, (void**)&dwArray);

 for(int nCount = 0; nCount < iMax ; nCount++)
 {
  dwArray[nCount] = (T1)arrySrc[nCount];
 }
 SafeArrayUnaccessData(pSafeArray);
 pVariant->parray = pSafeArray;
} 

template <class T,class T1> 
void GetVariant(VARIANT variant, T& arrySrc)
{
 long lStartBound = 0;
 long lEndBound = 0;
 
 SAFEARRAY* pSafeArray  = variant.parray;
 ASSERT(NULL!=pSafeArray);
 SafeArrayGetLBound(pSafeArray, 1, &lStartBound);
 SafeArrayGetUBound(pSafeArray, 1, &lEndBound);

 T1* arrayAccess = NULL;
 SafeArrayAccessData(pSafeArray, (void**)&arrayAccess);
 
 for(int iIndex = lStartBound; iIndex <= lEndBound; iIndex ++)
 {
  arrySrc.Add((T1)arrayAccess[iIndex]);
 }
 SafeArrayDestroy(pSafeArray);
 SafeArrayUnaccessData(pSafeArray);
}
