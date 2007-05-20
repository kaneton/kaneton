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

include			../env/.env.mk

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
	$(call launch,$(_COMPILE_PY_),,)

run:

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(_TEST_LO_),)

	$(call remove,.test.mk,)

	$(call remove,parse.pyc,)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)

  
-include		.test.mk
