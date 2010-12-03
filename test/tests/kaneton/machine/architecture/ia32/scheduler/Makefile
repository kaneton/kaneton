#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...ine/architecture/ia32/scheduler/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [fri dec  3 19:52:47 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/machine/architecture/scheduler

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

SUBDIRS			:=		complete			\
					context				\
					stack				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

SCHEDULER_LO		:=		scheduler.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(SCHEDULER_LO)

$(SCHEDULER_LO):		$(MODULES)
	$(call env_remove,$(SCHEDULER_LO),)

	$(call env_archive,$(SCHEDULER_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(SCHEDULER_LO),)

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
