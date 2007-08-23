#ifndef BLOCK_IO_H
#define BLOCK_IO_H

#include <dsx.h>

typedef struct block_io {
    int size;
    int left;
    unsigned char *ptr;
    unsigned char *buffer;
    dsx_mwmr_t io;
} block_io;

void block_io_init_in( block_io *io, void *buffer, int size, dsx_mwmr_t mwmr );
void block_io_init_out( block_io *io, void *buffer, int size, dsx_mwmr_t mwmr );

unsigned char block_io_read_char( block_io *io );
unsigned short block_io_read_short( block_io *io );
unsigned int block_io_read_int( block_io *io );

void block_io_write_char( block_io *io, unsigned char val );
void block_io_write_short( block_io *io, unsigned short val );
void block_io_write_int( block_io *io, unsigned int val );

void block_io_io(block_io *in, block_io *o, int len);

void block_io_skip( block_io *io, unsigned long len );

void block_io_flush( block_io *io );
void block_io_discard( block_io *io );

#endif /* BLOCK_IO_H */
