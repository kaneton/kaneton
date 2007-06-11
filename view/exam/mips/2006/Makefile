#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/exam/arch-mips/2006/Makefile
#
# created       julien quintard   [mon may 14 21:34:22 2007]
# updated       julien quintard   [sun jun 10 00:20:44 2007]
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

SUBDIRS			:=		mid				\
					final				

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
