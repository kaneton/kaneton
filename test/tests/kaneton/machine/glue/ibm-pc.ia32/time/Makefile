#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...n/machine/glue/ibm-pc.ia32/time/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sat nov 20 23:38:30 2010]
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

SUBDIRS			:=		data				\
					message				\
					precision			\
					repeat				\
					reserve				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

TIME_C			:=		time.c
TIME_O			:=		$(TIME_C:.c=.o)
TIME_H			:=		$(directory)/$(TIME_C:.c=.h)

TIME_LO			:=		time.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(TIME_LO)

$(TIME_LO):		$(MODULES)
	$(call env_remove,$(TIME_LO),)

	$(call env_compile-c,$(TIME_O),$(TIME_C),)

	$(call env_archive,$(TIME_LO),$(MODULES) $(TIME_O),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(TIME_O),)
	$(call env_remove,$(TIME_LO),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

	$(call env_prototypes,$(TIME_H),)

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(TIME_C),)

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
