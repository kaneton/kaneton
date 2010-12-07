#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/client/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [tue dec  7 17:05:53 2010]
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

list- list:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

display- display:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

display-%:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

submit- submit:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

submit-%:
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),$@,)

clear:
	$(call purge,)

prototypes:

headers:
