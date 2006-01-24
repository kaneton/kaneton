##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/programs/Makefile
##
## created       julien quintard   [fri feb 11 02:59:20 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:46:56 2006]
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

SUBDIRS			:=		$(_PROGRAMS_DIR_)

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
