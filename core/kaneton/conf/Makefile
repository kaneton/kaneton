##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/conf
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Aug 28 16:01:08 2005   mycure
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
 
CONF			:=		conf.a

CONF_C			:=		conf.c			

CONF_O			:=		$(CONF_C:.c=.o)

CONF_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/conf.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CONF):		$(CONF_O)
	$(call remove,$(CONF),)

	$(call archive,$(CONF),$(CONF_O),)

	$(call static-linker,$(CONF),)

clear:
	$(call remove,$(CONF_O),)

	$(call remove,$(CONF),)

	$(call purge,)

proto:
	$(call prototypes,$(CONF_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CONF_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
