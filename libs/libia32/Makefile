##
## licence       kaneton licence
##
## project       kaneton
##
## file          /home/buckman/kaneton/libs/libia32/Makefile
##
## created       matthieu bucchianeri   [tue dec 20 13:40:52 2005]
## updated       matthieu bucchianeri   [tue jul 25 16:00:37 2006]
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
					time				\
					task				\
					apic

PMODE_LO		:=		pmode/pmode.lo
PAGING_LO		:=		paging/paging.lo
INTERRUPT_LO		:=		interrupt/interrupt.lo
TIME_LO			:=		time/time.lo
TASK_LO			:=		task/task.lo
APIC_LO			:=		apic/apic.lo

LIBIA32_INCLUDE		:=		$(_LIBIA32_INCLUDE_DIR_)/libia32.h

#
# ---------- rules ------------------------------------------------------------
#

$(_LIBIA32_A_):		libs
	$(call remove,$(_LIBIA32_A_),)

	$(call static-library,$(_LIBIA32_A_),$(PMODE_LO) $(PAGING_LO)	\
				$(INTERRUPT_LO) $(TIME_LO) $(TASK_LO)	\
				$(APIC_LO),)

libs:
	$(call launch,Makefile,$(SUBDIRS),,)

clear:
	$(call launch,Makefile,$(SUBDIRS),clear,)

	$(call remove,$(LIBIA32_O),)

	$(call remove,$(_LIBIA32_A_),)

	$(call purge,)

proto:
	$(call launch,Makefile,$(SUBDIRS),proto,)

	$(call prototypes,$(LIBIA32_INCLUDE),)

dep:
	$(call launch,Makefile,$(SUBDIRS),dep,)
