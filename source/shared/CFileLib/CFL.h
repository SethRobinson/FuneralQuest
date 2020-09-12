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
//     warranty.  In no event will the authors be held liable for any damages
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
// What is CFL3?
///////////////////////////////////////////////
// 
// CFL3 is the third complete rewrite of my Compressed File Library library.
// CFL makes it possible to store several blocks of data, such as files,
// into one data file, from which they can be retrieved with a single call.
//
// In addition to making the resource loading easy, the stored data can be
// compressed and/or encrypted, and the compression can be defined for each
// file separately.
//
// CFL also enables you to have single executable file distribution - you
// can append the .cfl file after your executable, and CFL will happily
// read it from there.
//
// You can also use 'patch' CFL:s, ie. you can open several CFLs with the
// same resource names, and when retrieved only the latest opened resource
// will be loaded.
// 
// Using CFL3 is pretty straight-forward, and only major differences from
// CFL2 are:
// - Object-oriented design
// - You can now have several different directories in memory 
//   (for example coredata, leveldata etc) by creating several instances
//   of CFL.
// - compressors etc. are now implemented through pluggable class factories.
// - new file format
//
// Important note: you HAVE to link at least one resource handler with this
//                 library; preferably at least the "no compression" one.
//
// 
///////////////////////////////////////////////
// Creating CFL files
///////////////////////////////////////////////
//
// You can create CFL files in three different ways:
// - Programmatically using CFLMaker class (see below for details)
// - From command line using cflmake tool (see cflmake.cpp for details)
// - Visually in Windows using cflstudio (planned tool, unlikely to 
//   appear any time soon due to the fact that the author is tad bit busy,
//   but if you feel like contributing it, hey, suits me fine!)
//
//
///////////////////////////////////////////////
// Basic usage
///////////////////////////////////////////////
//
// You can create the CFL object by just new'ing it, or using the static
// Create functions to open an .cfl file while doing so.
// 
//   example 1:
//      CFL *filelib;
//      filelib = new CFL();
//      filelib->OpenLibrary("mylib.cfl");
//
//   example 2:
//      CFL *filelib;
//      filelib = CFL::Create("mylib.cfl");
//
// After opening the library, you can open others as well, and their 
// contents will be added to the directory. You do not need to care
// where the data comes from when you use it; CFL takes care of that.
//
// Getting the data is done in the following manner:
//   
//   example 3:
//      char *mydata;
//      mydata = filelib->getFile("mydata.dat");
//      useData(mydata);
//      delete[] mydata;
//
// You can also get file size etc. by using the CFL class methods.
//
//
///////////////////////////////////////////////
// CFL creator usage
///////////////////////////////////////////////
//
// Main reason for the creator is game saves, so let's say you have a huge
// adventure game with starting point game data in one CFL and then you alter
// the data on the fly - saving the differences into CFL. Loading a game
// can be as simple as first mounting the world data and then player data
// on top of it.
//
// Using the maker works like this:
//  
//   example 4:
//      CFLMaker *maker;
//      maker = CFLMaker::create("mysavefile.cfl");
//      maker->store("thisdata", thisdata, thisdatasize, compressFlags);
//      maker->store("thatdata", thatdata, thatdatasize, compressFlags);
//      maker->store("somedata", somedata, somedatasize, compressFlags);
//      maker->finish(compressFlags);
//
// The finish call also destructs the maker.
// Compress flags may be a combination of compressor, encrypter and preprocessor.
// Generally you only use a compressor, but you may need to create encrypters etc.
// for your own specific needs, so the functionality is there.
//
// If you specify CFLCOMPRESS_BEST as compress flag, CFL will try to compress
// the data with every single compressor it has available and finally compresses
// with the one that gave it the best compression ratio. (eg. it's slow, eats
// memory, but guarantees the best compression ratio).
//
//
///////////////////////////////////////////////
// Using encrypters
///////////////////////////////////////////////
//
// Before compressing or decompressing data with some encryption, you must
// set the key for the encrypter using the static function
//
// CFLResourceHandler::setKey(unsigned int tag, void *key);
//
// See documentation on each encrypter for details on the key.
// (Please note that encryption by no means makes your data
// secure, as you will need to provide the means to decrypt 
// it in order to use it!)
// 
// 
///////////////////////////////////////////////
// File format
///////////////////////////////////////////////
// 
// You should not need to know this, but here it is in any case..
//
// Header
//  Tag 'CFL3'
//  Offset of directory
//  size of decompressed library
// Resources
//  ..
//  ..
//  ..
// Directory
//  Compression type
//  Directory resource
// Footer
//  Offset of header
//  Tag '3CFL'
// 
//
///////////////////////////////////////////////

#ifndef CFL_H_INCLUDED
#define CFL_H_INCLUDED

// CFL needs fopen, fclose, fseek, ftell, fread, fwrite and memcpy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   



//Support for transparently handling real files added by Seth (www.rtsoft.com)
//Note, it's not a very fast implementation, it's intended for debug purposes, retail builds should
//use CFL only

enum
{
  CFL_OPEN_BEHAVIOR_CFL_ONLY = 0, //default
  CFL_OPEN_BEVAVIOR_DISK_CFL,
  CFL_OPEN_BEVAVIOR_CFL_DISK,

