#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/enguerrand/kaneton/tool/firmware/qemu-mips/Makefile
#
# created       enguerrand raymond   [sat oct 11 18:44:41 2008]
# updated       enguerrand raymond   [mon apr 27 11:24:35 2009]
#

#
# ---------- component --------------------------------------------------------
#

component		:=		qemu-mips-firmware

#
# ---------- dependencies -----------------------------------------------------
#

include			../../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

FIRMWARE		:=		firmware

FIRMWARE_BIN		:=		firmware.bin

FIRMWARE_C		:=		firmware.c				\
					libc.c					\
					uart.c					\
					cons.c

FIRMWARE_S		:=		entry.S

FIRMWARE_O		:=		$(FIRMWARE_S:.S=.o)			\
					$(FIRMWARE_C:.c=.o)

FIRMWARE_INCLUDE	:=		$(_FIRMWARE_DIR_)/firmware.h		\
					$(_FIRMWARE_DIR_)/uart.h		\
					$(_FIRMWARE_DIR_)/libc.h		\
					$(_FIRMWARE_DIR_)/cons.h

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:			$(FIRMWARE)

$(FIRMWARE):	$(FIRMWARE_O)
	$(call env_executable,$(FIRMWARE),				\
			      $(FIRMWARE_O),				\
			      $(_FIRMWARE_DIR_)/layout/firmware.lyt,	\
			      $(ENV_OPTION_NO_STANDARD))

clear:
	$(call env_remove,$(FIRMWARE_O),)

	$(call env_remove,$(FIRMWARE),)

	$(call env_remove,$(_FIRMWARE_LO_),)

	$(call env_remove,$(FIRMWARE_BIN),)

	$(call env_purge,)

prototypes:
	$(call env_prototypes,$(FIRMWARE_INCLUDE),)

headers:
	$(call env_remove,$(_DEPENDENCY_MK_),)

	$(call env_headers,$(FIRMWARE_C),)

dependencies:
	$(call env_launch,Makefile,.,$(FIRMWARE),)

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
