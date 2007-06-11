##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/libs/klibc/libdata/malloc/05/Makefile
##
## created       matthieu bucchianeri   [sun jun  4 19:36:03 2006]
## updated       matthieu bucchianeri   [sun jun  4 19:36:13 2006]
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

TEST_LO			:=		05.lo

TEST_C			:=		05.c

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

headers:
