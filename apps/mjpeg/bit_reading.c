#include <dsx.h>
#include "block_io.h"
#include "bit_reading.h"

/* utility and counter to return the number of bits from file */
/* right aligned, masked, first bit towards MSB's               */

unsigned long bitreader_get(bitreader_context *cont, int number)
{
    unsigned int ret = 0, nn = number;

    if (cont->available)
        cont->current &= (1<<cont->available)-1;

    while (number) {
        if ( cont->available == 0 ) {
            cont->current = block_io_read_char( cont->io );
            cont->available = 8;
        }
        if ( number == cont->available ) {
            cont->available = 0;
            ret = (ret<<number) | cont->current;
            break;
        }
        if ( number < cont->available ) {
            ret = (ret<<number) | (cont->current>>(cont->available-number));
            cont->available -= number;
            break;
        }
        if ( number > cont->available ) {
            ret = (ret<<cont->available) | cont->current;
            number -= cont->available;
            cont->available = 0;
        }
    }
    dsx_log_printf(DEBUG, "BITS Getting %d bits: %x\n", nn, ret);
    return ret;
}

unsigned char bitreader_get_one(bitreader_context *cont)
{
    unsigned int ret = 0, tmp;

    if ( cont->available == 0 ) {
        cont->current = block_io_read_char( cont->io );
        cont->available = 7;
    } else
        --(cont->available);
    tmp = 1<<(cont->available);
    ret = !!(cont->current&tmp);
    dsx_log_printf(DEBUG, "BITS Getting 1 bits: %x\n", ret);
    return ret;
}

void bitreader_clear(bitreader_context *cont)
{
    dsx_log_printf(NONE, "BITS clear\n");
    cont->available = 0;
}

void bitreader_init(bitreader_context *cont, block_io *io)
{
    cont->available = 0;
    cont->current = 0;
    cont->io = io;
}
