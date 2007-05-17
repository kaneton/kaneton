include  Makefile.rules

PROJECTNAME = epitar
MAKE	    = make
RM = /bin/rm -f


all:
	cd src && $(MAKE)
test:
	cd check && $(MAKE)

re :	distclean all

clean :
	-$(RM) *~
	-$(RM) check/*.o
	-$(RM) src/*.o
	-$(RM) src/*~ \#* *core
	-$(RM) Makefile.rules
	-$(RM) $(PROJECTNAME)

distclean : clean
	-$(RM) src/*.so
	-$(RM) src/*.a
	-$(RM) $(PROJECTNAME)



dist :
	cd ../ && tar cvjf akadir_a-tar.tar.bz2 akadir_a-tar
