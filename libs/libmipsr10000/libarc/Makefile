##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/libs/libmipsR10000/libarc/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:08:29 2006]
## updated       enguerrand raymond   [wed oct 18 14:08:29 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIBARC_LO		:=		libarc.lo

LIBARC_C		:=		arc.c

LIBARC_O		:=		$(LIBARC_C:.c=.o)

LIBARC_INCLUDE		:=		$(_LIBOCTANE_INCLUDE_DIR_)/libarc/arc.h		\
					$(_LIBOCTANE_INCLUDE_DIR_)/libarc/spb.h

#
# ---------- rules ------------------------------------------------------------
#

$(LIBARC_LO):		$(LIBARC_O)
	$(call remove,$(LIBARC_LO),)

	$(call archive,$(LIBARC_LO),$(LIBARC_O),)

clear:
	$(call remove,$(LIBARC_O),)

	$(call remove,$(LIBARC_LO),)

	$(call purge,)

proto:
	$(call prototypes,$(LIBARC_INCLUDE),)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(LIBARC_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
