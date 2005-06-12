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
## last update   Sun Jun 12 22:23:51 2005   mycure
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
	$(call archive,$(MACHDEP),$(MACHDEP_O),)
	$(call static-library,$(MACHDEP),)

distclean:
	$(call remove,$(MACHDEP_O),)

	$(call purge,)

	$(call remove,$(MACHDEP),)

proto:
	$(call prototypes,$(MACHDEP_INCLUDE),)
