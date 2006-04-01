##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/mycure/kaneton/libs/libia32/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 13:40:52 2005]
## updated       julien quintard   [sat apr  1 02:24:13 2006]
##

#
# ---------- dependencies -----------------------------------------------------
#

include			../../env/.env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		clear proto dep libs

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		pmode				\
					paging				\
					interrupt			\
					time				#\
#					#tss

PMODE_LO		:=		pmode/pmode.lo
PAGING_LO		:=		paging/paging.lo
INTERRUPT_LO		:=		interrupt/interrupt.lo
TIME_LO			:=		time/time.lo
TSS_LO			:=		#tss/tss.lo

LIBIA32_INCLUDE		:=		$(_LIBIA32_INCLUDE_DIR_)/libia32.h

#
# ---------- rules ------------------------------------------------------------
#

$(_LIBIA32_A_):		libs
	$(call remove,$(_LIBIA32_A_),)

	$(call static-library,$(_LIBIA32_A_),$(PMODE_LO) $(PAGING_LO)	\
				$(INTERRUPT_LO) $(TIME_LO) $(TSS_LO),)

libs:
	$(call makefile,$(SUBDIRS),)

clear:
	$(call makefile,$(SUBDIRS),clear)

	$(call remove,$(LIBIA32_O),)

	$(call remove,$(_LIBIA32_A_),)

	$(call purge,)

proto:
	$(call makefile,$(SUBDIRS),proto)

	$(call prototypes,$(LIBIA32_INCLUDE),)

dep:
	$(call makefile,$(SUBDIRS),dep)
