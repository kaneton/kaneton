#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/kaneton/services/mips_test/Makefile
#
# created       enguerrand raymond   [sat apr 11 03:18:19 2009]
# updated       enguerrand raymond   [sat apr 11 03:51:47 2009]
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

MIPS_SERVICE			:=		mips_test

MIPS_SERVICE_C			:=		mips_test.c

MIPS_SERVICE_O			:=		$(MIPS_SERVICE_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(MIPS_SERVICE)

$(MIPS_SERVICE):			$(MIPS_SERVICE_O)
	$(call env_executable,$(MIPS_SERVICE),				\
			      $(MIPS_SERVICE_O),			\
			      $(_SERVICE_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(MIPS_SERVICE_O),)

	$(call env_remove,$(MIPS_SERVICE),)

#	$(call env_launch,$(_LIBKANETON_DIR_)/Makefile,clear,)

	$(call env_purge,)

prototypes:

headers:

dependencies:
#	$(call env_launch,$(_LIBKANETON_DIR_)/Makefile,,)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
