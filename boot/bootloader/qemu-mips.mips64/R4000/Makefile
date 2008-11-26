#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/...otloader/qemu-mips.mips64/R4000/Makefile
#
# created       enguerrand raymond   [sat oct 11 18:44:41 2008]
# updated          [wed nov 26 14:59:30 2008]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		bootloader

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

BOOTLOADER		:=		bootloader

BOOTLOADER_C		:=		

BOOTLOADER_S		:=		bootloader.S

BOOTLOADER_O		:=		$(BOOTLOADER_C:.c=.o)		\
					$(BOOTLOADER_S:.S=.o)

BOOTLOADER_INCLUDE	:=

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(_BOOTLOADER_LO_)

$(_BOOTLOADER_LO_):	$(BOOTLOADER_O)
	$(call env_remove,$(_BOOTLOADER_LO_),)

	$(call env_archive,$(_BOOTLOADER_LO_),$(BOOTLOADER_O),)
	#$(call env_executable,$(_BOOTLOADER_),				\
	#		      $(BOOTLOADER_O),				\
	#		      $(_BOOTLOADER_DIR_)/layout/bootloader.lyt,\
	#		      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(BOOTLOADER_O),)

	$(call env_remove,$(BOOTLOADER),)

	$(call env_remove,$(_BOOTLOADER_LO_),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(BOOTLOADER_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(BOOTLOADER_C),)

dependencies:
	$(call env_launch,Makefile,.,$(BOOTLOADER),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
