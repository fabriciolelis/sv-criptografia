/***************************************************************************/
/* NOTES: The reason this file/unit exists is 'cuz I wanted to throw       */
/* exceptions on error and I figured, hey, if I'm throwing exceptions,     */
/* might as well use some objects too.  So, whatever.                      */
/*                                                                         */
/*                This file is public domain-- have at it!                 */
/*                           (C) Copyright 2004                            */
/*                               Punkroy -:(                               */
/*                                   Oi!                                   */
/***************************************************************************/
#ifndef CIPHER_HPP    
#define CIPHER_HPP

#include "General.hpp"
#include <gcrypt.h>

//-------------------------------------
// Generic cipher class
//-------------------------------------
class CipherClass
{
  private:
   int CipherAlgorithm;
   int CipherMode;
   int KeySize;
   int IV_Size;
   gcry_cipher_hd_t Cipher;
  public:
   CipherClass(
     int CipherAlgorithmParameter ,
     int CipherModeParameter ,
     int KeySizeParameter ,
     int IV_SizeParameter )
   : CipherAlgorithm( CipherAlgorithmParameter     ) ,
     CipherMode( CipherModeParameter ) ,
     KeySize( KeySizeParameter ) ,
     IV_Size( IV_SizeParameter )
   {}

   int CreateIV( unsigned char** IV );
   int InitilizeCipher( unsigned char const* Key , int KeySize , unsigned char const* IV );

   int EncryptBuffer( unsigned char* OutputBuffer , unsigned char const * InputBuffer , size_t BufferLength );
   int EncryptBuffer( unsigned char* Buffer , size_t BufferLength );

   int DecryptBuffer( unsigned char* OutputBuffer , unsigned char const * InputBuffer , size_t BufferLength );
   int DecryptBuffer( unsigned char* Buffer , size_t BufferLength );
   void CloseCipher();
};

//-------------------------------------
// Generic hash class
//-------------------------------------
class HashClass
{
  private:
   gcry_md_hd_t Hash;
   int HashAlgorithm;
   unsigned char* HashResult;
   bool IsActive;
  public:
   HashClass( int HashAlgorithmParameter ) 
   : HashAlgorithm( HashAlgorithmParameter  ) ,
     HashResult( NULL ) ,
     IsActive( false )
   {};

   void Initilize();
   void AddData( unsigned char* Buffer , size_t Length );
   void Finilize();
   void Flush();
   unsigned char const * GetHash() const
   { 
     return HashResult;
   }

   ~HashClass();
};

//-------------------------------------
// Key expantion class
//-------------------------------------
class KeyExpantionClass
{
  private:
   int HashAlgorithm;
   int SaltLength;
   int KeySize;
  public:
   KeyExpantionClass( 
     int HashAlgorithmParameter ,
     int SaltLengthParameter ,
     int KeySizeParameter ) 
   : HashAlgorithm( HashAlgorithmParameter  ) ,
     SaltLength(    SaltLengthParameter ) ,
     KeySize(       KeySizeParameter    )
   {};
   int ExpandKey( char const * Passphrase , unsigned char const* Salt , unsigned char** Key );
   int CreateNewExpandedKey( char const * Passphrase , unsigned char** Salt , unsigned char** Key );
};

#endif // CIPHER_HPP
