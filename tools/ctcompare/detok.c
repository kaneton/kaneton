/* detok: Detokenise a C token file into something a programmer can read.
 * Copyright (c) Warren Toomey, under the GPL license.
 *
 * $Revision: 1.1.1.1 $
 */
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

char *tokstring[] = { "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",
     "ERR ", "ERR ", "ERR ", "\n", "ERR ", "ERR ", ">>= ", "ERR ", "ERR ",
     "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",
     "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "/= ", "! ",
     "\"string\" ","->", "++ ", "% ", "& ","'c' ","( ",") ","* ","+ ",", ","- ",
     ". ", "/ ", "-- ", "&& ", "|| ", "++= ", "%= ", "-= ", "&= ", "*= ", "|= ",
     "NUM", ": ", "; ", "< ", "= ", "> ", "? ", "!= ", "<= ", "case ", "char ",
     "const ", "continue ", "default ", "do ", "...", "double ", "else ",
     "enum ", "extern ", "float ", "for ", "goto ", "if ", "int ", "long ",
     "register ", "return", "short ", "signed ", "sizeof ","static ", "struct ",
     "switch ", "[ ", "\\", "] ", "^ ", "id", "label: ", "typedef ", "union ",
     "unsigned ", "void ", "volatile ", "while ", "#define ", "#elif ",
     "#else ", "#endif ", "#error ", "#ifdef ", "#if ", "#ifndef ", "#include ",
     "#line ", "#pragma ", "#undef ", "#warning ", "~= ", "== ", "break ",">= ",
     "<< ", ">> ", "<<= ", "{ ", "| ", "} ", "~ ", "ERR " };

int main(int argc, char *argv[])
{
  unsigned int ch, linenum=0;
  int val;
  FILE *zin;

  if (argc!=2) { fprintf(stderr, "Usage: detok cft_file\n"); exit(1); }

  zin= fopen(argv[1], "r");
  if (zin==NULL) { fprintf(stderr, "Can't open %s\n", argv[1]); exit(1); }

  /* Check the ctf header */
  if ((fgetc(zin)!='c') || (fgetc(zin)!='t') || (fgetc(zin)!='f') ||
	(fgetc(zin)!=0x1) || (fgetc(zin)!=0x2)) {
    fprintf(stderr, "Input file %s is not a ctf file\n", argv[1]); exit(1);
  }

  /* Now read the tokens, including the filenames etc. */
  while ((ch=fgetc(zin))!=EOF) {
    ch &= 127;
    if (ch==FILENAME) {
      printf("\n\n");
      while ((ch=fgetc(zin))!=EOF) {
        if (ch=='\0') break;
	fputc(ch,stdout);
      }
      linenum=1;
      printf(":\n%5d:   ", linenum); continue;
    }
    fputs(tokstring[ch], stdout);

    /* Deal with the id numbers for certain tokens */
    switch (ch) {
      case IDENTIFIER:
      case INTVAL:
	val= (fgetc(zin)*256) + fgetc(zin);
	printf("%d ", val); break;
    }
    if (ch==LINE) { linenum++; printf("%5d:   ", linenum); }
  }
  exit(0);
}
