/*
 * Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_FREE_COPYRIGHT@
 */

#include <pexpert/pexpert.h>
#include <pexpert/protos.h>
#include <pexpert/device_tree.h>
#include <kern/debug.h>

static int DEBUGFlag;

int32_t gPESerialBaud = -1;

void pe_init_debug(void)
{
  if (!PE_parse_boot_argn("debug", &DEBUGFlag, sizeof (DEBUGFlag)))
    DEBUGFlag = 0;
}

void PE_enter_debugger(const char *cause)
{
  if (DEBUGFlag & DB_NMI)
    Debugger(cause);
}

/* extern references */
extern void vcattach(void);

/* Globals */
void (*PE_putc)(char c);

void PE_init_printf(boolean_t vm_initialized)
{
  if (!vm_initialized) {
    PE_putc = cnputc;
  } else {
    vcattach();
  }
}

uint32_t
PE_get_random_seed(unsigned char *dst_random_seed, uint32_t request_size)
{
	DTEntry		entryP;
	uint32_t	size = 0;
	void		*dt_random_seed;

        if ((DTLookupEntry(NULL, "/chosen", &entryP) == kSuccess)
	    && (DTGetProperty(entryP, "random-seed",
				(void **)&dt_random_seed, &size) == kSuccess)) {
		unsigned char *src_random_seed;
		unsigned int i;
		unsigned int null_count = 0;

		src_random_seed = (unsigned char *)dt_random_seed;

		if (size > request_size) size = request_size;

		/*
		 * Copy from the device tree into the destination buffer,
		 * count the number of null bytes and null out the device tree.
		 */
		for (i=0 ; i< size; i++, src_random_seed++, dst_random_seed++) {
			*dst_random_seed = *src_random_seed;
			null_count += *src_random_seed == (unsigned char)0;
			*src_random_seed = (unsigned char)0;
		}
		if (null_count == size)
			/* All nulls is no seed - return 0 */
			size = 0;
	}

	return(size);
}

