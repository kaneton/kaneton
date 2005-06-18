##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:28:05 2005   mycure
## last update   Sat Jun 18 13:00:22 2005   mycure
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

SUBDIRS			:=		$(_CONF_LIBC_) crt

#
# rules
#

all:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) 	 						; \
	$(_CD_) ..							; \
	done

clear:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) clear 						; \
	$(_CD_) ..							; \
	done

	$(call purge,)

proto:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) proto 						; \
	$(_CD_) .. 							; \
	done

dep:
	@for i in $(SUBDIRS) ; do					\
	$(_CD_) $$i 							; \
	$(_MAKE_) dep	 						; \
	$(_CD_) .. 							; \
	done
