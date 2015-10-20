/***************************************************************************/
/* Name: Cipher.cpp                                                        */
/* Uses: Basic cipher functions                                            */
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
#include "Cipher.hpp"
#include "Utilities.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <gcrypt.h>
#include <string.h>

//---------------------------------------------------------------------------
// Generate initilizing vector (IV)
// (It's just a random value)
//---------------------------------------------------------------------------
int CipherClass::CreateIV( unsigned char** IV )
{
	// (Allocate memory for and fill with strong random data)
	*IV =
			(unsigned char*)gcry_random_bytes( IV_Size , GCRY_STRONG_RANDOM );

	if ( ! *IV )
		throw GeneralError( "Unable to create IV" );

	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Initilize cipher
// We need a key (bit stream) and an initilizing vector (IV)
//---------------------------------------------------------------------------
int CipherClass::InitilizeCipher( 
		unsigned char const* Key ,
		int KeySize ,
		unsigned char const* IV )
{
	int CipherKeyLength;
	gcry_error_t Error = 0;

	// Open cipher
	Error =
			gcry_cipher_open( &Cipher , CipherAlgorithm , CipherMode , 0 );

	if ( Error )
		throw GeneralError(
				"Unable to initilize cipher: %s" , gpg_strerror( Error ) );

	// Get key length
	CipherKeyLength = gcry_cipher_get_algo_keylen( CipherAlgorithm );
	if ( ! CipherKeyLength )
		throw GeneralError( "gcry_cipher_get_algo_keylen failed" );

	// Truncate keysize
	if ( KeySize > CipherKeyLength )
		KeySize = CipherKeyLength;
	else
		if ( KeySize < CipherKeyLength )
			// Can't use a key that's too small (yet)
			throw GeneralError( "Supplied key size is too small" );

	// Set key
	Error = gcry_cipher_setkey( Cipher , Key , KeySize );
	if ( Error )
	{
		gcry_cipher_close( Cipher );
		throw GeneralError(
				"Cipher key setup failed: %s" , gpg_strerror( Error ) );
	}

	// Set Initilizeing Vector (IV)
	Error = gcry_cipher_setiv( Cipher , IV , IV_Size );
	if ( Error )
	{
		gcry_cipher_close( Cipher );
		throw GeneralError(
				"Unable to setup cipher IV: %s" , gpg_strerror( Error ) );
	}

	// All good
	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Encrypt buffer to seprate output buffer
//---------------------------------------------------------------------------
int CipherClass::EncryptBuffer( 
		unsigned char* OutputBuffer ,
		unsigned char const * InputBuffer ,
		size_t BufferLength )
{
	// Encrypt buffer
	gcry_error_t Error =
			gcry_cipher_encrypt(
					Cipher ,
					OutputBuffer ,
					BufferLength ,
					InputBuffer ,
					BufferLength );

	// Problems?
	if ( Error )
	{
		gcry_cipher_close( Cipher );
		throw GeneralError( "Encrption failed: %s" , gpg_strerror( Error ) );
	}

	return EXIT_SUCCESS;
}


//---------------------------------------------------------------------------
// Encrypt buffer to self
//---------------------------------------------------------------------------
int CipherClass::EncryptBuffer( 
		unsigned char* Buffer ,
		size_t BufferLength )
{
	// Encrypt buffer to self
	gcry_error_t Error =
			gcry_cipher_encrypt( Cipher , Buffer , BufferLength , NULL , 0 );

	if ( Error )
	{
		gcry_cipher_close( Cipher );
		throw GeneralError( "Encrption failed: %s" , gpg_strerror( Error ) );
	}

	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Decrypt buffer to seprate output buffer
//---------------------------------------------------------------------------
int CipherClass::DecryptBuffer(
		unsigned char* OutputBuffer ,
		unsigned char const * InputBuffer ,
		size_t BufferLength )
{
	// Decrypt input buffer and place in output buffer
	gcry_error_t Error =
			gcry_cipher_decrypt(
					Cipher ,
					OutputBuffer ,
					BufferLength ,
					InputBuffer ,
					BufferLength );

	if ( Error )
	{
		gcry_cipher_close( Cipher );

		throw GeneralError( "Decrypt failed: %s" , gpg_strerror( Error ) );
	}

	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Decrypt buffer to self
//---------------------------------------------------------------------------
int CipherClass::DecryptBuffer(
		unsigned char * Buffer ,
		size_t BufferLength )
{
	// Decrypt buffer to self
	gcry_error_t Error =
			gcry_cipher_decrypt(
					Cipher , Buffer , BufferLength , NULL , 0 );

	if ( Error )
	{
		gcry_cipher_close( Cipher );
		throw GeneralError( "Decrypt failed: %s" , gpg_strerror( Error ) );
	}

	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Close cipher
//---------------------------------------------------------------------------
void CipherClass::CloseCipher( void )
{
	gcry_cipher_close( Cipher );
}

//---------------------------------------------------------------------------
// Initilize hash
//---------------------------------------------------------------------------
void HashClass::Initilize()
{
	// Open message digest algorithm
	gcry_error_t Error = gcry_md_open( &Hash , HashAlgorithm , 0 );

	if ( Error )
		throw GeneralError(
				"Unable to open message digest algorithm: %s" ,
				gpg_strerror( Error ) );

	IsActive = true;
}

//---------------------------------------------------------------------------
// Add data to hash (data to be hashed)
//---------------------------------------------------------------------------
void HashClass::AddData( unsigned char* Buffer , size_t Length )
{
	if ( ! IsActive )
		throw GeneralError( "Atempt at using inactive hash" );

	// Add data to hash
	gcry_md_write( Hash , Buffer , Length );
}

//---------------------------------------------------------------------------
// Complete hash
//---------------------------------------------------------------------------
void HashClass::Finilize()
{
	if ( ! IsActive )
		throw GeneralError( "Atempt at using inactive hash" );

	// Retrive digest size
	int HashDigestSize = gcry_md_get_algo_dlen( HashAlgorithm );

	// Allocate memory for key
	// (Can't use the 'HashResult' because those resources are freed after the
	// hash is closed)
	HashResult = (unsigned char*)malloc( HashDigestSize );
	if ( ! HashResult )
	{
		gcry_md_close( Hash );
		throw GeneralError( "Unable to allocate memory for hash" );
	}

	// Fetch digest (the expanded key)
	unsigned char* TempHashResult;
	TempHashResult = gcry_md_read( Hash , HashAlgorithm );

	if ( ! TempHashResult )
	{
		gcry_md_close( Hash );
		throw GeneralError( "Unable to finilize hash" );
	}

	// Copy result to holding buffer
	memcpy( HashResult , TempHashResult , HashDigestSize );

	// Free hash
	gcry_md_close( Hash );

	IsActive = false;
}

//---------------------------------------------------------------------------
// Clear and releace all hash data
//---------------------------------------------------------------------------
void HashClass::Flush()
{
	if ( HashResult )
	{
		int HashDigestSize = gcry_md_get_algo_dlen( HashAlgorithm );
		memset( HashResult , 0 , HashDigestSize );
		free( HashResult );
	}

	HashResult = NULL;

	if ( IsActive )
		gcry_md_close( Hash );
}

//---------------------------------------------------------------------------
// Destructor (just flushs)
//---------------------------------------------------------------------------
HashClass::~HashClass()
{
	Flush();
}

//---------------------------------------------------------------------------
// Expand key
// Basiclly, we take in a passphrase and create a session key by adding 
// a "salt value" and hashing it
//---------------------------------------------------------------------------
int KeyExpantionClass::ExpandKey( 
		char const * Passphrase ,
		unsigned char const* Salt ,
		unsigned char** Key )
{
	gcry_md_hd_t Hash;
	gcry_error_t Error = 0;
	int HashDigestSize;
	unsigned char* HashResult;

	// Open message digest algorithm
	Error = gcry_md_open( &Hash , HashAlgorithm , 0 );
	if ( Error )
		throw GeneralError(
				"Unable to open message digest algorithm: %s" ,
				gpg_strerror( Error ) );

	// Retrive digest size
	HashDigestSize = gcry_md_get_algo_dlen( HashAlgorithm );

	// Add salt to hash
	gcry_md_write( Hash , Salt , SaltLength );

	// Add passphrase to hash
	gcry_md_write( Hash , Passphrase , strlen( Passphrase ) );

	// Fetch digest (the expanded key)
	HashResult = gcry_md_read( Hash , HashAlgorithm );

	if ( ! HashResult )
	{
		gcry_md_close( Hash );
		throw GeneralError( "Unable to finilize key" );
	}

	// Allocate memory for key
	// (Can't use the 'HashResult' because those resources are freed after the
	// hash is closed)
	*Key = (unsigned char*)malloc( KeySize );
	if ( ! *Key )
	{
		gcry_md_close( Hash );
		throw GeneralError( "Unable to allocate memory for key" );
	}

	// Digest size smaller then key size?
	if ( HashDigestSize < KeySize )
	{
		// Pad key with '0' at the end
		memset( *Key , 0 , KeySize );

		// Copy everything we have
		memcpy( *Key , HashResult , HashDigestSize );
	}
	else
		// Copy all the bytes we're using
		memcpy( *Key , HashResult , HashDigestSize );

	// Finished with hash
	gcry_md_close( Hash );

	return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------
// Create new expanded key
// Generates a new expanded key by creating a random salt value, then
// calling 'ExpandKey'
//---------------------------------------------------------------------------
int KeyExpantionClass::CreateNewExpandedKey( 
		char const * Passphrase ,
		unsigned char** Salt ,
		unsigned char** Key )
{
	// (Allocate memory for and fill with strong random data)
	*Salt =
			(unsigned char*)gcry_random_bytes( SaltLength , GCRY_STRONG_RANDOM );

	if ( ! *Salt )
		throw GeneralError( "Unable to create salt value" );

	return ExpandKey( Passphrase , *Salt , Key );
}
