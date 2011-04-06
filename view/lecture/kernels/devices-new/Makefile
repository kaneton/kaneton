#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kane...view/lecture/kernels/windows-nt/Makefile
#
# created       julien quintard   [wed may 16 18:44:45 2007]
# updated       julien quintard   [thu may  7 12:41:54 2009]
#

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

PAPER			:=		devices-new

SOURCES			:=		devices-new.tex		

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
