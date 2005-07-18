##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/id
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## lidt update   Mon Jun 20 17:10:14 2005   mycure
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

ID			:=		id.a

ID_C			:=		id.c				

ID_O			:=		$(ID_C:.c=.o)

ID_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/id.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(ID):			$(ID_O)
	$(call remove,$(ID),)

	$(call archive,$(ID),$(ID_O),)

	$(call static-linker,$(ID),)

clear:
	$(call remove,$(ID_O),)

	$(call remove,$(ID),)

	$(call purge,)

proto:
	$(call prototypes,$(ID_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(ID_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
