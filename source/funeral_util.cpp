#include "funeral_util.h"
#include "std_all.h"
//modified to be a global for speed, used in SethAnsi and SethAnsiStrip
char *g_p_final = NULL;
int g_i_buffer_save_size = 0; 




void add_button(CString *cst_final, int i_but, CString cst_option, int i_command, int i_sub_command)
{
   
	CString cst_key;
	CString cst_temp;
	if (i_but == 1) cst_key = "3";
	if (i_but == 2) cst_key = "2";
	if (i_but == 3) cst_key = "1";
	if (i_but == 4) cst_key = "0";
	if (i_but == 5) cst_key = "a";
 
	cst_temp.Format("st_option_%s",cst_key);
	add_var(cst_final, cst_temp, cbut(cst_option));
	cst_temp.Format("i_command_%s",cst_key);
	add_var(cst_final, cst_temp, i_command); //load map when we're done
	if (i_sub_command != C_SUB_NONE)
	{
	cst_temp.Format("i_command_sub_%s",cst_key);
 	add_var(cst_final, cst_temp, i_sub_command); //load map when we're done
	}

}



void load_strings(CString cst_fname, bool b_rating)
{
   	CString cst_filename = cst_fname;
	//read every line from this text file

   FILE *fp = open_file_cst(cst_filename);
   CString cst_line;
   
   while (get_line_from_file_cst(fp, &cst_line))
   {
     //read a line.
	
	   CString cst_num; //index
	   CString cst_data; //actual data
	   
	   separate_string_mfc(cst_line, 1, '|', &cst_num);
	   separate_string_mfc(cst_line, 2, '|', &cst_data);

	   int i_index = atol((LPCSTR)cst_num);

	   if (in_range(i_index, 1, 100))
	   {
		 //set data
	   	   if (b_rating)
			   strcpy(glo.a_st_rating[i_index-1], cst_data); else
			   strcpy(glo.a_st_karma[i_index-1], cst_data);
		   // SetItemText(i_index-1, 1, cst_data);
	   } else
	   {
//	   log_error("Can't add entry %s, not in range. (it said %s).",cst_num, cst_data);
	   }


   }
   fclose(fp);

  
}




bool get_random_yes_no(int i_luck, float f_weight)
{
 int i_rand = random(100);
i_luck += int((50-i_luck) * (1.0f - f_weight));
 if (i_rand > i_luck) return false;
 return true;
}


void luck_mod(int i_play, int i_value)
{
	//add/remove something from luck
	p_play[i_play].i_luck += i_value;
    force_range(&p_play[i_play].i_luck, 1, 100, "luck");
}

void exp_mod(int i_play, int i_value)
{
	//add/remove something from luck
	if (p_play[i_play].i_sp + i_value < 0)
	{
	  //special case for negetive stuff, we don't want to allow it to happen
		i_value = -p_play[i_play].i_sp;
	}
	p_play[i_play].i_sp += i_value;
	p_play[i_play].i_exp += i_value;

    force_range(&p_play[i_play].i_sp, 0, 2000000000, "sp");
    force_range(&p_play[i_play].i_exp, 0, 2000000000, "exp");
}


void exp_mod_force_total(int i_play, int i_value)
{
	//this version forces the total BP to go down, even if SP is 0.

	p_play[i_play].i_sp += i_value;
	p_play[i_play].i_exp += i_value;

    force_range(&p_play[i_play].i_sp, 0, 2000000000, "sp");
    force_range(&p_play[i_play].i_exp, 0, 2000000000, "exp");
}


