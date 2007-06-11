#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/exam/arch-mips/Makefile
#
# created       julien quintard   [mon may 14 21:34:22 2007]
# updated       julien quintard   [sun jun 10 00:21:30 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build clear

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		2006				\
					2007				

#
# ---------- rules ------------------------------------------------------------
#

main:		build

build:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,build,)			; \
	done

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge)
