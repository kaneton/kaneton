##
## copyright quintard julien
## 
## kaneton
## 
## kaneton.mk.linux
## 
## path          /home/mycure/kaneton/source
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Fri Feb 11 02:06:42 2005   mycure
## last update   Tue May 24 13:20:13 2005   mycure
##

#
# c compiler
#

CC			=		gcc

#
# binaries
#

_MAKE_			=		make
_RM_			=		rm -f
_PURGE_			=		$(RM) *~ .*~ \#* .\#*
_AR_			=		ar
_RANLIB_		=		ranlib
_CD_			=		cd
_CP_			=		cp
_LD_			=		ld
_NASM_			=		nasm
_CAT_			=		cat
_MCOPY_			=		mcopy -no
_MDEL_			=		mdel
_LN_			=		ln -s -f

#
# compiler and linker flags
#

CFLAGS			=		-nostdinc -nostdlib		\
					-O0 $(_INCLUDES_) -g
LDFLAGS			=		$(_INCLUDES_)
