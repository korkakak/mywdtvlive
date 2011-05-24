/****************************************************************************
 * Ralink Tech Inc.
 * Taiwan, R.O.C.
 *
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************/


/****************************************************************************
    Module Name:
    AES

    Abstract:
    FIPS PUBS 197: ADVANCED ENCRYPTION STANDARD (AES)
    NIST 800-38A: Recommendation for Block Cipher Modes of Operation
    
    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2009/01/19      Create AES-128, AES-192, AES-256, AES-CBC
***************************************************************************/

#ifndef __CRYPT_AES_H__
#define __CRYPT_AES_H__

#ifdef CRYPT_TESTPLAN
#include "crypt_testplan.h"
#else
#include "rt_config.h"
#endif /* CRYPT_TESTPLAN */

/* AES definition & structure */
#define AES_STATE_ROWS 4     /* Block size: 4*4*8 = 128 bits */
#define AES_STATE_COLUMNS 4
#define AES_BLOCK_SIZES AES_STATE_ROWS*AES_STATE_COLUMNS
#define AES_KEY_ROWS 4
#define AES_KEY_COLUMNS 8    /*Key length: 4*{4,6,8}*8 = 128, 192, 256 bits */
#define AES_KEY128_LENGTH 16
#define AES_KEY192_LENGTH 24
#define AES_KEY256_LENGTH 32
#define AES_CBC_IV_LENGTH 16

typedef struct {
    UINT8 State[AES_STATE_ROWS][AES_STATE_COLUMNS];
    UINT8 KeyWordExpansion[AES_KEY_ROWS][AES_KEY_ROWS*((AES_KEY256_LENGTH >> 2) + 6 + 1)];
} AES_CTX_STRUC, *PAES_CTX_STRUC;


/* AES operations */
VOID AES_KeyExpansion (
    IN UINT8 Key[],
    IN UINT KeyLength,
    INOUT AES_CTX_STRUC *paes_ctx);

VOID AES_Encrypt (
    IN UINT8 PlainBlock[],
    IN UINT PlainBlockSize,
    IN UINT8 Key[],
    IN UINT KeyLength,
    OUT UINT8 CipherBlock[],
    INOUT UINT *CipherBlockSize);

VOID AES_Decrypt (
    IN UINT8 CipherBlock[],
    IN UINT CipherBlockSize,
    IN UINT8 Key[],
    IN UINT KeyLength,
    OUT UINT8 PlainBlock[],
    INOUT UINT *PlainBlockSize);

/* AES-CBC operations */
VOID AES_CBC_Encrypt (
    IN UINT8 PlainText[],
    IN UINT PlainTextLength,
    IN UINT8 Key[],
    IN UINT KeyLength,
    IN UINT8 IV[],
    IN UINT IVLength,
    OUT UINT8 CipherText[],
    INOUT UINT *CipherTextLength);

VOID AES_CBC_Decrypt (
    IN UINT8 CipherText[],
    IN UINT CipherTextLength,
    IN UINT8 Key[],
    IN UINT KeyLength,
    IN UINT8 IV[],
    IN UINT IVLength,
    OUT UINT8 PlainText[],
    INOUT UINT *PlainTextLength);

#endif /* __CRYPT_AES_H__ */

