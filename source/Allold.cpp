
#include "stdafx.h"
//#include "windows.h"
#include <stdlib.h>
#include <stdarg.h>

#include "stdio.h"
#include <io.h>
#include <fcntl.h>
#include "string.h"
#include <process.h>
#include <direct.h>
#include "randbota.h"	 
#include "all.h"
#include "time.h"

TRanrotAGenerator class_random(time(NULL));


/* 

   compare - Compares two strings, not case sensitive.
   Author:  Seth A. Robinson ('97)

*/


	
bool compare(char *orig, char *comp)
{
	
	unsigned int len;
	
	//strcpy(comp, _strupr(comp));
	//strcpy(orig, _strupr(orig));
	
	len = strlen(comp);
	if (strlen(orig) != len) return(false);
	
	
	if (strnicmp(orig,comp,len) == 0)
	{
		return(true);
	}
	
	//Msg("I'm sorry, but %s does not equal %s.",orig, comp);
	
	return(false);
	
}


/* 

   strip_beginning_spaces - Removes whitespaces from the beginning of
   a string.
   Author:  Seth A. Robinson ('97)

*/


void strip_beginning_spaces(char *s)
{
	
	
	char * h;
	h = s;
	
	
	if (s[0] != 32) 
	{
		return;
	}
	
	while(h[0] == 32)
	{
		h = &h[1];
	}
	
	
	strcpy(s, h);
	
}




/************ THE READING .INI KIT *************** */


//open a file in read mode

FILE * open_file(char file[255])
{

	return(fopen( file, "r" ));

}

//get next line from a stream (opened with above)

bool get_line_from_file(FILE * fp, char * dest)

{
if( fgets( dest, 255, fp ) == NULL) return(false); else
return(true); 
			

}

/************ END OF THE READING .INI KIT *************** */

/* does file exist? */
bool exist(char name[255])
{
	FILE *fp;
	fp = fopen(name, "rb");
	if (!fp)
	{
		//	  fclose(fp);
		return(false);
	}
	
	fclose(fp);
	return(true);
}


//this one gets one line by number from a text file.  It puts the null
//terminated string into into *dest.

bool get_line_from_file(char file[255], int line_to_get,  char *dest)
{
int count = 0;
char line[500];
	
	FILE *stream;  

  if (!exist(file)) return false;

	if( (stream = fopen( file, "r" )) != NULL )   
	{
		while(1)
		{
			if( fgets( line, 255, stream ) == NULL) 
				goto done;
			else    
			{
				count++;
	
				if (count == line_to_get)
				{
                //this is it boys, let's remove the carriage return
				   line[strlen(line)-1] = 0;
					strcpy(dest, line);
					goto done;
				}
				//figure_out(line);
			}
		}
		
done:
		fclose( stream );  
	} else
	{
		return false;
	}
return(true);


}


// get number of lines in a text file
//0 indicates file not found or error

int get_num_lines_from_file( char file[255])
{
int count = 0;
char line[500];
	
	FILE *stream;  

  if (!exist(file)) return(0);


	if( (stream = fopen( file, "r" )) != NULL )   
	{
		while(1)
		{
			if( fgets( line, 255, stream ) == NULL) 
				goto done;
			else    
			{
				count++;
				//figure_out(line);
			}
		}
		
done:
		fclose( stream );  
	} else
	{
		return(0);
	}
return(count);
}


/* Add text adds a line of text to a text file.  It creates it if it doesn't
   exist. */

void add_text(char *tex ,char *filename)
{
	FILE *          fp;
	if (strlen(tex) < 1) return;
	//Msg(tex);
	if (exist(filename) == false)
	{
		
		fp = fopen(filename, "wb");
		fwrite( tex, strlen(tex), 1, fp);       /* current player */
		fclose(fp);
		return;
	} else
	{
		fp = fopen(filename, "ab");
		fwrite( tex, strlen(tex), 1, fp);       /* current player */
		fclose(fp);
	}
}



int getx( void )
{
return(		GetSystemMetrics(SM_CXSCREEN));		
}

int gety( void )
{
return(		GetSystemMetrics(SM_CYSCREEN));
		
}




/* 

   Msg - Generic way to send a string to the debugger.

*/


void show( HWND g_hWnd, char title[100], LPSTR fmt, ... )
{
    char    buff[350];
    va_list  va;
    va_start(va, fmt);
    //
    // format message with header and show via pop up window
    //
    lstrcpy( buff, "Msg:" );
    wvsprintf( &buff[lstrlen(buff)], fmt, va );
    lstrcat( buff, "\r\n" );
    MessageBox(g_hWnd, buff, title, MB_ICONINFORMATION | MB_SETFOREGROUND);
} /* Msg */


void Msg( LPSTR fmt, ... )
{
    char    buff[350];
    va_list  va;
    va_start(va, fmt);
    //
    // format message with header
    //
    lstrcpy( buff, "Msg:" );
    wvsprintf( &buff[lstrlen(buff)], fmt, va );
    lstrcat( buff, "\r\n" );
	OutputDebugString( buff );
	
} /* Msg */


/* 

   replace - Changes all instances of this to that in line.    
   Author:  Seth A. Robinson ('97)

*/




