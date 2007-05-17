#!/bin/sh

rm -Rf result
mkdir result

L=`ls -1 ctf/*.ctf`
COUNT=`ls -1 ctf/*.ctf |wc -l`
C=0

for A in $L; do
    J=$((COUNT - C))
    for B in `ls -1 ctf/*.ctf | tail -n $J`; do
        if [ "$A" != "$B" ]; then
            E=`basename $A .ctf`
            F=`basename $B .ctf`
            echo "comparing $E with $F..."
#            ctcompare/ctcompare -n 40 $A $B |grep -v '.h:' | sort -rn |sed -r 's/^([0-9]+)  (.*)  (.*)$/---\| \1\n   \| \2\n   \| \3\n/g' > result/${E}-${F}
	    content=`ctcompare/ctcompare -d -n 200 $A $B`
	    echo "${content}" > result/${E}-${F}
        fi
    done
    C=$(($C + 1))
done
