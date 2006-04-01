##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kernel/Makefile
##
## created       julien quintard   [fri feb 11 03:24:11 2005]
## updated       julien quintard   [sat apr  1 22:03:40 2006]
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

SUBDIRS			:=		$(_KERNEL_DEP_)

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
