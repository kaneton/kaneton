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

include			../../environment/env.mk

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
	$(call env_launch,Makefile,$(SUBDIRS),,)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_purge)

proto:

dep:
