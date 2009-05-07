#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/lecture/inline-assembly/Makefile
#
# created       julien quintard   [wed may 16 18:48:18 2007]
# updated       julien quintard   [thu may  7 12:39:37 2009]
#

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

PAPER			:=		inline-assembly

SOURCES			:=		inline-assembly.tex		

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
