/*
 * detok: Detokenise a C token file into something a programmer can read.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.19 $
 */
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

int main(int argc, char *argv[])
{
  unsigned int ch, linenum = 0;
  uint32_t val;
  ctfhandle *ctf;
  char **name;
  uint32_t offset;

  if (argc != 2) {
    fprintf(stderr, "Usage: detok cft_file\n");
    exit(1);
  }
  ctf = ctfopen(argv[1]);

  /* Initialise the name and offsets to after the header */
  name = (char **)malloc(sizeof(char *));
  offset = get_ctf_offset(ctf);

  /* While there is a token to be found */
  while ((ch = get_token(ctf, &offset, &val, name)) != 0) {
    switch (ch) {
    case FILENAME:
      linenum = 1;
      print_token(stdout, ch, linenum, val, *name);
      break;
    case LINE:
      linenum++;
    default:
      print_token(stdout, ch, linenum, val, *name);
    }
  }
  exit(0);
}
