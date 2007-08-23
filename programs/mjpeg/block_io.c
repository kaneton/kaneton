
#include <libc.h>
#include <dsx.h>

#include "block_io.h"

/* static inline void io_refill( block_io *io ) */
/* { */
/*     dsx_mwmr_read( io->io, io->buffer, io->size/4 ); */
/*     io->left = io->size; */
/*     io->ptr = io->buffer; */
/* } */

/* static inline void io_flush( block_io *io ) */
/* { */
/*     dsx_mwmr_write( io->io, io->buffer, io->size/4 ); */
/*     io->left = io->size; */
/*     io->ptr = io->buffer; */
/* } */

/* static void io_dump( block_io *io ) */
/* { */
/*     int i; */
/*     for ( i=0; i<io->size; ++i ) */
/*         dsx_log_printf(DEBUG, "%02x ", io->buffer[i]); */
/*     dsx_log_printf(DEBUG, "\n"); */
/* } */

#define io_refill( bio )                                 \
do                                                       \
{                                                        \
    dsx_log(DEBUG, "Refilling block\n" ); \
    dsx_mwmr_read( bio->io, bio->buffer, bio->size/4 );  \
    bio->left = bio->size;                               \
    bio->ptr = bio->buffer;                              \
} while (0)

#define io_flush( bio )                                  \
do                                                       \
{                                                        \
    dsx_log(DEBUG, "Flushing block\n" ); \
    dsx_mwmr_write( bio->io, bio->buffer, bio->size/4 ); \
    bio->left = bio->size;                               \
    bio->ptr = bio->buffer;                              \
} while (0)

void block_io_flush( block_io *io )
{
    dsx_log(DEBUG, "Forcing " );
    if ( io->left != io->size )
        io_flush(io);
}

void block_io_discard( block_io *io )
{
    dsx_log(DEBUG, "Forcing block discard\n" );
    io->left = 0;
}

void block_io_init_out( block_io *io, void *buffer, int size, dsx_mwmr_t mwmr )
{
    io->io = mwmr;
    io->buffer = buffer;
    io->ptr = io->buffer;
    io->size = size;
    io->left = size;
}

void block_io_init_in( block_io *io, void *buffer, int size, dsx_mwmr_t mwmr )
{
    io->io = mwmr;
    io->buffer = buffer;
    io->ptr = io->buffer;
    io->size = size;
    io->left = 0;
}


unsigned char block_io_read_char( block_io *io )
{
    unsigned char ret;

    if ( io->left == 0 )
        io_refill(io);
    io->left -= 1;
    ret = *(io->ptr++);
    dsx_log_printf(DEBUG, "bio_read_char: %02x\n", ret);
    return ret;
}

unsigned short block_io_read_short( block_io *io )
{
    unsigned short ret;

    if ( io->left == 1 ) {
        ret = *(io->ptr++);
        io_refill(io);
        io->left -= 1;
        io->ptr++;
        return (ret << 8) | io->buffer[0];
    }
    if ( io->left == 0 )
        io_refill(io);
    /* On sera pas toujours aligne */
    io->left -= 2;
    ret = *(io->ptr++);
    ret = (ret<<8) | *(io->ptr++);
    dsx_log_printf(DEBUG, "bio_read_short: %04x\n", ret);
    return ret;
}

unsigned int block_io_read_int( block_io *io )
{
    unsigned int ret = 0;
    int left, i;

    left = io->left;

    for ( i=0; i<4; ++i ) {
        if ( left == 0 ) {
            io_refill(io);
            left = io->size;
        }
        ret <<= 8;
        ret |= *(io->ptr++);
        left -= 1;
    }
    io->left = left;
    dsx_log_printf(DEBUG, "bio_read_int: %08x\n", ret);
    return ret;
}


void block_io_write_char( block_io *io, unsigned char val )
{
    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = val;
}

void block_io_write_short( block_io *io, unsigned short val )
{
    unsigned char tmp = val>>8;

    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = tmp;
    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = val;
}

void block_io_write_int( block_io *io, unsigned int val )
{
    unsigned char tmp3 = val>>24,
        tmp2 = val>>16,
        tmp1 = val>>8;

    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = tmp3;
    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = tmp2;
    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = tmp1;
    if ( io->left == 0 )
        io_flush(io);
    io->left -= 1;
    *(io->ptr++) = val;
}


void block_io_io(block_io *in, block_io *out, int len)
{
    int left = len;

    while (left) {
        int cs = left;

        if (in->left == 0)
            io_refill(in);
        if (out->left == 0)
            io_flush(out);
        if (cs > in->left)
            cs = in->left;
        if (cs > out->left)
            cs = out->left;
        dsx_assert(cs);
        memcpy(out->ptr, in->ptr, cs);
        out->left -= cs;
        in->left -= cs;
        out->ptr += cs;
        in->ptr += cs;
        left -= cs;
    }
}

void block_io_skip( block_io *io, unsigned long len )
{
    while ( len ) {
        unsigned long rlen;
        if (io->left == 0)
            io_refill(io);
        rlen = (len<io->left)?len:io->left;
        io->left -= rlen;
        io->ptr += rlen;
        len -= rlen;
    }
}

