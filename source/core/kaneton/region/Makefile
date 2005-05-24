##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/region
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:51:58 2005   mycure
##

include			../../../.common.mk

REGION			=		region.a

REGION_INCLUDE		=		$(_INCLUDE_DIR_)/region.h

REGION_O		=		region.o			

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(REGION_O)
	$(call ar-o-a,$(REGION),$(REGION_O),)
	$(call ranlib,$(REGION),)

distclean:
	$(call rm,$(REGION_O),)

	@$(_PURGE_)

	$(call rm,$(REGION),)

proto:
	$(call proto,$(REGION_INCLUDE),)
