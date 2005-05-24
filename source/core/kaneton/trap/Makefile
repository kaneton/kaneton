##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/trap
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:56:06 2005   mycure
##

include			../../../.common.mk

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
