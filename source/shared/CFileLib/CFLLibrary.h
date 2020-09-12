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
class CFLLibrary
{
public:
    //! file pointer for libraries that are opened with file pointer for some reason.
    /*! normally NULL; cfl doesn't keep files open for no reason.*/
    FILE *mFilePtr;
    //! filename to the library, or NULL if fileptr is not NULL.
    char *mFilename;
    //! offset to the library, from start of file (0 for standalone cfl's)
    int mOffset;
    //! pointer to the next library
    CFLLibrary *mNext;
    //! Constructor
    CFLLibrary();
    //! Destructor
    ~CFLLibrary();
    //! opens the library file if fileptr is NULL, otherwise returns it
    FILE * open();
    //! closes the pointer if fileptr is NULL, otherwise does nothing
    void close(FILE *aFilePtr);
};
