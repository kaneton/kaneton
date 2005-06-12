##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:18:07 2005   mycure
##
 
include			../../../.kaneton.mk

TASK			=		task.a

TASK_INCLUDE		=		$(_INCLUDE_DIR_)/task.h

TASK_O			=		task.o				

CFLAGS			+=		-D___kernel -D___kaneton

all:			$(TASK_O)
	$(call ar-o-a,$(TASK),$(TASK_O),)
	$(call ranlib,$(TASK),)

distclean:
	$(call rm,$(TASK_O),)

	@$(_PURGE_)

	$(call rm,$(TASK),)

proto:
	$(call proto,$(TASK_INCLUDE),)
