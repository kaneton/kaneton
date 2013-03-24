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

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main view build subject correction clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		kernels
PAPER_RATTRAPAGE	:=		kernels_rattrapage
SOURCES			:=		kernels.tex
SOURCES_RATTRAPAGE	:=		kernels_rattrapage.tex

#
# ---------- rules ------------------------------------------------------------
#

main:			build

view:			build
	$(call env_view,$(PAPER),)
	$(call env_view,$(PAPER_RATTRAPAGE),)

build:			correction

subject:
	$(call env_document,$(PAPER),)
	$(call env_document,$(PAPER_RATTRAPAGE),)

correction:
	$(call env_document,$(PAPER),$(ENV_OPTION_PRIVATE))
	$(call env_document,$(PAPER_RATTRAPAGE),$(ENV_OPTION_PRIVATE))

clear:
	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
