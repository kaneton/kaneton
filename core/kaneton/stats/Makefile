##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/stats
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Wed Aug 31 16:56:38 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#
 
include			../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

STATS_LO		:=		stats.lo

STATS_C			:=		stats.c				

STATS_O			:=		$(STATS_C:.c=.o)

STATS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/stats.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(STATS_LO):		$(STATS_O)
	$(call remove,$(STATS_LO),)

	$(call archive,$(STATS_LO),$(STATS_O),)

clear:
	$(call remove,$(STATS_O),)

	$(call remove,$(STATS_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(STATS_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(STATS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
