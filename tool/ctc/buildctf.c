/*
 * buildctf: Build a C token file given a directory containing C code.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.24 $
 */

#include <sys/types.h>
#include <stdint.h>
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
extern void yyrestart(FILE * input_file);
extern FILE *eligible(const char *file);

FILE *zin, *zout;

void output_filename(char *name)
{
  struct stat sb;
  uint32_t timestamp = 0;

  /* Get the file's last modification time */
  if (stat(name, &sb) == 0)
    timestamp = sb.st_mtime;

  /* Output the FILENAME token, the timestamp, the filename and a NUL */
  fputc(FILENAME, zout);
  fputc((timestamp >> 24) & 0xff, zout);
  fputc((timestamp >> 16) & 0xff, zout);
  fputc((timestamp >> 8) & 0xff, zout);
  fputc(timestamp & 0xff, zout);
  fputs(name, zout);
  fputc('\0', zout);
}

void usage(void)
{
  fprintf(stderr, "Usage: buildctf directory outputfile\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  struct stat sb;
  int err;
  char *dirlist[2];
  FTS *ftsptr;
  FTSENT *entry;

  if (argc != 3)
    usage();

  /* Check that the directory exists and is a directory */
  err = stat(argv[1], &sb);
  if (err < 0) {
    fprintf(stderr, "Err: %s doesn't exist\n", argv[1]);
    exit(1);
  }
  if (!S_ISDIR(sb.st_mode)) {
    fprintf(stderr, "Err: %s is not a directory\n", argv[1]);
    exit(1);
  }

  /* Open up the output file */
  zout = fopen(argv[2], "w");
  if (zout == NULL) {
    fprintf(stderr, "Can't open %s for writing", argv[2]);
    exit(1);
  }

  /* Output the ctf header and version 2.1 */
  fputs("ctf2.1", zout);

  /* Now search for files to tokenise */
  dirlist[0] = argv[1];
  dirlist[1] = NULL;
  ftsptr = fts_open(dirlist, FTS_LOGICAL, NULL);

  /* Process each entry that is a file */
  while (1) {
    entry = fts_read(ftsptr);
    if (entry == NULL)
      break;
    if (entry->fts_info != FTS_F)
      continue;

    zin = eligible(entry->fts_accpath);
    if (zin == NULL)
      continue;

    output_filename(entry->fts_accpath);
    yyrestart(zin);
    reset_comments();
    yylex();
    fclose(zin);
  }

  fts_close(ftsptr);
  putc(EOFTOKEN, zout);
  exit(0);
}
