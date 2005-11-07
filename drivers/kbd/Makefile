##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/drivers/kbd
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Mon Nov  7 16:10:48 2005   mycure
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

KBD			:=		kbd

KBD_C			:=		kbd.c

KBD_O			:=		$(KBD_C:.c=.o)

KBD_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(KBD):			libs $(KBD_O)
	$(call executable,$(KBD),$(KBD_O) $(_KLIBC_A_),			\
	  -T $(_MACHDEP_LDS_DIR_)/driver.lds)

libs:
	$(call make,$(_KLIBC_DIR_),)

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

-include		./$(_MAKEFILE_MK_)
