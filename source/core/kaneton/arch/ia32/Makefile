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
## last update   Sun Jun 19 21:06:48 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

MACHDEP			:=		machdep.a

MACHDEP_C		:=		as.c				\
					segment.c			

MACHDEP_O		:=		$(MACHDEP_C:.c=.o)

MACHDEP_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/machdep.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(MACHDEP):		$(MACHDEP_O)
	$(call archive,$(MACHDEP),$(MACHDEP_O),)

	$(call static-linker,$(MACHDEP),)

clear:
	$(call remove,$(MACHDEP_O),)

	$(call remove,$(MACHDEP),)

	$(call purge,)

proto:
	$(call prototypes,$(MACHDEP_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MACHDEP_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
