/*
Copyright (c) 2016, Conor Stokes
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LZSSE8_H__
#define LZSSE8_H__

#pragma once

/* LZSSE8 - x64/SSE targeted codec for better performance with lower compression ratio data/less optimal compressors.
* Supports minimum 4 byte matches, maximum 15 bytes of match per control word and 8 byte literal runs per control word. 
*/

/* Re-usable parse state object for compression. */
typedef struct LZSSE8_FastParseState LZSSE8_FastParseState;

/* Allocate the parse state for compression - returns null on failure */
LZSSE8_FastParseState* LZSSE8_MakeFastParseState();

/* De-allocate the parse state for compression */
void LZSSE8_FreeFastParseState( LZSSE8_FastParseState* toFree );

/* "Fast" compression routine.
* Will compress data into LZSSE8 format, uses a simple single entry hash/greedy matching to find matches. Requires SSE 4.1.
* state : Contains the hash table for matching, passed as a parameter so that allocations can be re-used. 
* input : Buffer containing uncompressed data to be compressed. May not be null.
* inputLength : Length of the compressed data in the input buffer - note should be under 2GB.
* output : Buffer that will receive the compressed output. 
* outputLength : The length reserved in the buffer for compressed data. This should be at least inputLength. Note,
*                The compressed data should never be longer than inputLength, as in this case the data is stored raw.
*
* Thread Safety - state can not be used on multiple threads with calls running concurrently. Can run multiple threads with separate state
* concurrently.
*
* Returns the size of the compressed data, or 0 in the case of error (e.g. outputLength is less than inputLength).
*/
size_t LZSSE8_CompressFast( LZSSE8_FastParseState* state, const void* input, size_t inputLength, void* output, size_t outputLength );

/* Decompression routine.
* This routine will decompress data in the LZSSE8 format and currently requires SSE 4.1 and is targeted at x64.
* It will perform poorly on x86 due to hunger for registers.
*  input : Buffer containing compressed input block. May not be null.
*  inputLength : Length of the compressed data in the input buffer - note, this should be under 2GB
*  output : Buffer that will received the de-compressed output. Note, that this needs to be at least outputLength long.
*           May not be null.
*  outputLength : The length of the compressed output - note, this should be under 2GB
*
* Provided that input and output are valid pointers to buffers of at least their specified size, this routine
* should be memory safe - both match pointer checks and input/output buffer checks exist.
*
* Returns the size of the decompressed data, which will be less than outputLength in the event of an error (number of bytes
* will indicate where in the output stream the error occured).
*
* Note that this data is not hash verified, errors that occur are either from a misformed stream or bad buffer sizes.
* Remember, corrupt data can still be valid to decompress.
*/ 
size_t LZSSE8_Decompress( const void* input, size_t inputLength, void* output, size_t outputLength );


#endif /* -- LZSSE8_H__ */
