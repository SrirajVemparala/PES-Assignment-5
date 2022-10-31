/*
 * isha.c
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 */

#include "isha.h"
#include "assemblyprocessmsgblk.h"

/*
 * circular shift macro
 */
#define ISHACircularShift(bits,word) \
  ((((word) << (bits)) & 0xFFFFFFFF) | ((word) >> (32-(bits))))

/*  
 * The message must be padded to an even 512 bits.  The first padding
 * bit must be a '1'.  The last 64 bits represent the length of the
 * original message.  All bits in between should be 0. This function
 * will pad the message according to those rules by filling the MBlock
 * array accordingly. It will also call ISHAProcessMessageBlock()
 * appropriately. When it returns, it can be assumed that the message
 * digest has been computed.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAPadMessage(ISHAContext *ctx)
{
  /*
   *  Check to see if the current message block is too small to hold
   *  the initial padding bits and length.  If so, we will pad the
   *  block, process it, and then continue padding into a second
   *  block.
   */
  if (ctx->MB_Idx > 55)
  {
    ctx->MBlock[ctx->MB_Idx++] = 0x80;
    memset((ctx->MBlock)+57,0x00,64-ctx->MB_Idx);
    ctx->MB_Idx = 64;
    ISHAProcessMessageBlock(ctx);

    while(ctx->MB_Idx < 56)
    {
      ctx->MBlock[ctx->MB_Idx++] = 0;
    }

  }
  else
  {
    ctx->MBlock[ctx->MB_Idx++] = 0x80;
    memset(ctx->MBlock+ctx->MB_Idx,0x00,56-ctx->MB_Idx);
    ctx->MB_Idx = 56;
  }

  /*
   *  Store the message length as the last 8 octets
   */
  ctx->MBlock[56] = 0;
  ctx->MBlock[57] = 0;
  ctx->MBlock[58] = 0;
  ctx->MBlock[59] = 0;
  ctx->MBlock[60] = (ctx->Length_Low >> 24) & 0xFF;
  ctx->MBlock[61] = (ctx->Length_Low >> 16) & 0xFF;
  ctx->MBlock[62] = (ctx->Length_Low >> 8) & 0xFF;
  ctx->MBlock[63] = (ctx->Length_Low) & 0xFF;

  ISHAProcessMessageBlock(ctx);
}


void ISHAReset(ISHAContext *ctx)
{
  ctx->Length_Low  = 0;
  ctx->MB_Idx      = 0;

  ctx->MD[0]       = 0x67452301;
  ctx->MD[1]       = 0xEFCDAB89;
  ctx->MD[2]       = 0x98BADCFE;
  ctx->MD[3]       = 0x10325476;
  ctx->MD[4]       = 0xC3D2E1F0;

  ctx->Computed    = 0;
  ctx->Corrupted   = 0;
}


void ISHAResult(ISHAContext *ctx, uint8_t *digest_out)
{
  uint32_t endianconv[5];
  if (!ctx->Computed)
  {
    ISHAPadMessage(ctx);
    ctx->Computed = 1;
  }
  for(int i =0;i<5;i++)
  {
	endianconv[i] = __builtin_bswap32(ctx->MD[i]);
  }
	memcpy(digest_out,endianconv,sizeof(endianconv));
	return;
}


void ISHAInput(ISHAContext *ctx, const uint8_t *message_array, size_t length)
{
  if (!length)
  {
    return;
  }
  ctx->Length_Low += length<<3;

   while(length--)
   {
    	ctx->MBlock[ctx->MB_Idx++] = (*(message_array++) & 0xFF);

    	if (ctx->MB_Idx == 64)
    	{
    		ISHAProcessMessageBlock(ctx);
    	}
   }
}


