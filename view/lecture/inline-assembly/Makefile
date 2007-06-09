#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/lecture/inline-assembly/Makefile
#
# created       julien quintard   [wed may 16 18:48:18 2007]
# updated       julien quintard   [wed may 16 18:48:43 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		inline-assembly

SOURCES			:=		inline-assembly.tex		

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.bbl)		\
					$(SOURCES:.tex=.blg)		\
					$(SOURCES:.tex=.nav)		\
					$(SOURCES:.tex=.out)		\
					$(SOURCES:.tex=.snm)		

#
# ---------- rules ------------------------------------------------------------
#

all:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_paper,$(PAPER))

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_purge)
