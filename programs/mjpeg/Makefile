#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/buckman/crypt/kaneton/programs/mjpeg/Makefile
#
# created       matthieu bucchianeri   [mon aug 20 19:52:55 2007]
# updated       matthieu bucchianeri   [tue sep 11 18:58:00 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		libs clear prototypes dependencies main

#
# ---------- variables --------------------------------------------------------
#

MJPEG			:=		mjpeg

MJPEG_C			:=		bit_reading.c		\
					block_io.c		\
					demux.c			\
					demux_launcher.c	\
					huff_tables.c		\
					idct.c			\
					idct_launcher.c		\
					iqzz.c			\
					iqzz_launcher.c		\
					libu.c			\
					libu_launcher.c		\
					main.c			\
					mwmr.c			\
					parser.c		\
					ramdac.c		\
					ramdac_launcher.c	\
					tg_launcher.c		\
					tg_posix.c		\
					vld.c			\
					vld_launcher.c

MJPEG_O			:=		$(MJPEG_C:.c=.o)

_CC_FLAGS_		+=		-I. -Iinclude

#
# ---------- rules ------------------------------------------------------------
#
ifeq ($(behaviour),default)

main:	dependencies $(MJPEG)

$(MJPEG):	libs $(MJPEG_O)
	$(call env_executable,$(MJPEG),					\
			      $(MJPEG_O)				\
			      $(_CRT_O_) $(_ULIBC_A_) $(_LIBPTHREAD_A_)	\
			      $(_LIBKANETON_A_),			\
			      $(_PROGRAM_LAYOUT_),			\
			      $(ENV_OPTION_NO_STANDARD))

libs:
	$(call env_launch,${_ULIBC_DIR_}/Makefile,,)
	$(call env_launch,${_LIBPTHREAD_DIR_}/Makefile,,)
	$(call env_launch,${_LIBKANETON_DIR_}/Makefile,,)

clear:
	$(call env_remove,$(MJPEG_O),)
	$(call env_remove,$(MJPEG),)

	$(call env_purge,)

prototypes:

dependencies:

endif
#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_DEPENDENCY_MK_)
