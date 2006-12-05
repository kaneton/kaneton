#include "dsx.h"
#include "dsx_endianness.h"
#include "ramdac_proto.h"
#include <klibc.h>

#include "jpeg.h"

unsigned char * const framebuffer = 0x000a0000;

FUNC(ramdac)
{
    dsx_mwmr_t input = GET_ARG(input);
    int i, j;
    char buffer[MAX_WIDTH*8];

    dsx_log(TRACE, "RAMDAC: thread is alive !\n");
    printf("rendering frame\n");
    for (i=0;i<MAX_HEIGHT;i+=8) {
      framebuffer[i*320] = 0;
        dsx_log_printf(TRACE, "copying line %d->%d\n", i, i+7);
        dsx_mwmr_read( input,
                       buffer,
                       MAX_WIDTH*8/4 );
	for ( j=0; j<8; ++j ) {
	  memcpy( &framebuffer[(i+j)*320], buffer+MAX_WIDTH*j, MAX_WIDTH );
	}
    }
    dsx_log(TRACE, "RAMDAC: display!\n");
}
