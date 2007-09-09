#ifndef ALLOC_H_
#define ALLOC_H_

#include <stdlib.h>

#define MEM_SCOPE_CONTEXT	0
#define MEM_SCOPE_SYS		0

#define	mem_alloc(_size_,_scope_)	malloc(_size_)
#define mem_free(_ptr_)			free(_ptr_)

#endif
