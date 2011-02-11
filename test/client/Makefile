#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/client/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [thu feb 10 10:35:45 2011]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build run clear prototypes headers information-	\
		information test- test list- list display- display	\
		submit submit-

#
# ---------- rules ------------------------------------------------------------
#

main:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),,)

information- information:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

test- test:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

test-%:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

retest- retest:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

retest-%:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

submit- submit:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

submit-%:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

clear:
	$(call purge,)

prototypes:

headers:
