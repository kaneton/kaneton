##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/arch/ia32/ia32
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Tue Aug 30 13:40:49 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

KANETON			:=		kaneton.a

KANETON_C		:=		as.c				\
					segment.c			

KANETON_O		:=		$(KANETON_C:.c=.o)

KANETON_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/kaneton/kaneton.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(KANETON):		$(KANETON_O)
	$(call remove,$(KANETON),)

	$(call archive,$(KANETON),$(KANETON_O),)

	$(call static-linker,$(KANETON),)

clear:
	$(call remove,$(KANETON_O),)

	$(call remove,$(KANETON),)

	$(call purge,)

proto:
	$(call prototypes,$(KANETON_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KANETON_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
