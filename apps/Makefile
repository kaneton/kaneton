##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/apps/Makefile
##
## created       matthieu bucchianeri   [sat dec  2 02:47:54 2006]
## updated       matthieu bucchianeri   [sat dec  2 03:02:45 2006]
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

SUBDIRS			:=		$(APP)

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
