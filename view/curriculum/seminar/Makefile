#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/curriculum/seminar/Makefile
#
# created       julien quintard   [mon may 14 21:14:19 2007]
# updated       julien quintard   [mon may 14 21:38:36 2007]
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

SUBDIRS			:=		2006				\
					2007				

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
