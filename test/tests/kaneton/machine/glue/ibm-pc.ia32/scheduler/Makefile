#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...hine/glue/ibm-pc.ia32/scheduler/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sun nov 21 10:11:38 2010]
#

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

SUBDIRS			:=		complete			

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

SCHEDULER_C		:=		scheduler.c
SCHEDULER_O		:=		$(SCHEDULER_C:.c=.o)
SCHEDULER_H		:=		$(directory)/$(SCHEDULER_C:.c=.h)

SCHEDULER_LO		:=		scheduler.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(SCHEDULER_LO)

$(SCHEDULER_LO):		$(MODULES)
	$(call env_remove,$(SCHEDULER_LO),)

	$(call env_compile-c,$(SCHEDULER_O),$(SCHEDULER_C),)

	$(call env_archive,$(SCHEDULER_LO),$(MODULES) $(SCHEDULER_O),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(SCHEDULER_O),)
	$(call env_remove,$(SCHEDULER_LO),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

	$(call env_prototypes,$(SCHEDULER_H),)

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(SCHEDULER_C),)

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
