/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/modules/test/include/test.h
 *
 * created       julien quintard   [wed jun  6 16:25:44 2007]
 * updated       julien quintard   [fri may  1 16:52:27 2009]
 */

#ifndef MODULES_TEST_TEST_H
#define MODULES_TEST_TEST_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * this type represents a test function pointer.
 */
typedef				void (*f_test)(void);

/*
 * the tests inventory.
 *
 * note that the inventory array 'tests' is automatically generated.
 */
typedef struct
{
  char*			symbol;
  f_test		function;
}			s_test;


typedef struct
{
  char			buffer[128];
  t_uint32		size;
}			m_test;

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * types
 */

#define TEST_TYPE_COMMAND 1
#define TEST_TYPE_TEXT 2

/*
 * magic
 */
#define TEST_MAGIC 0xf4859632

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../test.c
 */

/*
 * ../test.c
 */

t_uint32		test_checksum(void*			data,
				      t_uint32			size);

t_error			test_send(t_uint8			type,
				  char*				message,
				  t_uint32			length);

t_error			test_receive(t_uint8*			type,
				     char*			message);

t_error			test_issue(char*			command);

t_error			test_flush(void);

int			test_write(char				c);

f_test			test_locate(char*			symbol);

void			test_call(char*				symbol);

void			test_dump(void);

t_error			test_run(void);


/*
 * eop
 */

#endif
