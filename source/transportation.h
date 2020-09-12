#pragma once

#include "stdafx.h"
#include "funeral_util.h"

#include "CCustomArray.h"
#include "CTextParse.h"



class CTransports
{
public:
	CString cst_name;
	CString cst_description;
	int i_price;
    int i_bonus_turns;
	CString cst_url;
};

typedef CArray<CTransports,CTransports> TransportArray;

extern TransportArray a_transport;
void load_transport_from_file();
CString get_transportation(int i_play);
