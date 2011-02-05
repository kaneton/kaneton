#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/export/output/kaneton/test/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [sat feb  5 12:10:10 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test

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

SUBDIRS			:=		client				\
					packages			

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
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done
