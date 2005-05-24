##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/time
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:55:31 2005   mycure
##

include			../../../.common.mk

TIME			=		time.a

TIME_INCLUDE		=		$(_INCLUDE_DIR_)/time.h

TIME_O			=		timer.o				\
					clock.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(TIME_O)
	$(call ar-o-a,$(TIME),$(TIME_O),)
	$(call ranlib,$(TIME),)

distclean:
	$(call rm,$(TIME_O),)

	@$(_PURGE_)

	$(call rm,$(TIME),)

proto:
	$(call proto,$(TIME_INCLUDE),)
