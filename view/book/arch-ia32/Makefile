#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/book/arch-ia32/Makefile
#
# created       julien quintard   [mon may 14 19:03:18 2007]
# updated       julien quintard   [mon may 14 19:20:02 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		virtual				

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call env_launch,Makefile,$(SUBDIRS),,)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_purge)

proto:

dep:
