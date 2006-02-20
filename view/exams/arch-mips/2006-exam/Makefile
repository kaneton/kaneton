##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/view/lectures/arch-mips
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Fri Nov 18 20:47:51 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build subject correction clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		2006-exam

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

all:			build view

view:
	$(call view,$(PAPER),)

build:			correction

subject:
	$(call subject,$(PAPER),)

correction:
	$(call correction,$(PAPER),)

clear:
	$(call remove,$(GARBAGE),)

	$(call remove,$(_LATEX_MODE_),)

	$(call purge)
