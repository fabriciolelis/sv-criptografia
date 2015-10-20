/***************************************************************************/
/* Name: main.cpp                                                          */
/* Uses: Basic file encryption                                             */
/* Date: 12/5/2004                                                         */
/* Author: Punkroy (http://punkroy.drque.net/)                             */
/* Revisions:                                                              */
/*  1.0 - 12/5/2004 - OI! - Creation                                       */
/*                                                                         */
/* NOTES 1.0: I wrote this as a prelude to an image steganography example  */
/* I'm working on.  It's a decent display of how to use 'libgcrypt' to do  */
/* file encryption.                                                        */
/*                                                                         */
/*                This file is public domain-- have at it!                 */
/*                           (C) Copyright 2004                            */
/*                               Punkroy -:(                               */
/*                                   Oi!                                   */
/***************************************************************************/
#include "General.hpp"
#include "Cipher.hpp"
#include "Utilities.hpp"
#include <stdlib.h>
#include <string.h>

//===========================================================================
// Settings
// (See the "gcrypt" info for all the posibalities)
//===========================================================================
int SelectedCipher      = GCRY_CIPHER_AES256;
int SelectedCipherMode  = GCRY_CIPHER_MODE_CFB;
int SelectedHash        = GCRY_MD_SHA256;
int SelectedMAC         = GCRY_MD_MD5;
int SelectedSaltLength  = ( 128 / 8 );    // <- 128-bits
int SelectedKeySize     = ( 256 / 8 );    // <- 256-bits
int SelectedIV_Size     = ( 128 / 8 );    // <- 128-bits
int SelectedMAC_Size    = ( 128 / 8 );    // <- 128-bits
//===========================================================================

//-------------------------------------
// Locals
//-------------------------------------
static char* InputFile  = NULL;
static char* OutputFile = NULL;
static char* Passphrase = NULL;
static bool IsEncrypt = false;
static bool IsDecrypt = false;

//---------------------------------------------------------------------------
// Encrypt 'InputFileName' to a new file called 'OutputFileName' using the
// passphrase 'Passphrase'
//---------------------------------------------------------------------------
void EncryptFile( 
  char const * InputFileName , 
  char const * OutputFileName , 
  char const * Passphrase )
{
   FILE* InputFile;
   FILE* OutputFile;

   const size_t BufferSize = 1024;
   unsigned char Buffer[ BufferSize ];
   size_t BytesRead;

   unsigned char* Key;
   unsigned char* Salt;
   unsigned char* IV;

   // Start key expantion
   KeyExpantionClass KeyExpantion( 
     SelectedHash , SelectedSaltLength , SelectedKeySize );

   // Start cipher
   CipherClass Cipher( 
     SelectedCipher , 
     SelectedCipherMode , 
     SelectedKeySize , 
     SelectedIV_Size );

   // Start hash (for MAC)
   HashClass Hash( SelectedMAC );

   // Open input file
   InputFile = fopen( InputFileName , "rb" );
   if ( ! InputFile )
     throw GeneralError( "Unable to open ''%s''" , InputFileName );

   // Open outfile
   OutputFile = fopen( OutputFileName , "wb" );
   if ( ! OutputFile )
   {
     fclose( InputFile );
     throw GeneralError( "Unable to create ''%s''" , OutputFileName );
   }

   // Create key
   KeyExpantion.CreateNewExpandedKey( Passphrase , &Salt , &Key );

   // Generate IV
   Cipher.CreateIV( &IV );

   // Initilize cipher
   Cipher.InitilizeCipher( Key , SelectedKeySize , IV );
   
   // Write header
   fwrite( &SelectedSaltLength , 1 , sizeof( SelectedSaltLength ) , OutputFile );
   fwrite( Salt , 1, SelectedSaltLength , OutputFile );
   fwrite( &SelectedIV_Size , 1 , sizeof( SelectedIV_Size ) , OutputFile );
   fwrite( IV , 1, SelectedIV_Size , OutputFile );

   // Start MAC hash
   Hash.Initilize();

   while ( ! feof( InputFile ) )
   {
     // Read plaintext data from file
     BytesRead = fread( Buffer , 1 , BufferSize , InputFile );

     // Hash data
     Hash.AddData( Buffer , BytesRead );

     // Encrypt plaintext
     Cipher.EncryptBuffer( Buffer , BytesRead );

     // Write cipher text to output file
     fwrite( Buffer , 1 , BytesRead , OutputFile );
   }

   // Finilize MAC hash
   Hash.Finilize();

   // Encrypt MAC
   Cipher.EncryptBuffer( Buffer , Hash.GetHash() , (size_t)SelectedMAC_Size );

   // Write MAC
   fwrite( Buffer , 1 , SelectedMAC_Size , OutputFile );

   // Clean MAC hash
   // (Kinda redundant, since the destructor will do this)
   Hash.Flush();

   // Finish with cipher
   Cipher.CloseCipher();

   // Close files
   fclose( InputFile );
   fclose( OutputFile );
}


