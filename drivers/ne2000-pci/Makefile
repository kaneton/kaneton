##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/ne2000/Makefile
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

NE2000			:=		ne2000-pci

NE2000_C		:=		ne2000-pci.c

NE2000_O		:=		$(NE2000_C:.c=.o)

NE2000_INCLUDE		:=		ne2000-pci.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(NE2000)

$(NE2000):		libs $(NE2000_O)
	$(call env_executable,$(NE2000),				\
			      $(NE2000_O)				\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBPTHREAD_A_)	\
			      $(_LIBKANETON_A_),			\
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(NE2000_O),)
	$(call env_remove,$(NE2000),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,${_DRIVERS_DIR_}/ne2000-pci/$(NE2000_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(NE2000_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)
	$(call env_launch,$(_LIBKANETON_DIR_)/Makefile,,)
	$(call env_launch,$(_LIBPTHREAD_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
