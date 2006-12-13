/**********************************************************************/
/* File   : jpeg.h, header file for MJPEG-JFIF Multi-thread decoder   */
/* Author : Pascal GOMEZ, november 2001                               */
/* Error process                                                      */
/* Adaptation and cleanup for Kola example                            */
/**********************************************************************/
#ifndef JPEG_H_
#define JPEG_H_

/* define picture size (viewable size) */
#define WIDTH 48
#define HEIGHT 64

/* define maximum picture size (rounded up to 8x8) */
#define MAX_WIDTH 48
#define MAX_HEIGHT 64

/* define fixed MCU size (8) */
#define BLOCK_WIDTH 8
#define BLOCK_HEIGHT 8
#define BLOCKS_X (MAX_WIDTH/BLOCK_WIDTH)
#define BLOCKS_Y (MAX_HEIGHT/BLOCK_HEIGHT)
#define NBLOCKS (BLOCKS_Y*BLOCKS_X)
#define BLOCK_SIZE (BLOCK_WIDTH*BLOCK_HEIGHT)

/*----------------------------------*/
/* JPEG format parsing markers here */
/*----------------------------------*/
#define SOI_MK	0xFFD8		/* start of image	*/
#define APP_MK	0xFFE0		/* custom, up to FFEF */
#define COM_MK	0xFFFE		/* commment segment	*/
#define SOF_MK	0xFFC0		/* start of frame	*/
#define SOS_MK	0xFFDA		/* start of scan	*/
#define DHT_MK	0xFFC4		/* Huffman table	*/
#define DQT_MK	0xFFDB		/* Quant. table		*/
#define DRI_MK	0xFFDD		/* restart interval	*/
#define EOI_MK	0xFFD9		/* end of image		*/
#define MK_MSK	0xFFF0
#define RST_MK(x)	( (0xFFF8&(x)) == 0xFFD0 )
			/* is x a restart interval ? */

/*-------------------------------------------------------- */
/* all kinds of macros here				*/
/*-------------------------------------------------------- */
#define first_quad(c)   ((c)>>4)        /* first 4 bits in file order */
#define second_quad(c)  ((c)&15)
#define HUFF_ID(class, id)  (2*(class)+(id))
#define DC_CLASS        0
#define AC_CLASS        1

/*-------------------------------------------------------*/
/* JPEG data types here					*/
/*-------------------------------------------------------*/
/* component descriptor structure */
typedef struct {
	unsigned char	CID;	/* component ID */
	char		QT;	/* QTable index, 2bits 	*/
	char		DC_HT;	/* DC table index, 1bit */
	char		AC_HT;	/* AC table index, 1bit */
	int		PRED;	/* DC predictor value */
} cd_t;

#ifndef EOF
#define EOF -1
#endif

#define DATA_MAGIC 0xda1a0000

#endif /* JPEG_H_ */