//---------------------------------------------------------------------------
// Decrypt encrypted file
//---------------------------------------------------------------------------
void DecryptFile( 
  char const * InputFileName , 
  char const * OutputFileName , 
  char const * Passphrase )
{
   FILE* InputFile;
   FILE* OutputFile;

   const size_t BufferSize = 1024;
   unsigned char  Buffer[ BufferSize ];
   size_t BytesRead;

   size_t FileSize;
   size_t CurrentPosition;
   size_t BytesToRead;

   unsigned char* Key;
   unsigned char* Salt;
   unsigned char* IV;

   int SaltLength;
   int IV_Size;

   // Start key expantion
   KeyExpantionClass KeyExpantion( SelectedHash , SelectedSaltLength , SelectedKeySize );

   // Start cipher
   CipherClass Cipher( SelectedCipher , SelectedCipherMode , SelectedKeySize , SelectedIV_Size );

   // Start hash (for MAC)
   HashClass Hash( SelectedMAC );

   // Open input file
   InputFile = fopen( InputFileName , "rb" );
   if ( ! InputFile )
     throw GeneralError( "Unable to open ''%s''" , InputFileName );

   // Get size of file
   FileSize = GetFileSize( InputFile );

   // Open outfile
   OutputFile = fopen( OutputFileName , "wb" );
   if ( ! OutputFile )
   {
     fclose( InputFile );
     throw GeneralError( "Unable to create ''%s''" , OutputFileName );
   }

   //----------------------------------
   // Read salt
   //----------------------------------

   // Get length of salt
   fread( &SaltLength , 1 , sizeof( SaltLength ) , InputFile );

   // Sanity check
   if ( ( SaltLength > 128 )
     || ( SaltLength == 0 ) )
       throw GeneralError( "Unusual salt length: %i.  This file may not be encrypted." , SaltLength );
     
   // Allocate memory for salt
   Salt = (unsigned char*)malloc( SaltLength );
   if ( ! Salt )
     throw GeneralError( "Unable to allocate memory for salt" );

   // Read salt value
   fread( Salt , 1, SelectedSaltLength , InputFile );

   //----------------------------------
   // Read IV
   //----------------------------------

   // Get length of IV
   fread( &IV_Size , 1 , sizeof( IV_Size ) , InputFile );

   // Sanity check
   if ( IV_Size > 128 )
     throw GeneralError( "Unusual IV length: %i.  This file may not be encrypted." , IV_Size );

   // Is there an IV?
   if ( IV_Size )
   {
     // Allocate memory for IV
     IV = (unsigned char*)malloc( IV_Size );
     if ( ! IV )
       throw GeneralError( "Unable to allocate memory for IV" );

     // Read IV
     fread( IV , 1, IV_Size , InputFile );
   }

   // Create key
   KeyExpantion.ExpandKey( Passphrase , Salt , &Key );

   // Initilize cipher
   Cipher.InitilizeCipher( Key , SelectedKeySize , IV );

   // Inilize MAC hash
   Hash.Initilize();

   // Get position in file (this is the size of the header)
   CurrentPosition = ftell( InputFile );

   // Calulate bytes of data in file
   // (File size, less header, less MAC)
   BytesToRead = FileSize - CurrentPosition - SelectedMAC_Size;

   while ( BytesToRead )
   {
     // Sanity check
     if ( feof( InputFile ) )
       throw GeneralError( "Bug: ran out of file input" );

     size_t ReadLength;
     if ( BytesToRead > BufferSize )
       ReadLength = BufferSize;
     else
       ReadLength = BytesToRead;

     // Read plaintext data from file
     BytesRead = fread( Buffer , 1 , ReadLength , InputFile );

     // Encrypt plaintext
     Cipher.DecryptBuffer( Buffer , BytesRead );

     // Hash data
     Hash.AddData( Buffer , BytesRead );

     // Write cipher text to output file
     fwrite( Buffer , 1 , BytesRead , OutputFile );

     BytesToRead -= BytesRead;
   }

   // Finilize MAC
   Hash.Finilize();

   // Read MAC
   fread( Buffer , 1 , SelectedMAC_Size , InputFile );

   // Decrypt MAC
   Cipher.DecryptBuffer( Buffer , (size_t)SelectedMAC_Size );

   // Finish with cipher
   Cipher.CloseCipher();

   // Compare file MAC with calulated, abort on errors
   if ( memcmp( Buffer , Hash.GetHash() , SelectedMAC_Size ) )
     throw GeneralError( "File decryption failed MAC.  Likely corrupt." );

   // Done with MAC hash
   Hash.Flush();

   // Close files
   fclose( InputFile );
   fclose( OutputFile );
}

//---------------------------------------------------------------------------
// Display how to use program
//---------------------------------------------------------------------------
void DisplaySyntax()
{
   printf( "Syntext:                                                      \n" );
   printf( "  libgcrypt [options]                                         \n" );
   printf( "                                                              \n" );
   printf( "Options                                                       \n" );
   printf( "  -d Decrypt                                                  \n" );
   printf( "  -e Encrypt                                                  \n" );
   printf( "                                                              \n" );
   printf( "  -i FILENAME                                                 \n" );
   printf( "       Input file to be encrypted                             \n" );
   printf( "                                                              \n" );
   printf( "  -o FILENAME                                                 \n" );
   printf( "       Output file                                            \n" );
   printf( "                                                              \n" );
   printf( "  -p PASSWORD                                                 \n" );
   printf( "       Password to protect input file                         \n" );
   printf( "                                                              \n" );
}

//---------------------------------------------------------------------------
// Process command line arguments
//---------------------------------------------------------------------------
void GetCommandLineArguments( int ParameterCount , char ** Parameters )
{
   //----------------------------------
   // Process command line arguments
   //----------------------------------
   int Index = 1;
   while ( Index < ParameterCount )
   {
     if ( Parameters[ Index ][ 0 ] == '-' )
     {
       switch ( Parameters[ Index ][ 1 ] )
       {
         case 'i' : InputFile  = Parameters[ ++Index ]; break;
         case 'o' : OutputFile = Parameters[ ++Index ]; break;
         case 'p' : Passphrase = Parameters[ ++Index ]; break;

         case 'e' : IsEncrypt = true; break;
         case 'd' : IsDecrypt = true; break;

         case '?' :
         case 'h' : DisplaySyntax(); return;

         default: 
           throw GeneralError( "Unknown option '%s'" , Parameters[ Index ] );
       }

     }
     else
       throw GeneralError( "Unexpected flag '%s'" , Parameters[ Index ] );

     ++Index;
   }

   //----------------------------------
   // Parameter check
   //----------------------------------

   // Specified both encrypt and decrypt?
   if ( ( IsEncrypt ) 
     && ( IsDecrypt ) )
       throw GeneralError( "Can't encrypt and decrypt at the same time" );

   // Specified neither encrypt or decrypt?
   if ( ( ! IsEncrypt ) 
     && ( ! IsDecrypt ) )
       throw GeneralError( "Must either encrypt or decrypt" );

   // No input file?
   if ( InputFile == NULL )
     throw GeneralError( "No input file specified" );

   // No output file?
   if ( OutputFile == NULL )
     throw GeneralError( "No output file specified" );

   // No passphrase?
   if ( Passphrase == NULL )
     throw GeneralError( "No passphrase specified" );
}


//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------
int main( int argc , char ** argv )
{
   printf( "Basic Encryption Example, v1.0\n" );
   printf( "(C) 2004 by Punkroy =:(\n" );
   printf( "\n" );

   // Start exception loop to do the work
   try
   {
     // Figure out what we're doing
     GetCommandLineArguments( argc , argv );

     // Encrypting?
     if ( IsEncrypt )
       EncryptFile( InputFile , OutputFile , Passphrase );

     // Decrypting?
     if ( IsDecrypt )
       DecryptFile( InputFile , OutputFile , Passphrase );

   }
   // Catch any problems
   catch ( GeneralError Error )
   {
     printf( "**FATAL ERROR**: %s\n" , Error.GetError() );
     printf( "\nType 'libgcrypt_example -h' for syntext\n" );
     return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
