##
## licence       Kaneton licence
##
## project       kaneton
##
## file          /home/rhino/kaneton/check/core/kaneton/set/set_stack/05/Makefile
##
## created       renaud voltz   [fri feb 10 15:24:00 2006]
## updated       renaud voltz   [fri feb 10 15:24:00 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../env/.env.mk

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
	$(call remove,$(TEST_LO),)

	$(call archive,$(TEST_LO),$(TEST_O),)

run:

clear:
	$(call remove,$(TEST_LO),)

	$(call remove,$(TEST_O),)

	$(call remove,parse_res.pyc,)

	$(call purge,)

proto:

dep:
