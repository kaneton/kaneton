#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/kaneton/library/libc/Makefile
#
# created       julien quintard   [sun jun 10 14:14:02 2007]
# updated       matthieu bucchianeri   [sat sep  1 12:24:18 2007]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		libc

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers dependencies

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
ifeq ($(behaviour),default)

main:			dependencies $(_ULIBC_A_)

$(_ULIBC_A_):		$(LIBDATA_LO) $(LIBSTRING_LO) $(LIBSYS_LO)
	$(call env_remove,$(_ULIBC_A_),)

	$(call env_static-library,$(_ULIBC_A_),				\
		  $(LIBDATA_LO) $(LIBSTRING_LO)	$(LIBSYS_LO),)

clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_remove,$(_ULIBC_A_),)

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

endif
