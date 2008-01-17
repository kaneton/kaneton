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

ECHO			:=		echo

ECHO_C			:=		echo.c

ECHO_O			:=		$(ECHO_C:.c=.o)

ECHO_INCLUDE		:=		$(_SERVICES_DIR_)/echo/echo.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(ECHO)

$(ECHO):			libs $(ECHO_O)
	$(call env_executable,$(ECHO),					\
			      $(ECHO_O)					\
			      $(_CRT_O_) $(_ULIBC_A_)			\
			      $(_LIBKANETON_A_),			\
			      $(_SERVICE_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(ECHO_O),)
	$(call env_remove,$(ECHO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(ECHO_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(ECHO_C),)

dependencies:
	$(call env_launch,$(_ULIBC_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
