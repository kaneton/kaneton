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

MOD			:=		mod

MOD_C			:=		mod.c

MOD_O			:=		$(MOD_C:.c=.o)

MOD_INCLUDE		:=		$(_SERVICES_DIR_)/mod/mod.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(MOD)

$(MOD):			libs $(MOD_O)
	$(call env_executable,$(MOD),					\
			      $(MOD_O)					\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBKANETON_A_),\
			      $(_DRIVER_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(MOD_O),)
	$(call env_remove,$(MOD),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(MOD_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(MOD_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
