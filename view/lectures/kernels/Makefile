##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/view/lectures/kernels/Makefile
##
## created       matthieu bucchianeri   [tue dec 26 17:54:17 2006]
## updated       matthieu bucchianeri   [tue dec 26 18:16:02 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		bootstrap		\
					interrupts		\
					ipc			\
					memory-management	\
					prerequisites		\
					task-scheduling

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call launch,Makefile,$(SUBDIRS),,)

build:
	$(call launch,Makefile,$(SUBDIRS),build,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call purge)

proto:

dep:
