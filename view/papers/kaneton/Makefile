##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/papers/kaneton/Makefile
##
## created       matthieu bucchianeri   [mon jan 30 22:05:58 2006]
## updated       julien quintard   [thu mar  9 16:23:32 2006]
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

PAPER			:=		kaneton

SOURCES			:=		kaneton.tex			\
					goals.tex			\
					history.tex			\
					overview.tex			\
					source-tree.tex			\
					development-environment.tex	\
					nomenclature.tex		\
					coding-style.tex		\
					core.tex			\
					tools.tex			\
					check.tex			\
					glossary.tex			

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.dvi)		\
					$(SOURCES:.tex=.ps)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.toc)

#
# ---------- rules ------------------------------------------------------------
#

all:			build view

view:
	$(call view,$(PAPER),)

build:
	$(call paper,$(PAPER))

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
