##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/mod/Makefile
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

CONS_SIMPLE		:=		cons-simple

CONS_SIMPLE_C		:=		cons-simple.c

CONS_SIMPLE_O		:=		$(CONS_SIMPLE_C:.c=.o)

CONS_SIMPLE_INCLUDE	:=		$(_DRIVERS_DIR_)/cons-simple/cons-simple.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(CONS_SIMPLE)

$(CONS_SIMPLE):			libs $(CONS_SIMPLE_O)
	$(call env_executable,$(CONS_SIMPLE),				\
			      $(CONS_SIMPLE_O)				\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBKANETON_A_),	\
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(CONS_SIMPLE_O),)
	$(call env_remove,$(CONS_SIMPLE),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(CONS_SIMPLE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CONS_SIMPLE_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
