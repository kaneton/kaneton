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
## last update   Thu May  5 18:38:41 2005   mycure
##

include			../../.common.mk

KBD			=		kbd

KBD_O			=		kbd.o

KBD_INCLUDE		=		

all:			$(KBD_O)
	$(call ld,$(KBD),$(KBD_O) $(_LIBC_A_),				\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call rm,$(KBD_O),)
	$(call rm,$(KBD),)

	@$(_PURGE_)

proto:
	$(call proto,$(KBD_INCLUDE),)
