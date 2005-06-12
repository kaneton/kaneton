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
## last update   Sun Jun 12 22:25:22 2005   mycure
##
 
include			../../../.kaneton.mk

SEGMENT			=		segment.a

SEGMENT_INCLUDE		=		$(_INCLUDE_DIR_)/segment.h

SEGMENT_O		=		segment.o			

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(SEGMENT_O)
	$(call archive,$(SEGMENT),$(SEGMENT_O),)
	$(call static-library,$(SEGMENT),)

distclean:
	$(call remove,$(SEGMENT_O),)

	$(call purge,)

	$(call remove,$(SEGMENT),)

proto:
	$(call prototypes,$(SEGMENT_INCLUDE),)
