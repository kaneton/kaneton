#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...tests/kaneton/core/task/wait/03/Makefile
#
# created       julien quintard   [wed apr 15 04:30:04 2009]
# updated       julien quintard   [sat dec 18 17:55:27 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/core/task/wait/03

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

TEST_C			:=		03.c
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
