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
## last update   Sun Jun 12 22:42:21 2005   mycure
##

include			../../.kaneton.mk

CONS			=		cons

CONS_O			=		cons.o

CONS_INCLUDE		=		

all:			$(CONS_O)
	$(call dynamic-linker,$(CONS),$(CONS_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call remove,$(CONS_O),)
	$(call remove,$(CONS),)

	$(call purge,)

proto:
	$(call prototypes,$(CONS_INCLUDE),)
