#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/book/assignments/2007/Makefile
#
# created       julien quintard   [mon may 14 19:03:18 2007]
# updated       julien quintard   [sun jun 10 00:14:33 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build clear

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
