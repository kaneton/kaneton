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
## last update   Sun Aug 28 13:43:05 2005   mycure
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

STATS			:=		stats.a

STATS_C			:=		stats.c			

STATS_O			:=		$(STATS_C:.c=.o)

STATS_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/stats.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(STATS):		$(STATS_O)
	$(call remove,$(STATS),)

	$(call archive,$(STATS),$(STATS_O),)

	$(call static-linker,$(STATS),)

clear:
	$(call remove,$(STATS_O),)

	$(call remove,$(STATS),)

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
