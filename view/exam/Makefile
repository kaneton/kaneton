#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/exam/Makefile
#
# created       julien quintard   [sat jun  9 23:52:07 2007]
# updated       julien quintard   [mon jun 11 07:30:58 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build clear

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		mips				\
					kernels				

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
