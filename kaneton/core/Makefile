##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [tue may  8 13:05:45 2007]
##

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
					cpu				\
					debug				\
					event				\
					id				\
					interface			\
					io				\
					kernel				\
					map				\
					message				\
					region				\
					sched				\
					segment				\
					set				\
					thread				\
					task				\
					time				

# kaneton objects

KANETON_O		:=		$(_AS_LO_)			\
					$(_CAPABILITY_LO_)		\
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
					$(_SCHED_LO_)			\
					$(_SEGMENT_LO_)			\
					$(_SET_LO_)			\
					$(_THREAD_LO_)			\
					$(_TASK_LO_)			\
					$(_TIME_LO_)			

# core component

CORE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/core.h

CORE_C			:=		core.c

CORE_O			:=		$(CORE_C:.c=.o)

# core variables XXX ugly: to correct

_CC_FLAGS_		+=		-D___kernel

# version file

VERSION_C		:=		version.c

VERSION_O		:=		$(VERSION_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

main:			$(_CORE_LO_)

$(_CORE_LO_):		dependencies $(CORE_O)
	$(call env_archive,$(_CORE_LO_),$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_LO)			\
					$(_LIBC_A_),)

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
	$(call env_launch,$(_LIBC_DIR_)/Makefile,,)

	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done

	$(call env_version,$(VERSION_C))
	$(call env_compile-c,$(VERSION_O),$(VERSION_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
