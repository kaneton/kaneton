##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/2008-K1/check/kaneton/check/kaneton/core/as/as_with_regions_01/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 15:05:01 2005]
## updated       matthieu bucchianeri   [sat mar  3 21:45:18 2007]
##

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
TEST_LO			:=		as_with_regions_01.lo

TEST_C			:=		as_with_regions_01.c

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
	$(call env_remove,$(_MAKEFILE_MK_),)

	$(call env_dependencies,$(TEST_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
