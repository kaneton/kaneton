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
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Mon Oct 31 10:13:11 2005   mycure
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

SUBDIRS			:=						\
  $(shell $(_CAT_) $(_CONF_DIR_)/$(USER)/modules.conf |			\
          $(_SED_) -n "s/^programs\/\([[:alpha:]_-]*\)\/.*$$/\1/p")

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
