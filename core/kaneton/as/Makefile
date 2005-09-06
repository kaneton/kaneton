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
## last update   Tue Sep  6 11:09:08 2005   mycure
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

AS_LO			:=		as.lo

AS_C			:=		as.c				

AS_O			:=		$(AS_C:.c=.o)

AS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/as.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(AS_LO):		$(AS_O)
	$(call remove,$(AS_LO),)

	$(call archive,$(AS_LO),$(AS_O),)

clear:
	$(call remove,$(AS_O),)

	$(call remove,$(AS_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(AS_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(AS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
