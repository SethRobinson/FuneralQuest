//list of optional things people can buy
#pragma once

#include "stdafx.h"
#include "CCustomArray.h"
#include "CTextParse.h"

class COptionItem
{
public:
	CString cst_name;
	CString cst_a;
	int i_price;
};

class CRisks
{
public:
	CString cst_start_message;
	CString cst_start_url;
	CString cst_start_bg;
	CString cst_accept;
	CString cst_risk;

	CString cst_accept_message;
	CString cst_accept_url;
	CString cst_accept_bg;


	CString cst_fail_message;
	CString cst_fail_url;
	CString cst_fail_bg;

	CString cst_success_message;
	CString cst_success_url;
	CString cst_success_bg;


};


typedef CArray<COptionItem,COptionItem> OptionArray;
typedef CArray<CRisks,CRisks> RisksArray;

extern OptionArray a_option;
extern OptionArray a_upgrade;
extern RisksArray a_risk;

void init_options();
void load_risks_from_file();
