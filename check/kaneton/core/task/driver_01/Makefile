#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kan...eck/kaneton/core/task/driver_01/Makefile
#
# created       matthieu bucchianeri   [sun aug 26 16:44:03 2007]
# updated       matthieu bucchianeri   [sun aug 26 16:44:05 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

TEST_LO			:=		driver_01.lo

TEST_C			:=		driver_01.c

TEST_O			:=		$(TEST_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(TEST_LO)

$(TEST_LO):		$(TEST_O)
	$(call env_remove,$(TEST_LO),)

	$(call env_archive,$(TEST_LO),$(TEST_O),)

run:

clear:
	$(call env_remove,$(TEST_LO),)

	$(call env_remove,$(TEST_O),)

	$(call env_remove,parse_res.pyc,)

	$(call env_purge,)

prototypes:

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TEST_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
