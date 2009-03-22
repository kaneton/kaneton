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
# updated          [tue mar 17 00:57:11 2009]
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

BOOTLOADER_C		:=		bootloader.c				\
					opmode.c

BOOTLOADER_S		:=		entry.S

BOOTLOADER_O		:=		$(BOOTLOADER_S:.S=.o)			\
					$(BOOTLOADER_C:.c=.o)

BOOTLOADER_INCLUDE	:=		$(_BOOTLOADER_DIR_)/bootloader.h	\
					$(_BOOTLOADER_DIR_)/cp0.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(BOOTLOADER)

$(BOOTLOADER):	$(BOOTLOADER_O)
	#$(call env_remove,$(_BOOTLOADER_LO_),)

	#$(call env_archive,$(_BOOTLOADER_LO_),$(BOOTLOADER_O),)

	$(call env_executable,$(BOOTLOADER),				\
			      $(BOOTLOADER_O),				\
			      $(_BOOTLOADER_DIR_)/layout/bootloader.lyt,\
			      $(ENV_OPTION_NO_STANDARD))

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
