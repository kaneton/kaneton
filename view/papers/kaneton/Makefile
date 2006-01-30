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

GARBAGE			:=		$(PAPER).aux			\
					$(PAPER).log			\
					$(PAPER).dvi			\
					$(PAPER).ps			\
					$(PAPER).pdf			\
					$(PAPER).toc

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
