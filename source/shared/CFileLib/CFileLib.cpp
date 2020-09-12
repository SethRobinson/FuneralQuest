/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "CFilelib.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)
extern "C"
{

#ifdef WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif
}

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFileLib::CFileLib()
{
    //init stuff
    ZeroMemory(this, sizeof(CFileLib));
   
   m_p_cfl = new CFL;
}

                     

CFileLib::~CFileLib()
{

    //close any open files                 
   CloseZipIfOpened();
   SAFE_DELETE(m_p_cfl);
}


void CFileLib::CloseZipIfOpened()
{
     if (m_uf)
    {
        unzClose(m_uf);
       m_uf = NULL;
    }

}

bool CFileLib::OpenZip( char *st_name)
{
   CloseZipIfOpened();
   
   if (!exist(st_name)) return false;
    zlib_filefunc_def ffunc;
      fill_win32_filefunc(&ffunc);
	  m_uf = unzOpen2(st_name,&ffunc);
  
      if (!m_uf) return false;

      //success
      return true;
}

bool CFileLib::FileExistsInZip(char *st_file_name)
{
  
    char st_name_temp[256];
    strcpy(st_name_temp, st_file_name);
    
    //change any backslashes to forward slashes because that's how Winzip saves 'em

    for (int i=0; st_name_temp[i] != 0; i++)
    {
        if (st_name_temp[i] == '\\') st_name_temp[i] = '/';
    }

    int err = UNZ_OK;
    if (unzLocateFile(m_uf,st_name_temp,CASESENSITIVITY)!=UNZ_OK)
    {
        //LogError("file %s not found in the zipfile",st_name_temp);
        return false;
    }
   
    //guess we found it.  As far as I know I don't have to close m_uf or anything because
    //I haven't actually opened the file.  Hopefully I'm not wrong, watch this spot for
    //memory leaks.
    return true;
}


bool CFileLib::UnzipFileToMemory(char *st_file_name, void **p_out, unsigned long ul_max_bytes_to_read, unsigned long *p_ul_uncompressed_size)
{
   
    if (!IsActive()) return false;
    
    char st_name_temp[256];
    strcpy(st_name_temp, st_file_name);
    
    //change any backslashes to forward slashes because that's how Winzip saves 'em

    for (int i=0; st_name_temp[i] != 0; i++)
    {
        if (st_name_temp[i] == '\\') st_name_temp[i] = '/';
    }

    int err = UNZ_OK;
    if (unzLocateFile(m_uf,st_name_temp,CASESENSITIVITY)!=UNZ_OK)
    {
        //LogError("file %s not found in the zipfile",st_name_temp);
        return false;
    }
    
    unz_file_info file_info;
  
    char st_filename_inzip[256];
    
    
    err = unzGetCurrentFileInfo(m_uf,&file_info,st_filename_inzip,sizeof(st_filename_inzip),NULL,0,NULL,0);
    
    if (err!=UNZ_OK)
    {
        LogError("error %d with zipfile in unzGetCurrentFileInfo",err);
        return false;
    }
    
   
    if (ul_max_bytes_to_read != 0)
    {
    
    
    if (ul_max_bytes_to_read < file_info.uncompressed_size)
    {
        LogError("%d bytes isn't enough space to decompress %s into.", ul_max_bytes_to_read, st_file_name);
        return false;
    }
    } else
    {
        //let's allocate our own memory and pass the pointer back to them.
        *p_out = malloc(file_info.uncompressed_size+2); //the two extra is because I may need to add a null later in
        //CTextParse.
        if (*p_out)
        {
            //memory allocated
            *p_ul_uncompressed_size =  file_info.uncompressed_size;
        }   else
        {
            LogError("Couldn't allocate the required %d bytes to unzip into.", file_info.uncompressed_size+2);
            return false;
        }

    }
    err = unzOpenCurrentFile(m_uf);
    
    if (err!=UNZ_OK)
    {
        LogError("error %d with zipfile in unzOpenCurrentFile",err);
      return false;
    }
    
    err = unzReadCurrentFile(m_uf,*p_out,file_info.uncompressed_size);
    if (err<0)	
    {
        LogError("error %d with zipfile in unzReadCurrentFile",err);
      return false;
        
    }
     
    err = unzCloseCurrentFile (m_uf);
    if (err!=UNZ_OK)
    {
        LogError("error %d with zipfile in unzCloseCurrentFile",err);
      return false;
    }
  

   
    return true;
}




