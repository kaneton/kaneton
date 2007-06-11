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

.PHONY:		main view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		kaneton

SOURCES			:=		architectures.tex		\
					bibliography.tex		\
					boot.tex			\
					cheat.tex			\
					coding-style.tex		\
					core.tex			\
					development-environment.tex	\
					drivers.tex			\
					goals.tex			\
					history.tex			\
					kaneton.tex			\
					licenses.tex			\
					nomenclature.tex		\
					overview.tex			\
					portability.tex			\
					services.tex			\
					source-tree.tex			\
					test.tex			\
					tools.tex			

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.bbl)		\
					$(SOURCES:.tex=.blg)		

#
# ---------- rules ------------------------------------------------------------
#

main:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_document,$(PAPER),)

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
