
/**
 ** Auto-generated file, dont modify directly
 ** your changes will be lost !
 **
 ** Generated by DSX on 2006-04-12 16:13:48.073524
 ** by unknown@callisto
 **/
#ifndef _DEMUX_PROTO_H
#define _DEMUX_PROTO_H

#include <dsx.h>

typedef struct _demux_args_t demux_args_t;
struct _demux_args_t {
	dsx_mwmr_t input;
	#define _demux_arg_type_input dsx_mwmr_t;
	dsx_mwmr_t quanti;
	#define _demux_arg_type_quanti dsx_mwmr_t;
	dsx_mwmr_t huffman;
	#define _demux_arg_type_huffman dsx_mwmr_t;
	dsx_mwmr_t output;
	#define _demux_arg_type_output dsx_mwmr_t;

	/* Just to avoid empty arg list */
	int is_unused;
};

#ifndef DSX_INMAIN

#define GET_ARG(port) ({ \
dsx_log_printf( TRACE, "%s:%d: arg %s = %p\n", __FILE__, __LINE__, #port, _func_args->port ); \
/* (_demux_arg_type_ ## port)(_func_args->port)); */ \
(_func_args->port); \
})

typedef void demux_func_t(demux_args_t*);

#define FUNC(sym) void demux_func_ ## sym(demux_args_t *_func_args)

/* Forward declarations, to enforce types */

demux_func_t demux_func_demux;


#else /* is DSX_INMAIN */
/* Declaration of launcher */
pthread_t demux_launcher( demux_args_t * );
#endif /* DSX_INMAIN */

#endif /* _DEMUX_PROTO_H */
