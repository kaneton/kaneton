#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/internship/Makefile
#
# created       julien quintard   [mon may 14 22:16:45 2007]
# updated       julien quintard   [sat jun  9 23:48:01 2007]
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

SUBDIRS			:=		check				\
					lse				

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
