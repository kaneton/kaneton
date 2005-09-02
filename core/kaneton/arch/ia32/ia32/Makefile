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
## last update   Wed Aug 31 16:42:53 2005   mycure
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

IA32_LO			:=		ia32.lo

IA32_C			:=		paging.c			\

IA32_O			:=		$(IA32_C:.c=.o)

IA32_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/ia32/ia32.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(IA32_LO):		$(IA32_O)
	$(call remove,$(IA32_LO),)

	$(call archive,$(IA32_LO),$(IA32_O),)

clear:
	$(call remove,$(IA32_O),)

	$(call remove,$(IA32_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(IA32_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(IA32_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
