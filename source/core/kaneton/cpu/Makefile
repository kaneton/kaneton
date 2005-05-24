##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/cpu
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:19:06 2005   mycure
##

include			../../../.common.mk

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
