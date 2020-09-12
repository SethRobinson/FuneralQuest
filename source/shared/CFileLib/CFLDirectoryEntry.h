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

//! This class is used internally to CFL
/*! all data members are made public instead of using access methods;
    it is assumed that CFL itself does not mess things up. No external
    access to the data is given.
 */
class CFLDirectoryEntry
{
public:
    //! name is stored as short int size + char string
    char *mName;
    //! Compression type
    unsigned int mCompression;
    //! Uncompressed size of the data
    unsigned int mUnpackedSize;
    //! Offset to the data
    unsigned int mOffset;
    // Hash value of the name (for faster searching), calculated runtime
    int mNameHash;
    //! Pointer to the library in which this directory entry lies
    CFLLibrary *mLib;
    //! pointer to the next entry
    CFLDirectoryEntry *mNext;
    //! How many bytes does it take to store this entry on disk.
    int byteRequirement();
    //! Used to encode entry to buffer
    void store(char *aBuffer, int &aOffset); 
    //! Used to load entry from buffer
    static CFLDirectoryEntry * restore(char *aBuffer, int &aOffset); 
    //! Destructor
    ~CFLDirectoryEntry();
    //! Compares this directory entry to a name and hash.
    int equals(const char *aName, int aHash);
    //! Calculates hash from string
    static int calcHash(const char *aName);
    //! replaces this entry with new one.
    void replaceEntry(CFLDirectoryEntry *aReplacement);
};
