##
## kaneton
## 
## Makefile
## 
## path          /home/mycure/data/research/projects/kaneton/projects/k4
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Sun Nov 21 14:13:54 2004   mycure
## last update   Tue May 17 15:53:17 2005   mycure
##

NAME		=		k4-subject

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
PDFLATEX	=		pdflatex

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

$(NAME_PDF):	$(NAME_TEX)
	@$(PDFLATEX) $(NAME_TEX)

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
