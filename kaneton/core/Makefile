##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/kaneton/core/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       matthieu bucchianeri   [sat dec  2 03:32:50 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		arch				\
					kernel				\
					as				\
					conf				\
					id				\
					region				\
					segment				\
					debug				\
					map				\
					set				\
					stats				\
					event				\
					time				\
					thread				\
					task				\
					sched				\
					message				\
					capability			\
					interface			\
					cpu				\
					io				\
					$(_CHECK_DIR_)

# kaneton objects

KANETON_O		:=		$(_MACHDEP_LO_)			\
					$(_KERNEL_LO_)			\
					$(_AS_LO_)			\
					$(_CONF_LO_)			\
					$(_ID_LO_)			\
					$(_REGION_LO_)			\
					$(_SEGMENT_LO_)			\
					$(_DEBUG_LO_)			\
					$(_MAP_LO_)			\
					$(_SET_LO_)			\
					$(_STATS_LO_)			\
					$(_EVENT_LO_)			\
					$(_TIME_LO_)			\
					$(_THREAD_LO_)			\
					$(_TASK_LO_)			\
					$(_SCHED_LO_)			\
					$(_MESSAGE_LO_)			\
					$(_CAPABILITY_LO_)		\
					$(_INTERFACE_LO_)		\
					$(_CPU_LO_)			\
					$(_IO_LO_)			\
					$(_CHECK_LO_)

# core binary

CORE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/core.h

CORE_C			:=		core.c

CORE_O			:=		$(CORE_C:.c=.o)

# core variables

ifeq ($(APP),)
_CFLAGS_		+=		-D___kernel
else
_CFLAGS_		+=		-D___kernel -DAPP=$(APP)
endif

# version file

VERSION_C		:=		version.c

VERSION_O		:=		$(VERSION_C:.c=.o)

#
# ---------- rules ------------------------------------------------------------
#

libs:
	$(call launch,Makefile,$(_KLIBC_DIR_),,)
	$(call launch,Makefile,$(SUBDIRS),,)

	$(call launch,Makefile,.,$(_CORE_),)

$(_CORE_):		$(CORE_O) $(KANETON_O)
	$(call version,$(VERSION_C))

	$(call compile-c,$(VERSION_O),$(VERSION_C),)

ifneq ($(APP),)
	$(call executable,$(_CORE_),	$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_O)			\
					$(_KLIBC_A_)			\
					$(_APPS_DIR_)/$(APP)/$(APP).lo,	\
                 "--link-script $(_MACHDEP_LINK_DIR_)/core.lks"		\
                 "--no-standard-include"				\
                 "--no-standard-library")
else
	$(call executable,$(_CORE_),	$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_O)			\
					$(_KLIBC_A_),			\
                 "--link-script $(_MACHDEP_LINK_DIR_)/core.lks"		\
                 "--no-standard-include"				\
                 "--no-standard-library")
endif

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(VERSION_O),)
	$(call remove,$(VERSION_C),)

	$(call remove,$(CORE_O),)

	$(call remove,$(_CORE_),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

	$(call prototypes,$(CORE_INCLUDE),)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)

	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CORE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
