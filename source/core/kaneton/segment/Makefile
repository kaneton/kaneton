##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 19 21:08:24 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#
 
include			../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SEGMENT			:=		segment.a

SEGMENT_C		:=		segment.c			

SEGMENT_O		:=		$(SEGMENT_C:.c=.o)

SEGMENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/segment.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SEGMENT):		$(SEGMENT_O)
	$(call archive,$(SEGMENT),$(SEGMENT_O),)

	$(call static-linker,$(SEGMENT),)

clear:
	$(call remove,$(SEGMENT_O),)

	$(call remove,$(SEGMENT),)

	$(call purge,)

proto:
	$(call prototypes,$(SEGMENT_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(SEGMENT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
