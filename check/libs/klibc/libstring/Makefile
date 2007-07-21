##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/klibc/libstring/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:32:42 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all build run libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

LIST			:=		list

SUBDIRS			:=		$(shell $(call env_pull,$(LIST),))

MODULES			:=		$(foreach s,$(SUBDIRS),$s/$s.lo)

LIBSTRING_LO		:=		libstring.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(LIBSTRING_LO)

$(LIBSTRING_LO):		libs
	$(call env_remove,$(LIBSTRING_LO),)

	$(call env_archive,$(LIBSTRING_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(LIBSTRING_LO),)

	$(call env_purge,)

prototypes:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
