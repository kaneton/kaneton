##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/map
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:16:55 2005   mycure
##

include			../../../.kaneton.mk

MAP			=		map.a

MAP_INCLUDE		=		$(_INCLUDE_DIR_)/map.h

MAP_O			=		map.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(MAP_O)
	$(call ar-o-a,$(MAP),$(MAP_O),)
	$(call ranlib,$(MAP),)

distclean:
	$(call rm,$(MAP_O),)

	@$(_PURGE_)

	$(call rm,$(MAP),)

proto:
	$(call proto,$(MAP_INCLUDE),)
