
#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all view build clear

#
# ---------- variables --------------------------------------------------------
#

PAPER			:=		assignments

SOURCES			:=		assignments.tex			\
					k2.tex

GARBAGE			:=		$(SOURCES:.tex=.aux)		\
					$(SOURCES:.tex=.log)		\
					$(SOURCES:.tex=.toc)		\
					$(SOURCES:.tex=.dvi)		\
					$(SOURCES:.tex=.ps)		\
					$(SOURCES:.tex=.pdf)

#
# ---------- rules ------------------------------------------------------------
#

all:			build view

view:
	$(call view,$(PAPER),)

build:
	$(call paper,$(PAPER),)

clear:
	$(call remove,$(GARBAGE),)

	$(call purge)
