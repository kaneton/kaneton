##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/cursus/schedule/Makefile
##
## created       julien quintard   [sun dec  4 13:30:54 2005]
## updated       julien quintard   [sun dec  4 13:37:34 2005]
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

PAPER			:=		schedule

GARBAGE			:=		$(PAPER).aux			\
					$(PAPER).log			\
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
	$(call paper,$(PAPER),)

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
