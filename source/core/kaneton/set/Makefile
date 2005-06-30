##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/set
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Mon Jun 20 21:42:28 2005   mycure
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

SET			:=		set.a

SET_C			:=		set.c				\
					set_ll.c			

SET_O			:=		$(SET_C:.c=.o)

SET_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/set.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(SET):			$(SET_O)
	$(call remove,$(SET),)

	$(call archive,$(SET),$(SET_O),)

	$(call static-linker,$(SET),)

clear:
	$(call remove,$(SET_O),)

	$(call remove,$(SET),)

	$(call purge,)

proto:
	$(call prototypes,$(SET_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(SET_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
