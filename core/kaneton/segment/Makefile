##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/segment
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Mon Nov  7 17:37:46 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#
 
include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SEGMENT_LO		:=		segment.lo

SEGMENT_C		:=		segment.c			

SEGMENT_O		:=		$(SEGMENT_C:.c=.o)

SEGMENT_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/segment.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SEGMENT_LO):		$(SEGMENT_O)
	$(call remove,$(SEGMENT_LO),)

	$(call archive,$(SEGMENT_LO),$(SEGMENT_O),)

clear:
	$(call remove,$(SEGMENT_O),)

	$(call remove,$(SEGMENT_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(SEGMENT_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(SEGMENT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
