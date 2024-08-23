
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
#ifndef _QUEC_SHA1_H
#define _QUEC_SHA1_H

/*****************************************************************
* Function: ql_hmac_sha1
*
* Description:
* HMACSHA1 encryption algorithm
* The hash function mixes the key with the message data and uses the hash function to hash the mixed result.
* 
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* data           [in]  message data
* data_length    [in]  message data length
* digest         [out] hash result

* Return:
* void
*
*****************************************************************/
void ql_hmac_sha1(unsigned char *key, int key_length, unsigned char *data, int data_length, unsigned char *digest);

#endif
