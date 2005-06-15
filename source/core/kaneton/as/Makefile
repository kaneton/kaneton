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
## last update   Tue Jun 14 19:21:43 2005   mycure
##
 
include			../../../.kaneton.mk

AS			=		as.a

AS_INCLUDE		=		$(_CORE_INCLUDE_DIR_)/kaneton/as.h

AS_O			=		as.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(AS_O)
	$(call archive,$(AS),$(AS_O),)

	$(call static-linker,$(AS),)

distclean:
	$(call remove,$(AS_O),)

	$(call purge,)

	$(call remove,$(AS),)

proto:
	$(call prototypes,$(AS_INCLUDE),)
