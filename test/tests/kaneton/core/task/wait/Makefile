#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...st/tests/kaneton/core/task/wait/Makefile
#
# created       julien quintard   [wed apr 15 04:29:58 2009]
# updated       julien quintard   [sat dec 18 17:53:46 2010]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		test/tests/kaneton/core/task/wait

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		01				\
					02				\
					03				\
					04				\
					05				\
					06				

MODULES			:=		$(foreach s,$(SUBDIRS),${s}/${s}.lo)

WAIT_LO			:=		wait.lo

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(WAIT_LO)

$(WAIT_LO):		$(MODULES)
	$(call env_remove,$(WAIT_LO),)

	$(call env_archive,$(WAIT_LO),$(MODULES),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(WAIT_LO),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

	$(call env_remove,$(_DEPENDENCY_MK_),)

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