  //only add above this
  CFL_OPEN_BEHAVIOR_COUNT

};

//! Resource handler flags
/*! Please note that most of these have not been really implemented,
    but are specified just in case someone wants to do so.
    If you implement some, and want to contribute the implementation, 
    feel free to email me.
*/
enum CFLHANDLERFLAGS 
{
    CFLCOMPRESS_NONE  = 0x00000000, //!< No compression
    CFLCOMPRESS_ZLIB  = 0x00000001, //!< Zlib, normal compress
    CFLCOMPRESS_ZLIB9 = 0x00000901, //!< Zlib, max compress
    CFLCOMPRESS_LZSS  = 0x00000002, //!< LZSS
    CFLCOMPRESS_BZIP2 = 0x00000003, //!< BZip2
    CFLCOMPRESS_BEST  = 0x0000FFFF, //!< Tries out all registered compressors, uses best result

    CFLPREPROCESS_NONE    = 0x00000000, //!< No preprocess
    CFLPREPROCESS_DELTA8  = 0x00010000, //!< 8-bit delta encoding
    CFLPREPROCESS_DELTA16 = 0x00020000, //!< 16-bit delta encoding
    CFLPREPROCESS_DELTA32 = 0x00030000, //!< 32-bit delta encoding
    CFLPREPROCESS_BWT     = 0x00040000, //!< Burrow-Wheeler(sp?) transform
    CFLPREPROCESS_TURN8   = 0x00050000, //!< 'turn' encoding, 8bit
    CFLPREPROCESS_TURN16  = 0x00060000, //!< 'turn' encoding, 16bit
    CFLPREPROCESS_TURN24  = 0x00070000, //!< 'turn' encoding, 24bit
    CFLPREPROCESS_TURN32  = 0x00080000, //!< 'turn' encoding, 32bit
  
    CFLENCRYPT_NONE      = 0x00000000, //!< No encryption
    CFLENCRYPT_XOR       = 0x01000000, //!< Simple XOR crypt (generally stops casual hex-editor), key is one char.
    CFLENCRYPT_PRANDXOR  = 0x02000000, //!< XOR's every byte with data from pseudorandom generator, key is the random seed.
    CFLENCRYPT_STRINGXOR = 0x03000000, //!< XOR's every byte with a letter from entered string. Somewhat easy to crack if string is short, but is easy way to implement password protection.
    CFLENCRYPT_PGP       = 0x10000000, //!< Pretty Good Privacy
    CFLENCRYPT_GPG       = 0x20000000, //!< GPG
    CFLENCRYPT_DES       = 0x30000000, //!< Data Encryption Standard
    CFLENCRYPT_3DES      = 0x40000000, //!< Triple-DES
    CFLENCRYPT_BLOWFISH  = 0x50000000, //!< Blowfish
    CFLENCRYPT_IDEA      = 0x60000000, //!< IDEA
    CFLENCRYPT_RC4       = 0x70000000  //!< RC4
};

class CFLLibrary;
class CFLDirectoryEntry;

class CFL 
{

public:
    //! Constructor
    CFL();
    //! Create CFL and open lib from a file (checks offset and end of file)
    static CFL * create(const char *aLibFilename, int aOffset = 0);
    //! Create CFL and open lib from a file pointer (pointer must point at start of lib)
    static CFL * create(FILE *aLibFilePtr);
    //! Open lib from a file (checks beginning and end of file)
    int openLibrary(const char *aLibFilename);
    //! Open lib from a file (checks offset and end of file)
    int openLibrary(const char *aLibFilename, int aOffset);
    //! Open lib from a file (checks offset and end of file)
    int openLibrary(FILE *aLibFilePtr);
public:
    //! Get file data
    char * getFile(const char *aName);
    //! Get file data and data length
    char * getFile(const char *aName, int &aSize);
    //! Get file into preallocated buffer, returns bytes written
    int getFile(const char *aName, char *aBuffer, int aMaxsize);
    //! Get uncompressed file size in bytes
    int getFileSize(const char *aName);  
    //! Returns nonzero if file exists in directory
    int fileExists(const char *aName);
    //! Gets a file pointer at the beginning of (presumably) compressed data
    /*! client must use dropFilePtr to close the file pointer! */
    FILE * getFilePtr(const char *aName);
    //! drop file pointer
    void dropFilePtr(FILE *aFilePtr);
    //! get file offset to the beginning of (presumably) compressed data
    int getFileOfs(const char *aName);
    //! return printable string information on compression on this file
    const char * compressionInfoString(const char *aName);
    //! return printable string information on encryption on this file
    const char * encryptionInfoString(const char *aName);
    //! return printable string information on preprocessor on this file
    const char * preprocessorInfoString(const char *aName);
    
    void setOpenStrategy(int i_new){mOpenBehavior = i_new;}
    int getOpenStrategy(){return mOpenBehavior;}
    
    
    //! Destructor
    ~CFL();

protected:
    
    CFLLibrary *mRootLib;
    CFLDirectoryEntry *mRootDir;
    CFLDirectoryEntry * findEntry(const char *aName);
    int mOpenBehavior;
    char * ReadFileFromDisk(const char *aName, int &aSize);

};


#endif // CFL_H_INCLUDED