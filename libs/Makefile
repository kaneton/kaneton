##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/Makefile
##
## created       julien quintard   [fri feb 11 02:28:05 2005]
## updated       matthieu bucchianeri   [tue jan 24 11:50:09 2006]
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
