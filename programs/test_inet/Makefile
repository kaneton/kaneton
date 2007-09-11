#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/crypt/kaneton/programs/test_inet/Makefile
#
# created       matthieu bucchianeri   [sat aug 25 18:06:54 2007]
# updated       matthieu bucchianeri   [tue sep 11 14:46:16 2007]
#


#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear prototypes dependencies main

#
# ---------- variables --------------------------------------------------------
#

TEST			:=		test_inet

TEST_C			:=		test_inet.c

TEST_O			:=		$(TEST_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:	dependencies $(TEST)

$(TEST):	libs $(TEST_O)
	$(call env_executable,$(TEST),					\
			      $(TEST_O)					\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBKANETON_A_),\
			      $(_PROGRAM_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(TEST_O),)
	$(call env_remove,$(TEST),)

	$(call env_purge,)

prototypes:

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
