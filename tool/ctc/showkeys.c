/*
 * showkeys: Print out all the keys, and the number of values for each key,
 * in the ctfnode.db file.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.20 $
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "dbfiles.h"
#include "tokens.h"

void do_stats(void)
{
  uint8_t *key;
  Ctfhashnode *node;

  int i, firstfile;
  int numvals;
  int has_many_files;
  uint32_t percentage;
  int numkeys = 0;		/* Overall number of keys */
  int worstkeys[4];		/* Keys with the most values */
  int numwiththisvalues[10];	/* Num keys with value "index" */
  int multifilekeys = 0;	/* Number of multifile keys */
  int multifilenoval = 0;	/* # of multifile keys with valxor==0 */


  for (i = 0; i < 10; i++)	/* Zero the array contents */
    numwiththisvalues[i] = 0;
  for (i = 0; i < 4; i++)
    worstkeys[i] = 0;


  /* Get first key */
  if (get_firstkey(&key, &node, &numvals) == 0) {
    printf("Cannot get first key\n");
    exit(1);
  }

  /* Loop while we have a key */
  while (1) {
    numkeys++;
    if (numvals < 10)
      numwiththisvalues[numvals]++;

    /* Determine if this is a multi-file entry */
    has_many_files = 0;
    firstfile = node[0].fileid;
    for (i = 1; i < numvals; i++) {
      if (firstfile != node[i].fileid) {
	has_many_files = 1;
	break;
      }
    }

    if (has_many_files == 1) {
      multifilekeys++;
      if ((key[TUPLE_SIZE] == 0) && (key[TUPLE_SIZE + 1] == 0))
	multifilenoval++;
      if (numvals > worstkeys[3]) {
	worstkeys[0] = worstkeys[1];
	worstkeys[1] = worstkeys[2];
	worstkeys[2] = worstkeys[3];
	worstkeys[3] = numvals;
      }
    }
    if (get_nextkey(&key, &node, &numvals) == 0)
      break;
  }

  printf("Total number of keys: %d\n", numkeys);
  printf("Number of multi-file keys: %d", multifilekeys);
  percentage = 100 * multifilekeys / (numkeys + 1);	/* Avoid divby 0 */
  printf("  (%d%%)\n", percentage);
  printf("Number of multi-file keys, xor 0: %d", multifilenoval);
  percentage = 100 * multifilenoval / (numkeys + 1);	/* Avoid divby 0 */
  printf("  (%d%%)\n", percentage);
  printf("Worst multi-file key sizes: %d, %d, %d, %d\n", worstkeys[3],
	 worstkeys[2], worstkeys[1], worstkeys[0]);
  printf("Value |   # Keys\n");
  printf("======+==============\n");
  for (i = 1; i < 10; i++) {
    percentage = 100 * numwiththisvalues[i] / (numkeys + 1);	/* Avoid divby 0 */
    printf("  %d   | %7d  %d%%\n", i, numwiththisvalues[i], percentage);
  }
}

int main(int argc, char *argv[])
{
  uint8_t *key;
  Ctfhashnode *node;
  int showvalues = 0;
  int numvals;
  int i;

  if (argc < 2)
    {
      fprintf(stderr, "usage: ./showkeys [dbname] -v -s\n");
      exit(1);
    }

  // XXX
  char* dbname = argv[1];

  FILEID_DB = malloc(strlen(dbname) + 3 + 1 + 2 + 1);
  sprintf(FILEID_DB, "%s-id.db", dbname);

  CTFNODE_DB = malloc(strlen(dbname) + 5 + 1 + 2 + 1);
  sprintf(CTFNODE_DB, "%s-nodes.db", dbname);

  /* Set the value flag if an argument */
  if (argc == 3 && !strcmp(argv[2], "-s")) {
    do_stats();
    return (0);
  }

  if (argc == 3 && !strcmp(argv[2], "-v"))
    showvalues = 1;

  /* Get first key */
  if (get_firstkey(&key, &node, &numvals) == 0) {
    printf("Cannot get first key\n");
    exit(1);
  }

  /* Loop while we have a key */
  while (1) {

    /* Print out # values and key */
    printf("%4d\t", numvals);
    for (i = 0; i < TUPLE_SIZE; i++)
      printf("%02x", key[i]);
#if !defined(HCTF)
    /*
     * XXX: With hctffiles.c, the 2-byte XOR hash after the tuple is not
     * stored in the file, so we cannot guarantee to print it out
     */
    printf(":%02hhx%02hhx", key[TUPLE_SIZE], key[TUPLE_SIZE + 1]);
#endif
    printf("\n");

    /* Print out each value if required */
    if (showvalues) {
      printf("\t");
      for (i = 0; i < TUPLE_SIZE; i++)
	printf("%s", tok2str(key[i]));
      printf("\n");

      for (i = 0; i < numvals; i++) {
	printf(
	       "\tValue %d:\toffset %04x name %04x file %02d line %03d\n", i,
	       node[i].offset, node[i].name_offset, node[i].fileid,
	       node[i].line_number);
      }
      printf("\n");
    }

    if (get_nextkey(&key, &node, &numvals) == 0)
      break;
  }
  return (0);
}
