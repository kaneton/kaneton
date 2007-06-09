#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/exam/arch-mips/2007/Makefile
#
# created       julien quintard   [mon may 14 21:34:22 2007]
# updated       julien quintard   [mon may 14 21:46:32 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		mid				\
					final				

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
