##
## Makefile for epitar in /u/a1/ah-tec_m/ah-tec_m-tar/src
## 
## Made by mathieu ah-tec
## Login   <ah-tec_m@epita.fr>
## 
## Started on  Sat Jul  8 17:35:23 2006 mathieu ah-tec
## Last update Sat Jul  8 17:35:25 2006 mathieu ah-tec
##

CC	= /u/prof/acu/pub/NetBSD/bin/gcc
CFLAGS	= -Wall -W -Werror -ansi -pedantic
SOURCE	= compress.c extract.c header_copy.c main.c
OBJ	= ${SOURCE:.c=.o}

all: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o epitar

%.o : %.c
	${CC} ${CFLAGS} $< -c

clean:
	rm -f ${OBJ} *~

distclean: clean
	rm -f epitar
