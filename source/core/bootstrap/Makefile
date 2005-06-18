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
## last update   Sat Jun 18 15:26:18 2005   mycure
##

#
# dependencies
#

include			../../.kaneton.mk

#
# directives
#

.PHONY:		all clear proto dep

#
# variables
#

BOOTSTRAP		=		bootstrap

#
# rules
#

all:
	@$(_CD_) arch							; \
	$(_MAKE_)							; \
	$(_CD_) ..

clear:
	@$(_CD_) arch							; \
	$(_MAKE_) clear							; \
	$(_CD_) ..

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:
	@$(_CD_) arch							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..

dep:
	@$(_CD_) arch							; \
	$(_MAKE_) dep							; \
	$(_CD_) ..
