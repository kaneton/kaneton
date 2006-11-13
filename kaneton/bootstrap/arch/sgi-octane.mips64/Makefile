##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-octane.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:53 2006]
## updated       enguerrand raymond   [thu oct 26 18:47:57 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear proto dep

#
# ---------- variables --------------------------------------------------------
#

# bootstrap

BOOTSTRAP		=		bootstrap

BOOTSTRAP_S		=		bootstrap.S

BOOTSTRAP_O		=		$(BOOTSTRAP_S:.S=.o)

BOOTLOADER_LO		=		$(_MACHDEP_BOOTLOADER_DIR_)/bootloader.lo

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP): libs bootloader_lo
	$(call assemble-S, $(BOOTSTRAP_O), $(BOOTSTRAP_S),)

	$(call executable, $(BOOTSTRAP), $(BOOTSTRAP_O)	$(BOOTLOADER_LO)	\
		$(_KLIBC_A_) $(_LIBMIPSR10000_A_),				\
		"--link-script $(_MACHDEP_LINK_DIR_)/bootstrap.lks"		\
		"--no-standard-include"						\
		"--no-standard-library")

bootloader_lo:
	$(call launch,Makefile,$(_MACHDEP_BOOTLOADER_DIR_),,)
	echo $(_MACHDEP_BOOTLOADER_DIR_)

libs:
	$(call launch,Makefile,$(_KLIBC_DIR_),,)

	$(call launch,Makefile,$(_LIBMIPSR10000_DIR_),,)

clear:
	$(call remove,$(BOOTSTRAP_O),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
