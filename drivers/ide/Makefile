#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/crypt/kaneton/services/echo/Makefile
#
# created       matthieu bucchianeri   [fri sep  7 23:29:54 2007]
# updated       matthieu bucchianeri   [thu jan 17 19:30:10 2008]
#

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

IDE			:=		ide

IDE_C			:=		ide.c

IDE_O			:=		$(IDE_C:.c=.o)

IDE_INCLUDE		:=		$(_DRIVERS_DIR_)/ide/ide.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(IDE)

$(IDE):			libs $(IDE_O)
	$(call env_executable,$(IDE),					\
			      $(IDE_O)					\
			      $(_CRT_O_) $(_ULIBC_A_)			\
			      $(_LIBKANETON_A_),			\
			      $(_SERVICE_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(IDE_O),)
	$(call env_remove,$(IDE),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(IDE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(IDE_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
