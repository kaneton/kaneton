#include "dsx.h"
#include "demux_proto.h"
#include "dsx_endianness.h"

#include "jpeg.h"

#include "parser.h"

#include "block_io.h"

#define x_size WIDTH
#define y_size HEIGHT

FUNC(demux) {
    dsx_mwmr_t input_mwmr = GET_ARG(input);
    dsx_mwmr_t output_mwmr = GET_ARG(output);
    dsx_mwmr_t huffman_mwmr = GET_ARG(huffman);
    dsx_mwmr_t quanti_mwmr = GET_ARG(quanti);
    unsigned int aux, picture_no = 0;
    unsigned short mark = 0, pot_mark;
    unsigned char marker;

    unsigned char input_buffer[32];
    block_io input;

    unsigned char output_buffer[32];
    block_io output;

    unsigned char huffman_buffer[32];
    block_io huffman;

    unsigned char quanti_buffer[32];
    block_io quanti;

    int found_mk, done, end_nb = 0, in_frame = 0;

    block_io_init_in(&input, input_buffer, 32, input_mwmr);
    block_io_init_out(&output, output_buffer, 32, output_mwmr);
    block_io_init_out(&huffman, huffman_buffer, 32, huffman_mwmr);
    block_io_init_out(&quanti, quanti_buffer, 32, quanti_mwmr);

    dsx_log(TRACE, "starting\n");
    while(1) {
        do {
            aux = parser_get_next_mk(&input);
        } while (aux != SOI_MK);
        dsx_log(TRACE, "found the SOI marker\n");
        dsx_log_printf(TRACE, "picture %d\n", ++picture_no);
        found_mk = 0;
        done = 0;
        while (!done) {
            if (!found_mk)
                mark = parser_get_next_mk(&input);
            switch (mark) {
            case SOF_MK:
                dsx_log(TRACE, "found SOF marker\n");
                in_frame = 1;
                block_io_write_int(&output, DATA_MAGIC);
                /* Dont care about Header size */
                block_io_read_short(&input);
                /* Dont care about precision & sizes */
                block_io_read_int(&input);
                block_io_write_short(&output, x_size);
                block_io_write_short(&output, y_size);

                /* # of components */
                /* Sampling factor & QuantT index */
                aux = block_io_read_int(&input);
                block_io_write_char(&output, aux>>16);

                break;
            case DHT_MK:
                dsx_log(TRACE, "huffman table marker\n");
                found_mk = 0;
                while (!found_mk) {
                    marker = block_io_read_char(&input);
                    if (marker == 0xff) {
                        pot_mark = block_io_read_char(&input);
                        if (pot_mark == 0) {
                            block_io_write_char(&huffman, 0xff);
                        } else {
                            mark = 0xff00|pot_mark;
                            dsx_log_printf(DEBUG, "found marker %04x while in huff def\n", mark);
                            found_mk = 1;
                        }
                    } else /* isn't 0xff */ {
                        block_io_write_char(&huffman, marker);
                    }
                }
                block_io_flush(&huffman);
                break;

            case DQT_MK:
                dsx_log(TRACE, "quantization table marker\n");
                parser_load_quant_tables(&input, &quanti);
                block_io_flush(&quanti);
                break;

            case DRI_MK:
                /* skip sizes */
                dsx_log(TRACE, "DRI marker\n");
                block_io_read_int(&input);
                break;

            case SOS_MK:
                dsx_log(TRACE, "sos marker\n");
                found_mk = 0;
                while (!found_mk) {
                    marker = block_io_read_char(&input);
                    dsx_log_printf(DEBUG, "data %02x\n", marker);
                    if (marker == 0xff) {
                        pot_mark = block_io_read_char(&input);
                        if (pot_mark == 0) {
                            block_io_write_char(&output, 0xff);
                        } else {
                            mark = 0xff00|pot_mark;
                            dsx_log_printf(DEBUG, "found marker %04x while in data\n", mark);
                            found_mk = 1;
                        }
                    } else {
                        /* buf isn't 0xff */
                        block_io_write_char(&output, marker);
                    }
                }
                dsx_log(TRACE, "flushing data\n");
                in_frame = 0;
                break;

            case EOI_MK:
                block_io_flush(&output);
                dsx_log_printf(TRACE, "picture %d end marker\n", ++end_nb );
                done = 1;
                break;

            case COM_MK:
                dsx_log(TRACE, "comments marker, skipping\n");
                parser_skip_segment(&input);
                break;
            default:
                if ( (mark&MK_MSK) == APP_MK ) {
                    dsx_log(TRACE, "application data marker, skipping\n");
                    parser_skip_segment(&input);
                    break;
                }
                if (RST_MK(mark)) {
                    dsx_log(TRACE, "found RST marker\n");
                    break;
                }
                done = 1;
                ++end_nb;
                break;
            }
        }
    }
}
