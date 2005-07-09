##
## kaneton
## 
## makefile.mk
## 
## path          /home/mycure/data/research/projects/kaneton/courses/advanced-makefiles
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Sun Nov 21 14:13:54 2004   mycure
## last update   Tue Jul  5 13:24:35 2005   mycure
##

#
# intermidiate names
#

_FILE_TEX_		:=		$(_FILE_).tex
_FILE_DVI_		:=		$(_FILE_).dvi
_FILE_PS_		:=		$(_FILE_).ps
_FILE_PDF_		:=		$(_FILE_).pdf

_FILE_AUX_		:=		$(_FILE_).aux
_FILE_TOC_		:=		$(_FILE_).toc
_FILE_LOG_		:=		$(_FILE_).log
_FILE_LOF_		:=		$(_FILE_).lof
_FILE_NAV_		:=		$(_FILE_).nav
_FILE_OUT_		:=		$(_FILE_).out
_FILE_SNM_		:=		$(_FILE_).snm

#
# binaries
#

_MAKE_DVI_		:=		latex
_MAKE_PS_		:=		dvips -o $(_FILE_PS_)
_MAKE_PDF_		:=		pdflatex

_EDIT_			:=		emacs -nw
_VIEW_DVI_		:=		xdvi
_VIEW_PS_		:=		gv
_VIEW_PDF_		:=		xpdf

_RM_			:=		rm -f

#
# rules
#

all:			pdf

#
# files rules
#

$(_FILE_DVI_):		$(_FILE_TEX_)
	@$(_MAKE_DVI_) $(_FILE_TEX_)

$(_FILE_PS_):		$(_FILE_DVI_)
	@$(_MAKE_PS_) $(_FILE_DVI_)

$(_FILE_PDF_):		$(_FILE_TEX_)
	@$(_MAKE_PDF_) $(_FILE_TEX_)
	@$(_MAKE_PDF_) $(_FILE_TEX_)

dvi:			$(_FILE_DVI_)

ps:			$(_FILE_PS_)

pdf:			$(_FILE_PDF_)

#
# clean rules
#

clear:
	@$(_RM_) $(_FILE_AUX_) $(_FILE_TOC_) $(_FILE_LOG_)		\
	         $(_FILE_LOF_) $(_FILE_NAV_) $(_FILE_OUT_)		\
	         $(_FILE_SNM_) *~ \#*

clean:			clear
	@$(_RM_) $(_FILE_DVI_) $(_FILE_PS_) $(_FILE_PDF_)

#
# edit
#

edit:			$(_FILE_TEX_)
	@$(_EDIT_) $(_FILE_TEX_)

#
# view
#

vdvi:			$(_FILE_DVI_)
	@$(_VIEW_DVI_) $(_FILE_DVI_)

vps:			$(_FILE_PS_)
	@$(_VIEW_PS_) $(_FILE_PS_)

vpdf:			$(_FILE_PDF_)
	@$(_VIEW_PDF_) $(_FILE_PDF_)
