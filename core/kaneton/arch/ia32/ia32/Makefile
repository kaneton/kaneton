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
## last update   Tue Aug 30 13:39:47 2005   mycure
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

IA32			:=		ia32.a

IA32_C			:=		

IA32_O			:=		$(IA32_C:.c=.o)

IA32_INCLUDE		:=		$(_MACHDEP_INCLUDE_DIR_)/ia32/ia32.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(IA32):		$(IA32_O)
	$(call remove,$(IA32),)

	$(call archive,$(IA32),$(IA32_O),)

	$(call static-linker,$(IA32),)

clear:
	$(call remove,$(IA32_O),)

	$(call remove,$(IA32),)

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
