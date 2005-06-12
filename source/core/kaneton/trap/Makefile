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
## last update   Sun Jun 12 21:18:36 2005   mycure
##

include			../../../.kaneton.mk

TRAP			=		trap.a

TRAP_INCLUDE		=		$(_INCLUDE_DIR_)/trap.h

TRAP_O			=		trap.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(TRAP_O)
	$(call ar-o-a,$(TRAP),$(TRAP_O),)
	$(call ranlib,$(TRAP),)

distclean:
	$(call rm,$(TRAP_O),)

	@$(_PURGE_)

	$(call rm,$(TRAP),)

proto:
	$(call proto,$(TRAP_INCLUDE),)
