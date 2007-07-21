#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...e/architecture/ia32/educational/Makefile
#
# created       julien quintard   [sun dec 10 18:38:17 2006]
# updated       julien quintard   [sat jul 21 19:24:50 2007]
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

ARCHITECTURE_C		:=	context.c				\
				interrupt.c				\
				syscalls.c				\
				virtual.c

ARCHITECTURE_O		:=	$(ARCHITECTURE_C:.c=.o)

ARCHITECTURE_INCLUDE	:=	$(_ARCHITECTURE_INCLUDE_DIR_)/context.h	\
				$(_ARCHITECTURE_INCLUDE_DIR_)/interrupt.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/syscalls.h \
				$(_ARCHITECTURE_INCLUDE_DIR_)/virtual.h

GENERIC_DIR		:=	$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic

GENERIC_LO		:=	$(_MACHINE_ARCHITECTURE_DIR_)/ia32/generic/generic.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_ARCHITECTURE_LO_)

$(_ARCHITECTURE_LO_):		$(ARCHITECTURE_O) $(GENERIC_LO)
	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_archive,$(_ARCHITECTURE_LO_),$(ARCHITECTURE_O)	\
						$(GENERIC_LO),)

dependencies:
	$(call env_launch,$(GENERIC_DIR)/Makefile,,)

clear:
	$(call env_launch,$(GENERIC_DIR)/Makefile,clear,)

	$(call env_remove,$(ARCHITECTURE_O),)

	$(call env_remove,$(_ARCHITECTURE_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ARCHITECTURE_INCLUDE),)

	$(call env_launch,$(GENERIC_DIR)/Makefile,prototypes,)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ARCHITECTURE_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
