##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/libs/klibc/Makefile
##
## created       julien quintard   [fri feb 11 02:29:19 2005]
## updated       julien quintard   [thu jul 13 11:45:02 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libdata libstring libsys

LIBDATA_LO		:=		libdata/libdata.lo
LIBSTRING_LO		:=		libstring/libstring.lo
LIBSYS_LO		:=		libsys/libsys.lo

#
# ---------- rules ------------------------------------------------------------
#

#main:		relations

relations:
	$(call env_launch,Makefile,$(SUBDIRS),,)

	$(call env_launch,Makefile,.,$(_KLIBC_A_),)

$(_KLIBC_A_):		$(LIBDATA_LO) $(LIBSTRING_LO) $(LIBSYS_LO)
	$(call env_remove,$(_KLIBC_A_),)

	$(call env_static-library,$(_KLIBC_A_),$(LIBDATA_LO) $(LIBSTRING_LO) \
          $(LIBSYS_LO),)

clear:
	$(call env_launch,Makefile,$(SUBDIRS),clear,)

	$(call env_remove,$(_KLIBC_A_),)

	$(call env_purge,)

proto:
	$(call env_launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call env_launch,Makefile,$(SUBDIRS),dep,)
