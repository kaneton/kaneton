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
## last update   Sat Jun 18 15:36:39 2005   mycure
##

#
# dependencies
#

include			../../.kaneton.mk

#
# directives
#

.PHONY:		clear proto dep

#
# VARIABLES
#

DSH			:=		dsh

DSH_C			:=		dsh.c

DSH_O			:=		$(DSH_C:.c=.o)

DSH_INCLUDE		=		

#
# rules
#

$(DSH):			$(DSH_O)
	$(call dynamic-linker,$(DSH),$(DSH_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

clear:
	$(call remove,$(DSH_O),)
	$(call remove,$(DSH),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(DSH_C),$(_MAKEFILE_MK_),)

#
# dependencies
#

include			./$(_MAKEFILE_MK_)
