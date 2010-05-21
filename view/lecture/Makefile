##
## copyright quintard julien
##
## kaneton
##
## Makefile
##
## path          /home/mycure/kaneton/view/books
##
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
##
## started on    Fri Feb 11 02:59:20 2005   mycure
## Last update Sat Sep  2 11:27:57 2006 matthieu bucchianeri
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build clear

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		architecture			\
					c-preprocessor			\
					inline-assembly			\
					kernels				\
					linking				\
					prerequisites			\
					philosophy			

#
# ---------- rules ------------------------------------------------------------
#

main:		build

build:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,,)				; \
	done


clear:
	for d in $(SUBDIRS) ; do					\
	  $(call env_launch,$${d}/Makefile,clear,)			; \
	done

	$(call env_purge)
