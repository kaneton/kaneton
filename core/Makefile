##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/core/Makefile
##
## created       julien quintard   [fri feb 11 03:24:11 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:14:54 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		$(_CORE_DEP_)

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

dep:
	$(call makefile,$(SUBDIRS),dep)