void turns_mod(int i_play, int i_value)
{
	//add/remove something from luck
	p_play[i_play].i_turns += i_value;
    force_range(&p_play[i_play].i_turns, 0, 2000000000, "luck");
}
void log_msg_not_threadsafe(LPCSTR lpFormat, ...)
{
	
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s %s:%s\r\n",show_small_date(),show_time(),szBuf);
//	OutputDebugString(cst_final);
	add_text((char *)(LPCSTR)cst_final, "log.txt");

	//	g_dlg_main->Log(cst_final);
	
}
void LogMsg(const LPCSTR lpFormat, ...)
{
/*	
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);
//	OutputDebugString(cst_final);
	g_dlg_main->Log(cst_final);
	*/

		 va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);

	//Let's send it via a post to the window, safer this way.
	char *st_buffer;
  	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);

	st_buffer = (char *) calloc(4048,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, cst_final);
	if (g_dlg_main)
	{
	PostMessage(g_dlg_main->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
	}
    
#ifdef _DEBUG
    OutputDebugString(st_buffer);
    OutputDebugString("\r\n");
#endif
}

 void Msg(const LPCSTR lpFormat, ...)
{
/*	
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);
//	OutputDebugString(cst_final);
	g_dlg_main->Log(cst_final);
	*/

		 va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);

	//Let's send it via a post to the window, safer this way.
	char *st_buffer;
  	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);

	st_buffer = (char *) calloc(4048,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, cst_final);
	if (g_dlg_main)
	{
	PostMessage(g_dlg_main->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
	}
    
#ifdef _DEBUG
    OutputDebugString(st_buffer);
#endif
}

 void log_msg_thread_safe(LPCSTR lpFormat, ...)
{

	
	 va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);

	//Let's send it via a post to the window, safer this way.
	char *st_buffer;
  	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);

	st_buffer = (char *) calloc(4048,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, cst_final);
	PostMessage(g_dlg_main->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
	
}


void log_error(const LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	char *st_buffer;
  	cst_final.Format("WARNING: %s %s:%s\r\n",show_small_date(),show_time(),szBuf);
//	OutputDebugString(cst_final);
	//g_dlg_main->Log(cst_final);
	add_text((char*)(LPCSTR)cst_final,"errorlog.txt");

    st_buffer = (char *) calloc(4048,1); //allocate memory on the heap
	if (st_buffer)
    {
    
    //don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, cst_final);
	PostMessage(g_dlg_main->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);
    }

}


void LogError(const char * lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	char *st_buffer;
  	cst_final.Format("WARNING: %s %s:%s\r\n",show_small_date(),show_time(),szBuf);
//	OutputDebugString(cst_final);
	//g_dlg_main->Log(cst_final);
	add_text((char*)(LPCSTR)cst_final,"errorlog.txt");

    st_buffer = (char *) calloc(4048,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, cst_final);
	PostMessage(g_dlg_main->m_hWnd,WM_ADD_TO_LOG, 0, (LONG) st_buffer);

}


void add_var(CString *cst_buff, CString cst_var_name, int i_value)
{
  
  *cst_buff += cst_var_name;
  *cst_buff += "=";
  CString cst_temp;
  cst_temp.Format("%d",i_value);
  *cst_buff += cst_temp;
  *cst_buff += "&";

}

void add_var(CString *cst_buff, CString cst_var_name, CString cst_var_value)
{
  *cst_buff += cst_var_name;
  *cst_buff += "=";
  *cst_buff += cst_var_value;
  *cst_buff += "&";

}

