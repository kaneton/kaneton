##
## ---------- header ----------------------------------------------------------
##
## project       kaneton
##
## license       kaneton licence
##
## file          /home/enguerrand/kaneton/kaneton/bootstrap/arch/sgi-o2.mips64/Makefile
##
## created       enguerrand raymond   [wed oct 18 14:24:53 2006]
## updated       enguerrand raymond   [mon jan 22 22:36:06 2007]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

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

#_CFLAGS_		=		$(_CFLAGS32_)
#_LDFLAGS_		=		$(_LDFLAGS32_)

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP): libs bootloader_lo
	$(call env_env_assemble-S, $(BOOTSTRAP_O), $(BOOTSTRAP_S),)

	$(call env_env_executable, $(BOOTSTRAP), $(BOOTSTRAP_O)	$(BOOTLOADER_LO)	\
		$(_KLIBC_A_) $(_LIBMIPSR10000_A_),				\
		"--link-script $(_MACHDEP_LINK_DIR_)/bootstrap.lks"		\
		"--no-standard-include"						\
		"--no-standard-library")

	#$(call env_env_launch,Makefile,$(_KLIBC_DIR_),clear,)
	#$(call env_env_launch,Makefile,$(_LIBMIPSR10000_DIR_),clear,)

bootloader_lo:
	$(call env_env_launch,Makefile,$(_MACHDEP_BOOTLOADER_DIR_),,)
	echo $(_MACHDEP_BOOTLOADER_DIR_)

libs:
	$(call env_env_launch,Makefile,$(_KLIBC_DIR_),clear,)
	$(call env_env_launch,Makefile,$(_LIBMIPSR10000_DIR_),clear,)

	$(call env_env_launch,Makefile,$(_KLIBC_DIR_),MODE32=yes,)

	$(call env_env_launch,Makefile,$(_LIBMIPSR10000_DIR_),MODE32=yes,)

clear:
	$(call env_env_remove,$(BOOTSTRAP_O),)

	$(call env_env_remove,$(BOOTSTRAP),)

	$(call env_env_purge,)

proto:

dep:
