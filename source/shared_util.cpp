#include "shared_util.h"


void format_for_web_cst(CString *cst_temp)
{
// cst_temp->Replace("%%","%25"); //convert underline
  cst_temp->Replace(" ","+"); //first convert spaces
 // cst_temp.Replace("_","%5F"); //convert underline
  cst_temp->Replace("\"","%22"); //convert underline
  cst_temp->Replace("!","%21"); //convert underline
  cst_temp->Replace(",","%2C"); //convert underline
  //copy it back into the buffer
 // strcpy(st_buff, (LPCSTR)cst_temp);
    cst_temp->Replace(";","%3B"); //convert 
  cst_temp->Replace(":","%3A"); //convert 

}


void format_for_web(char *st_buff)
{
  CString cst_temp;
  cst_temp.Format("%s",st_buff);
//  cst_temp.Replace("%%","%25"); //convert underline
  cst_temp.Replace(" ","+"); //first convert spaces
 // cst_temp.Replace("_","%5F"); //convert underline
  cst_temp.Replace("\"","%22"); //
  cst_temp.Replace("!","%21"); //convert underline
  cst_temp.Replace(",","%2C"); //convert underline
  cst_temp.Replace(";","%3B"); //convert 
  cst_temp.Replace(":","%3A"); //convert 
  //copy it back into the buffer
  strcpy(st_buff, (LPCSTR)cst_temp);

}

void format_for_local(char *st_buff)
{
  CString cst_temp;
  cst_temp.Format("%s",st_buff);

  cst_temp.Replace("+"," "); //first convert spaces
  cst_temp.Replace("%5F","_"); //convert underline
  cst_temp.Replace("%40","@");
  cst_temp.Replace("%21","!");
  cst_temp.Replace("%22","\"");
  cst_temp.Replace("%23","#"); 
  cst_temp.Replace("%24","$"); 
  cst_temp.Replace("%26","");
  cst_temp.Replace("%2C",",");
  cst_temp.Replace("%2E",".");
  cst_temp.Replace("%5E",""); 
  cst_temp.Replace("%2A",""); 
  cst_temp.Replace("%28","("); 
  cst_temp.Replace("%29",")"); 
  cst_temp.Replace("%2B",""); 
  cst_temp.Replace("%2D","-"); 
  cst_temp.Replace("%60","`"); 
  cst_temp.Replace("%7E","~"); 
  cst_temp.Replace("%27","'"); 
  cst_temp.Replace("%3A",":"); 
  cst_temp.Replace("%3B",";"); 
  cst_temp.Replace("%3E",">"); 
  cst_temp.Replace("%3C",""); //should be < but we're stripping it..
  cst_temp.Replace("%2F","/");
  cst_temp.Replace("%3D","=");
  cst_temp.Replace("%3F","?");
  cst_temp.Replace("%7C","|");
  cst_temp.Replace("%7B","{");
  cst_temp.Replace("%7D","}");
  cst_temp.Replace("%5BD","[");
  cst_temp.Replace("%5BD","]");
  cst_temp.Replace("%0D","\r\n");
				   
  cst_temp.Replace("%25","%%");
  
  //copy it back into the buffer
  strcpy(st_buff, (LPCSTR)cst_temp);

}

void format_for_local_cst(CString *cst_temp)
{
 
  cst_temp->Replace("+"," "); //first convert spaces
  cst_temp->Replace("%5F","_"); //convert underline
  cst_temp->Replace("%40","@");
  cst_temp->Replace("%21","!");
  cst_temp->Replace("%22","\"");
  cst_temp->Replace("%23","#"); 
  cst_temp->Replace("%24","$"); 
  cst_temp->Replace("%26","");
  cst_temp->Replace("%2C",",");
  cst_temp->Replace("%2E",".");
  cst_temp->Replace("%5E",""); 
  cst_temp->Replace("%2A",""); 
  cst_temp->Replace("%28","("); 
  cst_temp->Replace("%29",")"); 
  cst_temp->Replace("%2B",""); 
  cst_temp->Replace("%2D","-"); 
  cst_temp->Replace("%60","`"); 
  cst_temp->Replace("%7E","~"); 
  cst_temp->Replace("%27","'"); 
  cst_temp->Replace("%3A",":"); 
  cst_temp->Replace("%3B",";"); 
  cst_temp->Replace("%3E",">"); 
  cst_temp->Replace("%3C",""); //should be < but we're stripping it..
  cst_temp->Replace("%2F","/");
  cst_temp->Replace("%3D","=");
  cst_temp->Replace("%3F","?");
  cst_temp->Replace("%7C","|");
  cst_temp->Replace("%7B","{");
  cst_temp->Replace("%7D","}");
  cst_temp->Replace("%5B","[");
  cst_temp->Replace("%5D","]");
  cst_temp->Replace("%0D","\r\n");
				   
  cst_temp->Replace("%25","%%");
  

}




void strip_dangerous_stuff(CString *cst_temp)
{
  //strip stuff we don't want the user to type, used on public name and funeral
  //name
  cst_temp->Remove('?');
  cst_temp->Remove('|');
  cst_temp->Remove('=');
  cst_temp->Remove('/');
  cst_temp->Remove('\\');
  cst_temp->Remove(';');
  cst_temp->Remove(':');
  cst_temp->Remove(',');
  cst_temp->Remove('{');
  cst_temp->Remove('}');
  cst_temp->Remove('@');
  cst_temp->Remove('+');
  cst_temp->Remove('`');
  cst_temp->Remove('<');
  cst_temp->Remove('>');
  cst_temp->Remove('%');
  cst_temp->Remove('$');
  CString cst_new;

  for (int i=0; i < cst_temp->GetLength(); i++)
  {
    if (cst_temp->GetAt(i) < 123)
	{
	  //valid char
		cst_new += cst_temp->GetAt(i);
	}
  }


  //copy it back
	  cst_temp->Format("%s", cst_new);

}


bool swear_fix(CString *cst_message)
{
  	bool b_replaced_something = false;
	
	char st_crap[500];
	strcpy(st_crap, *cst_message);
	if (replace("fucking", "fudging",st_crap)) b_replaced_something = true;
	if (replace("fuck", "fudge",st_crap)) b_replaced_something = true;
	if (replace("cunt", "kitty", st_crap)) b_replaced_something = true;
	if (replace("shit", "crap", st_crap)) b_replaced_something = true;
	if (replace("cock ", "ding-dong ", st_crap)) b_replaced_something = true;
	if (replace("penis", "wang", st_crap)) b_replaced_something = true;
	if (replace("vagina", "oriface", st_crap)) b_replaced_something = true;
	if (replace("cocksucker", "mean guy", st_crap)) b_replaced_something = true;
	if (replace("nigger", "guy", st_crap)) b_replaced_something = true;
	if (replace("phuck", "fudge", st_crap)) b_replaced_something = true;
	if (replace("f.u.c.k", "fudge", st_crap)) b_replaced_something = true;
	if (replace("f u c k", "fudge", st_crap)) b_replaced_something = true;
	if (replace("pissed", "cheesed", st_crap)) b_replaced_something = true;
	if (replace(" ass ", " butt ", st_crap)) b_replaced_something = true;
	if (replace("asshole", "twit", st_crap)) b_replaced_something = true;
	if (replace("pussy", "wimp", st_crap)) b_replaced_something = true;
	if (replace("twat", "loser", st_crap)) b_replaced_something = true;
	if (replace("bitch", "female dog", st_crap)) b_replaced_something = true;
	if (replace("faggot", "gay guy", st_crap)) b_replaced_something = true;
	if (replace("kike", "guy", st_crap)) b_replaced_something = true;
	if (replace("gook ", "stuff ", st_crap)) b_replaced_something = true;
	if (replace("dildo", "dude", st_crap)) b_replaced_something = true;
	if (replace("butthole", "bunghole", st_crap)) b_replaced_something = true;
	if (replace(" tits ", " thingies ", st_crap)) b_replaced_something = true;
	if (replace(" tit ", " thing ", st_crap)) b_replaced_something = true;



	//	if (replace("seth", "God", st_crap)) b_replaced_something = true;
//	LogMsg("String is now %s.",st_crap);
	
	cst_message->Format("%s",st_crap);
//if (b_replaced_something) LogMsg("Swear check worked!");
	  return b_replaced_something;
}

