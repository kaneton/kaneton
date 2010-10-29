#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/package/Makefile
#
# created       julien quintard   [wed may 16 18:08:19 2007]
# updated       julien quintard   [mon oct 25 20:53:32 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		ktp

#
# ---------- rules ------------------------------------------------------------
#

main:

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge)
