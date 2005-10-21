##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /data/kaneton/papers/lectures
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Fri Oct 21 09:11:47 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all clear proto dep

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		advanced-makefiles		\
					c-preprocessor			\
					development-environment		\
					distributed-operating-systems	\
					inline-assembly			\
					kernels				\
					requirements			

#
# ---------- rules ------------------------------------------------------------
#

all:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

	$(call purge)

proto:

dep:
