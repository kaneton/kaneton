#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/book/assignments/2005/Makefile
#
# created       julien quintard   [mon may 14 19:24:19 2007]
# updated       julien quintard   [mon may 14 19:37:33 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		assignments

SOURCES			:=		assignments.tex			\
					k0.tex				\
					k1.tex				\
					k2.tex				\
					k3.tex				\
					k4.tex				

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.bbl)		\
					$(SOURCES:.tex=.blg)		

#
# ---------- rules ------------------------------------------------------------
#

all:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_paper,$(PAPER),)

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_purge)
