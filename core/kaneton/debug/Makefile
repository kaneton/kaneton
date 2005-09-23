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
## last update   Thu Sep 15 16:56:37 2005   mycure
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

DEBUG_LO		:=		debug.lo

DEBUG_C			:=		cons.c				
#					serial.c			

DEBUG_O			:=		$(DEBUG_C:.c=.o)

DEBUG_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/serial.h
					
CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(DEBUG_LO):		$(DEBUG_O)
	$(call remove,$(DEBUG_LO),)

	$(call archive,$(DEBUG_LO),$(DEBUG_O),)

clear:
	$(call remove,$(DEBUG_O),)

	$(call remove,$(DEBUG_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(DEBUG_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(DEBUG_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
