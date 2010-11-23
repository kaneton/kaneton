#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...n/machine/glue/ibm-pc.ia32/task/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sat nov 20 23:08:42 2010]
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

SUBDIRS			:=		clone				\
					driver				\
					guest				\
					service				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

TASK_C			:=		task.c
TASK_O			:=		$(TASK_C:.c=.o)
TASK_H			:=		$(directory)/$(TASK_C:.c=.h)

TASK_LO			:=		task.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(TASK_LO)

$(TASK_LO):		$(MODULES)
	$(call env_remove,$(TASK_LO),)

	$(call env_compile-c,$(TASK_O),$(TASK_C),)

	$(call env_archive,$(TASK_LO),$(MODULES) $(TASK_O),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(TASK_O),)
	$(call env_remove,$(TASK_LO),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

	$(call env_prototypes,$(TASK_H),)

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TASK_C),)

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
