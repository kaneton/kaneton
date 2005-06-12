##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/arch/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:16:16 2005   mycure
##

include			../../../../.kaneton.mk

MACHDEP			=		machdep.a

MACHDEP_INCLUDE		=		$(_MACHDEP_INCLUDE_DIR_)/machdep.h

MACHDEP_O		=		as.o				\
					cpu.o				\
					map.o				\
					msg.o				\
					region.o			\
					sched.o				\
					segment.o			\
					set.o				\
					task.o				\
					thread.o			\
					time.o				\
					trap.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(MACHDEP_O)
	$(call ar-o-a,$(MACHDEP),$(MACHDEP_O),)
	$(call ranlib,$(MACHDEP),)

distclean:
	$(call rm,$(MACHDEP_O),)

	@$(_PURGE_)

	$(call rm,$(MACHDEP),)

proto:
	$(call proto,$(MACHDEP_INCLUDE),)
