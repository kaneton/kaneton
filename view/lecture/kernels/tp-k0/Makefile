##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/view/lectures/kernels/tp-k0/Makefile
##
## created       matthieu bucchianeri   [tue dec 26 17:58:59 2006]
## updated       matthieu bucchianeri   [wed jan 24 15:10:08 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		tp-k0

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

build:
	$(call compile-tex,$(PAPER))

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
