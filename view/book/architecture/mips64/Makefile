##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton
##
## file          /home/enguerrand/kaneton/view/books/MIPS64/Makefile
##
## created       enguerrand raymond   [wed oct 01 18:59:49 2008]
## updated       enguerrand raymond   [wed oct 01 18:59:09 2008]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		mips64

SOURCES			:=		mips64.tex			\
					overview.tex			\
					bootloader.tex			\
					memory.tex			\
					bibliography.tex

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
