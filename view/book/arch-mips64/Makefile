##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/mycure/kaneton/view/books/ia32-virtual/Makefile
##
## created       julien quintard   [sat oct 14 11:07:49 2006]
## updated       julien quintard   [sat oct 14 11:08:09 2006]
##

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

PAPER			:=		mips64

SOURCES			:=		

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.bbl)		\
					$(SOURCES:.tex=.blg)		

#
# ---------- rules ------------------------------------------------------------
#

XXX:

all:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_paper,$(PAPER))

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_purge)
