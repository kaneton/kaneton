##
## copyright quintard julien
##
## kaneton
##
## Makefile
##
## path          /home/mycure/kaneton/libs
##
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
##
## started on    Fri Feb 11 02:28:05 2005   mycure
## Last update Tue Dec 20 13:45:56 2005 matthieu bucchianeri
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		klibc crt libia32

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

dep:
	$(call makefile,$(SUBDIRS),dep)
