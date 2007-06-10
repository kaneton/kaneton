#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/library/libc/Makefile
#
# created       julien quintard   [sun jun 10 14:14:02 2007]
# updated       julien quintard   [sun jun 10 14:41:46 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear prototypes headers dependencies

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

$(_LIBC_A_):		dependencies
	$(call env_remove,$(_LIBC_A_),)

	$(call env_static-library,$(_LIBC_A_),$(LIBDATA_LO) $(LIBSTRING_LO) \
          $(LIBSYS_LO),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_LIBC_A_),)

	$(call env_purge,)

prototypes:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,prototypes,)			; \
	done

headers:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,headers,)			; \
	done

dependencies:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done
