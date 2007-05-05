all:
	cd src/ ; make -f Makefile
check: .PHONY
	cd check/ ; ./test.sh
clean:
	cd src/ ; make Makefile clean
	cd check/ ; make Makefile clean
distclean:
	cd src/ ; make Makefile distclean
	cd check/ ; make Makefile distclean
#	@rm -rf Makefile.rules
re:
	cd src/ ; make Makefile re
	cd check/ ; make Makefile re
dist:
	cd .. && tar cvjf amsell_y-tar.tar.bz2 amsell_y-tar