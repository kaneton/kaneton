/*
 * hctffiles: Functions that deal with home-grown ctf hash files and the
 * nodes contained in them.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.30 $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include "dbfiles.h"
#include "crc32.h"
#include "tokens.h"

#define assert
#ifdef assert
#include <assert.h>
#endif

char* FILEID_DB = NULL;
char* CTFNODE_DB = NULL;

/*
 * When we do find potential duplicate keys (given a 2^24 bit collision
 * space), we build a linked list of potential duplicate tuples: PDT. Each
 * node has a 32-bit hash which is different to CRC-32 (to make comparisons
 * easy and to eliminate collisions), and a pointer to the next node or NULL.
 * 
 * NOTE: in the on-disk serialisation of the database, we have: 16-octet tuple
 * CRC32 hash value FNV   hash value Ctfhashnode
 * 
 * In memory, once the CRC32 is used as a lookup, we overlay a PDT struct on top
 * of the CRC32 and FNV hashes.
 */
typedef struct pdt {
  uint8_t tuple[TUPLE_SIZE];
  union {
    uint32_t crc32hash;
    struct pdt *next;
  }   u;
  uint32_t fnvhash;
}   PDT;

/*
 * fnv - Fowler/Noll/Vo hash code.
 * Please do not copyright this code. This code is in the public domain.
 * 
 * By: chongo <Landon Curt Noll> /\oo/\ http://www.isthe.com/chongo/
 */
uint32_t
fnv32(void *buf, size_t len)
{
  uint32_t hval = 0x811c9dc5;
  uint8_t *bp = (uint8_t *) buf;/* start of buffer */
  uint8_t *be = bp + len;	/* beyond end of buffer */

  /*
   * FNV-1 hash each octet in the buffer
   */
  while (bp < be) {

    /* multiply by the 32 bit FNV magic prime mod 2^32 */
    hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);

    /* xor the bottom with the current octet */
    hval ^= (uint32_t) * bp++;
  }

  /* return our new hash value */
  return hval;
}

/************************
 *** Fileid functions ***
 ************************/
static int nextfileid;		/* Position of next free fileid */
static char **filelist = NULL;	/* List of CTF file names */

/*
 * Read in the fileid database and initialise the list of fileid pointers and
 * the nextfileid
 */
static void read_fileid_db(void)
{
  FILE *zin;
  char buffer[1024];
  int count = 0;

  /* Open the file just to create it, then open for reading */
  zin = fopen(FILEID_DB, "a+");
  zin = freopen(NULL, "r", zin);
  if (zin == NULL) {
    fprintf(stderr, "Unable to read the %s database: %s\n", FILEID_DB,
	    strerror(errno));
    exit(1);
  }

  /* Loop once just to count the number of lines */
  while (!feof(zin)) {
    char *fred = fgets(buffer, 1024, zin);
    if (fred == NULL)
      break;
    count++;
  }

  /* Allocate count+1 char pointers */
  count++;
  filelist = malloc(count * sizeof(char *));

  /* Now loop scanning each line in from the file */
  fseek(zin, 0, SEEK_SET);
  nextfileid = 1;		/* Start at position 1, 0 must be kept empty */
  while (!feof(zin)) {
    char *fred = fgets(buffer, 1024, zin);
    if (fred == NULL)
      break;

    /* Remove the newline */
    buffer[strlen(buffer) - 1] = '\0';

    /* Copy the filename into the list */
    filelist[nextfileid++] = strdup(buffer);
  }

  fclose(zin);
}

/*
 * Given a filename, add it to the fileid database and return the fileid. On
 * failure, return the value 0.
 */
int add_fileidname(char *name)
{
  FILE *zout;
  int i;

  /* Reject NULL or empty strings */
  if (name == NULL || strlen(name) == 0)
    return (0);

  /* Get biggest fileid so far */
  if (filelist == NULL)
    read_fileid_db();

  /* Check that the file isn't already in there */
  for (i = 1; i < nextfileid; i++)
    if (!strcmp(filelist[i], name))
      return (i);

  /* Write out the name */
  zout = fopen(FILEID_DB, "a");
  if (zout == NULL) {
    fprintf(stderr, "Unable to append to the %s database: %s\n", FILEID_DB,
	    strerror(errno));
    exit(1);
  }
  fprintf(zout, "%s\n", name);
  fclose(zout);
  return (nextfileid);
}

