#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton.STABLE/test/tests/kaneton/core/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [thu nov 18 15:39:54 2010]
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

NAME			:=		core

UNITS			:=		as				\
					event				\
					id				\
					map				\
					region				\
					segment				\
					set				


SUBDIRS			:=		$(_TEST_ENGINE_DIR_)		\
					$(UNITS)

MODULES			:=		$(_TEST_ENGINE_LO_)		\
					$(foreach s,$(UNITS),${s}/${s}.lo)

COMPONENT		:=		$(NAME:=.lo)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies
	$(call env_remove,$(COMPONENT),)

	$(call env_archive,$(COMPONENT),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

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
