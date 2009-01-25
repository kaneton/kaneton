#
# ---------- header -----------------------------------------------------------
#
# project       kaneton
#
# license       kaneton
#
# file          /home/mycure/kaneton/test/Makefile
#
# created       julien quintard   [fri jun 29 11:19:40 2007]
# updated       julien quintard   [fri jun 29 11:21:03 2007]
#

#
# ---------- dependencies -----------------------------------------------------
#

include			../environment/env.mk

#
# ---------- directives -------------------------------------------------------
#

.PHONY:		main build run clear prototypes headers

#
# ---------- variables --------------------------------------------------------
#

# Temporary, don't know where to put this
# machine dependant .env.conf
_COMPILE_PY_	:=	compile.py
_TEST_LO_	:=	test.lo

#
# ---------- rules ------------------------------------------------------------
#

main:

clear:
	$(call purge,)

prototypes:

headers:

tests.tar.bz2:
	$(call env_launch,$(_TEST_TARBALL_SCRIPT_),$(_TESTSUITE_),)

kaneton.tar.bz2: $(_IMAGE_)
	$(call env_launch,$(_KANETON_TARBALL_SCRIPT_),,)

$(_USER_)-test.tar.bz2: tests.tar.bz2 kaneton.tar.bz2
	-cp $(_TESTSUITE_) $$(basename $(_TESTSUITE_)) 2>/dev/null
	tar cjf main.tar.bz2 tests.tar.bz2 kaneton.tar.bz2
	tar cjf $(_USER_)-test.tar.bz2		\
		main.tar.bz2			\
		$$(basename $(_TESTSUITE_))	\
		tools
	rm main.tar.bz2
	

custom: $(_USER_)-test.tar.bz2
	md5 $(_USER_)-test.tar.bz2
	chmod 600 id_rsa_kaneton-test
	scp -i id_rsa_kaneton-test $(_USER_)-test.tar.bz2 $(_TEST_HOST_):custom-tarballs
	rm $(_USER_)-test.tar.bz2

test:
	tools/moulette.rb test.yml  res.yml
