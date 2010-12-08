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

PAPER			:=		development

SOURCES			:=		asm.tex				\
					c.tex				\
					cheat.tex			\
					community.tex			\
					configure.tex			\
					control-panel.tex		\
					development.tex			\
					environment.tex			\
					export.tex			\
					history.tex			\
					introduction.tex		\
					languages.tex			\
					latex.tex			\
					licenses.tex			\
					mailing-list.tex		\
					make.tex			\
					people.tex			\
					project-management.tex		\
					prototypes.tex			\
					python.tex			\
					repository.tex			\
					rules.tex			\
					source-tree.tex			\
					test.tex			\
					tools.tex			\
					transcript.tex			\
					view.tex			\
					wiki.tex			

#
# ---------- rules ------------------------------------------------------------
#

main:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_document,$(PAPER),)

clear:
	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
