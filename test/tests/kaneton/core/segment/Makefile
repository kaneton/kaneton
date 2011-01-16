#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...test/tests/kaneton/core/segment/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [fri jan 14 23:07:43 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/core/segment

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		clone				\
					coalesce			\
					copy				\
					flush				\
					give				\
					inject				\
					permissions			\
					read-write			\
					reserve				\
					resize				\
					split				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

SEGMENT_LO		:=		segment.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(SEGMENT_LO)

$(SEGMENT_LO):		$(MODULES)
	$(call env_remove,$(SEGMENT_LO),)

	$(call env_archive,$(SEGMENT_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(SEGMENT_LO),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
