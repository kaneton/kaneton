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
 * ---------- macros ----------------------------------------------------------
 */

/*
 * service name.
 */

#define SERVICE_IDE_NAME		"ide"

/*
 * service operations.
 */

#define IDE_SERVICE_IDE		1

/*
 * message type description.
 */

#define MESSAGE_TYPE_SERVICE_IDE	(MESSAGE_TYPE_CRT + 8)
#define MESSAGE_SIZE_SERVICE_IDE	(sizeof (t_service_ide_message))

/*
 * ide buffer size.
 */

#define IDE_SERVICE_MAX		100

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  union
  {
    struct
    {
      t_id			operation;
      union
      {
	struct
	{
	  char		text[IDE_SERVICE_MAX];
	}		ide;
      }			u;
    }			request;
    struct
    {
      union
      {
	struct
	{
	  char		text[IDE_SERVICE_MAX];
	}		ide;
      }			u;
    }			reply;
  } u;
}		t_service_ide_message;

#endif
