##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/check/libs/klibc/libdata/Makefile
##
## created       julien quintard   [mon nov 28 18:59:45 2005]
## updated       julien quintard   [thu jul 13 11:32:29 2006]
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

LIBDATA_LO		:=		libdata.lo

#
# ---------- rules ------------------------------------------------------------
#

main:			build run

build:			$(LIBDATA_LO)

$(LIBDATA_LO):		libs
	$(call env_remove,$(LIBDATA_LO),)

	$(call env_archive,$(LIBDATA_LO),$(MODULES),)

libs:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,,); done

run:

clear:
	for d in $(SUBDIRS) ; do $(call env_launch,$${d}/Makefile,clear,); done

	$(call env_remove,$(LIBDATA_LO),)

	$(call env_purge,)

prototypes:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

headers:
