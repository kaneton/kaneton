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
## updated       enguerrand raymond   [sun oct 22 23:22:35 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

# bootstrap

BOOTSTRAP		=		bootstrap

BOOTSTRAP_S		=		bootstrap.S

BOOTSTRAPEND_S		=		bootstrapend.S

BOOTLOADER_C		=		bootloader.c

BOOTSTRAP_O		=		$(BOOTSTRAP_S:.S=.o)

BOOTLOADER_O		=		$(BOOTLOADER_C:.c=.o)

BOOTSTRAPEND_O		=		$(BOOTSTRAPEND_S:.S=.o)

#
# ---------- rules ------------------------------------------------------------
#

$(BOOTSTRAP):
	$(call assemble-S, $(BOOTSTRAP_O), $(BOOTSTRAP_S),)

	$(call assemble-S, $(BOOTSTRAPEND_O), $(BOOTSTRAPEND_S),)

	$(call compile-c, $(BOOTLOADER_O), $(BOOTLOADER_C),			\
		"--no-standard-include"						\
                "--no-standard-library")

	$(call executable, $(BOOTSTRAP), $(BOOTSTRAP_O)	$(BOOTLOADER_O)		\
		$(BOOTSTRAPEND_O),						\
		"--link-script $(_MACHDEP_LINK_DIR_)/bootstrap.lks"		\
		"--no-standard-include"						\
		"--no-standard-library")

clear:
	$(call remove,$(BOOTSTRAP_O),)

	$(call remove,$(BOOTLOADER_O),)

	$(call remove,$(BOOTSTRAP),)

	$(call purge,)

proto:

dep:
