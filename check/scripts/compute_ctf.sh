#!/bin/sh

rm -Rf ctf
mkdir ctf

for i in `cat list`; do
    echo ""
    echo "Processing ${i}..."
    echo ""

    group=`echo ${i} |cut -d '-' -f 1`

    echo "Uncompressing..."
    tar xjf ${i}

    mkdir files

    cp kaneton/kaneton/core/segment/*.c files
    cp kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/region.c files

    ctcompare/buildctf files ctf/${group}.ctf

    rm -Rf kaneton files
done
