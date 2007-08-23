#include "dsx.h"
#include "dsx_endianness.h"
#include "huff_tables.h"
#include "bit_reading.h"

#include "parser.h"

void huff_tables_init( huff_context *cont )
{
    int i, j;
//    memset(cont, 0, sizeof(*cont));
    for ( j=0; j<16; ++j ) {
        for ( i=0; i<4; ++i ) {
            cont->MinCode[i][j] = 0;
            cont->MaxCode[i][j] = 0;
            cont->ValPtr[i][j] = 0;
        }
        cont->DC_Table0[j] = 0;
        cont->AC_Table0[j] = 0;
        cont->DC_Table1[j] = 0;
        cont->AC_Table1[j] = 0;
    }
    cont->HTable[0] = cont->DC_Table0;
    cont->HTable[1] = cont->DC_Table1;
    cont->HTable[2] = cont->AC_Table0;
    cont->HTable[3] = cont->AC_Table1;
}

#if 0
void huff_dump( huff_context *cont )
{
    int i, j;
    dsx_log_printf(DEBUG, "MinCode:\n");
    for ( i=0; i<4; ++i ) {
        for ( j=0; j<16; ++j ) {
            dsx_log_printf(DEBUG, " %08x", cont->MinCode[i][j]);
        }
        dsx_log_printf(DEBUG, "\n");
    }
    dsx_log_printf(DEBUG, "MaxCode:\n");
    for ( i=0; i<4; ++i ) {
        for ( j=0; j<16; ++j ) {
            dsx_log_printf(DEBUG, " %08x", cont->MaxCode[i][j]);
        }
        dsx_log_printf(DEBUG, "\n");
    }
    dsx_log_printf(DEBUG, "ValPtr:\n");
    for ( i=0; i<4; ++i ) {
        for ( j=0; j<16; ++j ) {
            dsx_log_printf(DEBUG, " %08x", cont->ValPtr[i][j]);
        }
        dsx_log_printf(DEBUG, "\n");
    }
    dsx_log_printf(DEBUG, "DC_Table0:\n");
    for ( j=0; j<14; ++j ) {
        dsx_log_printf(DEBUG, " %02x", cont->DC_Table0[j]);
    }
    dsx_log_printf(DEBUG, "\n");
    dsx_log_printf(DEBUG, "DC_Table1:\n");
    for ( j=0; j<14; ++j ) {
        dsx_log_printf(DEBUG, " %02x", cont->DC_Table1[j]);
    }
    dsx_log_printf(DEBUG, "\n");
    dsx_log_printf(DEBUG, "AC_Table0:\n");
    for ( j=0; j<162; ++j ) {
        dsx_log_printf(DEBUG, " %02x", cont->AC_Table0[j]);
    }
    dsx_log_printf(DEBUG, "\n");
    dsx_log_printf(DEBUG, "AC_Table1:\n");
    for ( j=0; j<162; ++j ) {
        dsx_log_printf(DEBUG, " %02x", cont->AC_Table1[j]);
    }
    dsx_log_printf(DEBUG, "\n");
}
#endif

/*----------------------------------------------------------*/
/* Loading of Huffman table, with leaves drop ability       */
/*----------------------------------------------------------*/
int huff_load_tables( huff_context *cont, block_io *input )
{
    unsigned char aux, buf;
    int size, class, id, max;
    int LeavesN, LeavesT, i;
    int AuxCode;

    size = parser_get_size(input)-2;
    dsx_log_printf(DEBUG, "size: %d\n", size);
    while ( size > 0 ) {
        aux = block_io_read_char(input);
        dsx_log_printf(DEBUG, "aux: %02x\n",aux);
        /* AC or DC */
        class = first_quad(aux);
        /* table no */
        id = second_quad(aux);
        dsx_log_printf(DEBUG, "class: %d, id: %d\n", class, id);
        if (id > 1) {
            dsx_log_printf(NONE, "ERROR: Bad HTable identity %d!\n", id);
            dsx_assert(0);
        }
        id = HUFF_ID(class, id);
        if (id > 3) {
            dsx_log_printf(NONE, "ERROR: Bad computed HTable identity %d!\n", id);
            dsx_assert(0);
        }
        dsx_log_printf(TRACE, "Loading Table %d\n", id);
        --size;
        LeavesT = 0;
        AuxCode = 0;
        for ( i=0; i<16; i++ ) {
            buf = block_io_read_char(input);
            LeavesN = buf;
            cont->ValPtr[id][i] = LeavesT;
            cont->MinCode[id][i] = AuxCode<<1;
            AuxCode = cont->MinCode[id][i] + LeavesN;
            cont->MaxCode[id][i] = (LeavesN) ? (AuxCode - 1) : (-1);
            LeavesT += LeavesN;
        }
        size -= 16;
        if ( LeavesT>MAX_SIZE(class) ) {
            max = MAX_SIZE(class);
            dsx_log_printf(NONE, "ERROR: Truncating Table by %d symbols\n", LeavesT - max);
            dsx_assert(0);
        } else
            max = LeavesT;
        dsx_log_printf(DEBUG, "pass2 max: %d, LeavesT: %d\n", max, LeavesT);
        dsx_log_printf(DEBUG, "pass2: %d->%d\n", 0, max);
        for ( i=0; i<max; ++i ) {
            /* get huffman table */
            buf = block_io_read_char(input);
            dsx_log_printf(DEBUG, "pass2 buffer=%02x\n", buf);
            cont->HTable[id][i] = buf;
            /* load in raw order */
        }
        dsx_log_printf(DEBUG, "pass2/2: %d->%d\n", max, LeavesT);
        for ( i=max; i<LeavesT; ++i ) {
            block_io_read_char(input); // DROP
        }
        size -= LeavesT;
        dsx_log_printf(DEBUG, "VLD: Using %d words of table memory\n", LeavesT);
        dsx_log_printf(DEBUG, "new size: %d\n", size);
    }
    /* loop on tables */
    return 0;
}

/*-----------------------------------*/
/* extract a single symbol from file */
/* using specified huffman table ... */
/*-----------------------------------*/
int huff_get_symbol( bitreader_context *cont, huff_context *huff, int select )
{
    long code = 0;
    int length;
    int index;

    for ( length=0; length<16; ++length ) {
        code = (code<<1) | bitreader_get_one( cont );
        if (code <= huff->MaxCode[select][length])
            break;
    }
    index = huff->ValPtr[select][length] + code - huff->MinCode[select][length];
    if (index < MAX_SIZE(select / 2)) {
        return huff->HTable[select][index];
    }
    dsx_log(NONE, "ERROR: Overflowing symbol table !\n");
    dsx_assert(0);
    return 0;
}
