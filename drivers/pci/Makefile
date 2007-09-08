##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/pci/Makefile
##
## created       julien quintard   [fri feb 11 03:00:09 2005]
## updated       matthieu bucchianeri   [sat may  5 18:22:24 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

PCI			:=		pci

PCI_C			:=		pci.c

PCI_O			:=		$(PCI_C:.c=.o)

PCI_INCLUDE		:=		pci.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(PCI)

$(PCI):		libs $(PCI_O)
	$(call env_executable,$(PCI),					\
			      $(PCI_O)					\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBKANETON_A_),\
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(PCI_O),)
	$(call env_remove,$(PCI),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,${_DRIVERS_DIR_}/pci/$(PCI_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(PCI_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
