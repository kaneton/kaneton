#include "dsx.h"
#include "dsx_endianness.h"

#include "jpeg.h"

#include "parser.h"

unsigned short parser_get_next_mk(block_io *f)
{
    unsigned char buf, ffmet = 0;
    
    do {
        buf = block_io_read_char(f);
        switch (buf) {
        case 0xff:
            ffmet = 1;
            break;
        default:
            if (ffmet) {
                dsx_log_printf(DEBUG, "found marker ff%02x\n", buf);
                return buf|0xff00;
            }
        case 0:
            ffmet = 0;
            break;
        }
    } while (1);
    return EOF;
}

unsigned short parser_get_size(block_io *f)
{
    unsigned char buf;
    buf = block_io_read_short(f);
    return buf; // dsx_short_from_be(buf);
}

void parser_load_quant_tables(block_io *input, block_io *quanti)
{
    unsigned char aux;
    unsigned int size;

    size = parser_get_size(input)-2;
    dsx_log_printf(TRACE, "Loading quanti tables, size: %d\n", (int)size);
    size /= 65;
    while (size>0) {
        aux = block_io_read_char(input);
        block_io_io(input, quanti, 64);
        --size;
    }
}

void parser_skip_segment(block_io *f)
{
    unsigned short size;
    unsigned char tag[5];

    size = parser_get_size(f);
    size -= 2;
    if (size > 5) {
        *(int*)tag = block_io_read_int(f);
        tag[4] = 0;
        dsx_log_printf(DEBUG, "SKIP: tag: %s, size: %d\n", tag, (int)size);
        size -= 4;
    }

    block_io_skip(f, size);
}


