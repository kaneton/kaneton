##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/apps/mjpeg/Makefile
##
## created       matthieu bucchianeri   [sat dec  2 02:50:18 2006]
## updated       matthieu bucchianeri   [sat dec  2 02:56:27 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

MJPEG_LO		:=		mjpeg.lo

MJPEG_C			:=		barrier.c		\
					bit_reading.c		\
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
					lock.c			\
					log.c			\
					main.c			\
					memspace.c		\
					mwmr.c			\
					parser.c		\
					pthread.c		\
					ramdac.c		\
					ramdac_launcher.c	\
					tg_launcher.c		\
					tg_posix.c		\
					vld.c			\
					vld_launcher.c

MJPEG_O			:=		$(MJPEG_C:.c=.o)

_CFLAGS_		+=		-I. -Iinclude

#
# ---------- rules ------------------------------------------------------------
#

$(MJPEG_LO):		$(MJPEG_O)
	$(call remove,$(MJPEG_LO),)

	$(call archive,$(MJPEG_LO),$(MJPEG_O),)

clear:
	$(call remove,$(MJPEG_O),)

	$(call remove,$(MJPEG_LO),)

	$(call purge,)

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(MJPEG_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
