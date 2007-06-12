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

include			../../../environment/env.mk

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
	$(call env_remove,$(LIBARC_LO),)

	$(call env_archive,$(LIBARC_LO),$(LIBARC_O),)

clear:
	$(call env_remove,$(LIBARC_O),)

	$(call env_remove,$(LIBARC_LO),)

	$(call env_purge,)

proto:
	$(call env_prototypes,$(LIBARC_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(LIBARC_C),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
