##
## licence       kaneton licence
##
## project       kaneton
##
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#
TEST_LO			:=		register.lo

TEST_C			:=		register.c

TEST_O			:=		$(TEST_C:.c=.o)

# XXX
_TESTSUITE_ := ../../../check/test.yml

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(TEST_LO)

register.c: $(_TESTSUITE_) gen_register.sh
	./gen_register.sh $(_TESTSUITE_) > register.c

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