void replace(const char *this1, char that[255], char *line)
{
	
	char hold[500];
	char thisup[200],lineup[500];
	unsigned int u,i;
	unsigned int checker;
start:
	strcpy(hold,"");
	strcpy(lineup,line);
	strcpy(thisup,this1);
	
	strupr(lineup);
	strupr(thisup);
	if (strstr(lineup,thisup) == NULL) return;
	checker = -1;
	strcpy(hold,"");
	for (u = 0; u < strlen(line); u++)
	{
		if (checker > -1)
		{
			if (toupper(line[u]) == toupper(this1[checker]))
			{
				if (checker+1 == strlen(this1))
				{
doit:
				u = u - strlen(this1);
				u++;
				for (i = 0; i < u; i++) hold[i] = line[i];
				for (i = 0; i < strlen(that); i++) hold[(u)+i]=that[i];
				hold[strlen(that)+u] = 0;
				for (i = 0; i < (strlen(line)-u)-strlen(this1); i++)
				{
					hold[(u+strlen(that))+i] = line[(u+strlen(this1))+i];
				}
				hold[(strlen(line)-strlen(this1))+strlen(that)] = 0;
				strcpy(line,hold);
				goto start;
				}
				checker++;
			  } else { checker = -1;    }
		}
		if( checker == -1)
		{
			if (toupper(line[u]) == toupper(this1[0]))
			{
			//	if (line[u] < 97) that[0] = toupper(that[0]);
				checker = 1;
				if (strlen(this1) == 1) goto doit;
			}
		}
	}
}


/* 

   seperate_string - Give it a string, a seperator symbol and tell it
   which one to return.	  (1 for left most, 2 for after the first seperator and so forth)
   Author:  Seth A. Robinson ('97)

*/

bool seperate_string (char str[800], int num, char liney, char *return1) 
{
	int l;
	unsigned int k;
	
	l = 1;
	strcpy(return1 ,"");
	
	for (k = 0; k <= strlen(str); k++)
	{
		
		if (str[k] == liney)
		{
			l++;
			if (l == num+1)
				goto done;
			
			if (k < strlen(str)) strcpy(return1,"");
		}
		if (str[k] != liney)
			sprintf(return1, "%s%c",return1 ,str[k]);
	}
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	
	return(false);
	
done:
	
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	
	//Msg("Took %s and turned it to %s.",str, return1);
	return(true);
}



/* 

   Get dir - send it the command line and it returns the path 
   (use GetCommandLine())
   Author:  Seth A. Robinson ('97)

Use like this:

  char mydir[100];
	char command_line[100];
	strcpy(command_line, GetCommandLine());
	getdir(command_line, mydir);
	

*/
//get full path.  Send something 256 or bigger or may crash.

void all_get_full_path(char *path)
{
  	
	GetModuleFileName(NULL, path, 255);

   Msg("Getmodule filename says %s.",path);

}



char *show_date(void)
{

   SYSTEMTIME lp_system;
	GetLocalTime(&lp_system);
	
	
	
	static char st_date[255];
	int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
	        NULL, //flags
			 &lp_system, //system time
			 "dddd, MMMM dd yyyy",
			 st_date,
			 255);
	if (result == 0)
	{
	 Msg("Get date function failed.\n");
	 return NULL;
	}
	return st_date;
}


char *show_date_month_and_day(void)
{

   SYSTEMTIME lp_system;
	GetLocalTime(&lp_system);
	
	
	
	static char st_date[255];
	int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
	        NULL, //flags
			 &lp_system, //system time
			 "MMM d",
			 st_date,
			 255);
	if (result == 0)
	{
	 Msg("Get date function failed.\n");
	 return NULL;
	}
	return st_date;
}


char *show_time(void)
{

   SYSTEMTIME lp_system;
	GetLocalTime(&lp_system);
	static char st_date[255];
	int result = GetTimeFormat(LOCALE_USER_DEFAULT, //locale
	        NULL, //flags
			 &lp_system, //system time
			 "h':'mm' 'tt",
			 st_date,
			 255);
	if (result == 0)
	{
	 Msg("Get date function failed.\n");
	 return NULL;
	}

	return st_date;
}

void switch_to_current_dir()
{
 	char dir_temp[256], dir_final[256];
    getdir(dir_temp, dir_final);
	//switch to dir run.exe is in
	chdir(dir_final);
}

//Random number, starts at 0
int random(long i_max)
{
 return (class_random.get_real_random(i_max));
}


//Random number + a number, starts at 0
int random(long i_max, long i_min)
{
 return (class_random.get_real_random(i_max))+i_min;
}

//Random number between a defined range
int random_range(long i_small, long i_large)
{
	return class_random.get_real_random(i_large-i_small)+i_small;
}
bool in_range(int i_num, int i_low, int i_high)
{
  if (i_num > i_high) return false;
  if (i_num < i_low) return false;
  return true;
}

char *show_small_date(void)
{

   SYSTEMTIME lp_system;
	GetLocalTime(&lp_system);
	
	
	
	static char st_date[255];
	int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
	        NULL, //flags
			 &lp_system, //system time
			 "MM-dd",
			 st_date,
			 255);
	if (result == 0)
	{
	 Msg("Get date function failed.\n");
	 return NULL;
	}
	return st_date;
}



char *show_army_time(void)
{

   SYSTEMTIME lp_system;
	GetLocalTime(&lp_system);
	static char st_date[255];
	int result = GetTimeFormat(LOCALE_USER_DEFAULT, //locale
	        NULL, //flags
			 &lp_system, //system time
			 "HH':'mm'",
			 st_date,
			 255);
	if (result == 0)
	{
	 Msg("Get date function failed.\n");
	 return NULL;
	}

	return st_date;
}

