#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/book/assignments/2006/Makefile
#
# created       julien quintard   [mon may 14 19:03:18 2007]
# updated       julien quintard   [mon may 14 19:39:45 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		k1				\
					k2				\
					k3				

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge)

proto:

dep:
