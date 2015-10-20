/***************************************************************************/
/* Name: Utilities.cpp                                                     */
/* Uses: Verious utilities                                                 */
/* Date: 12/5/2004                                                         */
/* Author: Punkroy (http://punkroy.drque.net/)                             */
/* Revisions:                                                              */
/*  1.0 - 12/5/2004 - OI! - Creation                                       */
/*                                                                         */
/*                This file is public domain-- have at it!                 */
/*                           (C) Copyright 2004                            */
/*                               Punkroy -:(                               */
/*                                   Oi!                                   */
/***************************************************************************/
#include "General.hpp"
#include "Utilities.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//---------------------------------------------------------------------------
// General error constructor
// Make an error string from suplied input
//---------------------------------------------------------------------------
GeneralError::GeneralError( const char *format , ... )
{
  va_list arg_ptr;

  va_start( arg_ptr , format );
  vsprintf( ErrorString , format , arg_ptr );
  va_end( arg_ptr );
}

//---------------------------------------------------------------------------
// Convert a byte buffer to hex characters
//---------------------------------------------------------------------------
char* BufferToHex( 
  char* StringBuffer , 
  unsigned char const * Buffer ,
  int BufferLength )
{
   bool IsFirst = true;
   int Index;
   char SubBuffer[ 5 ];

   // Truncate string
   StringBuffer[ 0 ] = 0;

   for ( Index = 0; Index < BufferLength; ++Index )
   {
     if ( ! IsFirst )
       strcat( StringBuffer , ":" );

     sprintf( SubBuffer , "%02X" , (int)Buffer[ Index ] );
     strcat( StringBuffer , SubBuffer );

     IsFirst = false;
   }

   return StringBuffer;
}

//---------------------------------------------------------------------------
// Return file size
//---------------------------------------------------------------------------
size_t GetFileSize( FILE* File )
{
   size_t Size;
   size_t CurrentPosition;

   // Get file size
   CurrentPosition = ftell( File );
   fseek( File , 0 , SEEK_END );
   Size = ftell( File );
   fseek( File , CurrentPosition , SEEK_SET );

   return Size;
}
