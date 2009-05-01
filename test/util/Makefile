#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/util/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [wed apr 15 11:29:04 2009]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build run clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

# XXX[should be put in the previous directory]
PYTHONPATH		:=		${PYTHONPATH}:${_TEST_PACKAGE_DIR_}

#
# ---------- rules ------------------------------------------------------------
#

main:

certificate:
	$(call env_launch,$(_TEST_CERTIFICATE_SCRIPT_),,)

code:
	$(call env_launch,$(_TEST_CODE_SCRIPT_),,)

capability:
	$(call env_launch,$(_TEST_CAPABILITY_SCRIPT_),,)

capability-%:
	$(call env_launch,$(_TEST_CAPABILITY_SCRIPT_),$*,)

database:
	$(call env_launch,$(_TEST_DATABASE_SCRIPT_),,)

database-%:
	$(call env_launch,$(_TEST_DATABASE_SCRIPT_),$*,)

inventory:
	$(call env_launch,$(_TEST_INVENTORY_SCRIPT_),$*,)

inventory-%:
	$(call env_launch,$(_TEST_INVENTORY_SCRIPT_),$*,)

clear:
	$(call purge,)

prototypes:

headers:
