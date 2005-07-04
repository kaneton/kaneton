##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:04:24 2005   mycure
## last update   Mon Jul  4 12:11:03 2005   mycure
##

#
# ---------- dependencies -----------------------------------------------------
#

include			./.kaneton.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		all init clean kaneton clear purge proto		\
		dep build install check info dist

#
# ---------- variables --------------------------------------------------------
#

SUBDIRS			:=		libs core services

#
# ---------- default rule -----------------------------------------------------
#

all:			kaneton

#
# ---------- environment ------------------------------------------------------
#

init:
	@cd env								; \
	./init								; \
	cd ..

clean:
	@cd env								; \
	./clean								; \
	cd ..

#
# ---------- development ------------------------------------------------------
#

kaneton:
	$(call make,$(SUBDIRS),)

clear:
	$(call make,$(SUBDIRS),clear)

purge:
	$(call purge,)

#
# ---------- prototypes -------------------------------------------------------
#

proto:
	$(call make,$(SUBDIRS),proto)

#
# ---------- dependencies -----------------------------------------------------
#

dep:
	$(call make,$(SUBDIRS),dep)

#
# ---------- boot -------------------------------------------------------------
#

build:
	@cd utils							; \
	./$(_MULTIBOOTLOADER_) build					; \
	cd ..

install:
	@cd utils/							; \
	./$(_MULTIBOOTLOADER_) install					; \
	cd ..

#
# ---------- check ------------------------------------------------------------
#
# XXX
#

check:

#
# ---------- information ------------------------------------------------------
#

info:
	@echo ""
	@echo -e '\E[;34m'"\033[1m---\033[0m \033[1mkaneton\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mauthors:\033[0m"
	@echo ""
	@echo -e "     \033[1mquintard julien - quinta_j@epita.fr\033[0m"
	@echo ""
	@echo -e "     \033[1mbillaud jean-pascal - billau_j@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1massistants:\033[0m"
	@echo ""
	@echo -e "     \033[1mrenaud lienhart - lienha_r@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mcontributors:\033[0m"
	@echo ""
	@echo -e "     \033[1mchristophe eymard - eymard_c@epita.fr\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mnews-group:\033[0m"
	@echo ""
	@echo -e "     \033[1mepita.cours.kaneton\033[0m"
	@echo ""
	@echo -e '\E[;34m'"\033[1m#\033[0m \033[1mmailing-list:\033[0m"
	@echo ""
	@echo -e "     \033[1mkaneton@lse.epita.fr\033[0m"
	@echo ""

#
# ---------- distribution -----------------------------------------------------
#
# XXX $(call dist)
#

dist:
	@echo ""
	@echo -e '\E[;34m'"\033[1m---\033[0m \033[1mbuilding kaneton...\033[0m"

	@tar czf /tmp/kaneton-$(_VERSION_).tar.gz .
	@mv /tmp/kaneton-$(_VERSION_).tar.gz .

	@echo ""
	@echo -e '\E[;34m'"\033[1m---\033[0m \033[1mkaneton built\033[0m"
	@echo ""
