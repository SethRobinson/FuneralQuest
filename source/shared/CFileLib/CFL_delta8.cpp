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
// This source file defines the 8-bit delta encoder, and it self-registers
// itself to cfl3. No header file is needed.
//

class CFLRH_Delta8 : public CFLResourceFilter
{
private:
    // Each extended class must have a private static instance of itself, like this:
    static CFLRH_Delta8 mMyself; 
protected:
    // Private constructor.
    // Constructor must:
    // - set tag and infostring
    // - call registerHandler(tag,this)
    CFLRH_Delta8();
    // process (eg, compress, encrypt..) data 
    virtual void process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
    // reverse process data (eg. decompress, decrypt..) data
    virtual void reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize);
};

CFLRH_Delta8 CFLRH_Delta8::mMyself;

CFLRH_Delta8::CFLRH_Delta8()
{
    static const char * const sn = "Delta8";
    static const char * const ln = "8-bit delta encoding";
    mTag = CFLPREPROCESS_DELTA8;
    mShortName = (char*)sn;
    mInfoString = (char*)ln;
    registerHandler(this);
}

void CFLRH_Delta8::process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize)
{
    char *din = aDataIn;
    char *dout = new char[aDataInSize];
    if (dout == NULL)
    {
        *aDataOut = NULL;
        aDataOutSize = 0;
        return;
    }
    *aDataOut = dout;
    int process = aDataInSize;
    dout[0] = din[0];
    for (int i = 1; i < process; i++)
        dout[i] = din[i] - din[i - 1];
    aDataOutSize = aDataInSize;
}

void CFLRH_Delta8::reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize)
{
    char *din = aDataIn;
    char *dout = new char[aDataInSize];
    if (dout == NULL)
    {
        *aDataOut = NULL;
        aDataOutSize = 0;
        return;
    }
    *aDataOut = dout;
    int process = aDataInSize;
    dout[0] = din[0];
    for (int i = 1; i < process; i++)
        dout[i] = din[i] + dout[i - 1];
    aDataOutSize = aDataInSize;
}
