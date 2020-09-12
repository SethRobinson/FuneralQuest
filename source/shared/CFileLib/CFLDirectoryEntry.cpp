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
#include "CFLDirectoryEntry.h"

int CFLDirectoryEntry::byteRequirement()
{
    int size=0;
    while (mName[size]) size++;
    size += 4 + 4 + 4 + 2; // unpacksize, offset, compression, name lenght
    return size;
}


void CFLDirectoryEntry::store(char *aBuffer, int &aOffset)
{
    memcpy(aBuffer + aOffset, &mUnpackedSize, 4);
    aOffset += 4;
    memcpy(aBuffer + aOffset, &mOffset, 4);
    aOffset += 4;
    memcpy(aBuffer + aOffset, &mCompression,4);
    aOffset += 4;

    unsigned short namelen = 0;
    while (mName[namelen]) 
    {
        aBuffer[aOffset + 2 + namelen] = mName[namelen];
        namelen++;
    }
    memcpy(aBuffer + aOffset, &namelen, 2);
    aOffset += namelen + 2;    
}
 
 
CFLDirectoryEntry * CFLDirectoryEntry::restore(char *aBuffer, int &aOffset)
{
    CFLDirectoryEntry *e = new CFLDirectoryEntry();
    if (e == NULL)
    {
        return NULL;
    }

    memcpy(&e->mUnpackedSize, aBuffer + aOffset, 4);
    aOffset += 4;
    memcpy(&e->mOffset, aBuffer + aOffset, 4);
    aOffset += 4;
    memcpy(&e->mCompression, aBuffer + aOffset, 4);
    aOffset += 4;

    unsigned short namelen = 0;
    memcpy(&namelen, aBuffer + aOffset, 2);
    aOffset += 2;
    e->mName = new char[namelen+1];
    if (e->mName == NULL)
    {
        delete e;
        return NULL;
    }
    memcpy(e->mName, aBuffer + aOffset, namelen);
    e->mName[namelen] = 0;

    aOffset += namelen;
    e->mNameHash = calcHash(e->mName);
    
    return e;
}


CFLDirectoryEntry::~CFLDirectoryEntry()
{
    delete[] mName;
}


int CFLDirectoryEntry::equals(const char *aName, int aHash)
{
    if (mNameHash != aHash) return 0;
    char *n1 = (char*)aName, *n2 = mName;
    while (*n1 && *n2 && *n1 == *n2) 
    {
        n1++;
        n2++;
    }
    if (!*n1 && !*n2) 
        return 1;
    return 0;
}


int CFLDirectoryEntry::calcHash(const char *aName)
{
    int hash = 0;
    char *d = (char*)aName;
    while (*d) 
    {
        int t = hash >> 11;
        hash = hash << (32 - 11);
        hash += t;
        hash ^= *d & 0xff;
        d++;
    }
    return hash;
}


void CFLDirectoryEntry::replaceEntry(CFLDirectoryEntry *aReplacement)
{
    delete[] mName;
    mName = aReplacement->mName;
    aReplacement->mName = NULL;
    mLib = aReplacement->mLib;
    mUnpackedSize = aReplacement->mUnpackedSize;
    mOffset = aReplacement->mOffset;
    mNameHash = aReplacement->mNameHash;
    delete aReplacement;
}


CFLDirectoryEntry * CFL::findEntry(const char *aName)
{
    CFLDirectoryEntry *walker;
    walker = mRootDir;
    int hash = CFLDirectoryEntry::calcHash(aName);
    while (walker)
    {
        if (walker->equals(aName, hash)) 
            return walker;
        walker = walker->mNext;
    }
    return NULL;
}
