##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton/cpu
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:16:37 2005   mycure
##

include			../../../.kaneton.mk

CPU			=		cpu.a

CPU_INCLUDE		=		$(_INCLUDE_DIR_)/cpu.h

CPU_O			=		cpu.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(CPU_O)
	$(call ar-o-a,$(CPU),$(CPU_O),)
	$(call ranlib,$(CPU),)

distclean:
	$(call rm,$(CPU_O),)

	@$(_PURGE_)

	$(call rm,$(CPU),)

proto:
	$(call proto,$(CPU_INCLUDE),)
