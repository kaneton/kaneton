#ifndef IDE_SHARE_H_
#define IDE_SHARE_H_

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libkaneton.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crt.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * service name.
 */

#define DRIVER_IDE_NAME			"ide"

/*
 * service operations.
 */

#define IDE_DRIVER_REQ_READ		1
#define IDE_DRIVER_REP_READ		2
#define IDE_DRIVER_REP_ERR		3
#define IDE_DRIVER_REP_WRITE		4
#define IDE_DRIVER_REQ_WRITE		5

/*
 * message type description.
 */

#define MESSAGE_TYPE_DRIVER_IDE	(MESSAGE_TYPE_CRT + 8)
#define MESSAGE_SIZE_DRIVER_IDE	(sizeof (t_driver_ide_message))

/*
 * ide buffer size.
 */

#define IDE_DRIVER_MAX		512 //it is also the sector size


/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_id		operation;
  union
  {
    struct
    {
      uint8_t	ctr;
      uint8_t	dev;
      uint32_t	start;
      char	buf[IDE_DRIVER_MAX];
    }		req_write;
    struct
    {
      uint8_t	ctr;
      uint8_t	dev;
      uint32_t	start;
    }		req_read;
    struct
    {
      char	buf[IDE_DRIVER_MAX];
    }		rep_read;
    struct
    {
      char	text[IDE_DRIVER_MAX];
    }		rep_err;
  } u;
}			t_driver_ide_message;

#endif