void seth_ansi(CString *pcst)
{
  //Expand old school seth-ansi codes into html code.  Much faster than a lot of .Replaces()
 
	const int c_required_leeway = 100;
	const int c_buffer_increment = 1024; 
	CString cst_final;	
  int i_num = 0;
  TCHAR ch_temp;
  static  char ch_add[200];

  int i_length = pcst->GetLength();
  char *p_st_temp = NULL;
//  p_st_temp = (char *) malloc(i_length+1);
    p_st_temp = pcst->GetBuffer(100); 

  // strcpy(p_st_temp, *pcst);

  int i_final_size = i_length + c_buffer_increment; //initial buffer size, we'll make it bigger if we need to
  int i_cur_size = 0;
  //I do this weird stuff because memory allocation is so slow we DON'T want to be doing it
  //on every add.
  if (g_i_buffer_save_size < i_final_size)
  {
  g_p_final = (char *) realloc(g_p_final, i_final_size);
  }
  g_p_final[0] = 0; //zero it out, this is fine for a string

  while(i_num < i_length)
  {

	  ch_temp = p_st_temp[i_num];
  	  ch_add[0] = ch_temp;
	  ch_add[1] = 0;
   if (ch_temp == '`')
   {
       //sethansi code.  Let's rock.
       if (i_num+1 < i_length)
       {
           //read next char
           ch_temp =p_st_temp[i_num+1];
           //add color codes as needed
           
           if (ch_temp == '=')
               strcpy(ch_add, "%3D");
           
           if (ch_temp == '1')
               strcpy(ch_add, "<font color=\"#0000A8\">");
           
           
           if (ch_temp == '2')
               strcpy(ch_add,  "<font color=\"#007000\">");
           
           
           if (ch_temp == '3')
               strcpy(ch_add,  "<font color=\"#008080\">");
           
           
           if (ch_temp == '4')
               //  strcpy(ch_add, "<font color=\"#A80000\">");
               
               strcpy(ch_add, "<font color=\"#FF0000\">");
           
           if (ch_temp == '5')
               // strcpy(ch_add, "<font color=\"#A800A8\">");
               strcpy(ch_add, "<font color=\"#CC66CC\">");
           
           
           
           if (ch_temp == '6')
               strcpy(ch_add, "<font color=\"#A85400\">");
           
           
           if (ch_temp == '7')
               strcpy(ch_add, "<font color=\"#808080\">");
           
           
           if (ch_temp == '8')
               strcpy(ch_add,  "<font color=\"#505050\">");
           
           
           if (ch_temp == '9')
               strcpy(ch_add, "<font color=\"#5454FC\">");
           
           
           if (ch_temp == '0')
               strcpy(ch_add, "<font color=\"#00B000\">");
           
           
           if (ch_temp == '!')
               strcpy(ch_add, "<font color=\"#00A0A0\">");
           
           
           if (ch_temp == '@')
               strcpy(ch_add,  "<font color=\"#FC5454\">");
           
           
           if (ch_temp == '#')
               strcpy(ch_add,  "<font color=\"#FC54FC\">");
           
           
           if ( (ch_temp == '$') || (ch_temp == 'y') )
               strcpy(ch_add, "<font color=\"#FFFF33\">");
           
           
           if (ch_temp == 'w')
               strcpy(ch_add, "<font color=\"#FFFFFF\">");
           
           
           if (ch_temp == '^')
               strcpy(ch_add,  "<font color=\"#000000\">");
           
           //FONT changes
           
           if (ch_temp == 'a')
           {
               //  strcpy(ch_add, "<font face=\"Arial\">");
           }
           
           //Font size changes
           if (ch_temp == 'f')
           {
               if (i_num+2 < i_length)
               {
                   //read next char
                   ch_temp =p_st_temp[i_num+2];
                   char crap[30];
                   sprintf(crap, "%c",ch_temp);
                   int i_font_size = 8 + (atol(crap) *2);
		                 sprintf(crap, "<font size=\"%d\">", i_font_size);
                         strcpy(ch_add, crap);
                         i_num++;
                         
               }
         
           }
           
           i_num++;           
       }
   }
	   i_cur_size += strlen(ch_add);
	   if (i_cur_size+c_required_leeway > i_final_size)
	   {
		 //make buffer bigger
		   i_final_size+=c_buffer_increment;
		   g_p_final = (char *) realloc(g_p_final, i_final_size); 
		   g_i_buffer_save_size = i_final_size;
	   }
	   strcat(g_p_final, ch_add);
   i_num++;
  }
	  
 // free(p_st_temp);
  pcst->ReleaseBuffer(i_length); 
 //All done
  pcst->Format("%s", g_p_final);
  //LogMsg("Created  from %s.", p_final);
 // free(p_final);
  
}



void StripSethAnsi(CString *pcst)
{
  //Remove old school seth-ansi codes  Much faster than a lot of .Replaces()
 
	const int c_required_leeway = 100;
	const int c_buffer_increment = 1024; 
	CString cst_final;	
  int i_num = 0;
  TCHAR ch_temp;
  static  char ch_add[200];

  int i_length = pcst->GetLength();
  char *p_st_temp = NULL;
//  p_st_temp = (char *) malloc(i_length+1);
    p_st_temp = pcst->GetBuffer(100); 

  // strcpy(p_st_temp, *pcst);

  int i_final_size = i_length + c_buffer_increment; //initial buffer size, we'll make it bigger if we need to
  int i_cur_size = 0;
  //I do this weird stuff because memory allocation is so slow we DON'T want to be doing it
  //on every add.
  if (g_i_buffer_save_size < i_final_size)
  {
  g_p_final = (char *) realloc(g_p_final, i_final_size);
  }
  g_p_final[0] = 0; //zero it out, this is fine for a string

  while(i_num < i_length)
  {

	  ch_temp = p_st_temp[i_num];
  	  ch_add[0] = ch_temp;
	  ch_add[1] = 0;
      if (ch_temp == '`')
      {
          //sethansi code.  Let's rock.
          if (i_num+1 < i_length)
          {
              //read next char
              ch_temp =p_st_temp[i_num+1];
              //add color codes as needed
              
              //Font size changes
              if (ch_temp == 'f')
              {
                  if (i_num+2 < i_length)
                  {
                      //read next char
                      i_num++;
                      
                  }
              }
              
              i_num++;           
          }
          ch_add[0] = 0; //clear it, add nothing.
      }
	   i_cur_size += strlen(ch_add);
	   if (i_cur_size+c_required_leeway > i_final_size)
	   {
		 //make buffer bigger
		   i_final_size+=c_buffer_increment;
		   g_p_final = (char *) realloc(g_p_final, i_final_size); 
		   g_i_buffer_save_size = i_final_size;
	   }
	   strcat(g_p_final, ch_add);
   i_num++;
  }
	  
 // free(p_st_temp);
  pcst->ReleaseBuffer(i_length); 
 //All done
  pcst->Format("%s", g_p_final);

  
}



