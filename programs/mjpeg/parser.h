#include "dsx.h"

#include "block_io.h"

unsigned short parser_get_next_mk(block_io *f);

unsigned short parser_get_size(block_io *f);

void parser_load_quant_tables(block_io *input, block_io *quanti);

void parser_skip_segment(block_io *f);
