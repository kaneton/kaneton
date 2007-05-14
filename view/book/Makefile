##
## copyright quintard julien
##
## kaneton
##
## Makefile
##
## path          /home/mycure/kaneton/view/books
##
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
##
## started on    Fri Feb 11 02:59:20 2005   mycure
## Last update Sat Sep  2 11:27:57 2006 matthieu bucchianeri
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		arch-ia32			\
					arch-mips64			\
					assignments			\
					development			\
					kaneton				

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge)

proto:

dep:
