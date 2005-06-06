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
## last update   Wed Jun  1 11:12:24 2005   mycure
##

all:			kaneton

#
# dependency
#

.common.mk:
	@if [ ! -f .common.mk ] ; then					\
	make init							; \
	fi

#
# environment
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
# development
#

SUBDIRS			=	libs core services

kaneton:		.common.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make								; \
	cd ..								; \
	done

distclean:		.common.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make distclean							; \
	cd ..								; \
	done

	@rm -f *~ .*~ \#~ .\#~

proto:			.common.mk
	@for i in $(SUBDIRS); do					\
	cd $$i								; \
	make proto							; \
	cd ..								; \
	done

#
# distribution
#

CONF			=		conf/$(USER)/$(USER).conf
MODULES			=		conf/$(USER)/modules.conf

BOOTLOADER		=		`cat $(CONF)			| \
					 grep -E "^BOOTLOADER = .*$$"	| \
					 cut -b 14-`

boot-init:		.common.mk
	@case "$(BOOTLOADER)" in					\
	  "grub")							\
	    cd utils							; \
	    ./grub.sh init						; \
	    cd ..							; \
	    ;;								\
	  "lilo")							\
	    cd utils							; \
	    ./lilo.sh init						; \
	    cd ..							; \
	    ;;								\
	  *)								\
	    echo "[!] unknown bootloader"				; \
	    echo ""							; \
	    echo "[!] please check your BOOTLOADER variable in $(CONF)"	; \
	    echo ""							; \
	    echo "[!] current supported bootloader are:"		; \
	    echo "[!]      grub"					; \
	    echo "[!]      lilo"					; \
	    ;;								\
	esac

boot-install:		.common.mk
	@case "$(BOOTLOADER)" in					\
	  "grub")							\
	    cd utils							; \
	    ./grub.sh boot						; \
	    cd ..							; \
	    ;;								\
	  "lilo")							\
	    cd utils							; \
	    ./lilo.sh boot						; \
	    cd ..							; \
	    ;;								\
	  *)								\
	    echo "[!] unknown bootloader"				; \
	    echo ""							; \
	    echo "[!] please check your BOOTLOADER variable in $(CONF)"	; \
	    echo ""							; \
	    echo "[!] current supported bootloader are:"		; \
	    echo "[!]      grub"					; \
	    echo "[!]      lilo"					; \
	    ;;								\
	esac
