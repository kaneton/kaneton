##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootloader/arch
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 12 22:29:28 2005   mycure
##

include			../../../.kaneton.mk

all:
	@$(_CD_) machdep						; \
	$(_MAKE_)							; \
	$(_CD_) ..

distclean:
	@$(_CD_) machdep						; \
	$(_MAKE_) distclean						; \
	$(_CD_) ..

	$(call purge,)

proto:
	@$(_CD_) machdep						; \
	$(_MAKE_) proto							; \
	$(_CD_) ..
