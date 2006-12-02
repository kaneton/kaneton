#ifndef BIT_READING_H_
#define BIT_READING_H_

#include <dsx.h>
#include "block_io.h"

typedef struct {
    block_io *io;
    unsigned char current;
    unsigned char available;
} bitreader_context;

unsigned long bitreader_get(bitreader_context *cont, int number);

void bitreader_clear(bitreader_context *cont);

unsigned char bitreader_get_one(bitreader_context *cont);

void bitreader_init(bitreader_context *cont, block_io *chan);

#endif /* BIT_READING_H_ */
