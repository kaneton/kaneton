##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/view/papers/kaneton/Makefile
##
## created       matthieu bucchianeri   [mon jan 30 22:05:58 2006]
## updated       matthieu bucchianeri   [mon jan 30 22:05:59 2006]
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

SRC			:=		1-goals.tex			\
					2-overview.tex			\
					3-devenv.tex			\
					4-source.tex			\
					5-coding.tex			\
					6-indep.tex			\
					7-tools.tex			\
					8-tests.tex			\
					9-glossary.tex			\
					kaneton.tex

GARBAGE			:=		$(SRC:.tex=.aux)		\
					$(SRC:.tex=.log)		\
					$(SRC:.tex=.dvi)		\
					$(SRC:.tex=.ps)			\
					$(SRC:.tex=.pdf)		\
					$(SRC:.tex=.toc)

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
