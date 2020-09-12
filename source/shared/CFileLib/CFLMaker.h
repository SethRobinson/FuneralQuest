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

//! You can create new CFL files with this class.
class CFLMaker 
{
protected:
    //! Constructor
    CFLMaker();
    //! Destructor
    ~CFLMaker();
    //! Linked list of directory entries
    CFLDirectoryEntry * mRootDir;
    //! Target file pointer
    FILE * mFilePtr;
public:
    //! Used to create CFLMaker class and to open the target file.
    static CFLMaker * create(const char *aTargetFilename);  
    //! Used to store a data block in the file.
    int store(const char *aFilename, const char *aData, int aDataSize, unsigned int aCompressionFlags);
    //! Used to finish the library construction, compress the library info, and destroy the CFLMaker object.
    int finish(unsigned int aLibraryCompressionFlags); 
};
