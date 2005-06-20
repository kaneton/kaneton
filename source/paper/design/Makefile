##
## copyright quintard julien
## 
## spargo
## 
## Makefile
## 
## path          /home/mycure/kaneton/paper/design
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Tue Mar 29 13:16:14 2005   mycure
## last update   Mon Jun 20 13:42:05 2005   mycure
##

NAME		=		design

#
# ---------- intermidiate ------------------------------------------------------
#

NAME_TEX	=		$(NAME).tex
NAME_DVI	=		$(NAME).dvi
NAME_PS		=		$(NAME).ps
NAME_PDF	=		$(NAME).pdf

NAME_AUX	=		$(NAME).aux
NAME_TOC	=		$(NAME).toc
NAME_LOG	=		$(NAME).log
NAME_LOF	=		$(NAME).lof

#
# ---------- binaries ----------------------------------------------------------
#

LATEX		=		latex
DVIPS		=		dvips
PS2PDF		=		ps2pdf
PDFLATEX	=		pdflatex

EMACS		=		emacs -nw
XDVI		=		xdvi
XPDF		=		xpdf
GV		=		gv

RM		=		rm -f

#
# ---------- rules -------------------------------------------------------------
#

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

clear:
	@$(RM) $(NAME_AUX) $(NAME_TOC) $(NAME_LOG) $(NAME_LOF)
	@$(RM) *~

clean:		clear
	@$(RM) $(NAME_DVI) $(NAME_PS) $(NAME_PDF)

tex:
	@$(EMACS) $(NAME_TEX)

xdvi:		dvi
	@$(XDVI) $(NAME_DVI)

gv:		ps
	@$(GV) $(NAME_PS)

xpdf:		pdf
	@$(XPDF) $(NAME_PDF)
