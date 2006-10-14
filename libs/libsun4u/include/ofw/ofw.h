/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/include/ofw/ofw.h
 *
 * created       matthieu bucchianeri   [thu aug 10 15:59:10 2006]
 * updated       matthieu bucchianeri   [mon aug 28 19:14:11 2006]
 */

#ifndef LIBSUN4U_OFW_H
#define LIBSUN4U_OFW_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

# define OFW_MAX_RETURNS	3
# define OFW_MAX_ARGS		(7 + (OFW_MAX_RETURNS))

/*
 * ---------- macro functions--------------------------------------------------
 */

/*
 * manipulate ofw arguments.
 */

# define SET_NAME(_args_,_name_)					\
  args.name = (t_uint64)(_name_)

# define SET_NARGS(_args_,_n_)						\
  args.nargs = (_n_)

# define SET_NRETS(_args_,_n_)						\
  args.nrets = (_n_)

# define SET_ARG(_args_,_n_,_val_)					\
  args.args[_n_] = (t_uint64)(_val_)

# define GET_RET(_args_,_n_)						\
  args.args[args.nargs + (_n_)]

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_uint64	name;
  t_uint64	nargs;
  t_uint64	nrets;
  t_uint64	args[OFW_MAX_ARGS];
}		t_ofw_args;

typedef unsigned int	t_ofw_handle;
typedef int		t_ofw_node;

#endif