unsigned char appleClut8[ 256 * 3 ] = {
// 00
	0xFF,0xFF,0xFF, 0xFF,0xFF,0xCC,	0xFF,0xFF,0x99,	0xFF,0xFF,0x66,
	0xFF,0xFF,0x33, 0xFF,0xFF,0x00,	0xFF,0xCC,0xFF,	0xFF,0xCC,0xCC,
	0xFF,0xCC,0x99, 0xFF,0xCC,0x66,	0xFF,0xCC,0x33,	0xFF,0xCC,0x00,
	0xFF,0x99,0xFF, 0xFF,0x99,0xCC,	0xFF,0x99,0x99,	0xFF,0x99,0x66,
// 10
	0xFF,0x99,0x33, 0xFF,0x99,0x00,	0xFF,0x66,0xFF,	0xFF,0x66,0xCC,
	0xFF,0x66,0x99, 0xFF,0x66,0x66,	0xFF,0x66,0x33,	0xFF,0x66,0x00,
	0xFF,0x33,0xFF, 0xFF,0x33,0xCC,	0xFF,0x33,0x99,	0xFF,0x33,0x66,
	0xFF,0x33,0x33, 0xFF,0x33,0x00,	0xFF,0x00,0xFF,	0xFF,0x00,0xCC,
// 20
	0xFF,0x00,0x99, 0xFF,0x00,0x66,	0xFF,0x00,0x33,	0xFF,0x00,0x00,
	0xCC,0xFF,0xFF, 0xCC,0xFF,0xCC,	0xCC,0xFF,0x99,	0xCC,0xFF,0x66,
	0xCC,0xFF,0x33, 0xCC,0xFF,0x00,	0xCC,0xCC,0xFF,	0xCC,0xCC,0xCC,
	0xCC,0xCC,0x99, 0xCC,0xCC,0x66,	0xCC,0xCC,0x33,	0xCC,0xCC,0x00,
// 30
	0xCC,0x99,0xFF, 0xCC,0x99,0xCC,	0xCC,0x99,0x99,	0xCC,0x99,0x66,
	0xCC,0x99,0x33, 0xCC,0x99,0x00,	0xCC,0x66,0xFF,	0xCC,0x66,0xCC,
	0xCC,0x66,0x99, 0xCC,0x66,0x66,	0xCC,0x66,0x33,	0xCC,0x66,0x00,
	0xCC,0x33,0xFF, 0xCC,0x33,0xCC,	0xCC,0x33,0x99,	0xCC,0x33,0x66,
// 40
	0xCC,0x33,0x33, 0xCC,0x33,0x00,	0xCC,0x00,0xFF,	0xCC,0x00,0xCC,
	0xCC,0x00,0x99, 0xCC,0x00,0x66,	0xCC,0x00,0x33,	0xCC,0x00,0x00,
	0x99,0xFF,0xFF, 0x99,0xFF,0xCC,	0x99,0xFF,0x99,	0x99,0xFF,0x66,
	0x99,0xFF,0x33, 0x99,0xFF,0x00,	0x99,0xCC,0xFF,	0x99,0xCC,0xCC,
// 50
	0x99,0xCC,0x99, 0x99,0xCC,0x66,	0x99,0xCC,0x33,	0x99,0xCC,0x00,
	0x99,0x99,0xFF, 0x99,0x99,0xCC,	0x99,0x99,0x99,	0x99,0x99,0x66,
	0x99,0x99,0x33, 0x99,0x99,0x00,	0x99,0x66,0xFF,	0x99,0x66,0xCC,
	0x99,0x66,0x99, 0x99,0x66,0x66,	0x99,0x66,0x33,	0x99,0x66,0x00,
// 60
	0x99,0x33,0xFF, 0x99,0x33,0xCC,	0x99,0x33,0x99,	0x99,0x33,0x66,
	0x99,0x33,0x33, 0x99,0x33,0x00,	0x99,0x00,0xFF,	0x99,0x00,0xCC,
	0x99,0x00,0x99, 0x99,0x00,0x66,	0x99,0x00,0x33,	0x99,0x00,0x00,
	0x66,0xFF,0xFF, 0x66,0xFF,0xCC,	0x66,0xFF,0x99,	0x66,0xFF,0x66,
// 70
	0x66,0xFF,0x33, 0x66,0xFF,0x00,	0x66,0xCC,0xFF,	0x66,0xCC,0xCC,
	0x66,0xCC,0x99, 0x66,0xCC,0x66,	0x66,0xCC,0x33,	0x66,0xCC,0x00,
	0x66,0x99,0xFF, 0x66,0x99,0xCC,	0x66,0x99,0x99,	0x66,0x99,0x66,
	0x66,0x99,0x33, 0x66,0x99,0x00,	0x66,0x66,0xFF,	0x66,0x66,0xCC,
// 80
	0x66,0x66,0x99, 0x66,0x66,0x66,	0x66,0x66,0x33,	0x66,0x66,0x00,
	0x66,0x33,0xFF, 0x66,0x33,0xCC,	0x66,0x33,0x99,	0x66,0x33,0x66,
	0x66,0x33,0x33, 0x66,0x33,0x00,	0x66,0x00,0xFF,	0x66,0x00,0xCC,
	0x66,0x00,0x99, 0x66,0x00,0x66,	0x66,0x00,0x33,	0x66,0x00,0x00,
// 90
	0x33,0xFF,0xFF, 0x33,0xFF,0xCC,	0x33,0xFF,0x99,	0x33,0xFF,0x66,
	0x33,0xFF,0x33, 0x33,0xFF,0x00,	0x33,0xCC,0xFF,	0x33,0xCC,0xCC,
	0x33,0xCC,0x99, 0x33,0xCC,0x66,	0x33,0xCC,0x33,	0x33,0xCC,0x00,
	0x33,0x99,0xFF, 0x33,0x99,0xCC,	0x33,0x99,0x99,	0x33,0x99,0x66,
// a0
	0x33,0x99,0x33, 0x33,0x99,0x00,	0x33,0x66,0xFF,	0x33,0x66,0xCC,
	0x33,0x66,0x99, 0x33,0x66,0x66,	0x33,0x66,0x33,	0x33,0x66,0x00,
	0x33,0x33,0xFF, 0x33,0x33,0xCC,	0x33,0x33,0x99,	0x33,0x33,0x66,
	0x33,0x33,0x33, 0x33,0x33,0x00,	0x33,0x00,0xFF,	0x33,0x00,0xCC,
// b0
	0x33,0x00,0x99, 0x33,0x00,0x66,	0x33,0x00,0x33,	0x33,0x00,0x00,
	0x00,0xFF,0xFF, 0x00,0xFF,0xCC,	0x00,0xFF,0x99,	0x00,0xFF,0x66,
	0x00,0xFF,0x33, 0x00,0xFF,0x00,	0x00,0xCC,0xFF,	0x00,0xCC,0xCC,
	0x00,0xCC,0x99, 0x00,0xCC,0x66,	0x00,0xCC,0x33,	0x00,0xCC,0x00,
// c0
	0x00,0x99,0xFF, 0x00,0x99,0xCC,	0x00,0x99,0x99,	0x00,0x99,0x66,
	0x00,0x99,0x33, 0x00,0x99,0x00,	0x00,0x66,0xFF,	0x00,0x66,0xCC,
	0x00,0x66,0x99, 0x00,0x66,0x66,	0x00,0x66,0x33,	0x00,0x66,0x00,
	0x00,0x33,0xFF, 0x00,0x33,0xCC,	0x00,0x33,0x99,	0x00,0x33,0x66,
// d0
	0x00,0x33,0x33, 0x00,0x33,0x00,	0x00,0x00,0xFF,	0x00,0x00,0xCC,
	0x00,0x00,0x99, 0x00,0x00,0x66,	0x00,0x00,0x33,	0xEE,0x00,0x00,
	0xDD,0x00,0x00, 0xBB,0x00,0x00,	0xAA,0x00,0x00,	0x88,0x00,0x00,
	0x77,0x00,0x00, 0x55,0x00,0x00,	0x44,0x00,0x00,	0x22,0x00,0x00,
// e0
	0x11,0x00,0x00, 0x00,0xEE,0x00,	0x00,0xDD,0x00,	0x00,0xBB,0x00,
	0x00,0xAA,0x00, 0x00,0x88,0x00,	0x00,0x77,0x00,	0x00,0x55,0x00,
	0x00,0x44,0x00, 0x00,0x22,0x00,	0x00,0x11,0x00,	0x00,0x00,0xEE,
	0x00,0x00,0xDD, 0x00,0x00,0xBB,	0x00,0x00,0xAA,	0x00,0x00,0x88,
// f0
	0x00,0x00,0x77, 0x00,0x00,0x55,	0x00,0x00,0x44,	0x00,0x00,0x22,
	0x00,0x00,0x11, 0xEE,0xEE,0xEE,	0xDD,0xDD,0xDD,	0xBB,0xBB,0xBB,
	0xAA,0xAA,0xAA, 0x88,0x88,0x88,	0x77,0x77,0x77,	0x55,0x55,0x55,
	0x44,0x44,0x44, 0x22,0x22,0x22,	0x11,0x11,0x11,	0x00,0x00,0x00
};

