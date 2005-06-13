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
## last update   Sun Jun 12 22:43:07 2005   mycure
##

include			../../.kaneton.mk

DSH			=		dsh

DSH_O			=		dsh.o

DSH_INCLUDE		=		

all:			$(DSH_O)
	$(call dynamic-linker,$(DSH),$(DSH_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call remove,$(DSH_O),)
	$(call remove,$(DSH),)

	$(call purge,)

proto:
	$(call prototypes,$(DSH_INCLUDE),)
