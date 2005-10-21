##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Fri Oct 21 19:19:41 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

# kaneton libraries

SUBDIRS			:=		arch				\
					as				\
					conf				\
					debug				\
					id				\
					segment				\
					set				\
					stats				

KANETON_LIBS		:=		$(_MACHDEP_LO_)			\
					$(_AS_LO_)			\
					$(_CONF_LO_)			\
					$(_DEBUG_LO_)			\
					$(_ID_LO_)			\
					$(_SEGMENT_LO_)			\
					$(_SET_LO_)			\
					$(_STATS_LO_)			

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

DATE			:=		$(shell date)

#
# ---------- rules ------------------------------------------------------------
#

$(KANETON):		libs $(KANETON_O)
	$(call version,$(VERSION_C))

	$(call compile-c,$(VERSION_O),$(VERSION_C),)

	$(call executable,$(KANETON),	$(KANETON_O)			\
					$(VERSION_O)			\
					$(KANETON_LIBS)			\
					$(_KLIBC_A_),			\
	  -T $(_LDS_DIR_)/kaneton.lds)

libs:
	$(call make,$(SUBDIRS),)

	$(call make,$(_KLIBC_DIR_),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call remove,$(VERSION_O),)
	$(call remove,$(VERSION_C),)

	$(call remove,$(KANETON_O),)

	$(call remove,$(KANETON),)

	$(call purge,)

proto:
	$(call make,$(SUBDIRS),proto)

	$(call prototypes,$(KANETON_INCLUDE),)

dep:
	$(call make,$(SUBDIRS),dep)

	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KANETON_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
