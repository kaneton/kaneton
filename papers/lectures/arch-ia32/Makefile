##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/papers/lectures
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Fri Sep  9 18:45:26 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all dvi ps pdf clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		arch-ia32

PAPER_TEX		:=		$(PAPER).tex
PAPER_DVI		:=		$(PAPER).dvi
PAPER_PS		:=		$(PAPER).ps
PAPER_PDF		:=		$(PAPER).pdf

PAPER_GARBAGE		:=		$(PAPER).aux			\
					$(PAPER).log			

#
# ---------- rules ------------------------------------------------------------
#

all:			pdf

dvi $(PAPER_DVI):	$(PAPER_TEX)
	$(call dvi,$(PAPER_DVI),$(PAPER_TEX),$(PAPER_TEX),)

ps $(PAPER_PS):		$(PAPER_DVI)
	$(call ps,$(PAPER_PS),$(PAPER_DVI),$(PAPER_TEX),)

pdf $(PAPER_PDF):	$(PAPER_PS)
	$(call pdf,$(PAPER_PDF),$(PAPER_PS),$(PAPER_TEX),)

clear:
	$(call remove,$(PAPER_GARBAGE),)

	$(call remove,$(PAPER_PDF),)
	$(call remove,$(PAPER_PS),)
	$(call remove,$(PAPER_DVI),)

	$(call purge)
