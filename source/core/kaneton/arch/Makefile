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
## last update   Sun Jun 12 22:26:31 2005   mycure
##

include			../../../.kaneton.mk

all:
	@(CD) machdep							; \
	$(MAKE)								; \
	$(CD) ..

distclean:
	@$(CD) machdep							; \
	$(MAKE) distclean						; \
	$(CD) ..

	$(call purge,)

proto:
	@$(CD) machdep							; \
	$(MAKE) proto							; \
	$(CD) ..
