#CFLAGS=-g -Wall
CFLAGS=-O2 -Wall

all: buildctf detok ctcompare

buildctf: buildctf.o clexer.o
	$(CC) -o buildctf $(LDFLAGS) buildctf.o clexer.o

clexer.c: clexer.l
	lex clexer.l
	mv lex.yy.c clexer.c
clean:
	rm -f buildctf detok ctcompare *.o lex.yy.c clexer.c
