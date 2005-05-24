##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/segment
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:53:51 2005   mycure
##
 
include			../../../.common.mk

SEGMENT			=		segment.a

SEGMENT_INCLUDE		=		$(_INCLUDE_DIR_)/segment.h

SEGMENT_O		=		segment.o			

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(SEGMENT_O)
	$(call ar-o-a,$(SEGMENT),$(SEGMENT_O),)
	$(call ranlib,$(SEGMENT),)

distclean:
	$(call rm,$(SEGMENT_O),)

	@$(_PURGE_)

	$(call rm,$(SEGMENT),)

proto:
	$(call proto,$(SEGMENT_INCLUDE),)
