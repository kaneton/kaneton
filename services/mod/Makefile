##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/services/mod
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Tue Sep  6 11:02:09 2005   mycure
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

MOD			:=		mod

MOD_C			:=		mod.c				\
					modfs.c				

MOD_O			:=		$(MOD_C:.c=.o)

MOD_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(MOD):			$(MOD_O)
	$(call executable,$(MOD),$(MOD_O) $(_LIBC_A_),			\
	  -T $(_LDS_DIR_)/service.lds)

clear:
	$(call remove,$(MOD_O),)
	$(call remove,$(MOD),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MOD_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
