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
## last update   Thu May  5 18:38:33 2005   mycure
##

include			../../.common.mk

DSH			=		dsh

DSH_O			=		dsh.o

DSH_INCLUDE		=		

all:			$(DSH_O)
	$(call ld,$(DSH),$(DSH_O) $(_LIBC_A_),				\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call rm,$(DSH_O),)
	$(call rm,$(DSH),)

	@$(_PURGE_)

proto:
	$(call proto,$(DSH_INCLUDE),)
