#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/lecture/kernels/boot/Makefile
#
# created       julien quintard   [wed may 16 18:54:58 2007]
# updated       julien quintard   [thu may  7 12:40:05 2009]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		boot

SOURCES			:=		boot.tex			

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
