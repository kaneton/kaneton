#ifndef CRT_H_
#define CRT_H_

#define MESSAGE_TYPE_CRT	32

#include <core/id.h>

static inline i_task	_crt_get_task_id(void)
{
  extern i_task __task_id;

  return __task_id;
}

static inline i_as	_crt_get_as_id(void)
{
  extern i_as __as_id;

  return __as_id;
}

static inline i_task	_crt_get_mod_id(void)
{
  extern i_task __mod_id;

  return __mod_id;
}

#endif
