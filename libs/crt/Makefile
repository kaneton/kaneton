##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton/libs
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:57:24 2005   mycure
## last update   Sun Jun 19 20:57:42 2005   mycure
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

CRT			:=		crt.a

CRT_C			:=		crt.c				

CRT_O			:=		$(CRT_C:.c=.o)

CRT_INCLUDE		:=		

#
# ---------- rules ------------------------------------------------------------
#

$(CRT):			$(CRT_O)
	$(call archive,$(CRT),$(CRT_O),)

	$(call static-library,$(CRT),)

clear:
	$(call remove,$(CRT_O),)
	$(call remove,$(CRT),)

	$(call purge,)

proto:

dep:
	$(call remove,$(_MAKEFILE_MK_),)

	$(call dependencies,$(CRT_C),$(_MAKEFILE_MK_),)

#
# ---------- dependencies -----------------------------------------------------
#

include			./$(_MAKEFILE_MK_)