/*
 * Return a pointer to an array of filenames in the database ordered by their
 * fileid, and the number of fileids (including the unused zero one).
 */
int get_fileidnames(char ***outarray)
{
  if (outarray == NULL)
    return (0);

  /* Get biggest fileid so far */
  if (filelist == NULL)
    read_fileid_db();

  *outarray = filelist;
  return (nextfileid);
}

/*********************************
 *** Ctfnode writing functions ***
 *********************************/
static FILE *ctf_dbf = NULL;

/* Open the ctfnode database */
static void open_ctfnodedb(int how)
{
  int len;
  if (how == O_RDONLY) {
    ctf_dbf = fopen(CTFNODE_DB, "r");
    return;
  }

  /* Open the file for writing */
  ctf_dbf = fopen(CTFNODE_DB, "a");

  /* Check PDT + Ctfhashnode are 4-aligned */
  len = sizeof(PDT) + sizeof(Ctfhashnode);
  if (len & 3) {
    fprintf(stderr, "Err: sizeof PDT+Ctfhashnode not 4-aligned: %d\n", len);
    exit(1);
  }
}

/*
 * Create & add a new ctfnode into the ctfnode file. Returns 1 if OK, 0 if
 * fail.
 */
int add_ctfnode(uint8_t * tuple, Ctfhashnode * value)
{
  uint32_t crc32hash;
  uint32_t fnvhash;

  /* Inputs must exist */
  if ((tuple == NULL) || (value == NULL))
    return (0);

  /* Open the ctfnode.db file */
  if (ctf_dbf == NULL)
    open_ctfnodedb(O_RDWR);

  crc32hash = crc32(tuple, TUPLE_SIZE + sizeof(uint16_t));
  fnvhash = fnv32(tuple, TUPLE_SIZE + sizeof(uint16_t));

  /* Write out the tuple, the 2 hashes, then the Ctfhashnode */
  fwrite(tuple, TUPLE_SIZE, 1, ctf_dbf);
  fwrite(&crc32hash, sizeof(crc32hash), 1, ctf_dbf);
  fwrite(&fnvhash, sizeof(fnvhash), 1, ctf_dbf);
  fwrite(value, sizeof(Ctfhashnode), 1, ctf_dbf);
  return (1);
}


/*********************************
 *** Ctfnode reading functions ***
 *********************************/
static ctfhandle ctf;		/* mmap'd handle on readonly database */


#define BITSINTABLE	24	/* 24-bit table */
#define TABLE_SIZE (1 << BITSINTABLE)

/*
 * This TABLE_SIZE table holds either the offset in the ctfnode.db file for a
 * unique tuple key, or a pointer to a PDT list where there are possibly
 * duplicate tuple keys.
 */
void **offset_table = NULL;

/*
 * Bit table corresponding to entries in the offset table. If bit is 0, entry
 * in offset_table is unique so far and is an offset. If bit is 1, entry is a
 * pointer to a list.
 */
uint8_t duplicate[TABLE_SIZE / 8];

/*
 * Lookup table to convert modulo 8 value into bit value in the duplicate
 * table. Last value ensures nothing found when bitposition == 8.
 */
uint8_t dupLUT[9] = {1, 2, 4, 8, 16, 32, 64, 128, 0};

/* State values, for repeated use of the get_nextkey() function. */
uint32_t duplindex = 0;		/* Duplicate table index */
int bitposition = 0;		/* Bit position in duplicate table */
PDT *pdtnode;			/* Pointer to next node in a PDT list */
Ctfhashnode *vallist = NULL;	/* Previously malloc'd list of hashnodes */


/* Close the ctf database */
void close_ctfnodedb(void)
{
  if (ctf_dbf) fclose(ctf_dbf);
  if (filelist) free(filelist);
  if (offset_table) free(offset_table);
  filelist=NULL;
  offset_table=NULL;
  ctf_dbf=NULL;
}

