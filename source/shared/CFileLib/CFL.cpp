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
#include "CFLResource.h"
#include "CFLResourceFilter.h"
#include "CFLLibrary.h"
#include "CFLDirectoryEntry.h" 
#include "all.h"
#include <fcntl.h>
#include <io.h>
#include <cassert>


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFL::CFL()
{
    mRootLib = NULL;
    mRootDir = NULL;   
    mOpenBehavior = CFL_OPEN_BEHAVIOR_CFL_ONLY;
}


CFL * CFL::create(const char *aLibFilename, int aOffset)
{
    CFL *c = new CFL();
    if (c == NULL)
    {
        return NULL;
    }
    if (!c->openLibrary(aLibFilename, aOffset))
    {
        delete c;
        return NULL;
    }
    return c;
}


CFL * CFL::create(FILE * aLibFilePtr)
{
    CFL *c = new CFL();
    if (c == NULL)
    {
        return NULL;
    }
    if (!c->openLibrary(aLibFilePtr))
    {
        delete c;
        return NULL;
    }
    return c;
}


int CFL::openLibrary(const char *aLibFilename)
{
    return openLibrary(aLibFilename, 0);
}


int CFL::openLibrary(const char *aLibFilename, int aOffset)
{
    int tag = 0;
    int offs = 0;
    int dirsize = 0;
    FILE *f;
    f = fopen(aLibFilename, "rb");
    if (f == NULL) return 0;

    // check given offset first

    fseek(f, aOffset, SEEK_SET);
    fread(&tag, 1, 4, f);
    if (tag != 0x334c4643) // 'CFL3'    
    {
        // okay, so tag wasn't there. Check footer next
        fseek(f, -4, SEEK_END);
        fread(&tag, 1, 4, f);
        if (tag != 0x4c464333) // '3CFL'
        {
            // nope, give up.
            fclose(f);
            return 0;
        }
        fseek(f, -8, SEEK_END);
        fread(&offs, 1, 4, f);
        fseek(f, -offs, SEEK_END);
        fread(&tag, 1, 4, f);
        if (tag != 0x334c4643) // 'CFL3'    
        {
            // footer matches but header doesn't. Give up.
            fclose(f);
            return 0;            
        }
    }

    // If we get this far, we've just passed the header CFL3 tag.

    aOffset = ftell(f) - 4;
    fread(&offs, 1, 4, f);
    fread(&dirsize, 1, 4, f);
    fseek(f, aOffset + offs, SEEK_SET); // go to the beginning of directory

    CFLResource *directory;
    int directoryCompression;
    fread(&directoryCompression, 1, 4, f);
    directory = CFLResource::load(f);    
    fclose(f);

    if (directory==NULL)
    {
        return 0;
    }

    char *d = CFLResourceFilter::unhandle(directory, directoryCompression);

    delete directory;

    if (d == NULL)
    {
        // Decompressing the directory failed. (probably unsupported format)
        return 0;    
    }
    
    // so now we're pretty certain that this is a good cfl file..
    
    CFLLibrary *lib = new CFLLibrary();
    if (lib == NULL)
    {
        delete[] d;
        return 0;
    }

    int fn = 0;
    while (aLibFilename[fn]) fn++;
    lib->mFilename = new char[fn+1];

    if (lib->mFilename == NULL)
    {
        delete[] d;
        delete lib;
        return 0;
    }

    fn = 0;
    while (aLibFilename[fn]) 
    {
        lib->mFilename[fn] = aLibFilename[fn];
        fn++;
    }

    lib->mFilename[fn] = 0;
    lib->mOffset = aOffset;
    lib->mNext = mRootLib;
    mRootLib = lib;
    
    offs = 0;
    while (offs < dirsize)
    {
        // restore-call updates offs
        CFLDirectoryEntry *e = CFLDirectoryEntry::restore(d, offs);
        if (e==NULL)
        {
            delete[] d;
            return 1; // partial success
        }
        e->mLib = lib;        
        CFLDirectoryEntry *old = findEntry(e->mName);
        if (old != NULL)
        {
            old->replaceEntry(e);
        }
        else
        {
            e->mNext = mRootDir;
            mRootDir = e;
        }
    }
    delete[] d;
    
    // success!

    return 1;
}


int CFL::openLibrary(FILE *aLibFilePtr)
{
    int tag = 0;
    int offs = 0;
    int dirsize = 0;
    int offset;
    FILE *f = aLibFilePtr;
    if (f == NULL) return 0;

    // check current offset first

    fread(&tag,1,4,f);
    if (tag != 0x334c4643) // 'CFL3'    
    {
        // okay, so tag wasn't there. Check footer next
        fseek(f, -4, SEEK_END);
        fread(&tag, 1, 4, f);
        if (tag != 0x4c464333) // '3CFL'
        {
            // nope, give up.
            fclose(f);
            return 0;
        }
        fseek(f, -8, SEEK_END);
        fread(&offs, 1, 4, f);
        fseek(f, -offs, SEEK_END);
        fread(&tag, 1, 4, f);
        if (tag != 0x334c4643) // 'CFL3'    
        {
            // footer matches but header doesn't. Give up.
            fclose(f);
            return 0;            
        }
    }

    // If we get this far, we've just passed the header CFL3 tag.

    offset = ftell(f) - 4;
    fread(&offs, 1, 4, f);
    fread(&dirsize, 1, 4, f);
    fseek(f, offset + offs, SEEK_SET); // go to the beginning of directory
    CFLResource *directory;

    int directoryCompression;
    fread(&directoryCompression, 1, 4, f);
    directory = CFLResource::load(f);    
    if (directory == NULL)
    {
        return 0;
    }

    char * d = CFLResourceFilter::unhandle(directory, directoryCompression);
    delete directory;
    if (d == NULL)
    {
        // Decompressing the directory failed. (probably unsupported format)
        return 0;    
    }
    
    // so now we're pretty certain that this is a good cfl file..
    
    CFLLibrary *lib = new CFLLibrary();
    if (lib == NULL)
    {
        delete[] d;
        return 0;
    }

    lib->mFilePtr = f;
    lib->mOffset = offset;
    lib->mNext = mRootLib;
    mRootLib = lib;
    
    offs = 0;
    while (offs < dirsize)
    {
        // restore-call updates ofs
        CFLDirectoryEntry *e = CFLDirectoryEntry::restore(d, offs);
        if (e == NULL)
        {
            delete[] d;
            return 1; // partial success
        }
        e->mLib = lib;        
        CFLDirectoryEntry *old = findEntry(e->mName);
        if (old != NULL)
        {
            old->replaceEntry(e);
        }
        else
        {
            e->mNext = mRootDir;
            mRootDir = e;
        }
    }
    delete[] d;
    // success!
    return 1;
}

//you do need to delete [] the return of this.

char * CFL::getFile(const char *aName)
{
    
    
    int s; // ignored parameter
    return getFile(aName,s);
}

char * CFL::ReadFileFromDisk(const char *aName, int &aSize)
{
    //is it on disk?
    int handle = open(aName, O_RDONLY | O_BINARY);
    if (handle != -1) 
    {
        //LogMsg("Loading %s from disk", aName);
        //it existed.  Let's use this instead.
        aSize = filelength(handle);
        char *p_buff = new char[aSize];
        int i_read = read(handle, p_buff, aSize);
        assert(i_read == aSize && "Didn't read correctly from file maybe");
        return p_buff;
    }
  return NULL;
}


char * CFL::getFile(const char *aName, int &aSize)
{
  
    if (mOpenBehavior == CFL_OPEN_BEVAVIOR_DISK_CFL) 
    {
        char *p_buff = ReadFileFromDisk(aName, aSize);
        if (p_buff) return p_buff;
    }
    

    
    CFLDirectoryEntry *e = findEntry(aName);
    aSize = 0;
    if (e == NULL) 
    {
        if (mOpenBehavior == CFL_OPEN_BEVAVIOR_CFL_DISK) 
        {
            char *p_buff = ReadFileFromDisk(aName, aSize);
            if (p_buff) return p_buff;
        }

        return NULL;
    }
    aSize = e->mUnpackedSize;
    CFLResource *r;
    FILE *f;
    f = e->mLib->open();
    fseek(f, e->mLib->mOffset + e->mOffset, SEEK_SET);
    r = CFLResource::load(f);
    e->mLib->close(f);
    char *d = CFLResourceFilter::unhandle(r, e->mCompression);
    delete r;
    return d;  
    
    
   
}


int CFL::getFile(const char *aName, char *aBuffer, int aMaxSize)
{
    int s; 
    char *d = getFile(aName, s); // decompress to temporary buffer
    if (d == NULL) return 0;
    if (s > aMaxSize) s = aMaxSize;
    memcpy(aBuffer, d, s);
    delete[] d; // release temporary buffer
    return s;
}


int CFL::getFileSize(const char *aName)
{
    assert(!"TODO: Fix for files from disk too..");
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) return 0;
    return e->mUnpackedSize;    
}


int CFL::fileExists(const char *aName)
{
    if (mOpenBehavior == CFL_OPEN_BEVAVIOR_DISK_CFL) 
    {
      if (exist(aName)) return 1;
    }
    
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) return 0;
    return 1;    
}


FILE * CFL::getFilePtr(const char *aName)
{
    FILE *f;
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) 
        return NULL;
    f = e->mLib->open();
    fseek(f, e->mLib->mOffset + e->mOffset + sizeof(int), SEEK_SET);
    return f;
}

void CFL::dropFilePtr(FILE *aFilePtr)
{
    CFLLibrary *walker;
    walker = mRootLib;
    while (walker)
    {
        if (walker->mFilePtr == aFilePtr)
            return;
        walker = walker->mNext;
    }
    fclose(aFilePtr);
}


int CFL::getFileOfs(const char *aName)
{
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) 
        return 0;
    return e->mLib->mOffset + e->mOffset;
}


const char * CFL::compressionInfoString(const char *aName)
{
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) 
        return NULL;
    return CFLResourceFilter::handlerInfoString(e->mCompression & 0x0000ffff);
}


const char * CFL::encryptionInfoString(const char *aName)
{
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) 
        return NULL;
    return CFLResourceFilter::handlerInfoString(e->mCompression & 0xff000000);
}


const char * CFL::preprocessorInfoString(const char *aName)
{
    CFLDirectoryEntry *e = findEntry(aName);
    if (e == NULL) 
        return NULL;
    return CFLResourceFilter::handlerInfoString(e->mCompression & 0x00ff0000);
}


CFL::~CFL()
{
    CFLLibrary *libwalker = mRootLib;
    while (libwalker)
    {
        CFLLibrary *last = libwalker;
        libwalker = libwalker->mNext;
        delete last;
    }
    CFLDirectoryEntry *dirwalker = mRootDir;
    while (dirwalker)
    {
        CFLDirectoryEntry *last = dirwalker;
        dirwalker = dirwalker->mNext;
        delete last;
    }
}


