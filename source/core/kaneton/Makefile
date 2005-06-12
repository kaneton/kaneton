##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 22:24:44 2005   mycure
##

include			../../.kaneton.mk

SUBDIRS			=		arch				\
					segment				

KANETON			=		kaneton

KANETON_INCLUDE		=		$(_INCLUDE_DIR_)/kaneton.h

KANETON_O		=		kaneton.o

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(KANETON_O)
	@for i in $(SUBDIRS) ; do					\
	$(CD) $$i							; \
	$(MAKE)								; \
	$(CD) ..							; \
	done

	$(call dynamic-library,$(KANETON),	$(KANETON_O)		\
						$(_MACHDEP_A_)		\
						$(_AS_A_)		\
						$(_CPU_A_)		\
						$(_MAP_A_)		\
						$(_MSG_A_)		\
						$(_REGION_A_)		\
						$(_SCHED_A_)		\
						$(_SEGMENT_A_)		\
						$(_SET_A_)		\
						$(_TASK_A_)		\
						$(_THREAD_A_)		\
						$(_TIME_A_)		\
						$(_TRAP_A_)		\
						$(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/kaneton.lds)

distclean:
	@for i in $(SUBDIRS) ; do					\
	$(CD) $$i							; \
	$(MAKE) distclean						; \
	$(CD) ..							; \
	done

	$(call remove,$(KANETON_O),)

	$(call purge,)

	$(call remove,$(KANETON),)

proto:
	$(call prototypes,$(KANETON_INCLUDE),)