/*
 * Given a PDT, take the pointer in the offset_table, put it into our node
 * and point the offset_table at us, thus prepending us in the list.
 */
void buildPDT(PDT * pdtnode, uint32_t crc)
{
  /* Fill it in */
  pdtnode->u.next = (struct pdt *)offset_table[crc];
  offset_table[crc] = (void *)pdtnode;
#if 0
  printf("Linking new PDT %x with crc24 %x, fnv %x --> %x\n",
	 (uint32_t) pdtnode, crc,
	 pdtnode->fnvhash, (uint32_t) pdtnode->u.next);
#endif
}


/*
 * Get the next key in the database and its value. Size is # Ctfhashnodes in
 * the value. Returns 1 if OK, 0 otherwise.
 */
int get_nextkey(uint8_t ** tuple, Ctfhashnode ** value, int *size)
{
  PDT *node;
  Ctfhashnode *v;
  int count;
  uint32_t crc24;
  uint32_t pdtfnv;

  /* Inputs must exist */
  if ((tuple == NULL) || (value == NULL) || (size == NULL))
    return (0);

get_a_pdtnode:
  /*
   * If there is no pdtnode, find the head of a new PDT list from the offset
   * table.
   */
  if (pdtnode == NULL) {
    while (1) {
      /* Firstly search the remaining bit positions */
      uint8_t duplbit = dupLUT[bitposition];
      /* XXX Can we do this more efficiently? */
      if ((duplicate[duplindex] & duplbit) != 0)
	break;
      bitposition++;
      if (bitposition < 8)
	continue;

      /*
       * OK, so nothing at that duplindex, move up to the next one
       */
      bitposition = 0;
      while (1) {
	duplindex++;
	if (duplindex >= (TABLE_SIZE / 8))
	  return (0);		/* End of the road */
	if (duplicate[duplindex] != 0)
	  break;		/* Stop when we have bits */
      }

      /*
       * We get here with duplicate[duplindex] having some bits so simply go
       * back to the top of the loop to find one.
       */
    }

    /* Found a list of potential duplicates. Get the list head */
    crc24 = (duplindex << 3) + bitposition;
    pdtnode = (PDT *) offset_table[crc24];
  }

  /*
   * Using the pdtnode as the basis for comparison, find other nodes in the
   * list which have the same fnv hash. Ignore the nodes with FNV 0 as we
   * have already done those. When there is a match, read from the database
   * to get the Ctfhashnode value.
   */
  /* We walk the list once to count the number of matching tuples */
  count = 1;			/* including pdtnode */
  for (node = pdtnode->u.next; node; node = node->u.next)
    if (node->fnvhash && node->fnvhash == pdtnode->fnvhash)
      count++;

  /*
   * Damn, we got all the way here with a valid pdtnode, and none of the
   * potentials matched. Move up to the next pdtnode in the chain and jump
   * backwards to try it all again.
   */
  if (count == 1) {
    pdtnode = pdtnode->u.next;
    /* And if we fall off the end, bump up bitposition */
    if (pdtnode == NULL)
      bitposition++;
    goto get_a_pdtnode;		/* Yuk, I hate gotos */
  }

  /* Fetch a copy of the tuple */
  *tuple = pdtnode->tuple;

  /* Allocate room for the Ctfhashnodes */
  vallist = (Ctfhashnode *) realloc(vallist, count * sizeof(Ctfhashnode));
  if (vallist == NULL)
    return (0);
  *size = count;

  /*
   * Now fetch the Ctfhashnodes in. Keep a copy of the pdtnode's fnvhash
   * fnvhash as we will wipe it out inside the loop.
   */
  pdtfnv = pdtnode->fnvhash;
  *value = vallist;
  v = vallist;
  for (node = pdtnode; node; node = node->u.next)
    if (node->fnvhash && node->fnvhash == pdtfnv) {
      /* XXX Wish we didn't have to copy these */
      memcpy(v, (void *)node + sizeof(PDT), sizeof(Ctfhashnode));
      v++;
      node->fnvhash = 0;	/* Mark that we have processed this PDT */
    }

  /*
   * Just before we return, move pdtnode up to the next node with a valid
   * fnvhash for the next call to this function.
   */
  for (node = pdtnode, pdtnode = pdtnode->u.next; pdtnode && pdtnode->fnvhash == 0;
       node = pdtnode, pdtnode = pdtnode->u.next);

  /* And if we fall off the end, bump up bitposition */
  if (pdtnode == NULL)
    bitposition++;

  return (1);
}

