#include <dsx.h>
#include "idct_proto.h"

#include "jpeg.h"

/* Minimum and maximum values a `signed int' can hold.  */
#define	INT_MAX	((int)0x7fffffff)
#define	INT_MIN	((int)0x80000000)

/* Useful constants: */

/* ck = cos(k*pi/16) = s8-k = sin((8-k)*pi/16) times 1 << C_BITS and rounded */
#define c0_1  16384
#define c0_s2 23170
#define c1_1  16069
#define c1_s2 22725
#define c2_1  15137
#define c2_s2 21407
#define c3_1  13623
#define c3_s2 19266
#define c4_1  11585
#define c4_s2 16384
#define c5_1  9102
#define c5_s2 12873
#define c6_1  6270
#define c6_s2 8867
#define c7_1  3196
#define c7_s2 4520
#define c8_1  0
#define c8_s2 0
#define sqrt2 c0_s2

/* The number of bits of accuracy in all (signed) integer operations:
   May lie between 1 and 32 (bounds inclusive).
*/
#define ARITH_BITS      16

/* The minimum signed integer value that fits in ARITH_BITS: */
#define ARITH_MIN       (-1 << (ARITH_BITS-1))
#define ARITH_MAX       (~ARITH_MIN)

/* The number of bits coefficients are scaled up before 2-D idct: */
#define S_BITS           3
/* The number of bits in the fractional part of a fixed point constant: */
#define C_BITS          14

/* This version is vital in passing overall mean error test. */
#define descale(x, n) (((x) + (1 << ((n) - 1)) - ((x) < 0)) >> (n))

const int COS[2][8] = {
    {c0_1, c1_1, c2_1, c3_1, c4_1, c5_1, c6_1, c7_1},
    {c0_s2, c1_s2, c2_s2, c3_s2, c4_s2, c5_s2, c6_s2, c7_s2}
};

void rot(int f, int k, int x, int y, int *rx, int *ry)
{
#define Cos(k)  COS[f][k]
#define Sin(k)  Cos(8-k)
    *rx = (Cos(k) * x - Sin(k) * y) >> C_BITS;
//    r = (r + (1 << (C_BITS - 1))) >> C_BITS;
    *ry = (Sin(k) * x + Cos(k) * y) >> C_BITS;
//    r = (r + (1 << (C_BITS - 1))) >> C_BITS;
#undef Cos
#undef Sin
}

/* Butterfly: but(a,b,x,y) = rot(sqrt(2),4,a,b,x,y) */
#define but(a,b,x,y)   do { x = a - b; y = a + b; } while(0)

/* Inverse 1-D Discrete Cosine Transform.
   Result Y is scaled up by factor sqrt(8).
   Original Loeffler algorithm.
*/
void idct_1d(int *Y)
{
    int z1[8], z2[8], z3[8];

    /* Stage 1: */
    but(Y[0], Y[4], z1[1], z1[0]);
    rot(1, 6, Y[2], Y[6], &z1[2], &z1[3]);
    but(Y[1], Y[7], z1[4], z1[7]);
    z1[5] = (sqrt2 * Y[3]) >> C_BITS;
//    r = (r + (1 << (C_BITS - 1))) >> C_BITS;
    z1[6] = (sqrt2 * Y[5]) >> C_BITS;
//    r = (r + (1 << (C_BITS - 1))) >> C_BITS;

    /* Stage 2: */
    but(z1[0], z1[3], z2[3], z2[0]);
    but(z1[1], z1[2], z2[2], z2[1]);
    but(z1[4], z1[6], z2[6], z2[4]);
    but(z1[7], z1[5], z2[5], z2[7]);

    /* Stage 3: */
    z3[0] = z2[0];
    z3[1] = z2[1];
    z3[2] = z2[2];
    z3[3] = z2[3];
    rot(0, 3, z2[4], z2[7], &z3[4], &z3[7]);
    rot(0, 1, z2[5], z2[6], &z3[5], &z3[6]);

    /* Final stage 4: */
    but(z3[0], z3[7], Y[7], Y[0]);
    but(z3[1], z3[6], Y[6], Y[1]);
    but(z3[2], z3[5], Y[5], Y[2]);
    but(z3[3], z3[4], Y[4], Y[3]);
}

FUNC(idct)
{
    dsx_mwmr_t input = GET_ARG(input);
    dsx_mwmr_t output = GET_ARG(output);

#define Y(i,j)          Y[8*i+j]
#define Idct(i,j)          Idct[8*i+j]
    int Y[BLOCK_SIZE];
    int row, column;
    int in[BLOCK_SIZE];
    unsigned char Idct[BLOCK_SIZE];
    int block;

    dsx_log(TRACE, "IDCT thread is alive!\n");

    for ( block=0; block<NBLOCKS; ++block ) {
        dsx_log_printf(TRACE, "IDCT processing block %d/%d\n",
                       block, NBLOCKS);

        dsx_mwmr_read( input, (unsigned char *)in, BLOCK_SIZE*4/4 );

        for (row = 0; row < BLOCK_HEIGHT; row++) {
            for (column = 0; column < BLOCK_WIDTH; column++)
                Y(row, column) = in[row*BLOCK_WIDTH+column] << S_BITS;
            idct_1d(&Y(row, 0));
            /* Result Y is scaled up by factor sqrt(8)*2^S_BITS. */
        }

        for (column = 0; column < BLOCK_WIDTH; column++) {
            int Yc[BLOCK_HEIGHT];

            for (row = 0; row < BLOCK_HEIGHT; row++)
                Yc[row] = Y(row, column);

            idct_1d(Yc);
            for (row = 0; row < BLOCK_HEIGHT; row++) {
                /* Result is once more scaled up by a factor sqrt(8). */
                int r = 128 + descale(Yc[row], 2*S_BITS);
                /* Clip to 8 bits unsigned: */
                r = r > 0 ? (r < 255 ? r : 255) : 0;
                Idct(row, column) = r;
            }
        }
        dsx_mwmr_write( output, (unsigned char *)Idct, BLOCK_SIZE/4 );
    }
}
