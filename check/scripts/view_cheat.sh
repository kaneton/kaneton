#!/bin/sh

tmp=`mktemp`

rm -Rf kaneton files_1 files_2

for i in result/*; do
    cat -e ${i} | grep -v '^$$' | grep -v -- '-----' | tr -d ' ' > ${tmp}
    while read first; do
	read second

	i2=`basename ${i}`
	login_1=`echo ${i2} | cut -d'-' -f1`
	login_2=`echo ${i2} | cut -d'-' -f2`

	file_1=`echo ${first} | cut -d':' -f 1`
	file_1=`basename ${file_1}`
	line_1=`echo ${first} | cut -d':' -f 2  | tr -d '$'`

	file_2=`echo ${second} | cut -d':' -f 1`
	file_2=`basename ${file_2}`
	line_2=`echo ${second} | cut -d':' -f 2  | tr -d '$'`

	echo "${login_1} @ ${file_1}:${line_1} - ${login_2} @ ${file_2}:${line_2}"

	tar xjf ${login_1}-K2.tar.bz2

	mkdir files_1

	cp kaneton/libs/libia32/interrupt/*.c files_1
	cp kaneton/libs/libia32/pmode/idt.c files_1
	cp kaneton/libs/libia32/include/interrupt/*.h files_1
	cp kaneton/libs/libia32/include/pmode/idt.h files_1
	cp kaneton/libs/libia32/time/timer.c files_1
	cp kaneton/libs/libia32/include/time/pit.h files_1
	cp kaneton/kaneton/core/time/*.c files_1
	cp kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/timer.c files_1
	cp kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/event.c files_1

	rm -Rf kaneton

	tar xjf ${login_2}-K2.tar.bz2

	mkdir files_2

	cp kaneton/libs/libia32/interrupt/*.c files_2
	cp kaneton/libs/libia32/pmode/idt.c files_2
	cp kaneton/libs/libia32/include/interrupt/*.h files_2
	cp kaneton/libs/libia32/include/pmode/idt.h files_2
	cp kaneton/libs/libia32/time/timer.c files_2
	cp kaneton/libs/libia32/include/time/pit.h files_2
	cp kaneton/kaneton/core/time/*.c files_2
	cp kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/timer.c files_2
	cp kaneton/kaneton/core/arch/ibm-pc.ia32-virtual/event.c files_2

	rm -Rf kaneton

	emacs files_1/${file_1} files_2/${file_2}

	rm -Rf files_1 files_2

	xmessage "Goto next!" >/dev/null 2>&1
    done < ${tmp}
done
