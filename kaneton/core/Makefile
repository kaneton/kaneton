##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/kaneton/core/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [sat apr  1 23:30:43 2006]
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
					as				\
					conf				\
					debug				\
					id				\
					region				\
					segment				\
					map				\
					set				\
					stats				\
					event				\
					time				\
					task				\
					$(_CHECK_DIR_)

# kaneton libraries

KANETON_LIBS		:=		$(_MACHDEP_LO_)			\
					$(_AS_LO_)			\
					$(_CONF_LO_)			\
					$(_DEBUG_LO_)			\
					$(_ID_LO_)			\
					$(_REGION_LO_)			\
					$(_SEGMENT_LO_)			\
					$(_MAP_LO_)			\
					$(_SET_LO_)			\
					$(_STATS_LO_)			\
					$(_EVENT_LO_)			\
					$(_TIME_LO_)			\
					$(_TASK_LO_)			\
					$(_CHECK_LO_)

# kaneton binary

CORE_C			:=		core.c

CORE_O			:=		$(CORE_C:.c=.o)

CORE_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/core.h

CFLAGS			+=		-D___kernel

# version file

VERSION_C		:=		version.c

VERSION_O		:=		$(VERSION_C:.c=.o)

DATE			:=		$(call date)

#
# ---------- rules ------------------------------------------------------------
#

$(_CORE_):		libs $(CORE_O)
	$(call version,$(VERSION_C),$(USER),$(HOSTNAME),$(DATE))

	$(call compile-c,$(VERSION_O),$(VERSION_C),)

	$(call executable,$(_CORE_),	$(CORE_O)			\
					$(VERSION_O)			\
					$(KANETON_LIBS)			\
					$(_KLIBC_A_),			\
                 "--ld-script $(_MACHDEP_LDS_DIR_)/kaneton.lds"		\
                 "--no-standard-include"				\
                 "--no-standard-library")

libs:
	$(call makefile,$(SUBDIRS),)

	$(call makefile,$(_KLIBC_DIR_),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(VERSION_O),)
	$(call remove,$(VERSION_C),)

	$(call remove,$(CORE_O),)

	$(call remove,$(_CORE_),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

	$(call prototypes,$(CORE_INCLUDE),)

dep:
	$(call makefile,$(SUBDIRS),dep)

	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CORE_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
