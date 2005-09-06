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
## last update   Tue Sep  6 11:09:22 2005   mycure
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
 
CONF_LO			:=		conf.lo

CONF_C			:=		conf.c			

CONF_O			:=		$(CONF_C:.c=.o)

CONF_INCLUDE		:=		$(_CORE_INCLUDE_DIR_)/kaneton/conf.h

CFLAGS			+=		-D___kernel

#
# ---------- rules ------------------------------------------------------------
#

$(CONF_LO):		$(CONF_O)
	$(call remove,$(CONF_LO),)

	$(call archive,$(CONF_LO),$(CONF_O),)

clear:
	$(call remove,$(CONF_O),)

	$(call remove,$(CONF_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(CONF_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CONF_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
