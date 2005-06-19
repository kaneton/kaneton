##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/services
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Sun Jun 19 20:55:47 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

KBD			:=		kbd

KBD_C			:=		kbd.c

KBD_O			:=		$(KBD_C:.c=.o)

KBD_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(KBD):			$(KBD_O)
	$(call dynamic-linker,$(KBD),$(KBD_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

clear:
	$(call remove,$(KBD_O),)
	$(call remove,$(KBD),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(KBD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
