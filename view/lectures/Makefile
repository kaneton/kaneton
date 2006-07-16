##
## copyright     (c) julien quintard
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/lectures/Makefile
##
## created       julien quintard   [sat nov 19 18:06:43 2005]
## updated       julien quintard   [thu jul 13 11:45:55 2006]
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

SUBDIRS			:=		advanced-makefiles		\
					arch-ia32			\
					arch-mips			\
					c-preprocessor			\
					development-environment		\
					inline-assembly			\
					kernels				\
					prerequisites			\
					seminar				\
					lse01				

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