void free_seth_buffer()
{
 SAFE_FREE(g_p_final);
 g_i_buffer_save_size = 0;
}







void send_fail(LPREQUEST lpReq)
{
   	CString cst_final;
	add_var(&cst_final, "i_sent","-1"); //this tells the flash client we had a problem
	send_msg(lpReq, (LPCSTR)cst_final);
}

bool get_token_st(LPREQUEST lpReq, char name[255], CString * cst_var)
{
  for (int i=0; i < d_max_tokens; i++)
  {
    if (compare(lpReq->token[i].data, name))
	{
	  cst_var->Format("%s",	lpReq->token_amount[i].data);
	  cst_var->TrimLeft();
	  cst_var->TrimRight();

	  return true;
	}
  }
return false;
}

bool get_token_int(LPREQUEST lpReq, char name[255], int * cst_var)
{
  for (int i=0; i < d_max_tokens; i++)
  {
    if (compare(lpReq->token[i].data, name))
	{
	  	*cst_var = atoi(lpReq->token_amount[i].data);
	  return true;
	}
  }
return false;
}

bool tokenize_input(LPREQUEST lpReq, CString cst_request)
{ 
	//first clear the old info
  for (int i=0; i < d_max_tokens; i++)
  {
    lpReq->token[i].data[0] = 0;
	lpReq->token_amount[i].data[0] = 0;
  }
 
  //break down the URL sent to us

  char seps[]   = "&";
  char *pch_token;
  static char st_request[5000]; //we need this to do strtok operations. I'll dynamically allocate this later..
 // LogMsg("Tokenizing %s.", (LPCSTR) cst_request);
  sprintf(st_request, "%s", (LPCSTR)cst_request);
  pch_token = strtok( st_request, seps);
  
  int i_cur = 0;
 
  char *pst;
  while (pch_token != NULL)
  {
   	  pst = strchr(pch_token, '=');
	  if (pst == 0) 
	  {
	   //no seperator here, invalid format.
		  return false;

	  }
	  pst++; //move past the = sign
	  if (pst != NULL)
	  {
	      //There is a var/value match here.
		  if (strlen(pst) < d_max_token_size -1)
		{
		  strcpy(lpReq->token_amount[i_cur].data, pst);
		  } else
		  {
		    //too big to chew
		    log_msg_thread_safe("Can't decipher %s, too long.", pst);
		    return false;
		  }
	  
	  	pch_token[strlen(pch_token) - strlen(pst)-1] = 0; //truncate answer
		if (strlen(pch_token) < d_max_token_size -1)
		{
	  	  strcpy(lpReq->token[i_cur].data, pch_token); 
	
		} else
		{
		    //too big to chew
   	       log_msg_thread_safe("Can't decipher %s, too long.", pst);
	        return false;
		}
	  }
#ifdef _DEBUG 
	  if (sys.b_extra_debug)
	  {
	  		  
///			  log_msg_thread_safe("%s sent: %s = %s.",my_ip(lpReq), lpReq->token[i_cur].data,
	//		 lpReq->token_amount[i_cur].data);
		
	  }
#endif
	  //Let's change the web conversions back to normal
	  format_for_local(lpReq->token[i_cur].data);
	  format_for_local(lpReq->token_amount[i_cur].data);
	  //strip spaces and newlines
	
	
//	  if (lpReq->token_amount[i_cur].data[0] == 0) return false;
	  /* Get next token: */
      pch_token = strtok( NULL, seps );
   	  i_cur++;
  }
  return true;
}



char * my_ip(LPREQUEST lpReq)
{
  static char st_ip[25];
  SOCKADDR_IN * sock;
  sock = (SOCKADDR_IN*) lpReq->lpSockAddr;
  if (sock == NULL) return "RTSOFT";
  sprintf(st_ip, "%s",inet_ntoa(sock->sin_addr));
  return (char *)&st_ip;
}
