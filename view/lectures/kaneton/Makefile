##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/lectures/kaneton/Makefile
##
## created       julien quintard   [sun dec  4 16:39:25 2005]
## updated       julien quintard   [sun dec  4 16:39:55 2005]
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
	$(call lecture,$(PAPER),)

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
