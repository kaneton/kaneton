##
## copyright quintard julien
## 
## kaneton
## 
## kaneton.mk.mac
## 
## path          /home/mycure/kaneton/source
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Tue May 24 13:20:18 2005   mycure
##

## c compiler

CC			=		i386-rtems-gcc

## binaries

_MAKE_			=		make
_RM_			=		rm -f
_PURGE_			=		$(RM) *~ .*~ \#* .\#*
_AR_			=		i386-rtems-ar cq
_RANLIB_		=		i386-rtems-ranlib
_CD_			=		cd
_CP_			=		cp
_LD_			=		i386-rtems-ld
_NASM_			=		nasm
_CAT_			=		cat
_MOUNT_			=		mount
_UMOUNT_		=		umount
_HDIUTIL_		=		hdiutil

#
# compiler and linker flags
#

CFLAGS			=		-nostdinc -nostdlib		\
					-O0 $(_INCLUDES_) -g
LDFLAGS			=		$(_INCLUDES_)
