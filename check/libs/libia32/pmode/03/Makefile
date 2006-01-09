##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/check/libs/libia32/pmode/03/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       matthieu bucchianeri   [mon jan  9 12:00:30 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../env/.env.mk


#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

TEST_LO			:=		03.lo

TEST_C			:=		03.c

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
