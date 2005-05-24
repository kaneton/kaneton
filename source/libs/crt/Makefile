##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/src/libs/crt
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Fri May  6 00:12:42 2005   mycure
##

include			../../.common.mk

CRT			=		crt.a

CRT_O			=		crt.o				

all:			$(CRT_O)
	$(call ar-o-a,$(CRT),$(CRT_O),)
	$(call ranlib,$(CRT),)

distclean:
	$(call rm,$(CRT_O),)

	@$(_PURGE_)

	$(call rm,$(CRT),)

proto:
	
