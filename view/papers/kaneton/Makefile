##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/view/papers/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Tue Nov  8 12:59:46 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all dvi ps pdf clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		kaneton

GARBAGE			:=		$(PAPER).aux			\
					$(PAPER).log			\
					$(PAPER).dvi			\
					$(PAPER).ps			\
					$(PAPER).pdf			

#
# ---------- rules ------------------------------------------------------------
#

all:			view

view:			build
	$(call view,$(PAPER),)

build:
	$(call compile-tex,$(PAPER))

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
