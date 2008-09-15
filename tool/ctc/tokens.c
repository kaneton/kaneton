/*
 * tokens.c: Functions dealing with the token stream in a CTF file.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.23 $
 */

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tokens.h"

/*
 * Open the named CTF file for reading, checking the header as well. Returns
 * the FILE handle to the open file.
 */
ctfhandle *ctfopen(char *name)
{
  ctfhandle *ctf;
  struct stat sb;

  if ((ctf = malloc(sizeof(*ctf))) == NULL) {
    fprintf(stderr, "ctfopen: Out of memory!\n");
    exit(1);
  }

  if ((ctf->fd = open(name, O_RDONLY)) == -1) {
    fprintf(stderr, "Can't open %s\n", name);
    exit(1);
  }

  fstat(ctf->fd, &sb);
  ctf->start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, ctf->fd, 0);
  if (ctf->start == NULL) {
    fprintf(stderr, "Can't mmap %s\n", name);
    exit(1);
  }

  /* Initialise ctf */
  ctf->cursor = ctf->start;
  ctf->end = ctf->start + sb.st_size;

  /* Check the ctf header */
  if ((*(ctf->cursor++) != 'c') || (*(ctf->cursor++) != 't') ||
      (*(ctf->cursor++) != 'f') || (*(ctf->cursor++) != '2') ||
      (*(ctf->cursor++) != '.') || (*(ctf->cursor++) != '1')) {
    fprintf(stderr, "Input file %s is not a ctf file\n", name);
    exit(1);
  }
  return (ctf);
}

/* Close an open ctf handle. Returns 0 if OK, -1 on error. */
int ctfclose(ctfhandle *ctf)
{
  if (ctf==NULL) return(-1);
  if (munmap(ctf->start, ctf->end - ctf->start) < 0) return(-1);
  return(close(ctf->fd));
}


/*
 * Given a ctf handle and an offset, return the next token from the file. The
 * offset is updated to point at the next token. Any filename associated with
 * a FILENAME token is returned, and the id param is used to return the
 * timestamp. Any id-value associated with the the token is returned, or 0 if
 * there is no value. On any error, 0 is returned. The space for the filename
 * is malloc'd here.
 */
unsigned int get_token(ctfhandle * ctf, uint32_t * offset, uint32_t * id, char **name)
{
  unsigned int ch, token;
  uint32_t idvalue;

  /* Give up if ctf, ctf->start or offset don't exist */
  if ((ctf == NULL) || (ctf->start == NULL) || (offset == NULL))
    return (0);

  /* EOF handling. */
  ctf->cursor = ctf->start + *offset;
  if (ctf->cursor >= ctf->end)
    return (0);

  /* Get the token */
  token = *(ctf->cursor++);

  /* Set a default id value */
  if (id)
    *id = 0;

  /*
   * Deal with filenames and the id numbers for certain tokens
   */
  switch (token) {
  case STRINGLIT:
  case CHARCONST:
  case LABEL:
  case IDENTIFIER:
  case INTVAL:
    /* Read in 2 bytes to get the id value */
    idvalue = *(ctf->cursor++) << 8;
    idvalue += *(ctf->cursor++);
    if (id)
      *id = idvalue;
    break;

  case FILENAME:
    /* Read in 4 bytes to get the timestamp value */
    idvalue = *(ctf->cursor++) << 24;
    idvalue += *(ctf->cursor++) << 16;
    idvalue += *(ctf->cursor++) << 8;
    idvalue += *(ctf->cursor++);
    if (id)
      *id = idvalue;

    /* Pass back the pointer to the filename, and move the cursor up */
    if (name)
      *name = (char *)ctf->cursor;
    while (((ch = *(ctf->cursor++)) != '\0') &&
	   (ctf->cursor < ctf->end));
  }

  *offset = ctf->cursor - ctf->start;
  return (token);
}

char *tokstring[] = {
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 0 */
  "ERR ", "ERR ", "\n", "ERR ", "ERR ", ">>= ", "ERR ", "ERR ",					/* 8 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 16 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 24 */
  "/= ", "! ", "\"string\"", "->", "++ ", "% ", "& ", "'c'",					/* 32 */
  "( ", ") ", "* ", "+ ", ", ", "- ", ". ", "/ ",						/* 40 */
  "-- ", "&& ", "|| ", "++= ", "%= ", "-= ", "&= ", "*= ",					/* 48 */
  "|= ", "NUM", ": ", "; ", "< ", "= ", "> ", "? ",						/* 56 */
  "!= ", "<= ", "case ", "char ", "const ", "continue ", "default ", "do ",			/* 64 */
  "...", "double ", "else ", "enum ", "extern ", "float ", "for ", "goto ",			/* 72 */
  "if ", "int ", "long ", "register ", "return", "short ", "signed ", "sizeof ",		/* 80 */
  "static ", "struct ", "switch ", "[ ", "\\", "] ", "^ ", "id",				/* 88 */
  "label: ", "typedef ", "union ", "unsigned ", "void ", "volatile ", "while ", "#define ",	/* 96 */
  "#elif ", "#else ", "#endif ", "#error ", "#ifdef ", "#if ", "#ifndef ", "#include ",		/* 104 */
  "#line ", "#pragma ", "#undef ", "#warning ", "~= ", "== ", "break ", ">= ",			/* 112 */
  "<< ", ">> ", "<<= ", "{ ", "| ", "} ", "~ ", "abstract ",					/* 120 */
  "boolean ", "byte ", "extends ", "final ","finally ","implements ", "import ", "instanceof ", /* 128 */
  "interface ", "native ", "new ", "null ", "package ", "private ", "protected ", "public ",	/* 136 */
  "strictfp ", "super ", "synchronized ", "this ", "throw ", "throws ", "transient ", "try",	/* 144 */
  ">>> ", ">>>= ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 152 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 160 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 168 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 176 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 184 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 192 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 200 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 208 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 216 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 224 */
  "ERR ", "ERR ", "[ctc-gate] ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 232 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",				/* 240 */
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR "				/* 248 */
};

char *tok2str(unsigned int ch)
{
  return (tokstring[ch]);
}

void print_token(FILE* output, unsigned int ch, uint32_t linenum, uint32_t id, char *filename)
{
  time_t time;
  switch (ch) {
  case FILENAME:
    time = id;
    fprintf(output, "\n\n%s:\t%s\n%5d:   ", filename, ctime(&time), linenum);
    break;
  case LINE:
    fprintf(output, "\n%5d:   ", linenum);
    break;
  case IDENTIFIER:
  case INTVAL:
    fprintf(output, "%s%d ", tokstring[ch], id);
    break;
  case STRINGLIT:
    fprintf(output, "\"str%d\" ", id);
    break;
  case LABEL:
    fprintf(output, "L%d: ", id);
    break;
  case CHARCONST:
    fprintf(output, "'c%d' ", id);
    break;
  default:
    fputs(tokstring[ch], output);
  }
}
