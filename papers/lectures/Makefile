##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/papers
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:59:20 2005   mycure
## last update   Fri Sep  9 18:58:14 2005   mycure
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
					distributed-operating-systems	\
					inline-assembly			\
					kernels				

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
