##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/core/kaneton/Makefile
##
## created       julien quintard   [fri feb 11 02:57:24 2005]
## updated       julien quintard   [thu mar  9 16:21:11 2006]
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

KANETON			:=		kaneton

KANETON_C		:=		kaneton.c

KANETON_O		:=		$(KANETON_C:.c=.o)

KANETON_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/kaneton.h

CFLAGS			+=		-D___kernel

# version file

VERSION_C		:=		version.c

VERSION_O		:=		$(VERSION_C:.c=.o)

VERSION_H		:=		$(_CORE_INCLUDE_DIR_)/version.h

DATE			:=		$(call date)

#
# ---------- rules ------------------------------------------------------------
#

$(KANETON):		libs $(KANETON_O)
	$(call version,$(VERSION_C),$(USER),$(HOSTNAME),$(DATE))

	$(call compile-c,$(VERSION_O),$(VERSION_C),)

	$(call executable,$(KANETON),	$(KANETON_O)			\
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

	$(call remove,$(KANETON_O),)

	$(call remove,$(KANETON),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

	$(call prototypes,$(KANETON_INCLUDE),)

dep:
	$(call makefile,$(SUBDIRS),dep)

	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KANETON_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
