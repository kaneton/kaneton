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
## last update   Sun Jun 12 22:16:15 2005   mycure
##

include			../../.kaneton.mk

BOOTSTRAP		=		bootstrap

all:
	@$(CD) arch							; \
	$(MAKE)								; \
	$(CD) ..

distclean:
	@$(CD) arch							; \
	$(MAKE) distclean						; \
	$(CD) ..

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:
	@$(CD) arch							; \
	$(MAKE) proto							; \
	$(CD) ..
