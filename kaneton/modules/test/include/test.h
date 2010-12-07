/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...ABLE/kaneton/modules/test/include/test.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [mon nov 29 12:20:35 2010]
 */

#ifndef MODULES_TEST_TEST_H
#define MODULES_TEST_TEST_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type represents a test function pointer.
 */

typedef				void (*f_module_test)(void);

/*
 * the tests inventory.
 *
 * note that the inventory array 'tests' is automatically generated.
 */

typedef struct
{
  char*			symbol;
  f_module_test		function;
}			s_module_test_function;

typedef struct
{
  char*			command;
  t_error		(*function)(char*);
}			s_module_test_command;

/*
 * the module structure.
 */

typedef struct
{
  char			buffer[128];
  t_uint32		size;
}			m_module_test;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * types
 */

#define MODULE_TEST_TYPE_COMMAND	0x1
#define MODULE_TEST_TYPE_TEXT		0x2

/*
 * magic
 */

#define MODULE_TEST_MAGIC		0xf4859632

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../test.c
 */

/*
 * ../test.c
 */

t_uint32		module_test_checksum(void*		data,
					     t_uint32		size);

t_error			module_test_send(t_uint8		type,
					 char*			message,
					 t_uint32		length);

t_error			module_test_receive(t_uint8*		type,
					    char*		message);

t_error			module_test_issue(char*			command);

t_error			module_test_flush(void);

void			module_test_character(char			c);

t_error			module_test_locate(char*		symbol,
					   f_module_test*	function);

t_error			module_test_call(char*			symbol);

void			module_test_dump(void);

t_error			module_test_run(void);

t_error			module_test_load(void);

t_error			module_test_unload(void);


/*
 * eop
 */

#endif
