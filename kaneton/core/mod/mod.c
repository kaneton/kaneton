/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/core/mod/mod.c
 *
 * created       matthieu bucchianeri   [sat may  5 18:43:57 2007]
 * updated       matthieu bucchianeri   [sat may  5 20:20:59 2007]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * the module manager initialize the drivers and services passed to
 * the kernel.
 *
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * mod manager variable.
 */

m_mod*			mod = NULL;

/*
 * ---------- externs ---------------------------------------------------------
 */

extern t_init*		init;
extern i_as		kasid;

/*
 * ---------- functions -------------------------------------------------------
 */

// XXX move elsewhere

#define PAGED_SIZE(_size_)						\
  ((_size_) % PAGESZ ?							\
   (_size_) + PAGESZ - (_size_) % PAGESZ :				\
   (_size_))

/*
 * this function loads a module (32 bits).
 *
 * steps:
 *
 * 1) reserve the task.
 * 2) loop through the ELF segments.
 * 3) reserve the segment and map in the module's address space.
 * 4) copy from the file to the memory.
 * 5) create the thread.
 * 6) run the module!
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
  o_thread*		o;
  t_thread_context	ctx;
  t_stack		stack;

  MOD_ENTER(mod);

  /*
   * 1)
   */

  if (task_reserve(TASK_CLASS_PROGRAM,
		   TASK_BEHAV_TIMESHARING,
		   TASK_PRIOR_TIMESHARING,
		   &tsk) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  if (as_reserve(tsk, &as) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  /*
   * 2)
   */

  for (i = 0, phdr = (Elf32_Phdr*)((char*)header + header->e_phoff);
       i < header->e_phnum;
       i++, phdr++)
    {
      if (phdr->p_type != PT_LOAD)
	{
	  cons_msg('!', "Unsupported program header, skipping.\n");
	  continue;
	}

      cons_msg('#', "Phdr LOAD at 0x%p (%c%c%c) of size %u\n",
	       phdr->p_vaddr,
	       phdr->p_flags & PF_R ? 'r' : '-',
	       phdr->p_flags & PF_W ? 'w' : '-',
	       phdr->p_flags & PF_X ? 'x' : '-',
	       phdr->p_memsz);

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
	MOD_LEAVE(mod, ERROR_UNKNOWN);

      if (region_reserve(as,
			 seg,
			 0,
			 REGION_OPT_FORCE,
			 phdr->p_vaddr,
			 PAGED_SIZE(phdr->p_memsz),
			 &reg) != ERROR_NONE)
	MOD_LEAVE(mod, ERROR_UNKNOWN);

      /*
       * 4)
       */

      if (segment_write(seg,
			0,
			(char*)header + phdr->p_offset,
			phdr->p_filesz) != ERROR_NONE)
	MOD_LEAVE(mod, ERROR_UNKNOWN);

      if (segment_perms(seg, perms) != ERROR_NONE)
	MOD_LEAVE(mod, ERROR_UNKNOWN);
    }

  /*
   * 5)
   */

  if (thread_reserve(tsk, THREAD_PRIOR, &thr) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  stack.base = 0;
  stack.size = THREAD_MIN_STACKSZ;

  if (thread_stack(thr, stack) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  if (thread_get(thr, &o) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  ctx.sp = o->stack + o->stacksz - 16;
  ctx.pc = (t_vaddr)header->e_entry;

  if (thread_load(thr, ctx) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  /*
   * 6)
   */

  if (task_state(tsk, SCHED_STATE_RUN) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  MOD_LEAVE(mod, ERROR_NONE);
}

/*
 * this function loads a module (64 bits).
 */

t_error			mod_load_64(Elf64_Ehdr*		header,
				    char*		name)
{
  MOD_ENTER(mod);

  cons_msg('!', "Elf64 format not supported\n");

  MOD_LEAVE(mod, ERROR_UNKNOWN);
}

/*
 * this function launches all modules passed to the kernel.
 *
 * steps:
 *
 * 1) map the modules in the kernel.
 * 2) get elf header.
 * 3) load the module.
 * 4) go on to the next module.
 * 5) unmap modules.
 */

t_error			mod_launch(void)
{
  int			i;
  t_module*		p;
  t_modules*		modules;
  i_region		reg;
  char*			name;
  Elf32_Ehdr*		elf;

  MOD_ENTER(mod);

  /*
   * 1)
   */

  if (region_reserve(kasid,
		     (i_segment)(t_paddr)init->modules,
		     0,
		     REGION_OPT_NONE,
		     0,
		     init->modulessz,
		     &reg) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  modules = (t_modules*)(t_vaddr)reg;

  for (i = 0, p = (t_module*)(modules + 1);
       i < modules->nmodules;
       i++)
    {
      name = (char*)modules + (p->name - (char*)init->modules);

      cons_msg('+', "loading module %s ... ", name);

      /*
       * 2)
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
	  cons_msg('!', "invalid ELF class\n");
	  goto next;
	}

      /*
       * 3)
       */

      if (elf->e_ident[EI_CLASS] == ELFCLASS32)
	{
	  if (mod_load_32(elf, name) != ERROR_NONE)
	    {
	      cons_msg('!', "error loading module\n");
	      goto next;
	    }
	}
      else
	{
	  if (mod_load_64((Elf64_Ehdr*)elf, name) != ERROR_NONE)
	    {
	      cons_msg('!', "error loading module\n");
	      goto next;
	    }
	}

      cons_msg('+', "loaded.\n");

      /*
       * 4)
       */

    next:
      p = (t_module*)((char*)p + sizeof (t_module) + p->size + strlen(name) + 1);
    }

  /*
   * 5)
   */

  if (region_release(kasid, reg) != ERROR_NONE)
    MOD_LEAVE(mod, ERROR_UNKNOWN);

  MOD_LEAVE(mod, ERROR_NONE);
}

/*
 * this function initialises the mod manager.
 *
 * steps:
 *
 * 1) allocate some memory for the manager structure.
 * 2) initialise a statistic object.
 */

t_error			mod_init(void)
{
  /*
   * 1)
   */

  if ((mod = malloc(sizeof(m_mod))) == NULL)
    {
      cons_msg('!', "mod: cannot allocate memory for the mod "
	       "manager structure\n");

      return (ERROR_UNKNOWN);
    }

  memset(mod, 0x0, sizeof(m_mod));

  /*
   * 2)
   */

  STATS_RESERVE("mod", &mod->stats);

  return (ERROR_NONE);

}

/*
 * this function cleans the mod manager.
 *
 * steps:
 *
 * 1) release the statistics object.
 * 2) free the manager structure.
 */

t_error			mod_clean(void)
{
  /*
   * 1)
   */

  STATS_RELEASE(mod->stats);

  /*
   * 2)
   */

  free(mod);

  return (ERROR_NONE);
}
