##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libsun4u/Makefile
##
## created       matthieu bucchianeri   [thu aug 10 15:01:26 2006]
## updated       matthieu bucchianeri   [mon aug 21 20:10:53 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep libs

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		misc				\
					mmu				\
					ofw				\
					traps

OFW_LO			:=		ofw/ofw.lo
MMU_LO			:=		mmu/mmu.lo
TRAPS_LO		:=		traps/traps.lo
MISC_LO			:=		misc/misc.lo

LIBSUN4U_INCLUDE	:=		$(_LIBSUN4U_INCLUDE_DIR_)/libsun4u.h

#
# ---------- rules ------------------------------------------------------------
#

$(_LIBSUN4U_A_):	libs
	$(call remove,$(_LIBSUN4U_A_),)

	$(call static-library,$(_LIBSUN4U_A_),$(OFW_LO) $(MISC_LO)	\
						$(TRAPS_LO) $(MMU_LO),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(_LIBSUN4U_A_),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

	$(call prototypes,$(LIBSUN4U_INCLUDE),)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
