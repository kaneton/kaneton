#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...e/architecture/ia32/educational/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       matthieu bucchianeri   [sat jun 16 17:41:27 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

ARCHITECTURE_LOW_C	:=	gdt.c				\
				idt.c				\
				ldt.c				\
				paging.c			\
				pd.c				\
				pmode.c				\
				pt.c				\
				tlb.c

ARCHITECTURE_HI_C	:=	context.c			\
				syscalls.c			\
				virtual.c

ARCHITECTURE_C		:=	$(ARCHITECTURE_LOW_C)		\
				$(ARCHITECTURE_HI_C)

ARCHITECTURE_O		:=	$(ARCHITECTURE_C:.c=.o)

ARCHITECTURE_INCLUDE	:=	$(_ARCHITECTURE_INCLUDE_DIR_)/gdt.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/idt.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/paging.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/pmode.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/syscalls.h\
				$(_ARCHITECTURE_INCLUDE_DIR_)/virtual.h

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_ARCHITECTURE_LO_)

$(_ARCHITECTURE_LO_):		$(ARCHITECTURE_O)
	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_archive,$(_ARCHITECTURE_LO_),$(ARCHITECTURE_O),)

clear:
	$(call env_remove,$(ARCHITECTURE_O),)

	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ARCHITECTURE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ARCHITECTURE_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
