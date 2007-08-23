#include <dsx.h>
#include "iqzz_proto.h"

#include "jpeg.h"
#include "parser.h"

static const int G_ZZ[64] = {
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

FUNC(iqzz)
{
    dsx_mwmr_t input = GET_ARG(input);
    dsx_mwmr_t output = GET_ARG(output);
    dsx_mwmr_t quanti = GET_ARG(quanti);

    int i,j;
    unsigned char QTable[64];
    short in[BLOCK_SIZE];
    int UnZZ[BLOCK_SIZE];

    /*
     * three quantization tables/image for JPEG, only one table for
     * JFIF.  64 elements for JFIF table.
     */

    dsx_log(TRACE, "IQZZ thread is alive!\n");

    dsx_mwmr_read( quanti, QTable, 64/4 );

    for ( j=0; j<NBLOCKS; ++j ) {
        dsx_log_printf(TRACE, "IQZZ processing block %d/%d\n",
                       j, NBLOCKS);

        /* get from vld */
        dsx_mwmr_read( input, (unsigned char *)in, 2*BLOCK_SIZE/4 );
        for ( i=0; i<BLOCK_SIZE; ++i ) {
            short buf;

            buf = in[i];
            /* unquantify & UnZZ block */
            UnZZ[G_ZZ[i]] = buf*QTable[i];
            dsx_log_printf(DEBUG,
                            "i: %d, G: %d, UG: %d, buf: %d, QTable: %d\n",
                            i, G_ZZ[i], UnZZ[G_ZZ[i]], buf, QTable[i] );
        }
        dsx_mwmr_write( output, (unsigned char *)UnZZ, 4*BLOCK_SIZE/4 );
    }
}
