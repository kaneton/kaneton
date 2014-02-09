/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/libs/libsun4u/ofw/ofw.c
 *
 * created       matthieu bucchianeri   [wed aug  2 19:16:42 2006]
 * updated       matthieu bucchianeri   [mon aug 28 19:35:26 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the PROM pointer.
 */

int			(*prom)(t_ofw_args*) = ofw_wrap_init;

/*
 * standard output handle.
 */

t_ofw_handle		stdout;
t_ofw_node		root_node;

/*
 * the init wrapper, used to retrieve the real PROM pointer.
 */

int			ofw_wrap_init(t_ofw_args*		args)
{
  asm volatile ("mov %%g7, %0" : "=r" (prom));

  return openfw(args);
}

/*
 * make a call to OpenFirmware with specified command.
 */

t_status	ofw_call(t_ofw_args*		args)
{
  if (openfw(args) == -1)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * initialize OpenFirmware lib.
 */

t_status	ofw_init(void)
{
  t_ofw_handle	chosen;

  if (ofw_finddevice("/chosen", &chosen) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  if (ofw_getprop(chosen,
		  "stdout",
		  &stdout,
		  sizeof (stdout),
		  NULL) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  if (ofw_sibling(0, &root_node) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * get node's child.
 */

t_status	ofw_child(t_ofw_node		node,
			  t_ofw_node*		child)
{
  t_ofw_args	args;

  SET_NAME(args, "child");
  SET_NARGS(args, 1);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, node);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  *child = GET_RET(args, 0);

  if (*child <= 0)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * get node's sibling.
 */

t_status	ofw_sibling(t_ofw_node		node,
			    t_ofw_node*		sibling)
{
  t_ofw_args	args;

  SET_NAME(args, "peer");
  SET_NARGS(args, 1);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, node);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  *sibling = GET_RET(args, 0);

  if (*sibling <= 0)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * call OpenFirmware to get a device handle by its name.
 */

t_status	ofw_finddevice(const char*	dev,
			       t_ofw_handle*	handle)
{
  t_ofw_args	args;

  SET_NAME(args, "finddevice");
  SET_NARGS(args, 1);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, dev);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  *handle = GET_RET(args, 0);

  return STATUS_OK;
}

/*
 * call OpenFirmware to get an OpenBoot property.
 */

t_status	ofw_getprop(t_ofw_handle	handle,
			    const char*		prop,
			    void*		buf,
			    t_uint64		size,
			    t_uint64*		read)
{
  t_ofw_args	args;

  SET_NAME(args, "getprop");
  SET_NARGS(args, 4);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, handle);
  SET_ARG(args, 1, prop);
  SET_ARG(args, 2, buf);
  SET_ARG(args, 3, size);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  if (read)
    *read = GET_RET(args, 0);

  return STATUS_OK;
}

/*
 * call OFW to open a device.
 */

t_status	ofw_open(const char*		name,
			 t_ofw_handle*		handle)
{
  t_ofw_args	args;

  SET_NAME(args, "open");
  SET_NARGS(args, 1);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, name);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  *handle = GET_RET(args, 0);

  return STATUS_OK;
}

/*
 * call OFW to close a device.
 */

t_status	ofw_close(t_ofw_handle		handle)
{
  t_ofw_args	args;

  SET_NAME(args, "close");
  SET_NARGS(args, 1);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, handle);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * OpenFirmware reading.
 */

t_status	ofw_read(t_ofw_handle		handle,
			 char*			string,
			 t_uint64		len,
			 t_uint64*		read)
{
  t_ofw_args	args;

  SET_NAME(args, "read");
  SET_NARGS(args, 3);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, handle);
  SET_ARG(args, 1, string);
  SET_ARG(args, 2, len);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  if (read)
    *read = GET_RET(args, 0);

  return STATUS_OK;
}

/*
 * call OpenFirmware to write some data to a file.
 */

t_status	ofw_write(t_ofw_handle		handle,
			  const char*		string,
			  t_uint64		len,
			  t_uint64*		written)
{
  t_ofw_args	args;

  SET_NAME(args, "write");
  SET_NARGS(args, 3);
  SET_NRETS(args, 1);
  SET_ARG(args, 0, handle);
  SET_ARG(args, 1, string);
  SET_ARG(args, 2, len);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  if (written)
    *written = GET_RET(args, 0);

  return STATUS_OK;
}

/*
 * call OpenFirmware to exit to OpenBoot.
 */

t_status	ofw_exit(void)
{
  t_ofw_args	args;

  SET_NAME(args, "exit");
  SET_NARGS(args, 0);
  SET_NRETS(args, 0);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * call OpenFirmware to pause to OpenBoot.
 */

t_status	ofw_enter(void)
{
  t_ofw_args	args;

  SET_NAME(args, "enter");
  SET_NARGS(args, 0);
  SET_NRETS(args, 0);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}

/*
 * call OpenFirmware to interpret a Forth program.
 */

t_status	ofw_interpret(const char*	cmd)
{
  t_ofw_args	args;

  SET_NAME(args, "interpret");
  SET_NARGS(args, 1);
  SET_NRETS(args, 0);
  SET_ARG(args, 0, cmd);

  if (ofw_call(&args) != STATUS_OK)
    return STATUS_UNKNOWN_ERROR;

  return STATUS_OK;
}
