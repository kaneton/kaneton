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

include			../../env/.env.mk

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

$(_KLIBC_A_):		libs
	$(call remove,$(_KLIBC_A_),)

	$(call static-library,$(_KLIBC_A_),$(LIBDATA_LO) $(LIBSTRING_LO) \
          $(LIBSYS_LO),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(_KLIBC_A_),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
