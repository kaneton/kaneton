##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/set_array/02/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [mon dec 19 18:30:31 2005]
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

TEST_LO			:=		02.lo

TEST_C			:=		02.c

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

	$(call purge,)

proto:

dep:
