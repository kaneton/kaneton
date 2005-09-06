##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs/crt
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Tue Sep  6 11:03:48 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep

#
# ---------- variables --------------------------------------------------------
#

CRT_A			:=		crt.a

CRT_C			:=		crt.c				

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(CRT_A):		$(CRT_O)
	$(call dynamic-library,$(CRT_A),$(CRT_O),)

clear:
	$(call remove,$(CRT_O),)

	$(call remove,$(CRT_A),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CRT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

-include		./$(_MAKEFILE_MK_)
