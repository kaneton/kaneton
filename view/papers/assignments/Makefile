##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/view/papers/assignments
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Tue Nov  8 12:56:10 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		assignments

GARBAGE			:=		$(PAPER).aux			\
					$(PAPER).log			\
					$(PAPER).toc			\
					$(PAPER).dvi			\
					$(PAPER).ps			\
					$(PAPER).pdf			

#
# ---------- rules ------------------------------------------------------------
#

all:			build view

view:
	$(call view,$(PAPER),)

build:
	$(call paper,$(PAPER),)

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
