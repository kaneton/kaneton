##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/arch/ia32/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Mon Nov  7 17:25:58 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

KANETON_LO		:=		kaneton.lo

KANETON_C		:=		as.c				\
					segment.c			

KANETON_O		:=		$(KANETON_C:.c=.o)

KANETON_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/kaneton/kaneton.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(KANETON_LO):		$(KANETON_O)
	$(call remove,$(KANETON_LO),)

	$(call archive,$(KANETON_LO),$(KANETON_O),)

clear:
	$(call remove,$(KANETON_O),)

	$(call remove,$(KANETON_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(KANETON_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KANETON_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
