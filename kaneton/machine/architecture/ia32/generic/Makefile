#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...chine/architecture/ia32/generic/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       matthieu bucchianeri   [mon dec 24 19:29:49 2007]
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

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

GENERIC_LO	:=	$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/generic.lo

GENERIC_C	:=	apic.c					\
			cpuid.c					\
			gdt.c					\
			idt.c					\
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
			tlb.c					\
			tss.c

GENERIC_O	:=	$(GENERIC_C:.c=.o)

GENERIC_INCLUDE	:=	$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/apic.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/gdt.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/idt.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/ipi.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/mapping.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/misc.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/paging.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/pmode.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/segment.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/tss.h	\
			$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/include/local.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(GENERIC_LO)

$(GENERIC_LO):	$(GENERIC_O)
	$(call env_remove,$(GENERIC_LO),)

	$(call env_archive,$(GENERIC_LO),$(GENERIC_O),)

clear:
	$(call env_remove,$(GENERIC_O),)

	$(call env_remove,$(GENERIC_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(GENERIC_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(GENERIC_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
