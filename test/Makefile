##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/julian/kaneton/kaneton/test/Makefile
##
## created       julian pidancet   [thu mar 22 22:36:45 2007]
## updated       julian pidancet   [thu mar 22 22:36:45 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run clear proto dep

#
# ---------- variables --------------------------------------------------------
#

# Temporary, don't know where to put this
# machine dependant .env.conf
_COMPILE_PY_	:=	compile.py
_TEST_LO_	:=	test.lo

#
# ---------- rules ------------------------------------------------------------
#

all:			build run

build:			.test.mk
	$(MAKE) compile

.test.mk:
	$(call env_launch,$(_COMPILE_PY_),,)

run:

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(_TEST_LO_),)

	$(call env_remove,.test.mk,)

	$(call env_remove,parse.pyc,)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)

  
-include		.test.mk
