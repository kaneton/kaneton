#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/view/book/assignments/2006/Makefile
#
# created       julien quintard   [mon may 14 19:24:19 2007]
# updated       julien quintard   [mon jun 11 06:13:53 2007]
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

PAPER			:=		assignments

SOURCES			:=		assignments.tex			\
					k0.tex				\
					k1.tex				\
					k2.tex				\
					k3.tex				\
					k4.tex				

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
