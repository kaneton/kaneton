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
## Last update Sun Feb 26 17:52:17 2006 Renaud Voltz
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

SRC			:=		k0.tex				\
					k1.tex				\
					k2.tex				\
					k3.tex				\
					k4.tex				\
					assignments.tex

GARBAGE			:=		$(SRC:.tex=.aux)		\
					$(SRC:.tex=.log)		\
					$(SRC:.tex=.toc)		\
					$(SRC:.tex=.dvi)		\
					$(SRC:.tex=.ps)			\
					$(SRC:.tex=.pdf)

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
