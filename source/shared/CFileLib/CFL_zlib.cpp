    ////// ////// //     /////
   //     //     //         //
  //     ////   //       ///
 //     //     //         //
////// //     ////// /////

///////////////////////////////////////////////
// Copyright
///////////////////////////////////////////////
//
// Compressed File Library 3
// Copyright (c) 2001 Jari Komppa
//
//
///////////////////////////////////////////////
// License
///////////////////////////////////////////////
// 
//     This software is provided 'as-is', without any express or implied
//     warranty.    In no event will the authors be held liable for any damages
//     arising from the use of this software.
// 
//     Permission is granted to anyone to use this software for any purpose,
//     including commercial applications, and to alter it and redistribute it
//     freely, subject to the following restrictions:
// 
//     1. The origin of this software must not be misrepresented; you must not
//        claim that you wrote the original software. If you use this software
//        in a product, an acknowledgment in the product documentation would be
//        appreciated but is not required.
//     2. Altered source versions must be plainly marked as such, and must not be
//        misrepresented as being the original software.
//     3. This notice may not be removed or altered from any source distribution.
// 
// (eg. same as ZLIB license)
// 
//
///////////////////////////////////////////////
//
// See cfl.h for documentation
//
///////////////////////////////////////////////

#include "CFL.h"
#include "CFLResourceFilter.h"
#include "zlib.h"

///////////////////////////////////////////////
// Creating resource handler plugins
///////////////////////////////////////////////
// 
// Different compressors, encrypters, and preprocessors are plugged
// into cfl3 using class factory mechanism.
//
// No changes to cfl3.cpp and/or cfl3.h are needed.
//
// This source file defines the zlib compressor, and it self-registers
// itself to cfl3. No header file is needed.
//
// 7.5.2001: Changed to work with zlib 1.1.3

class CFLRH_Zlib : public CFLResourceFilter
{
private:
    // Each extended class must have a private static instance of itself, like this:
    static CFLRH_Zlib mMyself; 
protected:
    // Private constructor.
    // Constructor must:
    // - set tag and infostring
    // - call registerHandler(tag,this)
    CFLRH_Zlib();
    // process (eg, compress, encrypt..) data 
    virtual void process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
    // reverse process data (eg. decompress, decrypt..) data
    virtual void reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
};

CFLRH_Zlib CFLRH_Zlib::mMyself;

CFLRH_Zlib::CFLRH_Zlib()
{
    static const char * const sn = "ZLIB";
    static const char * const ln = "ZLIB data compression library (C) 1995-1998 Jean-loup Gailly and Mark Adler";
    mTag = CFLCOMPRESS_ZLIB;
    mShortName = (char*)sn;
    mInfoString = (char*)ln;
    registerHandler(this);
}

void CFLRH_Zlib::process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize)
{
    // output buffer "must be at least 0.1% larger than sourceLen plus 12 bytes"
    aDataOutSize = (unsigned long)(aDataInSize * 1.01f) + 12;    
    char *data = new char[aDataOutSize];
    if (data == NULL)
    {
        *aDataOut = NULL;
        aDataOutSize = 0;
        return; 
    }  
    
    //SETH changed this from..
   // if (compress(data, (unsigned long*)&aDataOutSize, (const char*)aDataIn, aDataInSize) >= 0) // negative values are errors

        if (compress((byte*)data, (unsigned long*)&aDataOutSize, (const byte*)aDataIn, aDataInSize) >= 0) // negative values are errors
    {
        // We also want to store the length in the beginning of the block, since we
        // need it when decompressing.
        *aDataOut = new char[aDataOutSize + 4];
        if (*aDataOut == NULL)
        {            
            aDataOutSize = 0;
            delete[] data;
            return;
        }        
        memcpy(*aDataOut, &aDataInSize, 4);
        memcpy(*aDataOut + 4, data, aDataOutSize);        
        delete[] data;
    }
    else // failed
    {
        delete[] data;
        *aDataOut = NULL;
        aDataOutSize = 0;
    }
}

void CFLRH_Zlib::reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize)
{
    memcpy(&aDataOutSize, aDataIn, 4);    
    char *data = new char[aDataOutSize];
    if (data == NULL)
    {
        *aDataOut = NULL;
        aDataOutSize = 0;
        return;
    } 
    // ZLIB 1.1.3 gives out Z_BUF_ERROR even if no error on decompress.
    //Seth changed from this
    //int res = uncompress(data, (unsigned long*)&aDataOutSize, (const char*)(aDataIn + 4), aDataInSize - 4);
    
    int res = uncompress((byte*) data, (unsigned long*)&aDataOutSize, (const byte*)(aDataIn + 4), aDataInSize - 4);
    
    if (res == Z_BUF_ERROR || res >= 0) // negative values are errors
    {
        *aDataOut = new char[aDataOutSize];
        if (*aDataOut == NULL)
        {            
            aDataOutSize = 0;
            delete[] data;
            return;
        }        
        memcpy(*aDataOut, data, aDataOutSize);        
        delete[] data;
    }
    else // failed
    {
        delete[] data;
        *aDataOut = NULL;
        aDataOutSize = 0;
    }
}
