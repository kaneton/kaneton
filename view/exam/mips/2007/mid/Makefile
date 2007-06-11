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

PAPER			:=		mid

SOURCES			:=		mid.tex				

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

build:			correction

subject:
	$(call env_document,$(PAPER),)

correction:
	$(call env_document,$(PAPER),$(ENV_OPTION_PRIVATE))

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
