##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/as
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:16:27 2005   mycure
##

include			../../../.kaneton.mk

AS			=		as.a

AS_INCLUDE		=		$(_INCLUDE_DIR_)/as.h

AS_O			=		as.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(AS_O)
	$(call ar-o-a,$(AS),$(AS_O),)
	$(call ranlib,$(AS),)

distclean:
	$(call rm,$(AS_O),)

	@$(_PURGE_)

	$(call rm,$(AS),)

proto:
	$(call proto,$(AS_INCLUDE),)
