##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/libs/Makefile
##
## created       julien quintard   [fri feb 11 02:28:05 2005]
## updated       julien quintard   [wed jun  7 13:14:25 2006]
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

SUBDIRS			:=		klibc				\
					crt				\
					libia32

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
