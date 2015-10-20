/***************************************************************************/
/*                This file is public domain-- have at it!                 */
/*                           (C) Copyright 2004                            */
/*                               Punkroy -:(                               */
/*                                   Oi!                                   */
/***************************************************************************/
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

//-------------------------------------
// Generic error class
// (So you can throw it in an exception)
//-------------------------------------
class GeneralError
{
  protected:
   char ErrorString[ 256 ];
  public:
   GeneralError( const char *format , ... );
   char const * GetError() const
   {
     return ErrorString;
   }
};

char* BufferToHex( char* StringBuffer , unsigned char const * Buffer , int BufferLength );

size_t GetFileSize( FILE* File );

#endif // UTILITIES_HPP
