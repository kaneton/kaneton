#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/feedback/Makefile
#
# created       julien quintard   [mon may 14 22:16:45 2007]
# updated       julien quintard   [mon may 14 22:17:26 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		arch-ia32			\
					arch-mips			\
					kaneton				\
					kernels				

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
