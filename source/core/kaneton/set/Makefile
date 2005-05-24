##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/set
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:54:14 2005   mycure
##
 
include			../../../.common.mk

SET			=		set.a

SET_INCLUDE		=		$(_INCLUDE_DIR_)/set.h

SET_O			=		set.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(SET_O)
	$(call ar-o-a,$(SET),$(SET_O),)
	$(call ranlib,$(SET),)

distclean:
	$(call rm,$(SET_O),)

	@$(_PURGE_)

	$(call rm,$(SET),)

proto:
	$(call proto,$(SET_INCLUDE),)
