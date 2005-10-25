##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/papers
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Fri Sep  9 19:01:41 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all dvi ps pdf clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		design

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

$(PAPER_DVI):		$(PAPER_TEX)
	$(call dvi,$(PAPER_DVI),$(PAPER_TEX),$(PAPER_TEX),)

$(PAPER_PS):		$(PAPER_DVI)
	$(call ps,$(PAPER_PS),$(PAPER_DVI),$(PAPER_TEX),)

$(PAPER_PDF):		$(PAPER_PS)
	$(call pdf,$(PAPER_PDF),$(PAPER_PS),$(PAPER_TEX),)

dvi:			$(PAPER_DVI)

ps:			$(PAPER_PS)

pdf:			$(PAPER_PDF)

clear:
	$(call remove,$(PAPER_GARBAGE),)

	$(call remove,$(PAPER_PDF),)
	$(call remove,$(PAPER_PS),)
	$(call remove,$(PAPER_DVI),)

	$(call purge)
