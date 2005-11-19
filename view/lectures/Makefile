##
## copyright     (c) julien quintard
##
## project       kaneton
##
## file          /home/mycure/kaneton/view/lectures/Makefile
##
## created       julien quintard   [sat nov 19 18:06:43 2005]
## updated       julien quintard   [sat nov 19 18:10:02 2005]
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
					distributed-operating-systems	\
					inline-assembly			\
					kernels				\
					prerequisites			\
					seminar				

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call purge)

proto:

dep:
