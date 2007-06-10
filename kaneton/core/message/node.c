/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          kaneton/core/message/node.c
 *
 * created       julian pidancet   [thu mar 29 19:31:14 2007]
 * updated       julian pidancet   [thu mar 29 19:31:14 2007]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

extern m_kernel *kernel;
extern i_task ktask;

/*
 * ---------- functions -------------------------------------------------------
 */

i_machine get_node_machine(i_node node)
{
  return node.machine;
}

i_task get_node_task(i_node node)
{
  return node.task;
}

void get_node(i_machine m, i_task t, i_node* node)
{
  node->machine = m;
  node->task = t;
}

void get_kernel_node(i_node* node)
{
  node->machine = kernel->machine;
  node->task = ktask;
}

t_uint32 is_local_node(i_node node)
{
  return (node.machine == kernel->machine);
}

t_uint32 node_cmp(i_node a, i_node b)
{
  return (a.machine == b.machine && a.task == b.task);
}

