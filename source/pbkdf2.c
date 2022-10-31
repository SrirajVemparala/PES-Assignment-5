/*
 * pbkdf2.c
 *
 * A perfectly legitimate implementation of HMAC and PBKDF2, but based
 * on the "ISHA" insecure and bad hashing algorithm.
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 */

#include <assert.h>

#include "pbkdf2.h"
#include "assemblyprocessmsgblk.h"

//flag Variable is used to enure reset and input for key_pad happens once.
uint8_t flag = 0;
/*
 * See function description in pbkdf2.h
 */
void hmac_isha(const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len,
    uint8_t *digest)
{

	//Static is used so that it stores the data
	static uint8_t ipad[ISHA_BLOCKLEN];
	//Static is used so that it stores the data.
	static  uint8_t opad[ISHA_BLOCKLEN];
	uint8_t keypad[ISHA_BLOCKLEN];
	uint8_t inner_digest[ISHA_DIGESTLEN];
	//register is used to store i in registers and make it work faster.
	register size_t i;
	ISHAContext ctx;
	static ISHAContext innerpad;//Added for computing reset and input for ipad
	static ISHAContext outerpad;//Added for computing reset and input for opad
	if(flag == 0)
	{
		memcpy(keypad,key,key_len);
		memset(keypad+key_len,0,ISHA_BLOCKLEN-key_len);


  // XOR key into ipad and opad
		for (i=0; i<key_len; i++)
		{
			ipad[i] = keypad[i] ^ 0x36;
			opad[i] = keypad[i] ^ 0x5c;
		}
		//used memset for to reduce the time
		memset(ipad+key_len,0x36,ISHA_BLOCKLEN-key_len);
		//used memset for to reduce the time
		memset(opad+key_len,0x5C,ISHA_BLOCKLEN-key_len);
		//The below function is required to be called only once
		ISHAReset(&innerpad);
		//The below function is required to be called only once
		ISHAInput(&innerpad, ipad, ISHA_BLOCKLEN);
		//The below function is required to be called only once
		ISHAReset(&outerpad);
		//The below function is required to be called only once
		ISHAInput(&outerpad, opad, ISHA_BLOCKLEN);

	}
	else
	{
		/*Do Nothing*/
	}
	//copying the structure values of innerpad to ctx
	ctx = innerpad;
	ISHAInput(&ctx, msg, msg_len);
	ISHAResult(&ctx, inner_digest);
	//copying the structure values of outerpad to ctx
	ctx = outerpad;
	ISHAInput(&ctx, inner_digest, ISHA_DIGESTLEN);
	ISHAResult(&ctx, digest);
}


/*
 * Implements the F function as defined in RFC 8018 section 5.2
 *
 * Parameters:
 *   pass      The password
 *   pass_len  length of pass
 *   salt      The salt
 *   salt_len  length of salt
 *   iter      The iteration count ("c" in RFC 8018)
 *   blkidx    the block index ("i" in RFC 8018)
 *   result    The result, which is ISHA_DIGESTLEN bytes long
 * 
 * Returns:
 *   The result of computing the F function, in result
 */
static void F(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len,
    int iter, unsigned int blkidx, uint8_t *result)
{

  uint8_t temp[ISHA_DIGESTLEN];
  uint8_t saltplus[2048];
  //used to reverse the array values
  unsigned int swap[4];
  assert(salt_len + 4 <= sizeof(saltplus));

  	 //Used to copy the values from salt to saltplus
  	memcpy(saltplus,salt,salt_len);
  	//To reverse the array
	for(int i =0;i<4;i++)
	{
		swap[i] = __builtin_bswap32(blkidx);
	}
	//Store the swapped value into saltplus+salt_len
	memcpy(saltplus+salt_len,swap,sizeof(swap));

	hmac_isha(pass, pass_len, saltplus, salt_len+4, temp);
	flag =1;
	//memcpy to copy the temp value to result
	memcpy(result,temp,ISHA_DIGESTLEN);

	//register is added to make the access to the value to j faster.
	register int j = 1;
	while(j<iter)
    {
         hmac_isha(pass, pass_len, temp, ISHA_DIGESTLEN, temp);
         //Stored the value of temp into final result
		  	  result[0] ^= temp[0];
		  	  result[1] ^= temp[1];
	    	  result[2] ^= temp[2];
	    	  result[3] ^= temp[3];
	    	  result[4] ^= temp[4];
	    	  result[5] ^= temp[5];
	    	  result[6] ^= temp[6];
	    	  result[7] ^= temp[7];
	    	  result[8] ^= temp[8];
	    	  result[9] ^= temp[9];
	    	  result[10] ^= temp[10];
	    	  result[11] ^= temp[11];
	    	  result[12] ^= temp[12];
	    	  result[13] ^= temp[13];
	    	  result[14] ^= temp[14];
	    	  result[15] ^= temp[15];
	    	  result[16] ^= temp[16];
	    	  result[17] ^= temp[17];
	    	  result[18] ^= temp[18];
	    	  result[19] ^= temp[19];
	    	  j++;
    }

}


/*
 * See function description in pbkdf2.h
 */
void pbkdf2_hmac_isha(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len, int iter, size_t dkLen, uint8_t *DK)
{
  uint8_t accumulator[2560];
  assert(dkLen < sizeof(accumulator));

  int l = dkLen / ISHA_DIGESTLEN + 1;
  for (int i=0; i<l; i++)
  {
    F(pass, pass_len, salt, salt_len, iter, i+1, accumulator+i*ISHA_DIGESTLEN);
  }
 	 flag =0;//set to zero after the complete execution
     memcpy(DK,accumulator,dkLen);
}



