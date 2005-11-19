##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/view/papers/seminar
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Tue Nov  8 13:00:56 2005   mycure
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

PAPER			:=		seminar

GARBAGE			:=		$(PAPER).aux			\
					$(PAPER).log			\
					$(PAPER).nav			\
					$(PAPER).out			\
					$(PAPER).snm			\
					$(PAPER).toc			\
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
