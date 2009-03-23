#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /data/mycure/repositories/kaneton/test/client/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [mon mar 23 00:08:39 2009]
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
	$(call env_launch,$(_TEST_CLIENT_SCRIPT_),,)

clear:
	$(call purge,)

prototypes:

headers:
