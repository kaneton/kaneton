##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/set/set_stack/09/Makefile
##
## created       renaud voltz   [fri feb 10 15:24:53 2006]
## updated       matthieu bucchianeri   [thu jul 20 17:58:59 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

TEST_LO			:=		09.lo

TEST_C			:=		09.c

TEST_O			:=		$(TEST_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

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

proto:

dep:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TEST_C),$(_DEPENDENCY_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
