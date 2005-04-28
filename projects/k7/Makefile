##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /root/data/research/projects/svn/kaneton/projects/k7
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Tue Mar 29 04:09:00 2005   mycure
## last update   Sat Apr  2 01:11:14 2005   mycure
##

NAME		=		k7-subject

## intermidiate names

NAME_TEX	=		$(NAME).tex
NAME_DVI	=		$(NAME).dvi
NAME_PS		=		$(NAME).ps
NAME_PDF	=		$(NAME).pdf

NAME_AUX	=		$(NAME).aux
NAME_TOC	=		$(NAME).toc
NAME_LOG	=		$(NAME).log
NAME_LOF	=		$(NAME).lof

## binaries

LATEX		=		latex
DVIPS		=		dvips
PS2PDF		=		ps2pdf

EMACS		=		emacs -nw
XDVI		=		xdvi
XPDF		=		xpdf
GV		=		gv

RM		=		rm -f

all:		pdf

$(NAME_DVI):	$(NAME_TEX)
	@$(LATEX) $(NAME_TEX)

	@$(LATEX) $(NAME_TEX)
	@$(LATEX) $(NAME_TEX)

$(NAME_PS):	$(NAME_DVI)
	@$(DVIPS) -o $(NAME_PS) $(NAME_DVI)

$(NAME_PDF):	$(NAME_PS)
	@$(PS2PDF) $(NAME_PS)

dvi:		$(NAME_DVI)
ps:		$(NAME_PS)
pdf:		$(NAME_PDF)

clean:
	@$(RM) $(NAME_AUX) $(NAME_TOC) $(NAME_LOG) $(NAME_LOF)
	@$(RM) *~

distclean:	clean
	@$(RM) $(NAME_DVI) $(NAME_PS) $(NAME_PDF)

tex:
	@$(EMACS) $(NAME_TEX)

xdvi:		dvi
	@$(XDVI) $(NAME_DVI)

gv:		ps
	@$(GV) $(NAME_PS)

xpdf:		pdf
	@$(XPDF) $(NAME_PDF)
