##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/drivers/cons
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Mon Nov  7 16:11:18 2005   mycure
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

CONS			:=		cons

CONS_C			:=		cons.c

CONS_O			:=		$(CONS_C:.c=.o)

CONS_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(CONS):		libs $(CONS_O)
	$(call executable,$(CONS),$(CONS_O) $(_KLIBC_A_),		\
	  -T $(_MACHDEP_LDS_DIR_)/driver.lds)

libs:
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
