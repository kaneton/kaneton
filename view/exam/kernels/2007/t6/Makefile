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

include			../../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		t6

SOURCES			:=		t6.tex				\
					annex.tex			\
					fpu.tex				\
					interrupts.tex			\
					mistakes.tex			\
					scheduling.tex			\
					sleep_wakeup.tex		

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

build:			correction

subject:
	$(call env_subject,$(PAPER),)

correction:
	$(call env_correction,$(PAPER),)

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_remove,$(_KANETON_LATEX_),)

	$(call env_purge)
