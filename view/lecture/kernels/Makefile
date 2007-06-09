#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/lecture/kernels/Makefile
#
# created       julien quintard   [wed may 16 18:54:58 2007]
# updated       julien quintard   [wed may 16 18:57:46 2007]
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

SUBDIRS			:=		bootstrap			\
					capabilities			\
					interrupts			\
					ipc				\
					memory-management		\
					prerequisites			\
					task-scheduling			\
					tp-k0				

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
