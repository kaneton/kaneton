#include <dsx.h>

#include <libc.h>

#include "libu_proto.h"

#include "jpeg.h"

#define WIDTH_RX (WIDTH-(WIDTH%BLOCK_WIDTH))
#define WIDTH_RY (HEIGHT-(HEIGHT%BLOCK_HEIGHT))


static inline int intceil(int N, int D)
{
    int i = N / D;

    if (N > D * i)
        i++;
    return i;
}

static inline int intfloor(int N, int D)
{
    int i = N / D;

    if (N < D * i)
        i--;
    return i;
}

FUNC(libu)
{
    dsx_mwmr_t input = GET_ARG(input);
    dsx_mwmr_t output = GET_ARG(output);
    unsigned char ColorBuffer[BLOCK_SIZE];
    unsigned char LineBuffer[MAX_WIDTH*BLOCK_HEIGHT];

    dsx_log(TRACE, "LIBU thread is alive !\n");

    int line, column;

    dsx_log_printf(DEBUG, "m: %dx%d, r: %dx%d\n",
                    BLOCKS_X, BLOCKS_Y, WIDTH_RX, WIDTH_RY );
    dsx_assert(WIDTH == WIDTH_RX && HEIGHT == WIDTH_RY );

    for ( line=0; line<BLOCKS_Y; ++line ) {
        dsx_log_printf(TRACE, "LIBU processing stripe %d/%d\n",
                        line, BLOCKS_Y);

        for ( column=0; column<BLOCKS_X; ++column ) {
            int i;
            dsx_mwmr_read( input, ColorBuffer, BLOCK_SIZE/4 );

            /* Should maybe put less columns on last pass */
            for ( i=0; i<BLOCK_HEIGHT; ++i ) {
                memcpy( &LineBuffer[i*WIDTH+BLOCK_WIDTH*column],
                        &ColorBuffer[i*BLOCK_WIDTH],
                        BLOCK_WIDTH );
            }
        }
        /* Should maybe put less lines on last pass */
        dsx_mwmr_write( output, LineBuffer, WIDTH*BLOCK_HEIGHT/4 );
    }
}
