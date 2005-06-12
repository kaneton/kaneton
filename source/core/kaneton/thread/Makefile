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
## last update   Sun Jun 12 21:18:20 2005   mycure
##

include			../../../.kaneton.mk

THREAD			=		thread.a

THREAD_INCLUDE		=		$(_INCLUDE_DIR_)/thread.h

THREAD_O		=		thread.o			

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(THREAD_O)
	$(call ar-o-a,$(THREAD),$(THREAD_O),)
	$(call ranlib,$(THREAD),)

distclean:
	$(call rm,$(THREAD_O),)

	@$(_PURGE_)

	$(call rm,$(THREAD),)

proto:
	$(call proto,$(THREAD_INCLUDE),)
