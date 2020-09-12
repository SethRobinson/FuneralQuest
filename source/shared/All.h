/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//

//  This is a group of somewhat generic utilities that can be used with MSVC 6.  Can be compiled with
//  with or without MFC, ANSI or unicode.  Not all functions are unicode aware though.

//  Also, some functions use TCHAR and some use WCHAR, the reason is I needed some unicode functions
//  in non-unicode builds and vice-versa, for instance, Teenage Lawnmower is not built with _UNICODE but it reads
//  unicode scripts.

// It's a mix-mash of useful functions, mixed styles and mixed quality, most written by Seth, some
// snipped from various sources.  Some of these date back ten years and could use rewriting...

//After adding this to your project you may get errors about LogMsg and LogError not
//being defined.  You need to add this to your program yourself, this is so you can choose
//how to handle error messages.  (write to disk or show on the screen or whatever)

//you can define _NOLOGMSG if you really don't want to use them.


//You can cut and paste these into your program to get you going:
/*


void LogMsg(const char *lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	char stTemp[4048];
	sprintf(stTemp, "%s\r\n", szBuf);
    OutputDebugString(stTemp);
}

void LogError(const char *lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	char stTemp[4048];
	sprintf(stTemp, "Error: %s\r\n", szBuf);
	OutputDebugString(stTemp);
}
*/
#ifndef all_HEADER_INCLUDED // include guard
#define all_HEADER_INCLUDED  // include guard


#pragma warning (disable:4996) //SETH added for VC8
#pragma warning (disable:4786)
#include <windows.h>
#include <cstdio>
#include <cassert>
#include <Mmsystem.h>
#include <list>
#include <malloc.h>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

 /*
#ifdef _DEBUG
void DumpUnfreed();
void AddTrack(DWORD addr,  DWORD asize,  const char *fname, DWORD lnum);
void RemoveTrack(DWORD addr);

inline void * __cdecl operator new(unsigned int size,
                                   const char *file, int line)
{
    void *ptr = (void *)malloc(size);
    AddTrack((DWORD)ptr, size, file, line);
    return(ptr);
};

inline void __cdecl operator delete(void *p)
{
    RemoveTrack((DWORD)p);
    free(p);
};
#endif
 */


#include <tchar.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p)      { if(p) { free(p); (p)=NULL; } }
#endif

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)      { if(p) { delete (p); (p)=NULL; } }

#endif


#define GET_BYTE(l, bytenum)  (((l) >> (bytenum * 8)) & 0xFF)
#define RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }

void ConvertLastErrorToString(TCHAR * szDest, int nMaxStrLen);  //for windows errors that this works with
void LogLastError(); //sends the last error to LogError() (uses ConvertLastErrorToString)

BOOL LaunchControlPanelApplet(TCHAR * pApplet);
DWORD LaunchURL(LPTSTR addy);
TCHAR * stristr(const TCHAR text[], const TCHAR search[]);  //just like strstr but case insensitive
bool in_range_long(long i_num, long i_low, long i_high);
bool TrimTextFile(TCHAR *p_filename, int i_number_of_lines);
void LogError(const TCHAR * lpFormat, ...);
void LogMsg(const TCHAR * lpFormat, ...);
void log_error(const CHAR * lpFormat, ...);  //for toolfish, for non unicode
void log_msg(const CHAR * lpFormat, ...);  //for toolfish, for non unicode
char *float_to_money( double num, char *buf, int dec);
bool compare(char *orig, char *comp);
void switch_to_current_dir();
void getdir(TCHAR t_final_out[]);
TCHAR * GetExeDir();
bool exist(const char * name);
bool exist(const TCHAR * name); //unicode version
bool in_range(int i_num, int i_low, int i_high);
bool in_range_float(float i_num, float i_low, float i_high);
void add_text(const char *tex ,const char *filename);
FILE * open_file(const char *fname);
bool get_line_from_file(FILE * fp, char * dest);
char *replace(char *szSearch, char *szReplace, char *szBuffer);
void randomize();
int random( long i_max);
float frandom(float f_max);
int random_range(long i_small, long i_large);
float frandom_range(float min, float max);
#define seperate_string separate_string //to support legacy code.. uh..
bool separate_string (const char str[], int num, char liney, char *return1);
void strip_beginning_spaces(char *s);
void show( HWND g_hWnd, const char title[], const char * fmt, ... );
int delete_wildcard ( char st_path[255], char st_search_pattern[255]);
bool get_files_with_wildard ( TCHAR st_path[], TCHAR st_search_pattern[], TCHAR *st_return);
bool open_file(HWND hWnd, const char st_file[]);
//should template this.  
bool number_is_close(float f_orig, float f_target, float f_range);
bool number_is_close(int i_orig, int i_target, int i_range);
void append_file(char st_input[], const char st_output[]);
TCHAR * get_path_from_string(const TCHAR * st_in);
void change_file_extension(char * st_filename, char st_extension[100]);
unsigned int compute_checksum_from_string(const char st_dir[]);
unsigned long ul_compute_checksum_from_string(const char *str);
bool file_extension_is(const TCHAR *st_filename, const TCHAR * st_extension);
bool get_filename_only(TCHAR *p_st_fname_out, const TCHAR * p_st_in);
bool load_file_into_string(char *st_out, char st_file[], int i_max_size);
int altmod(int a, int b);   
double altfmod(double a, double b);

void strip_ending_spaces(TCHAR *p_string);
unsigned int HashString(const char *str);

#ifdef _UNICODE

void ProcessError(HWND hw, int result, const TCHAR filename[], const TCHAR url[]);

#else
//NON UNICODE
void getdir(char final[]);
bool get_files_with_wildard ( char st_path[], char st_search_pattern[], char *st_return);
void ProcessError(HWND hw, int result, char filename[255], char url[255]);
#endif


#endif                  // include guard

