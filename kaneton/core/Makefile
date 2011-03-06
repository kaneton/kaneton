#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/kaneton/core/Makefile
#
# created       julien quintard   [tue jun 26 15:39:11 2007]
# updated       julien quintard   [wed mar  2 23:04:20 2011]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		kaneton/core

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		as				\
					capability			\
					clock				\
					cpu				\
					event				\
					id				\
					interface			\
					io				\
					kernel				\
					map				\
					message				\
					region				\
					scheduler			\
					segment				\
					set				\
					task				\
					thread				\
					timer

# kaneton objects

KANETON_LO		:=		$(_AS_LO_)			\
					$(_CAPABILITY_LO_)		\
					$(_CLOCK_LO_)			\
					$(_CPU_LO_)			\
					$(_DEBUG_LO_)			\
					$(_EVENT_LO_)			\
					$(_ID_LO_)			\
					$(_INTERFACE_LO_)		\
					$(_IO_LO_)			\
					$(_KERNEL_LO_)			\
					$(_MAP_LO_)			\
					$(_MESSAGE_LO_)			\
					$(_REGION_LO_)			\
					$(_SCHEDULER_LO_)		\
					$(_SEGMENT_LO_)			\
					$(_SET_LO_)			\
					$(_TASK_LO_)			\
					$(_THREAD_LO_)			\
					$(_TIMER_LO_)			

# core component

CORE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/core.h

CORE_C			:=		core.c

CORE_O			:=		$(CORE_C:.c=.o)

# version file

VERSION_C		:=		version.c

VERSION_O		:=		$(VERSION_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			dependencies $(_CORE_LO_)

$(_CORE_LO_):		$(CORE_O) $(KANETON_LO)
	$(call env_remove,$(_CORE_LO_),)

	$(call env_version,$(VERSION_C))
	$(call env_compile-c,$(VERSION_O),$(VERSION_C),)

	$(call env_archive,$(_CORE_LO_),$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_LO),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(VERSION_O),)
	$(call env_remove,$(VERSION_C),)

	$(call env_remove,$(CORE_O),)

	$(call env_remove,$(_CORE_LO_),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

	$(call env_prototypes,$(CORE_INCLUDE),)

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(CORE_C),)

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
