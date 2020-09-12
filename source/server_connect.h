	  #pragma once

#include "stdafx.h"
#include "funeral_util.h" 
#include "misc_utils.h"

#include "rankings.h"
#include "FQDlg.h"
#include "wininet.h"
#include "CRobot.h"
#include "CRobotInternet.h"
#include "funeral_util.h"
#include "alert_socket.h"
void convert_and_send_to_server(CString *cst_new);


UINT get_server_dat(LPVOID pCrap);

extern alert_socket main_socket; //for communicating with the license server
UINT listener_init(LPVOID pCrap);

void force_reconnect();
UINT say_hello();
extern alert_socket main_socket;
