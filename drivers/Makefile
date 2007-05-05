##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/drivers/Makefile
##
## created       julien quintard   [fri feb 11 02:59:20 2005]
## updated       matthieu bucchianeri   [sat may  5 18:18:27 2007]
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

SUBDIRS			:=		$(_DRIVERS_DEP_)

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge,)

proto:
	$(call launch,makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,makefile,$(SUBDIRS),dep,)
