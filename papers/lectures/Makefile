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
## last update   Thu Oct 20 23:18:06 2005   mycure
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
					development-tree		\
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
