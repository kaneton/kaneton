##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/Makefile
##
## created       julien quintard   [fri feb 11 02:28:05 2005]
## updated       matthieu bucchianeri   [thu aug 10 15:56:41 2006]
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

SUBDIRS			:=		klibc				\
					crt

ARCHSUBDIRS		:=		libia32				\
					libsun4u			\
					liboctane

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call launch,Makefile,$(ARCHSUBDIRS),clear,)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

	$(call launch,Makefile,$(ARCHSUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)

	$(call launch,Makefile,$(ARCHSUBDIRS),dep,)
