##
## copyright     (c) julien quintard
##
## project       kaneton
##
## file          /home/buckman/kaneton/view/lectures/Makefile
##
## created       julien quintard   [sat nov 19 18:06:43 2005]
## updated       matthieu bucchianeri   [sat jan  6 21:35:06 2007]
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
					arch-mips			\
					c-preprocessor			\
					development-environment		\
					inline-assembly			\
					kernels				\
					prerequisites			\
					seminar				\
					lse01				\
					project-overview

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
