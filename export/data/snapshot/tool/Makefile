#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/output/kaneton/tool/Makefile
#
# created       julien quintard   [fri nov 28 16:24:37 2008]
# updated       julien quintard   [sat feb  5 12:11:38 2011]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		

#
# ---------- rules ------------------------------------------------------------
#

main:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge,)

prototypes:

headers:
