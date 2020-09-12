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

///////////////////////////////////////////////
// Creating resource handler plugins
///////////////////////////////////////////////
// 
// Different compressors, encrypters, and preprocessors are plugged
// into cfl3 using class factory mechanism.
//
// No changes to cfl3.cpp and/or cfl3.h are needed.
//
// This source file defines the do-nothing compressor, and it self-registers
// itself to cfl3. No header file is needed.
//

class CFLRH_None : public CFLResourceFilter
{
private:
    // Each extended class must have a private static instance of itself, like this:
    static CFLRH_None mMyself; 
protected:
    // Private constructor.
    // Constructor must:
    // - set tag and infostring
    // - call registerHandler(tag,this)
    CFLRH_None();
    // process (eg, compress, encrypt..) data 
    virtual void process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
    // reverse process data (eg. decompress, decrypt..) data
    virtual void reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
};

CFLRH_None CFLRH_None::mMyself;

CFLRH_None::CFLRH_None()
{
    static const char * const sn = "None";
    static const char * const ln = "No compression";
    mTag = CFLCOMPRESS_NONE;
    mShortName = (char*)sn;
    mInfoString = (char*)ln;
    registerHandler(this);
}

void CFLRH_None::process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize)
{
    *aDataOut = new char[aDataInSize];
    if (*aDataOut == NULL)
    {
        aDataOutSize = 0;
        return;
    }
    memcpy(*aDataOut, aDataIn, aDataInSize);
    aDataOutSize = aDataInSize;
}


void CFLRH_None::reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int & aDataOutSize)
{
    process(aDataIn, aDataOut, aDataInSize, aDataOutSize);
}
