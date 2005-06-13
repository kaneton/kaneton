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
## last update   Sun Jun 12 22:39:05 2005   mycure
##

include			../../.kaneton.mk

CRT			=		crt.a

CRT_O			=		crt.o				

all:			$(CRT_O)
	$(call archive,$(CRT),$(CRT_O),)

	$(call static-library,$(CRT),)

distclean:
	$(call remove,$(CRT_O),)

	$(call purge,)

	$(call remove,$(CRT),)

proto:
	;
