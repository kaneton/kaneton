#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/crypt/kaneton/view/book/assignments/2007/Makefile
#
# created       julien quintard   [mon may 14 19:03:18 2007]
# updated       matthieu bucchianeri   [tue may 29 14:56:27 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		k1				\
					k2				\
					k3				\
					k4

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
