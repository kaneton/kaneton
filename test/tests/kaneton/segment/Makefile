#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ABLE/test/tests/kaneton/segment/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [wed oct 20 12:56:49 2010]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		catch				\
					clone				\
					coalesce			
#					copy				\
#					flush				\
#					give				\
#					inject				\
#					multiple			\
#					permissions			\
#					read-write			\
#					resize				\
#					simple				\
#					split				\
#					type				

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
