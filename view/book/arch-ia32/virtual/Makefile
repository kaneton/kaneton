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

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		ia32-virtual

SOURCES			:=		bibliography.tex		\
					bootloader.tex			\
					event.tex			\
					ia32-virtual.tex		\
					io.tex				\
					memory.tex			\
					overview.tex			\
					references.tex			\
					task.tex			

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.dvi)		\
					$(SOURCES:.tex=.ps)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.toc)

#
# ---------- rules ------------------------------------------------------------
#

all:			build

view:
	$(call view,$(PAPER),)

build:
	$(call paper,$(PAPER))

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
