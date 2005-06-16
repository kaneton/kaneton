##
## copyright quintard julien
## 
## kaneton
## 
## .common.mk
## 
## path          /home/mycure/kaneton
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:08:31 2005   mycure
## last update   Thu Jun 16 16:28:56 2005   mycure
##

#
# kaneton directories
#

_CORE_DIR_		=		$(_SRC_DIR_)/core
_CORE_BOOTSTRAO_DIR_	=		$(_SRC_DIR_)/core/bootstrap
_CORE_BOOTLOADER_DIR_	=		$(_SRC_DIR_)/core/bootloader
_CORE_INCLUDE_DIR_	=		$(_SRC_DIR_)/core/include
_CORE_KANETON_DIR_	=		$(_SRC_DIR_)/core/kaneton
_MACHDEP_DIR_		=		$(_CORE_KANETON_DIR_)/arch/machdep
_MACHDEP_INCLUDE_DIR_	=		$(_CORE_INCLUDE_DIR_)/arch/machdep
_DIST_DIR_		=		$(_SRC_DIR_)/dist
_ENV_DIR_		=		$(_SRC_DIR_)/env
_LDS_DIR_		=		$(_SRC_DIR_)/lds/arch/machdep
_LIB_DIR_		=		$(_SRC_DIR_)/libs
_LIB_INCLUDE_DIR_	=		$(_LIB_DIR_)
_INCLUDES_		=		-I$(_CORE_INCLUDE_DIR_)		\
					-I$(_LIB_INCLUDE_DIR_)
_CONF_DIR_		=		$(_SRC_DIR_)/conf
_UTILS_DIR_		=		$(_SRC_DIR_)/utils

#
# kaneton utils
#

_PROTO_			=		$(_UTILS_DIR_)/mkp.pl
_GRUB_			=		$(_UTILS_DIR_)/grub.sh
_LILO_			=		$(_UTILS_DIR_)/lilo.sh

#
# kaneton binaries
#

_BOOTSTRAP_		=		$(_CORE_BOOTSTRAP_DIR_)/bootstrap
_BOOTLOADER_		=		$(_CORE_BOOTLOADER_DIR_)/bootloader
_KANETON_		=		$(_CORE_KANETON_DIR_)/kaneton

#
# kaneton libraries
#

_LIBC_A_		=		$(_LIB_DIR_)/libc.a
_LIBC_H_		=		$(_LIB_DIR_)/libc.h

_CRT_A_			=		$(_LIB_DIR_)/crt.a

_MACHDEP_A_		=		$(_MACHDEP_DIR_)/machdep.a
_SEGMENT_A_		=		$(_CORE_KANETON_DIR_)/segment/segment.a
_AS_A_			=		$(_CORE_KANETON_DIR_)/as/as.a
