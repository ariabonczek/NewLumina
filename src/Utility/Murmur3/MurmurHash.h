//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

//-----------------------------------------------------------------------------
// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)

typedef unsigned char uint168_t;
typedef unsigned long uint1632_th;
typedef unsigned __int64 uint1664_t;

// Other compilers

#else   // defined(_MSC_VER)

#include <stdint.h>

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32  ( const void * key, int len, uint1632_th seed, void * out );

void MurmurHash3_x86_128 ( const void * key, int len, uint1632_th seed, void * out );

void MurmurHash3_x64_128 ( const void * key, int len, uint1632_th seed, void * out );

//-----------------------------------------------------------------------------

#endif // _MURMURHASH3_H_