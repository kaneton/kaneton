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
## last update   Sun Jun 12 22:43:31 2005   mycure
##

include			../../.kaneton.mk

KBD			=		kbd

KBD_O			=		kbd.o

KBD_INCLUDE		=		

all:			$(KBD_O)
	$(call dynamic-linker,$(KBD),$(KBD_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call remove,$(KBD_O),)
	$(call remove,$(KBD),)

	$(call purge,)

proto:
	$(call prototypes,$(KBD_INCLUDE),)
