#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...itecture/ia32/educational/event/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sat dec 18 11:20:48 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/machine/architecture/event

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		context				\
					data				\
					exception			\
					flags				\
					idt				\
					interrupt			\
					irq				\
					map				\
					pic				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

EVENT_LO		:=		event.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(EVENT_LO)

$(EVENT_LO):		$(MODULES)
	$(call env_remove,$(EVENT_LO),)

	$(call env_archive,$(EVENT_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(EVENT_LO),)

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
