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
#include "CFLResource.h"
#include "CFLEnumerator.h"

CFLResourceFilter *CFLResourceFilter::mRoot = NULL;


CFLResourceFilter::CFLResourceFilter()
{
    mEnabled = 1;
}


void CFLResourceFilter::registerHandler(CFLResourceFilter *aHandler)
{
    aHandler->mNext = mRoot;
    mRoot = aHandler;
}


CFLResourceFilter::~CFLResourceFilter()
{
}



CFLResourceFilter * CFLResourceFilter::findHandler(unsigned int aTag) 
{
    CFLResourceFilter *walker;
    walker = mRoot;
    while (walker) 
    {
        if (walker->mTag == aTag) 
            return walker;
        walker = walker->mNext;        
    }
    return NULL;
}


char * CFLResourceFilter::unhandle(const CFLResource *aRes, unsigned int aCompressionFlags)
{
#ifdef _DEBUG
    if (mRoot == NULL)
    {
        int i = 0, j = 0;
        j/=i; // cause division by zero
        // If you hit this check, it's because you haven't
        // linked any resource handlers to your project.
        // * You do not need to change anything in this file *
        // just add cfl_none.cpp to your project and you're go. 
        // See cfl3.h for more info.
    }
#endif
    CFLResourceFilter *h;    
    char *data = aRes->mData;
    unsigned int datasize = aRes->mCompressedSize;
    char *d;
    unsigned int newsize;
    
    // encryption
    
    if (aCompressionFlags & 0xff000000) 
    {
        h = findHandler(aCompressionFlags & 0xff000000);
        if (h == NULL) 
            return NULL;
        h->reverseProcess(data, &d, datasize, newsize);
        data = d;
        datasize = newsize;
    }
    
    // compression
    
    h = findHandler(aCompressionFlags & 0x0000ffff);
    // if not found, try to find the base variant:
    if (h == NULL) 
        h = findHandler(aCompressionFlags & 0x000000ff);
    if (h == NULL) 
    {
        if (data != aRes->mData)
            delete[] data;
        return NULL;
    }

    h->reverseProcess(data, &d, datasize, newsize);
    if (data != aRes->mData)
        delete[] data;
    data = d;
    datasize = newsize;

    // preprocess

    if (aCompressionFlags & 0x00ff0000) 
    {
        h = findHandler(aCompressionFlags & 0x00ff0000);
        if (h == NULL) 
        {
            if (data != aRes->mData)
                delete[] data;
            return NULL;
        }
        h->reverseProcess(data, &d, datasize, newsize);
        if (data != aRes->mData)
            delete[] data;
        data = d;
        datasize = newsize;
    }
    return data;
}


CFLResource * CFLResourceFilter::handle(const char *aData, unsigned int aDataSize, unsigned int &aCompressionFlags)
{
#ifdef _DEBUG
    if (mRoot==NULL)
    {
        int i=0,j=0;
        j/=i;
        // If you hit this check, it's because you haven't
        // linked any resource handlers to your project.
        // * You do not need to change anything in this file *
        // just add cfl_none.cpp to your project and you're go. 
        // See the cfl.h file for more info.
    }
#endif
    CFLResource *res = new CFLResource();
    if (res == NULL)
    {
        return NULL;
    }
    res->mData = (char*)aData;
    CFLResourceFilter *h;    
    unsigned int currentsize = aDataSize;
    char *d;
    unsigned int newsize;
    
    // preprocess
    
    if (aCompressionFlags & 0x00ff0000) 
    {
        h = findHandler(aCompressionFlags & 0x00ff0000);
        if (h == NULL) 
            return NULL;
        h->process(res->mData, &d, currentsize, newsize);
        res->mData = d;
        currentsize = newsize;
    }
    
    // compression
    
    if ((aCompressionFlags & 0x0000ffff) == CFLCOMPRESS_BEST)
    {
        CFLResourceFilter *walker = mRoot;
        int decided = 0;
        unsigned int smallest = 0xffffffff;
        char *smallest_data = NULL;
        while (walker)
        {
            unsigned int tempsize;
            char *tempdata;
            walker->process(res->mData, &tempdata, currentsize, tempsize);            
            if (tempdata != NULL && tempsize < smallest)
            {
                decided = walker->mTag;
                smallest = tempsize;
                if (smallest_data != NULL)
                    delete[] smallest_data;
                smallest_data = tempdata;
            } 
            else
            {
                delete[] tempdata;
            }
            walker = walker->mNext;
        }
        aCompressionFlags &= 0xffff0000;
        aCompressionFlags |= decided;
        if (res->mData != aData) {
            delete[] res->mData;
        }
        res->mData = smallest_data;
        currentsize = smallest;
    } 
    else // not _BEST
    {
        h = findHandler(aCompressionFlags & 0x0000ffff);
        if (h == NULL) 
        {
            if (res->mData != aData) 
                delete[] res->mData;
            delete res;
            return NULL;
        }
        h->process(res->mData, &d, currentsize, newsize);
        if (res->mData != aData) {
            delete[] res->mData;
        }
        res->mData = d;
        currentsize = newsize;
    }

    // encryption

    if (aCompressionFlags & 0xff000000) 
    {
        h = findHandler(aCompressionFlags & 0xff000000);
        if (h == NULL) 
        {
            if (res->mData != aData)
                delete[] res->mData;
            delete res;
            return NULL;
        }
        h->process(res->mData, &d, currentsize, newsize);
        if (res->mData != aData)
            delete[] res->mData;
        res->mData = d;
        currentsize = newsize;
    }
    res->mCompressedSize = currentsize;
    return res;
}


const char * CFLResourceFilter::handlerShortName(unsigned int aTag)
{
    static const char * const findbest_sn = "best";
    if (aTag == 0xffff)
    {
        return findbest_sn;
    }
    CFLResourceFilter *h = findHandler(aTag);
    if (h == NULL) return (const char * const)"Unknown";
    return h->mShortName;    
}


const char * CFLResourceFilter::handlerInfoString(unsigned int aTag)
{
    static const char * const findbest_info = "Finds the best compressor by testing them all";
    if (aTag == 0xffff)
    {
        return findbest_info;
    }
    CFLResourceFilter *h = findHandler(aTag);
    if (h == NULL) return (const char * const)"Unknown";
    return h->mInfoString;    
}


void CFLResourceFilter::enumerateHandlers(CFLEnumerator * aEnumerator)
{
    CFLResourceFilter *walker;
    walker = mRoot;
    while (walker) 
    {
        aEnumerator->infoCallback(walker->mTag, walker->mInfoString, walker->mShortName);
        walker = walker->mNext;        
    }
}

void CFLResourceFilter::enableHandler(unsigned int aTag, int aState)
{
    CFLResourceFilter * h=findHandler(aTag);
    if (h != NULL) 
        h->mEnabled = aState;
}

void CFLResourceFilter::setKey(unsigned int aTag, void *aKey)
{
    CFLResourceFilter * h = findHandler(aTag);
    if (h != NULL) 
        h->setCipherKey(aKey);
}


void CFLResourceFilter::setCipherKey(void * aKey)
{
    // do nothing by default
}
