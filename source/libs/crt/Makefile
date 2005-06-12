##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/crt
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 21:19:11 2005   mycure
##

include			../../.kaneton.mk

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
	;
