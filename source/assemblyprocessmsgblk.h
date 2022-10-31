#ifndef ASSEMBLYPROCESSMSGBLK_H_
#define ASSEMBLYPROCESSMSGBLK_H_

#include "isha.h"
/*
 * Processes the next 512 bits of the message stored in the MBlock
 * array.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
void ISHAProcessMessageBlock(ISHAContext *ctx);
#endif /* ASSEMBLYPROCESSMSGBLK_H_ */
