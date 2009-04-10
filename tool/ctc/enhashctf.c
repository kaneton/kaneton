/*
 * enhashctf: Take the tokens in a ctf file and add them to a db file as a
 * set of token tuples which allow fast lookup of matching runs of tokens
 * between multiple ctf files.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.34 $
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tokens.h"
#include "dbfiles.h"

#undef DUMMY_PRINT

/* Create and save a hash/node into the database */
void save_tuple(uint8_t * hash, uint16_t * valhash, Ctfhashnode node)
{
  int i;

  /* Calculate the XOR of all the id hashes */
  uint16_t valxor = valhash[0];
  for (i = 1; i < TUPLE_SIZE; i++)
    valxor ^= valhash[i];

#ifdef DUMMY_PRINT
  for (i = 0; i < TUPLE_SIZE; i++)
    printf("%02x", hash[i]);
  printf(":%04x\n  offset %04x name %04x file %02d line %03d\n",
	 valxor, node.offset, node.name_offset, node.fileid,
	 node.line_number);
  printf("  ");
  for (i = 0; i < TUPLE_SIZE; i++)
    printf("%04x ", valhash[i]);
  printf("\n");
#else

  /* Insert the valxor into the tuple array */
  hash[TUPLE_SIZE] = valxor >> 8;
  hash[TUPLE_SIZE + 1] = valxor & 0xff;
  add_ctfnode(hash, &node);
#endif
}

int main(int argc, char *argv[])
{
  /* Note: tuple[] is 2 bytes wider so we can insert the XOR'd valhash later */
  uint8_t tuple[TUPLE_SIZE + sizeof(uint16_t)];	/* tuple of tokens found */
  int token_offset[TUPLE_SIZE];	/* Offset of last TUPLE_SIZE tokens */
  int token_linenum[TUPLE_SIZE];/* Line number of last TUPLE_SIZE tokens */
  uint16_t valhash[TUPLE_SIZE];	/* Value for each of the tokens in tuple */
  int i;
  unsigned int ch;		/* Incoming token */
  Ctfhashnode node;		/* Node to insert into the database */
  int token_count = 0;		/* # of valid tokens found since filename */
  uint32_t old_offset;		/* Offset of current token */
  uint32_t offset = 0;		/* Offset of next token */
  uint32_t name_offset = 0;	/* Offset of last file name */
  int line_number = 0;		/* Current line number */
  int index = 0;		/* Index into the lists (modulo TUPLE_SIZE) */
  int fileid;			/* New file id of this file */
  uint32_t idval;		/* Id value for token fetched */
  char **name;			/* Pointer to each filename in the CTF file */
  ctfhandle *ctf;		/* Handle to opened CTF file */
  int fnum;			/* Index into argv, for each file to insert */

  /* Check the input arguments */
  if (argc < 3) {
    fprintf(stderr, "Usage: enhashctf [dhname] [ctffile ...]\n");
    exit(1);
  }

  // XXX
  char* dbname = argv[1];

  FILEID_DB = malloc(strlen(dbname) + 1 + 2 + 1 + 2 + 1);
  sprintf(FILEID_DB, "%s-id.db", dbname);

  CTFNODE_DB = malloc(strlen(dbname) + 1 + 5 + 1 + 2 + 1);
  sprintf(CTFNODE_DB, "%s-nodes.db", dbname);

  /* Process every file on the command-line */
  for (fnum = 2; fnum < argc; fnum++) {

    /* Open up the named input file */
    ctf = ctfopen(argv[fnum]);

    /* Initialise the name and offsets to after the header */
    name = (char **)malloc(sizeof(char *));
    offset = get_ctf_offset(ctf);
    old_offset = offset;

    /* Add the filename to the database and get the fileid back */
    fileid = add_fileidname(argv[fnum]);

    /* While there is a token to be found */
    while ((ch = get_token(ctf, &offset, &idval, name)) != 0) {
      switch (ch) {
      case FILENAME:
	/*
	 * We found a new filename. Clear the tuple, save the offset and
	 * reset some counters.
	 */
	name_offset = old_offset;
	for (i = 0; i < TUPLE_SIZE; i++)
	  tuple[i] = 0;
	token_count = 0;
	line_number = 1;
	index = 0;
	//printf("%s\n", *name);
	break;

      case LINE:
	line_number++;
	break;

      default:
	/* Add the token and id value to the tuple */
	for (i = 0; i < TUPLE_SIZE - 1; i++) {
	  tuple[i] = tuple[i + 1];
	  valhash[i] = valhash[i + 1];
	}

	tuple[TUPLE_SIZE - 1] = ch;
	token_count++;

	/* Only add the id hash if not an IDENTIFIER. Otherwise, use 0 */
	switch (ch) {
	case IDENTIFIER:
	  valhash[TUPLE_SIZE - 1] = 0;
	  break;
	case LABEL:
	case INTVAL:
	case CHARCONST:
	case STRINGLIT:
	  valhash[TUPLE_SIZE - 1] = idval & 0xffff;
	  break;
	default:
	  valhash[TUPLE_SIZE - 1] = 0;
	}

	/*
	 * Before we get the next token, save this one's offset and line
	 * number
	 */
	token_offset[index] = old_offset;
	token_linenum[index] = line_number;
	index = (index + 1) % TUPLE_SIZE;

	/* Save any existing tuple */
	if (token_count >= TUPLE_SIZE) {
	  node.offset = token_offset[index];
	  node.line_number = token_linenum[index];
	  node.name_offset = name_offset;
	  node.fileid = fileid;
	  save_tuple(tuple, valhash, node);
	}
      }
      old_offset = offset;
    }
    ctfclose(ctf);		/* End of this CTF file, close it */
    close_ctfnodedb();		/* and the database, so it can save state */
  }
  exit(0);
}
