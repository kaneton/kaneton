##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootstrap
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:05:35 2005   mycure
## last update   Sun Jun 12 21:15:09 2005   mycure
##

include			../../.kaneton.mk

BOOTSTRAP		=		bootstrap

all:
	@cd arch							; \
	make								; \
	cd ..

distclean:
	@cd arch							; \
	make distclean							; \
	cd ..

	$(call rm,$(BOOTSTRAP),)

proto:
	@cd arch							; \
	make proto							; \
	cd ..