/*
 * Get the first key in the database and its value. Size is # Ctfhashnodes in
 * the value. Returns 1 if OK, 0 otherwise.
 */
int get_firstkey(uint8_t ** returntuple, Ctfhashnode ** value, int *size)
{
  PDT *oldpdt;
  struct stat sb;
  uint32_t index;
  uint8_t duplbit;
  uint8_t *offset;

  /* Inputs must exist */
  if ((returntuple == NULL) || (value == NULL) || (size == NULL))
    return (0);

  /*
   * Before we can do any key retrieval, we first have to read in the keys
   * from the database, and build lists of potentially duplicate keys.
   */

  /* Create and clear the offset_table */
  offset_table = (void *)calloc(TABLE_SIZE, sizeof(uint32_t));
  if (offset_table == NULL) {
    fprintf(stderr, "Unable to create the offset_table: %s\n",
	    strerror(errno));
    exit(1);
  }

  /* Open the ctfnode.db file */
  if ((ctf.fd = open(CTFNODE_DB, O_RDONLY)) == -1) {
    fprintf(stderr, "Can't open %s\n", CTFNODE_DB);
    exit(1);
  }

  fstat(ctf.fd, &sb);
  ctf.start = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, ctf.fd, 0);
  if (ctf.start == NULL) {
    fprintf(stderr, "Can't mmap %s\n", CTFNODE_DB);
    exit(1);
  }

  offset = ctf.start;
  ctf.end = ctf.start + sb.st_size;

  /* Read every tuple from the database file */
  while (offset < ctf.end) {
    PDT *pdt = (PDT *) offset;

    /* Get 24 bits of a CRC32 checksum on the tuple */
    uint32_t crc24 = pdt->u.crc32hash & 0xffffff;

    /*
     * Convert into index and bit position into the duplicate table
     */
    index = crc24 >> 3;
    duplbit = dupLUT[crc24 & 0x7];

    /*
     * If the duplicate bit is set, we already have a list hanging from the
     * offset_table at position crc24.
     */
    if ((duplicate[index] & duplbit) != 0) {
      /*
       * printf("Need to insert new node into list at crc24 %x\n", crc24);
       */
      /* Insert a new PDT node into the list */
      buildPDT(pdt, crc24);
      goto next_iteration;	/* Yuk, I hate gotos */
    }

    /*
     * No duplicate set, so this could be the first duplicate. Check out the
     * offset table to see if we already have a value.
     */
    if (offset_table[crc24] == NULL) {
      /* First value for this crc24, so record the offset */
      /* printf("First offset for crc24 %x\n", crc24); */
      offset_table[crc24] = (void *)offset;
      goto next_iteration;	/* Yuk, I hate gotos */
    }
    /*
     * We get to here when there is already a value in the offset table.
     */
    /* printf("First duplicate for crc24 %x\n", crc24); */

    /* Fetch the old PDT */
    oldpdt = (PDT *) offset_table[crc24];

    /*
     * Link in the PDT node representing the old tuple. We have to clear the
     * offset in the offset_table so that the PDT node will get NULL in its
     * next field.
     */
    offset_table[crc24] = NULL;
    buildPDT(oldpdt, crc24);

    /* Now link in the new tuple, and set the duplicate bit */
    buildPDT(pdt, crc24);
    duplicate[index] |= duplbit;

next_iteration:
    /* Skip the PDT and Ctfhashnode, and update the offset */
    offset += sizeof(PDT) + sizeof(Ctfhashnode);
  }

  /* Now we can search for the first key */
  return (get_nextkey(returntuple, value, size));
}
