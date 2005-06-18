##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/core
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 03:24:11 2005   mycure
## last update   Sat Jun 18 13:24:07 2005   mycure
##

#
# dependencies
#

include			../.kaneton.mk

#
# directives
#

.PHONY:		all clear proto dep

#
# variables
#

SUBDIRS			=		bootstrap bootloader kaneton

#
# rules
#

all:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_)							; \
	$(_CD_) ..							; \
	done

clear:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) clear							; \
	$(_CD_) ..							; \
	done

	$(call purge,)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) proto							; \
	$(_CD_) ..							; \
	done

dep:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i							; \
	$(_MAKE_) dep							; \
	$(_CD_) ..							; \
	done
