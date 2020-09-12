/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



//This class contains two separate file systems - one is based on Jari Komppas CFL system, which you will want to
//use for most stuff, the secondary system can load files directly from standard .zip files.  Both require zlib to
//operate.

#pragma once

#include "..\\all.h"
#include "CFL.h"

extern "C"
{
#include "unzip.h"
}


#ifndef __CFILELIB
#define __CFILELIB


class CFileLib
{
public:
  CFileLib();
  ~CFileLib();
    
  bool OpenZip(char *st_name);
  bool UnzipFileToMemory(char *st_file_name, void **p_out, unsigned long ul_max_bytes_to_read, unsigned long *p_ul_uncompressed_size);
  bool IsActive(){if (m_uf) return true; return false;}  //returns true if we're attached to a zip file
  bool FileExistsInZip(char *st_file_name);

  CFL * GetCFL(){return m_p_cfl;}
  
private:

  unzFile m_uf; 
  void CloseZipIfOpened();
  CFL * m_p_cfl; //main cfl object

};

#endif