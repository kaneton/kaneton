#include "dsx.h"
#include "dsx_endianness.h"
#include "ramdac_proto.h"
#include <klibc.h>

#include "jpeg.h"

unsigned char * const framebuffer = 0x000a0000;

unsigned char offscreen[320 * 200];

FUNC(ramdac)
{
    dsx_mwmr_t input = GET_ARG(input);
    int i, j;
    const int shiftx = 136;
    const int shifty = 76;
    char buffer[MAX_WIDTH*8];

    dsx_log(TRACE, "RAMDAC: thread is alive !\n");
    for (i=0;i<MAX_HEIGHT;i+=8) {
      offscreen[(i)*320] = 0;
        dsx_mwmr_read( input,
                       buffer,
                       MAX_WIDTH*8/4 );
	for ( j=0; j<8; ++j ) {
	  memcpy( &offscreen[(i+j)*320], buffer+MAX_WIDTH*j, MAX_WIDTH );
	}
    }
    memcpy(framebuffer, offscreen, 320 * 200);
    dsx_log(TRACE, "RAMDAC: display!\n");
}
