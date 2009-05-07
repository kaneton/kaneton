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

.PHONY:		main view build subject correction clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		t5

SOURCES			:=		t5.tex				\
					annex.tex			\
					asm.tex				\
					buddy.tex			\
					cow.tex				\
					region_mips.tex			

#
# ---------- rules ------------------------------------------------------------
#

main:			build

view:			build
	$(call env_view,$(PAPER),)

build:			correction

subject:
	$(call env_document,$(PAPER),)

correction:
	$(call env_document,$(PAPER),$(ENV_OPTION_PRIVATE))

clear:
	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
