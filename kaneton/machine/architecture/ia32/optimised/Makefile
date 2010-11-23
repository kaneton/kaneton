#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/XYZ/...ine/architecture/ia32/optimised/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [tue nov 23 20:02:41 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/machine/architecture

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

ARCHITECTURE_C		:=	apic.c					\
				context.c				\
				cpuid.c					\
				gdt.c					\
				idt.c					\
				interrupt.c				\
				ipi.c					\
				ldt.c					\
				local.c					\
				mapping.c				\
				paging.c				\
				pd.c					\
				pmode.c					\
				pt.c					\
				segment.c				\
				sse.c					\
				syscalls.c				\
				tlb.c					\
				tss.c					\
				virtual.c

ARCHITECTURE_O		:=	$(ARCHITECTURE_C:.c=.o)

ARCHITECTURE_INCLUDE	:=	$(_ARCHITECTURE_INCLUDE_DIR_)/apic.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/context.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/gdt.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/idt.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/interrupt.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/ipi.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/local.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/mapping.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/misc.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/paging.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/pmode.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/segment.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/syscalls.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/tss.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/virtual.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_ARCHITECTURE_LO_)

$(_ARCHITECTURE_LO_):		$(ARCHITECTURE_O)
	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_archive,$(_ARCHITECTURE_LO_),$(ARCHITECTURE_O),)

dependencies:

clear:
	$(call env_remove,$(ARCHITECTURE_O),)

	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ARCHITECTURE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ARCHITECTURE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
