##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/services/cons
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:00:09 2005   mycure
## last update   Thu May  5 18:38:24 2005   mycure
##

include			../../.common.mk

CONS			=		cons

CONS_O			=		cons.o

CONS_INCLUDE		=		

all:			$(CONS_O)
	$(call ld,$(CONS),$(CONS_O) $(_LIBC_A_),			\
	  -T $(_LDS_DIR_)/service.lds)

distclean:
	$(call rm,$(CONS_O),)
	$(call rm,$(CONS),)

	@$(_PURGE_)

proto:
	$(call proto,$(CONS_INCLUDE),)
