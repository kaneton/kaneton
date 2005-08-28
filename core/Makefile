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
## last update   Sun Aug 28 15:02:46 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			=		bootstrap bootloader kaneton

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call purge,)

proto:
	$(call make,$(SUBDIRS),proto)

dep:
	$(call make,$(SUBDIRS),dep)
