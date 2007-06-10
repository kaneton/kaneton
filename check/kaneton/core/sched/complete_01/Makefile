##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/kaneton/core/region/01/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 15:05:01 2005]
## updated       matthieu bucchianeri   [thu jul 20 19:16:05 2006]
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

TEST_LO			:=		complete_01.lo

TEST_C			:=		complete_01.c

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
