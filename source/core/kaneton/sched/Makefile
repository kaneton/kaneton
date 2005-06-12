##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/sched
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:17:24 2005   mycure
##

include			../../../.kaneton.mk

SCHED			=		sched.a

SCHED_INCLUDE		=		$(_INCLUDE_DIR_)/sched.h

SCHED_O			=		sched.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(SCHED_O)
	$(call ar-o-a,$(SCHED),$(SCHED_O),)
	$(call ranlib,$(SCHED),)

distclean:
	$(call rm,$(SCHED_O),)

	@$(_PURGE_)

	$(call rm,$(SCHED),)

proto:
	$(call proto,$(SCHED_INCLUDE),)
