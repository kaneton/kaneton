##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core/bootstrap/arch
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sat Jun 18 15:27:12 2005   mycure
##

#
# dependencies
#

include			../../../.kaneton.mk

#
# directives
#

.PHONY:		all clear proto dep

#
# rules
#

all:
	@$(_CD_) machdep						; \
	$(_MAKE_)							; \
	$(_CD_) ..

clear:
	@$(_CD_) machdep						; \
	$(_MAKE_) clear							; \
	$(_CD_) ..

	$(call purge,)

proto:
	@$(_CD_) machdep						; \
	$(_MAKE_) proto							; \
	$(_CD_) ..

dep:
	@$(_CD_) machdep						; \
	$(_MAKE_) dep							; \
	$(_CD_) ..
