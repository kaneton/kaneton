#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/boot/loader/Makefile
#
# created       julien quintard   [sun jun 10 14:54:43 2007]
# updated       julien quintard   [sat dec 18 11:15:05 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		${_LOADER_DIR_}

#
# ---------- rules ------------------------------------------------------------
#

main:

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge,)

prototypes:

headers:
