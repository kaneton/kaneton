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

include			../environment/env.mk

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
					libmipsr10000

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call env_launch,Makefile,$(SUBDIRS),,)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_launch,Makefile,$(ARCHSUBDIRS),clear,)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

	$(call env_launch,Makefile,$(ARCHSUBDIRS),proto,)

dep:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)

	$(call env_launch,Makefile,$(ARCHSUBDIRS),dep,)
