#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/feedback/kernels/Makefile
#
# created       julien quintard   [tue may 15 21:52:06 2007]
# updated       julien quintard   [mon jun 11 06:15:37 2007]
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

PAPER			:=		kernels

SOURCES			:=		kernels.tex

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.pdf)		\
					$(SOURCES:.tex=.bbl)		\
					$(SOURCES:.tex=.blg)		

#
# ---------- rules ------------------------------------------------------------
#

XXX:

main:			build

view:			build
	$(call env_view,$(PAPER),)

build:
	$(call env_document,$(PAPER),)

clear:
	$(call env_remove,$(GARBAGE),)

	$(call env_remove,$(_DEPENDENCY_TEX_),)

	$(call env_purge)
