/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/buckman/kaneton/services/mod/mod.c
 *
 * created       matthieu bucchianeri   [sat may  5 18:43:57 2007]
 * updated       matthieu bucchianeri   [fri feb 29 11:26:18 2008]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the module service load modules and act as a modules directory.
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libkaneton.h>
#include <crt.h>

#include "mod.h"
#include "mod-service.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

struct
{
  char*		name;
  i_task	id;
}		services[MOD_MAX_SERVICE];

/*
 * ---------- macro-functions -------------------------------------------------
 */


#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * this function loads a input (32 bits).
 *
 * steps:
 *
 * 1) reserve the task.
 * 2) loop through the ELF segments.
 * 3) reserve the segment and map in the input's address space.
 * 4) copy from the file to the memory.
 * 5) create the thread.
 * 6) pushes arguments onto the stack.
 * 7) run the input!
 * 8) register to the directory.
 */

t_error			mod_load_32(Elf32_Ehdr*		header,
				    char*		name)
{
  Elf32_Phdr*		phdr;
  int			i;
  i_task		tsk;
  i_as			as;
  i_segment		seg;
  i_region		reg;
  t_perms		perms;
  i_thread		thr;
  t_thread_context	ctx;
  t_stack		stack;
  struct
  {
    i_task		tskid;
    i_as		asid;
    i_task		mod;
    int			argc;
    char*		argv;
    char*		envp;
  }			args;

  /*
   * 1)
   */

  if (task_reserve(TASK_CLASS_PROGRAM,
		   TASK_BEHAV_TIMESHARING,
		   TASK_PRIOR_TIMESHARING,
		   &tsk) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (as_reserve(tsk, &as) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (i = 0, phdr = (Elf32_Phdr*)((char*)header + header->e_phoff);
       i < header->e_phnum;
       i++, phdr++)
    {
      if (phdr->p_type != PT_LOAD)
	{
	  printf(" -- mod: Unsupported program header, skipping.\n");
	  continue;
	}

      printf(" -- mod: Phdr LOAD at 0x%p (%c%c%c) of size %u\n",
	     phdr->p_vaddr,
	     phdr->p_flags & PF_R ? 'r' : '-',
	     phdr->p_flags & PF_W ? 'w' : '-',
	     phdr->p_flags & PF_X ? 'x' : '-',
	     phdr->p_memsz);

      if (phdr->p_memsz == 0)
	continue;

      /*
       * 3)
       */

      perms = 0;

      if (phdr->p_flags & PF_R)
	perms |= PERM_READ;
      if (phdr->p_flags & PF_W)
	perms |= PERM_WRITE;
      if (phdr->p_flags & PF_X)
	perms |= PERM_EXEC;

      if (segment_reserve(as,
			  PAGED_SIZE(phdr->p_memsz),
			  PERM_WRITE,
			  &seg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPT_FORCE,
			 phdr->p_vaddr,
			 PAGED_SIZE(phdr->p_memsz),
			 &reg) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /*
       * 4)
       */

      if (as_copy(_crt_get_as_id(), (t_vaddr)((char*)header + phdr->p_offset),
		  as, phdr->p_vaddr, phdr->p_filesz) != ERROR_NONE)
	return (ERROR_UNKNOWN);

      /* XXX blank BSS */

      if (segment_perms(seg, perms) != ERROR_NONE)
	return (ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (thread_reserve(tsk, THREAD_PRIOR, &thr) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = 4 * PAGESZ;

  if (thread_stack(thr, stack) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  if (thread_attribute_stack(thr, &stack.base) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  ctx.sp = stack.base + stack.size - 16;
  ctx.pc = (t_vaddr)header->e_entry;

  if (thread_load(thr, ctx) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 6)
   */

  args.tskid = tsk;
  args.asid = as;
  args.mod = _crt_get_task_id();
  args.argc = 0;
  args.argv = NULL;
  args.envp = NULL;

  if (thread_args(thr, &args, sizeof (args)) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 7)
   */

  if (task_state(tsk, SCHEDULER_STATE_RUN) != ERROR_NONE)
    return (ERROR_UNKNOWN);

  /*
   * 8)
   */

  for (i = 0; i < MOD_MAX_SERVICE && services[i].name != NULL; i++)
    ;

  if (i == MOD_MAX_SERVICE)
    {
      printf("warning: too many services. mod directory is full.\n");
    }
  else
    {
      services[i].name = strrchr(name, '/') + 1;
      services[i].id = tsk;

#if 0
      /* XXX */
      if (!strcmp(services[i].name, "cons-simple"))
	_crt_attach_cons_to(tsk);
      printf(" -- mod: attached to console driver.\n");
#endif
    }


  return (ERROR_NONE);
}

/*
 * this function loads a input (64 bits).
 */

t_error			mod_load_64(Elf64_Ehdr*		header,
				    char*		name)
{
  printf(" -- mod: Elf64 format not supported\n");

  return (ERROR_UNKNOWN);
}

/*
 * this function launches all inputs passed to the kernel.
 *
 * steps:
 *
 * 1) get elf header.
 * 2) load the input.
 * 3) go on to the next input.
 */

void			mod_launch(t_inputs*	inputs)
{
  int			i;
  t_input*		p;
  char*			name;
  Elf32_Ehdr*		elf;

  for (i = 0, p = (t_input*)(inputs + 1);
       i < inputs->ninputs;
       i++)
    {
      name = p->name;

      printf(" -- mod: loading input %s ... ", name);

      /*
       * 1)
       */

      elf = (Elf32_Ehdr*)(p + 1);

      if (strncmp(elf->e_ident, ELFMAG, SELFMAG))
	{
	  printf("not an ELF file\n");
	  goto next;
	}

      printf("\n");

      if (elf->e_ident[EI_CLASS] != ELFCLASS32 &&
	  elf->e_ident[EI_CLASS] != ELFCLASS64)
	{
	  printf(" -- mod: invalid ELF class\n");
	  goto next;
	}

      /*
       * 2)
       */

      if (elf->e_ident[EI_CLASS] == ELFCLASS32)
	{
	  if (mod_load_32(elf, name) != ERROR_NONE)
	    {
	      printf(" -- mod: error loading input\n");
	      goto next;
	    }
	}
      else
	{
	  if (mod_load_64((Elf64_Ehdr*)elf, name) != ERROR_NONE)
	    {
	      printf (" -- mod: error loading input\n");
	      goto next;
	    }
	}

      printf(" -- mod: loaded.\n");

      /*
       * 3)
       */

    next:
      p = (t_input*)((char*)p + sizeof (t_input) + p->size + strlen(name) + 1);
    }
}

/*
 * modules directory serving loop.
 */

void				mod_serve(void)
{
  t_service_mod_message*	message;
  i_node			sender;
  t_vsize			size;
  int				i;

  if ((message = malloc(MESSAGE_SIZE_SERVICE_MOD)) == NULL)
    {
      /* XXX fatal */
    }

  while (1)
    {
      if (message_receive(MESSAGE_TYPE_SERVICE_MOD,
			  (t_vaddr)message,
			  &size,
			  &sender) == ERROR_NONE)
	{
	  if (message->u.request.operation == MOD_SERVICE_GET_SERVICE)
	    {
	      for (i = 0; i < MOD_MAX_SERVICE && services[i].name != NULL; i++)
		if (!strcmp(services[i].name,
			    message->u.request.u.get_service.name))
		  break;

	      if (i == MOD_MAX_SERVICE || services[i].name == NULL)
		message->u.reply.u.get_service.id = ID_UNUSED;
	      else
		message->u.reply.u.get_service.id = services[i].id;

	      if (message_send(sender,
			       MESSAGE_TYPE_SERVICE_MOD,
			       (t_vaddr)message,
			       sizeof (*message)) != ERROR_NONE)
		{
		  /* XXX fatal */
		}
	    }
	}
    }
}

/*
 * service main.
 */

int			main(int argc, char** argv, char** envp)
{
  memset(services, 0, sizeof (services));

  mod_launch((t_inputs*)argv[0]);

  printf(" -- mod: moduler loader service started.\n");

  mod_serve();

  return 0;
}
