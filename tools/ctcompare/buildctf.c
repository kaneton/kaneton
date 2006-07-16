/* buildctf: Build a C token file given a directory containing C code.
 * Copyright (c) Warren Toomey, under the GPL license.
 *
 * $Revision: 1.1.1.1 $
 */

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fts.h>
#include <sys/stat.h>
#include "tokens.h"

extern void reset_comments(void);
extern void initialise_lists(void);
extern int yylex(void);
extern void yyrestart(FILE *input_file );

int ntokens = 0;

#define MIN_PRINTABLE       0.9

FILE *zin, *zout;

/*
 * Is the specified file eligible to be compared?
 * This code comes from Eric Raymond's comparator 1.0 source code,
 * which is (c) Eric Raymond under the GPL.
 */
FILE * eligible(const char *file)
{
  /* fast check for the most common suffixes */
#define endswith(suff)	!strcmp(suff, file + strlen(file) - strlen(suff))
  if (endswith(".o") || endswith("~"))
    return (NULL);
  else if (strstr(file, "CVS") || strstr(file, "RCS") || strstr(file, "SCCS"))
    return (NULL);

  zin = fopen(file, "r");
  if (zin == NULL) return (NULL);
  if (endswith(".c") || endswith(".h") ||
      endswith(".C") || endswith(".H") ||
      endswith(".C++") || endswith(".Cpp"))
    return (zin);

  fclose(zin); return (NULL);
#undef endswith
}

void output_filename(char *name)
{
 /* Output the FILENAME token, the filename and a NUL */
 fputc(FILENAME, zout);
 fputs(name, zout);
 fputc('\0', zout);
}


int main(int argc, char *argv[])
{
  struct stat sb;
  int err;
  char *dirlist[2];
  FTS *ftsptr;
  FTSENT *entry;

  if (argc != 3) {
    fprintf(stderr, "Usage: buildctf directory outputfile\n");
    exit(1);
  }

  /* Check that the directory exists and is a directory */
  err = stat(argv[1], &sb);
  if (err < 0) {
    fprintf(stderr, "Err: %s doesn't exist\n", argv[1]); exit(1);
  }
  if (!S_ISDIR(sb.st_mode)) {
    fprintf(stderr, "Err: %s is not a directory\n", argv[1]); exit(1);
  }

  /* Open up the output file */
  zout = fopen(argv[2], "w");
  if (zout == NULL) {
    fprintf(stderr, "Can't open %s for writing", argv[2]); exit(1);
  }

  /* Output the ctf header */
  fputs("ctf", zout); fputc(0x1, zout); fputc(0x2, zout);
  initialise_lists();

  /* Now search for files to tokenise */
  dirlist[0] = argv[1]; dirlist[1] = NULL;
  ftsptr = fts_open(dirlist, FTS_LOGICAL, NULL);

  /* process each entry that is a file */
  while (1) {
    entry = fts_read(ftsptr);
    if (entry == NULL) break;
    if (entry->fts_info != FTS_F) continue;

    zin = eligible(entry->fts_accpath);
    if (zin == NULL) continue;

    ntokens = 0;

    output_filename(entry->fts_accpath);
    yyrestart(zin); reset_comments(); yylex();
    fclose(zin);
  }
  fts_close(ftsptr);
  exit(0);
}
