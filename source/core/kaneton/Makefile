##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 18:39:59 2005   mycure
##

include			../../.common.mk

SUBDIRS			=		arch				\
					as				\
					cpu				\
					map				\
					msg				\
					region				\
					sched				\
					segment				\
					set				\
					task				\
					thread				\
					time				\
					trap

KANETON			=		kaneton

KANETON_INCLUDE		=		$(_INCLUDE_DIR_)/kaneton.h

KANETON_O		=		kaneton.o

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(KANETON_O)
	@for i in $(SUBDIRS) ; do					\
	cd $$i								; \
	make								; \
	cd ..								; \
	done

	$(call ld,$(KANETON),	$(KANETON_O)				\
				$(_MACHDEP_A_)				\
				$(_AS_A_)				\
				$(_CPU_A_)				\
				$(_MAP_A_)				\
				$(_MSG_A_)				\
				$(_REGION_A_)				\
				$(_SCHED_A_)				\
				$(_SEGMENT_A_)				\
				$(_SET_A_)				\
				$(_TASK_A_)				\
				$(_THREAD_A_)				\
				$(_TIME_A_)				\
				$(_TRAP_A_)				\
				$(_LIBC_A_),				\
	  -T $(_LDS_DIR_)/kaneton.lds)

distclean:
	@for i in $(SUBDIRS) ; do					\
	cd $$i								; \
	make distclean							; \
	cd ..								; \
	done

	$(call rm,$(KANETON_O),)

	@$(_PURGE_)

	$(call rm,$(KANETON),)

proto:
	$(call proto,$(KANETON_INCLUDE),)
