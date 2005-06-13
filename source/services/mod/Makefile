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
## last update   Sun Jun 12 22:43:59 2005   mycure
##

include			../../.kaneton.mk

MOD			=		mod

MOD_O			=		mod.o				\
					modfs.o

MOD_INCLUDE		=		

all:			$(MOD_O)
	$(call dynamic-linker,$(MOD),$(MOD_O) $(_LIBC_A_),		\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call remove,$(MOD_O),)
	$(call remove,$(MOD),)

	$(call purge,)

proto:
	$(call prototypes,$(MOD_INCLUDE),)
