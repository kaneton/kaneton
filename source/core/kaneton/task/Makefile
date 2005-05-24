##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/core/kaneton/task
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Thu May  5 23:54:36 2005   mycure
##
 
include			../../../.common.mk

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
