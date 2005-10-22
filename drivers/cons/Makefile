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
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Fri Oct 21 19:21:02 2005   mycure
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

CONS			:=		cons

CONS_C			:=		cons.c

CONS_O			:=		$(CONS_C:.c=.o)

CONS_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(CONS):		$(CONS_O)
	$(call executable,$(CONS),$(CONS_O) $(_KLIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

libs:
	$(call make,$(SUBDIRS),)

	$(call make,$(_KLIBC_DIR_),)

clear:
	$(call remove,$(CONS_O),)
	$(call remove,$(CONS),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CONS_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
