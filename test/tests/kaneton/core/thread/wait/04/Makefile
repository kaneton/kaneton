#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repo...sts/kaneton/core/thread/wait/04/Makefile
#
# created       julien quintard   [wed apr 15 04:30:04 2009]
# updated       julien quintard   [tue nov 30 16:20:58 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/core/thread/wait/04

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

TEST_C			:=		04.c
TEST_O			:=		$(TEST_C:.c=.o)

TEST_H			:=		$(directory)/$(TEST_C:.c=.h)

TEST_LO			:=		$(TEST_O:.o=.lo)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(TEST_LO)

$(TEST_LO):		$(TEST_O)
	$(call env_remove,$(TEST_LO),)

	$(call env_archive,$(TEST_LO),$(TEST_O),)

clear:
	$(call env_remove,$(TEST_O),)

	$(call env_remove,$(TEST_LO),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(TEST_H),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TEST_C),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
