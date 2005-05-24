##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/services
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Thu May  5 18:38:50 2005   mycure
##

include			../../.common.mk

MOD			=		mod

MOD_O			=		mod.o				\
					modfs.o

MOD_INCLUDE		=		

all:			$(MOD_O)
	$(call ld,$(MOD),$(MOD_O) $(_LIBC_A_),				\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call rm,$(MOD_O),)
	$(call rm,$(MOD),)

	@$(_PURGE_)

proto:
	$(call proto,$(MOD_INCLUDE),)
