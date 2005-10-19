##
## copyright quintard julien
## 
## kaneton
## 
## Makefile
## 
## path          /root/data/research/projects/svn/kaneton/projects
## 
## made by mycure
##         quintard julien   [quinta_j@epita.fr]
## 
## started on    Tue Mar 29 01:44:40 2005   mycure
## last update   Tue Mar 29 02:36:34 2005   mycure
##

SUBDIRS = kaneton k0 k1 k2 k3 k4 k5 k6 k7 k8 k9 k10 kn

all:
	@for i in $(SUBDIRS); do					\
	cd $$i;								\
	make;								\
	cd ..;								\
	done

distclean:
	@for i in $(SUBDIRS); do					\
	cd $$i;								\
	make distclean;							\
	cd ..;								\
	done;
	@rm -f *~ .*~ \#~ .\#~

dvi:
	@for i in $(SUBDIRS); do					\
	cd $$i;								\
	make dvi;							\
	cd ..;								\
	done;

ps:
	@for i in $(SUBDIRS); do					\
	cd $$i;								\
	make ps;							\
	cd ..;								\
	done;

pdf:
	@for i in $(SUBDIRS); do					\
	cd $$i;								\
	make pdf;							\
	cd ..;								\
	done;
