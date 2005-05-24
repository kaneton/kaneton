##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/msg
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:50:09 2005   mycure
##

include			../../../.common.mk

MSG			=		msg.a

MSG_INCLUDE		=		$(_INCLUDE_DIR_)/msg.h

MSG_O			=		msg.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(MSG_O)
	$(call ar-o-a,$(MSG),$(MSG_O),)
	$(call ranlib,$(MSG),)

distclean:
	$(call rm,$(MSG_O),)

	@$(_PURGE_)

	$(call rm,$(MSG),)

proto:
	$(call proto,$(MSG_INCLUDE),)
