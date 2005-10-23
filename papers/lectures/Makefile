##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/papers/lectures
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Sun Oct 23 23:12:58 2005   mycure
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
					prerequisites

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
