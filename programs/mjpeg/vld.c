#include <dsx.h>
#include "vld_proto.h"

#include "jpeg.h"
#include "block_io.h"
#include "bit_reading.h"
#include "huff_tables.h"
#include "parser.h"

static int vld_decode_unpack_block(
    bitreader_context *bits,
    dsx_mwmr_t output,
    huff_context *huff,
    cd_t *comp
    );

inline static int intceil(int N, int D)
{
    int i = N / D;

    if (N > D * i)
        i++;
    return i;
}

/* transform JPEG number format into usual 2's complement format */
inline static long reformat(unsigned long S, int good)
{
    unsigned long ext, sign;

    if (!good)
        return 0;
    sign = !((1<<(good-1))&(S));
    ext = 0-(sign<<good);
    return (S|ext)+sign;
}


FUNC(vld)
{
    dsx_mwmr_t input_mwmr = GET_ARG(input);
    dsx_mwmr_t output = GET_ARG(output);
    dsx_mwmr_t huffman_mwmr = GET_ARG(huffman);
    cd_t comp;
    huff_context huff;
    int left;
    int x_size, y_size;
    int mx_size, my_size;
    unsigned char tmp;
    bitreader_context bits;

    unsigned char input_buffer[32];
    block_io input;

    unsigned char huffman_buffer[32];
    block_io huffman;

    block_io_init_in(&input, input_buffer, 32, input_mwmr);
    block_io_init_in(&huffman, huffman_buffer, 32, huffman_mwmr);

    dsx_log(TRACE, "VLD thread is alive\n");
    huff_tables_init(&huff);
    /* DHT */
    huff_load_tables(&huff, &huffman);
    block_io_discard(&huffman);
    huff_load_tables(&huff, &huffman);
    block_io_discard(&huffman);

    dsx_log(TRACE, "huffman tables loaded\n");
//    huff_dump( &huff );

#if 0
    /*
     * This code can help if we want to tolerate desynchronizations
     */
    dsx_log(TRACE, "Looking for magic... ");
    do {
        int magic = block_io_read_int(&input);
        if ( magic == DATA_MAGIC ) {
            dsx_log(TRACE, "got it\n");
            break;
        }
        block_io_discard(&input);
    } while (1);
#else
    dsx_assert ( block_io_read_int(&input) == DATA_MAGIC );
#endif

    x_size = block_io_read_short(&input);
    y_size = block_io_read_short(&input);

    block_io_read_char(&input);
    dsx_log_printf(DEBUG, "picture size: %dx%d\n", x_size, y_size);

    mx_size = intceil(x_size, BLOCK_WIDTH);
    my_size = intceil(y_size, BLOCK_HEIGHT);

    /* SOS */
    block_io_read_int(&input);

    tmp = block_io_read_char(&input);
    comp.DC_HT = first_quad(tmp);
    comp.AC_HT = second_quad(tmp);
    dsx_log_printf(DEBUG,
                    "DC: %d, AC: %d\n",
                    comp.DC_HT, comp.AC_HT);

    block_io_read_char(&input);
    block_io_read_short(&input);

    bitreader_init(&bits, &input);
    comp.PRED = 0;
    left = mx_size*my_size;

    while (left > 0) {
        vld_decode_unpack_block(&bits, output, &huff, &comp);
        dsx_log_printf(DEBUG, "Put uncompressed block no %d\n", left);
        --left;
    }
}


/*
 * private huffman.c defines and macros
 */
#define HUFF_EOB                0x00
#define HUFF_ZRL                0xF0

/*
 * here we unpack a 8x8 DCT block
 */
static int vld_decode_unpack_block(
    bitreader_context *bits,
    dsx_mwmr_t output,
    huff_context *huff,
    cd_t *comp
    )
{
    unsigned long temp;
    unsigned int i, run, cat;
    int value;
    unsigned char symbol;
    short T[BLOCK_SIZE];

    for (i = 0; i < BLOCK_SIZE; i++)
        T[i] = 0;

    /* first get the DC coefficient */
    symbol = huff_get_symbol( bits, huff, HUFF_ID(DC_CLASS,comp->DC_HT) );
    temp = bitreader_get( bits, symbol );
    value = reformat( temp, symbol );
    value += comp->PRED;
    comp->PRED = value;

	/* reoganize and unquantify -> move to ZZ and IQ  */
    T[0] = value;
	/* then the AC ones
     * if symbol found is EOB and process not finish, missing values
     * are replaced by zero
     */
    for ( i=1; i<BLOCK_SIZE; i++ ) {
        symbol = huff_get_symbol( bits, huff, HUFF_ID(AC_CLASS,comp->AC_HT) );
        dsx_log_printf(DEBUG, "huff_symbol %d: %x\n", i, symbol);
        if (symbol == HUFF_EOB) {
            dsx_log(DEBUG, "HUFF_EOB\n");
            break;
        }
        if (symbol == HUFF_ZRL) {
            dsx_log(DEBUG, "HUFF_ZRL\n");
            i += 15;
            continue;
        }
        cat = symbol & 0xf;
        run = symbol >> 4;
        i += run;
        temp = bitreader_get( bits, cat );
        value = reformat( temp, cat );
        T[i] = value;
    }
    dsx_mwmr_write(output, (unsigned char *)T, 2*BLOCK_SIZE/4);
    return 0;
}
