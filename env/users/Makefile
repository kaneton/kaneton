##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/export
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Mon Oct 31 11:56:05 2005   mycure
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

KANETON_CONF		:=		kaneton.conf

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(_SHELL_) $(_CONF_SH_)

clear:
	$(call remove,$(KANETON_CONF),)

	$(call purge)

proto:

dep:
