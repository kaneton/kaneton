#!/bin/sh

#
# moulette for kaneton
#
# usage: moulette.sh Kn
#
# requires:
#  - check-students.tar.bz2	Tarball containing tests
#  - patch.sh			Script to run on each tarball
#

if [ ${#} -ne 1 ]; then
    echo "usage: ${0} Kn"
    exit 1
fi

echo "Cleaning..."
rm -Rf kaneton >/dev/null 2>&1

echo "Setting up environment..."
export KANETON_USER="check"

for i in `cat list`; do
    echo ""
    echo "Processing ${i}..."
    echo ""

    group=`echo ${i} |cut -d '-' -f 1`

    echo "Uncompressing..."
    tar xjf ${i}
    tar xjf check-students.tar.bz2

    cd kaneton

    if [ -x '../patch.sh' ]; then
	echo "Patching tarball..."
	../patch.sh
    fi

    echo
    echo "" | make init >../${group}.log 2>&1
    echo "" | make build >../${group}.log 2>&1
    echo "" | make install >>../${group}.log 2>&1

    if [ ${?} -eq 0 ]; then
	echo "Running tests..."

	cd check
	./check.py /dev/pts/3 ${group} ${1}
	cp ${group}-${1}.html ../..
	cp ${group}-${1}.moul ../..
	cd ..
	killall -9 qemu
    else
	echo "FAILED."
    fi

    cd ..

    echo "Cleaning..."
    rm -Rf kaneton >/dev/null 2>&1
done
