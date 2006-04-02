##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/04/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 15:05:01 2005]
## updated       matthieu bucchianeri   [sun apr  2 19:06:49 2006]
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

TEST_LO			:=		04.lo

TEST_C			:=		04.c

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
