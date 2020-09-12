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

class CFLEnumerator;
class CFLResource;

//! Class factory to handle compressors, preprocessors and encryptors.
class CFLResourceFilter 
{
private:
    //! Each extended class must have a private static instance of itself, like this:
    static CFLResourceFilter mMyself; 
protected:
    //! As root singleton object this one is created when some other is created.
    static CFLResourceFilter *mRootObject;
    //! Root of the linked list of resource handlers
    static CFLResourceFilter *mRoot;
    //! Pointer to the next handler, or NULL if at end of list.
    CFLResourceFilter *mNext;
    //! Pointer to the printable info string.
    char *mInfoString;
    //! Pointer to a printable short name (10 chars or less preferably)
    char *mShortName;
    //! Flag: is this handler enabled or not?
    int mEnabled;
    //! Tag of the current handler. 
    /*! Tag format:
       EPOC
       |||+- Compressor
       ||+-- Compressor variant (option)
       |+--- Preprocessor
       +---- Encryptor
       Each handler must leave the rest of the bits to zero (eg. 00ZZ, 0D00, X000)
    */
    unsigned int mTag;
protected:
    //! Private constructor.
    /*!
       Constructor must:
       - set tag and infostring
       - call registerHandler(tag,this)
     */
    CFLResourceFilter();
    //! Static method to register handler
    static void registerHandler(CFLResourceFilter *aHandler);
    //! process (eg, compress, encrypt..) data 
    virtual void process(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize) = 0;
    //! reverse process data (eg. decompress, decrypt..) data
    virtual void reverseProcess(char *aDataIn, char **aDataOut, unsigned int aDataInSize, unsigned int &aDataOutSize) = 0;
    //! Finds handler for a tag
    static CFLResourceFilter * findHandler(unsigned int aTag);
    //! Sets the cipher key.
    virtual void setCipherKey(void * aKey);
public:
    //! destructor.
    virtual ~CFLResourceFilter();
    //! decompress, decrypt and deprocess data
    static char * unhandle(const CFLResource *aRes, unsigned int aCompressionFlags);
    //! compress, encrypt and process data. CompressionFlags may change during handling.
    static CFLResource * handle(const char *aData, unsigned int aDataSize, unsigned int &aCompressionFlags);
    //! get a printable string on a handler.
    static const char * handlerInfoString(unsigned int aTag);
    //! get a printable short name on a handler.
    static const char * handlerShortName(unsigned int aTag);
    //! enumerate handlers (for gui use generally)
    static void enumerateHandlers(CFLEnumerator *aEnumerator);
    //! enable or disable handlers (for gui use generally)
    static void enableHandler(unsigned int aTag, int aState);
    //! set key for encrypter
    static void setKey(unsigned int aTag, void *aKey);
};
