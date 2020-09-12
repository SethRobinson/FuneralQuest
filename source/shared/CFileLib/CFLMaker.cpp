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
#include "CFLMaker.h"
#include "CFLDirectoryEntry.h"
#include "CFLResourceFilter.h"
#include "CFLResource.h"

CFLMaker::CFLMaker()
{
    mRootDir = NULL;
    mFilePtr = NULL;
}


CFLMaker::~CFLMaker()
{
    CFLDirectoryEntry *dirwalker = mRootDir;
    while (dirwalker)
    {
        CFLDirectoryEntry *last = dirwalker;
        dirwalker = dirwalker->mNext;
        delete last;
    }
}


CFLMaker * CFLMaker::create(const char *aTargetFilename)
{
    CFLMaker *m = new CFLMaker();
    if (m == NULL)
    {
        return NULL;
    }
    m->mFilePtr = fopen(aTargetFilename, "wb");
    if (m->mFilePtr == NULL) 
        return NULL;
    int data = 0x334c4643;
    fwrite(&data,1,4,m->mFilePtr); // header
    fwrite(&data,1,4,m->mFilePtr); // placeholder for directory offset
    fwrite(&data,1,4,m->mFilePtr); // placeholder for directory decompressed size    
    return m;
}


int CFLMaker::store(const char *aFilename, const char *aData, int aDataSize, unsigned int aCompressionFlags)
{
    CFLDirectoryEntry *dir = new CFLDirectoryEntry();
    if (dir == NULL)
    {
        return 0;
    }
    dir->mOffset = ftell(mFilePtr);
    dir->mCompression = aCompressionFlags;
    dir->mUnpackedSize = aDataSize;
    
    int namelen = 0;
    while (aFilename[namelen]) namelen++;
    
    dir->mName = new char[namelen+1];
    if (dir->mName == NULL)
    {
        delete dir;
        return 0;
    }

    namelen = 0;
    while (aFilename[namelen]) 
    {
        dir->mName[namelen] = aFilename[namelen];
        namelen++;
    }

    dir->mName[namelen] = 0;
    CFLResource *res = CFLResourceFilter::handle(aData, aDataSize, dir->mCompression);    

    // if compression fails, return 0;
    if (res == NULL)
    {
        delete dir;
        return 0;
    }
    res->save(mFilePtr);
    int packedsize = res->mCompressedSize;
    delete res;
    dir->mNext = mRootDir;
    mRootDir = dir;
    return packedsize;
}


int CFLMaker::finish(unsigned int aLibraryCompressionFlags)
{
    int libofs = ftell(mFilePtr);
    
    int unpackedDirSize = 0;
    CFLDirectoryEntry *dirwalker = mRootDir;
    while (dirwalker)
    {
        unpackedDirSize += dirwalker->byteRequirement();
        dirwalker = dirwalker->mNext;
    }

    char *rawdir = new char[unpackedDirSize];
    // memory allocation failed; return 0
    if (rawdir == NULL)
    {
        return 0;
    }

    dirwalker = mRootDir;
    unpackedDirSize = 0;
    while (dirwalker)
    {
        dirwalker->store(rawdir, unpackedDirSize);
        dirwalker = dirwalker->mNext;
    }

    CFLResource *resdir = CFLResourceFilter::handle(rawdir, unpackedDirSize, aLibraryCompressionFlags);
    delete[] rawdir;    
    // compression failed; return 0
    if (resdir==NULL)
    {
        return 0;
    }

    fwrite(&aLibraryCompressionFlags, 1, 4, mFilePtr);
    resdir->save(mFilePtr);
    delete resdir;
    int offsetOfHeader = ftell(mFilePtr)+8;
    fwrite(&offsetOfHeader, 1, 4, mFilePtr);
    int tag = 0x4c464333; // '3CFL'
    fwrite(&tag, 1, 4, mFilePtr);
    fseek(mFilePtr, 4, SEEK_SET);
    fwrite(&libofs, 1, 4, mFilePtr); 
    fwrite(&unpackedDirSize, 1, 4, mFilePtr);
    fclose(mFilePtr);
    delete this;
    return 1; // success!
}
