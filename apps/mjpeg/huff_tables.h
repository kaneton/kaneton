#ifndef HUFF_TABLES_H_
#define HUFF_TABLES_H_

#include "jpeg.h"
#include "bit_reading.h"
#include "block_io.h"

#define MAX_SIZE(class)         ((class)?162:14) /* Memory size of HTables */

typedef struct {
    unsigned char *HTable[4];
    int MinCode[4][16];
    int MaxCode[4][16];
    int ValPtr[4][16];
    unsigned char DC_Table0[MAX_SIZE(DC_CLASS)];
    unsigned char DC_Table1[MAX_SIZE(DC_CLASS)];
    unsigned char AC_Table0[MAX_SIZE(AC_CLASS)];
    unsigned char AC_Table1[MAX_SIZE(AC_CLASS)];
} huff_context;

void huff_tables_init( huff_context *cont );
int huff_load_tables( huff_context *cont, block_io *input );
int huff_get_symbol( bitreader_context *cont, huff_context *huff, int select );

#endif /* HUFF_TABLES_H_ */

