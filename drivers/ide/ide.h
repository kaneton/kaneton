#ifndef IDE_H_
#define IDE_H_

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libkaneton.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>

/*
 * ---------- define ----------------------------------------------------------
 */
//port head and disk
#define PHD_1	0x1f6 
#define PHD_2	0x176
#define PHD_3	0xf6
#define PHD_4	0x76
//head and disk
#define	HD_M	0xa0	
#define	HD_S	0xb0


/*
 * ---------- macros ----------------------------------------------------------
 */



/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  uint8_t		dev[8];
  uint16_t		buf[256];
}			t_driver_ide;


#endif
