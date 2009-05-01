/*
 * Find and report all code similarities as exist in the ctfnode.db database.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * 
 * $Revision: 1.70 $
 */

/*
 * The following are debug defines, you do not want them enabled in a
 * production version.
 */
#undef STATS			/* Keep some counters of operations performed */
#undef LISTDEBUG		/* Print out list processing messages */
#undef QUIET			/* Don't print any results, used with STATS */

/* Other compile-time choices */
#undef FREE_MEMORY		/* Define this if you are tight on RAM; note */
				/* it will significantly slow the code down */
#define NO_INTRA_TREE_COMPARISONS  /* Don't compare files within same tree */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "avltree.h"
#include "tokens.h"

char **namearray;		/* Array of CTF files in the database */
ctfhandle **ctf_handle;		/* Array of CTF handles */
int numfileids;			/* Size of the above two arrays */

int threshold = 16;		/* Threshold above which we report a run */
int do_isomorph_comparison = 0;	/* Do an isomorphic comparison on the code */
int isomorph_count_threshold = 6;	/* Only allow 3 isomorphic relations */
				/* The number has to be doubled, see below */
int showtokens = 0;		/* If 1, print out tokens in a run */
int crosscompare = 0;		/* If 1, print out actual code in a run */
int side_by_side = 0;		/* Do we display the output side by side? */
int sort_results = 0;		/* If 1, print results sorted by length */
#ifdef STATS
uint64_t potential_count = 0;	/* Count the # of potential runs found */
uint64_t actual_count = 0;	/* Count the # of actual runs found */
uint64_t comp_count = 0;	/* Count the # of actual comparisons */
uint64_t skip_count = 0;	/* Count of fileid comparison mismatches */
uint64_t final_count = 0;	/* Count final runs printed out */
uint64_t multi_count = 0;	/* Count # of multi-file collision keys */
uint64_t multi_hashcount = 0;	/* Count # of multi-file ctfhashnodes */
#endif

/* Isomorphic comparison tables */
/* We need two tables: one to record the match from a dst value */
/* to a src value, and another to record the src to dst value match */
uint16_t isodtos[65536];	/* Destination to source isomorphism */
uint16_t isostod[65536];	/* Source to destination isomorphism */
uint16_t isoseen[65536];	/* =1 if we have seen this id value */
int max_isoseen = 0;		/* Number of relationships seen */

// XXX[output file]
FILE* output = NULL;

/*
 * Once we evaluate and find a real run from a tuple in the database, we
 * store it into the potential AVL tree, which is sorted by source start
 * offset order. Later, we compare these runs with runs stored in a hashed
 * set of linked lists, sorted by source end offset. This allows us to remove
 * runs which are simply subsets of the existing ones in the linked lists.
 */

AVL_IX_DESC potential;		/* The potential AVL tree */
struct run *Actarray[1048576];	/* Linked list of runs, indexed by hash */

#ifdef LISTDEBUG
/* Print out all of the runs given a specific hash. */
void debug_print_listruns(uint32_t hashval)
{
  struct run *Head, *this;

  Head = Actarray[hashval];

  for (this = Head; this != NULL; this = this->next)
    printf("   %d: %d-%d,%d-%d 0x%x\n", this->src->fileid,
	   this->src->offset, this->src_end_offset,
	   this->dst->offset, this->dst_end_offset, (int)this);
}
#endif

/*
 * Checks new run against existing one. If new one is a subset, it is freed.
 * Otherwise it is inserted into the actual linked list, given the hash
 * value.
 */
void check_run(struct run *newnode)
{
  struct run *Head, *existnode;
  Ctfhashnode *newsrc, *existsrc;
  Ctfhashnode *newdst, *existdst;
  uint32_t indexhash;

#ifdef STATS
  actual_count++;
#endif

  newsrc = newnode->src;
  newdst = newnode->dst;

  /*
   * Calculate the 20-bit index hash, using the two end offsets and the two
   * fileids. The magic numbers 7, 8 and 12 here were found by extracting the
   * 4 numbers from a large database, and exploring the space of 3 shift
   * values until we found the "best" values: fewest collisions.
   */
  /* XXX
  indexhash = newnode->src_end_offset
    + (newnode->dst_end_offset << 7)
    + (newnode->src->fileid << 8)
    + (newnode->dst->fileid << 12);
  */
  /* XXX[patch to prevent false positive] */
  {
      char **srcname, **dstname;
      uint32_t timestamp, off;

      srcname = (char **)malloc(sizeof(char *));
      dstname = (char **)malloc(sizeof(char *));
      off = newnode->src->name_offset;
      get_token(ctf_handle[newnode->src->fileid], &off, &timestamp, srcname);
      off = newnode->dst->name_offset;
      get_token(ctf_handle[newnode->dst->fileid], &off, &timestamp, dstname);

      /*
      indexhash = (unsigned int)*srcname
	+ ((unsigned int)*dstname << 7)
	+ (newnode->src->fileid << 8)
	+ (newnode->dst->fileid << 12);
      */
      indexhash = newnode->src->name_offset
	+ (newnode->dst->name_offset << 7)
	+ (newnode->src->fileid << 8)
	+ (newnode->dst->fileid << 12);

      indexhash &= 0xfffff;
  }

#ifdef LISTDEBUG
  printf("In check_run, with new %d:%d-%d,%d-%d len %d\n",
	 newsrc->fileid, newsrc->offset, newnode->src_end_offset,
	 newdst->offset, newnode->dst_end_offset, newnode->runlength);
  debug_print_listruns(indexhash);
#endif

  /* Get the head of the list given the hash value */
  Head = Actarray[indexhash];

  /* Walk the list, looking for any larger matching run */
  for (existnode = Head; existnode != NULL; existnode = existnode->next) {
    existsrc = existnode->src;
    existdst = existnode->dst;

#ifdef LISTDEBUG
    printf("In check_run, with new %d:%d-%d,%d-%d to %d:%d-%d,%d-%d\n",
	   newsrc->fileid, newsrc->offset, newnode->src_end_offset,
	   newdst->offset, newnode->dst_end_offset,
	   existsrc->fileid, existsrc->offset, existnode->src_end_offset,
	   existdst->offset, existnode->dst_end_offset);
#endif

    /* Skip mismatching fileids */
    if ((newsrc->fileid != existsrc->fileid) ||
	(newdst->fileid != existdst->fileid)) {
#ifdef STATS
      skip_count++;
#endif
      continue;
    }

#ifdef LISTDEBUG
    printf("In check_run, comparing new %d:%d-%d,%d-%d to %d:%d-%d,%d-%d\n",
	   newsrc->fileid, newsrc->offset, newnode->src_end_offset,
	   newdst->offset, newnode->dst_end_offset,
	   existsrc->fileid, existsrc->offset, existnode->src_end_offset,
	   existdst->offset, existnode->dst_end_offset);
#endif

    /*
     * Because entries in this list are here because of the hash, we have to
     * compare everything against everything! All we know is that the
     * incoming run is not larger than the existing one.
     */
#ifdef STATS
    comp_count++;
#endif
    if ((existsrc->offset <= newsrc->offset) &&
	(existdst->offset <= newdst->offset) &&
	(existnode->src_end_offset >= newnode->src_end_offset) &&
	(existnode->dst_end_offset >= newnode->dst_end_offset)) {
#ifdef LISTDEBUG
      printf("   *** This one can be junked, subset of existing\n\n");
#endif

      return;
    }
  }

#ifdef LISTDEBUG
  printf("In check_run, inserting %d:%d-%d,%d-%d\n\n",
	 newsrc->fileid, newsrc->offset, newnode->src_end_offset,
	 newdst->offset, newnode->dst_end_offset);
#endif

  /* Add the run to the list */
  newnode->next = Actarray[indexhash];
  Actarray[indexhash] = newnode;
}


/*
 * Given a run, print out the tokens in the run in much the same way as we do
 * in detok.c
 */
void print_tokens(struct run *node)
{
  uint32_t val;
  unsigned int ch;
  int length = node->runlength;
  int fid = node->src->fileid;
  uint32_t offset = node->src->offset;
  uint32_t line = node->src->line_number;

  fprintf(output, "%5d:   ", line);
  while ((length >= 0) &&
	 ((ch = get_token(ctf_handle[fid], &offset, &val, NULL)) != 0)) {
    switch (ch) {
    case FILENAME:
      line = 1;
      break;
    case LINE:
      line++;
      break;
    default:
      length--;
    }
    print_token(output, ch, line, val, "");
  }
  fprintf(output, "\n\n");
}

/*
 * Print out lines of two files side-by-side. This is best viewed with a
 * terminal window 160 characters wide! If neither file exists, the function
 * simply returns with no error message.
 */
void paste_files(char *file1, char *file2, struct run *node)
{
  FILE *f1in, *f2in;
  char buf[1024];
  int count_to_eighty = 0;
  int i, bptr, maxlines;
  int numlines1, numlines2;
  int tab_upto = 0;
  int side_side = side_by_side;	/* This may change */
  int start1 = node->src->line_number;
  int end1 = node->src_last_line_number;
  int start2 = node->dst->line_number;
  int end2 = node->dst_last_line_number;

  f1in = fopen(file1, "r");
  if (f1in == NULL)
    side_side = 0;

  f2in = fopen(file2, "r");
  if (f2in == NULL)
    side_side = 0;

  /* We can't display either file, simply show the tokens */
  if ((f1in == NULL) && (f2in == NULL)) {
    print_tokens(node);
    return;
  }
  /* Get maximum number of lines */
  numlines1 = end1 - start1 + 1;
  numlines2 = end2 - start2 + 1;
  maxlines = (numlines1 > numlines2) ? numlines1 : numlines2;
  start1--;
  start2--;

  /* Read up to line start1 in file1 */
  if (f1in != NULL)
    while (start1--)
      fgets(buf, 1020, f1in);

  /* Read up to line start2 in file2 */
  if (f2in != NULL)
    while (start2--)
      fgets(buf, 1020, f2in);

  if (side_side) {
    /* Now print out the lines */
    for (i = 0; i < maxlines; i++, numlines1--, numlines2--) {

      /* Print the left-hand line */
      if (numlines1 > 0) {
	fgets(buf, 1020, f1in);
	buf[strlen(buf) - 1] = '\0';	/* Remove newline */

	/*
	 * I used to printf("%-80s",buf); here, but it never did what I
	 * wanted, so I now print out exactly 80 chars by hand. This is
	 * because we have to deal with tabs, dammit!
	 */
	tab_upto = 0;
	bptr = 0;
	for (count_to_eighty = 0; count_to_eighty < 80; count_to_eighty++) {
	  if (tab_upto > count_to_eighty) {
	    fputc(' ', output);
	    continue;
	  }
	  if (buf[bptr] == '\0')
	    break;
	  if (buf[bptr] == '\t') {

	    /* Calculate the next highest multiple of 8 */
	    tab_upto = (count_to_eighty + 8) & ~7;
	    if (tab_upto >= 80)
	      break;
	    fputc(' ', output);
	    bptr++;
	    continue;
	  }
	  fputc(buf[bptr++], output);
	}
	for (; count_to_eighty < 80; count_to_eighty++) {
	  fputc(' ', output);
	}
	fprintf(output, " ");
      } else
	fprintf(output, "%81s", " ");

      /* Print the right-hand line */
      if (numlines2 > 0) {
	fgets(buf, 1020, f2in);
	fputs(buf, output);
      } else
	fprintf(output, "\n");
    }
  } else {			/* Not side-by-side */
    if (f1in != NULL)
      for (i = 0; i < numlines1; i++) {
	fgets(buf, 1020, f1in);
	fputs(buf, output);
      }
    else
      print_tokens(node);
    fprintf(output, "=====================================\n");
    if (f2in != NULL)
      for (i = 0; i < numlines2; i++) {
	fgets(buf, 1020, f2in);
	fputs(buf, output);
      }
    else
      print_tokens(node);
  }
  fprintf(output, "\n");
  if (numlines1 > numlines2)
    fprintf(output, "\n");
  if (f1in != NULL)
    fclose(f1in);
  if (f2in != NULL)
    fclose(f2in);
}

/*
 * Given a run, print out the 3-column header. Then determine if and how to
 * print out the details of the run.
 */
void print_listrun(struct run *node)
{
  char **srcname, **dstname;
  uint32_t timestamp, off;

#ifdef STATS
  final_count++;
#endif
#ifdef QUIET
  return;
#endif

  srcname = (char **)malloc(sizeof(char *));
  dstname = (char **)malloc(sizeof(char *));
  off = node->src->name_offset;
  get_token(ctf_handle[node->src->fileid], &off, &timestamp, srcname);
  off = node->dst->name_offset;
  get_token(ctf_handle[node->dst->fileid], &off, &timestamp, dstname);

  fprintf(output, "%d  %s:%d-%d  %s:%d-%d\n", node->runlength, *srcname,
	  node->src->line_number, node->src_last_line_number, *dstname,
	  node->dst->line_number, node->dst_last_line_number);
#ifdef LISTDEBUG
  printf("\tsource %d-%d, dest %d-%d\n",
	 node->src->offset, node->src_end_offset,
	 node->dst->offset, node->dst_end_offset);
#endif

  if (showtokens)
    print_tokens(node);
  if (crosscompare)
    paste_files(*srcname, *dstname, node);
  free(srcname);
  free(dstname);
}

/* Print out all of the runs. */
void print_listruns(void)
{
  struct run *Head, *this;
  AVL_IX_REC pe;
  AVL_IX_DESC sorted;		/* The actual AVL tree, sorted */
  uint32_t i;

  avl_create_index(&sorted);

  for (i = 0; i < 1048576; i++) {
    Head = Actarray[i];
    for (this = Head; this != NULL; this = this->next)
      if (sort_results) {
	pe.key = this->runlength;
	pe.recptr = (void *)this;
	avl_add_key(&pe, &sorted);
      } else
	print_listrun(this);
  }

  /* Print the sorted results if necessary */
  if (sort_results) {
    avl_last_key(&sorted);
    while (avl_prev_key(&pe, &sorted) == AVL_IX_OK) {
      this = (struct run *)pe.recptr;
      print_listrun(this);
    }
  }
}

/*
 * Given two Ctfhashnodes, do a full comparison and return the run length
 * found.
 */
int compare_runs(Ctfhashnode * src, Ctfhashnode * dst,
		     uint32_t * srcend, uint32_t * dstend,
		     uint32_t * srceline, uint32_t * dsteline)
{
  int runlength = 0;
  ctfhandle *src_handle, *dst_handle;
  uint8_t *srcoffset, *dstoffset;
  uint32_t srcendline = src->line_number;
  uint32_t dstendline = dst->line_number;
  uint8_t *nextsrcoff, *nextdstoff;
  uint32_t nextsrcline, nextdstline;
  uint8_t srctoken, dsttoken;
  uint32_t srcid = 0, dstid = 0;/* XXX: Shut the compiler up, could be a bug */

#ifdef STATS
  potential_count++;
#endif
#undef MYassert
#ifdef MYassert
  if ((src->fileid > numfileids) || (dst->fileid > numfileids)) {
    printf("One of the fids is out of bounds!\n");
    exit(1);
  }
#endif
  src_handle = ctf_handle[src->fileid];
  dst_handle = ctf_handle[dst->fileid];
  srcoffset = src_handle->start + src->offset;
  dstoffset = dst_handle->start + dst->offset;

  /*
   * Here are some comments on the isomorph[] arrays. We would like to
   * identify code fragments that are similar, even if variables have been
   * renamed or even declared in a different order. For example, the
   * following two functions are essentially identical.
   * 
   * int maxofthree(int x, int y, int z)    int bigtriple(int a, int b, int c) {
   * if ((x>y) && (x>z)) return(x);       {  if ((a>b) && (a>c)) return(a);
   * if (y>z) return(y);                     if (b>c) return(b); return(z);
   * return(c); }                                      }
   * 
   * The tokenised forms of these functions are obviously different. However, it
   * is possible to compare these and show that they are identical. If we
   * start with the first "if" statement, a straight lexical comparison shows
   * that they are different. But at the first difference (id1123 cf. id456),
   * we put an entry into the isodtos[] array so that isodtos[456]= 123, i.e.
   * the identifier #456 on the RHS is isomorphic to identifier #123 on the
   * LHS, and vice versa. Thus when we get to the first "return" statement,
   * we can see that they are also isomorphic. We can thus conclude that
   * everything from the "if" down to the "}" is isomorphic and thus
   * identical.
   */

  /*
   * Clear the identifer isomorph table for this run. I used to simply
   * memset() both isodtos[] and isostod[], but it is faster to track and
   * delete only those entries that we used.
   */
  if (do_isomorph_comparison) {
    while (--max_isoseen >= 0) {
      isodtos[isoseen[max_isoseen]] = 0;
      isostod[isoseen[max_isoseen]] = 0;
    }
    max_isoseen = 0;
  }
  /*
   * Set up the next variables, because we have to fall back to the previous
   * values when there is a match.
   */
  nextsrcoff = srcoffset;
  nextdstoff = dstoffset;
  nextsrcline = srcendline;
  nextdstline = dstendline;

  /*
   * We know that the first TUPLE_SIZE tokens match and the XOR. Now do a
   * full match from the start offsets in both until something fails.
   */
  while (1) {

    /* Get the next token from each file */
    while (1) {
      srctoken = *nextsrcoff++;
      switch (srctoken) {
	// XXX[patch to use match breakers]
      case CTC_GATE:
      case EOFTOKEN:
      case FILENAME:
	goto end_of_run;	/* Yuk, a goto! */
      case LINE:
	nextsrcline++;
	break;			/* i.e. get next src token */
      default:
	goto found_srctoken;	/* Yuk, a goto! */
      }
    }

found_srctoken:

    while (1) {
      dsttoken = *nextdstoff++;
      switch (dsttoken) {
	// XXX[patch to use match breakers]
      case CTC_GATE:
      case EOFTOKEN:
      case FILENAME:
	goto end_of_run;	/* Yuk, a goto! */
      case LINE:
	nextdstline++;
	break;			/* i.e. get next dst token */
      default:
	goto found_dsttoken;	/* Yuk, a goto! */
      }
    }

found_dsttoken:

    /* Stop if no match, or if we hit a new file */
    if (srctoken != dsttoken)
      break;

    /* Extract the ids as required */
    switch (srctoken) {
    case STRINGLIT:
    case CHARCONST:
    case LABEL:
    case IDENTIFIER:
    case INTVAL:
      srcid = *(nextsrcoff++) << 8;
      srcid += *(nextsrcoff++);
      dstid = *(nextdstoff++) << 8;
      dstid += *(nextdstoff++);
      /* Short-circuit the isomorph test if both ids the same */
      if (srcid == dstid)
	goto loopback2;		/* Yuk, a goto! */
      break;
    default:
      srcid=dstid=0;
    }

    /* If we hit an IDENTIFIER, do an isomorphic comparison */
    if (do_isomorph_comparison && (srctoken == IDENTIFIER)) {

      /* First thing, record a mapping each way if there is none */
      if (isodtos[dstid] == 0) {
	isodtos[dstid] = srcid;
	isoseen[max_isoseen++] = dstid;
      }
      if (isostod[srcid] == 0) {
	isostod[srcid] = dstid;
	isoseen[max_isoseen++] = srcid;
      }
      /* Now reject if the mappings fail in either direction */
      if (isodtos[dstid] != srcid)
	break;
      if (isostod[srcid] != dstid)
	break;

      /*
       * Stop now if we have reach the threshold on the number of isomorphic
       * relations that we can have. isomorph_count_threshold is always
       * doubled because we always have a 2-way relation.
       */
      if (max_isoseen >= isomorph_count_threshold)
	break;
    } else {
      /* Not isomorphic and/or not IDENTIFIER, any id mismatch ends the run */

      if (srcid != dstid)
	break;
    }

    /*
     * If we got here, there was one more token+value match. Update the
     * offsets, linenumbers and the run length.
     */

loopback2:
    runlength++;
    srcoffset = nextsrcoff;
    dstoffset = nextdstoff;
    srcendline = nextsrcline;
    dstendline = nextdstline;
  }

end_of_run:

  /* Update the end offsets and end line numbers */
  *srcend = (uint32_t) (srcoffset - src_handle->start);
  *dstend = (uint32_t) (dstoffset - dst_handle->start);
  *srceline = srcendline;
  *dsteline = dstendline;

  return (runlength);
}

void usage(void)
{
  fprintf(stderr,
	  "Usage: ctcompare [outputfile] [dbname] [-n nnn] [-r] [-s] [-t] [-x] [-i] [-I nnn] [file1.ctf] [file2.ctf]\n");
  fprintf(stderr, "\t-n nnn: set the minimum matching run length to nnn\n");
  fprintf(stderr, "\t-r: print results sorted by run length descending\n");
  fprintf(stderr, "\t-s: print results side by side on same line, implies -x\n");
  fprintf(stderr, "\t-t: show matching tokens when match is found\n");
  fprintf(stderr, "\t-x: show matching source lines when match is found\n");
  fprintf(stderr, "\t-i: enable isomorphic code comparison\n");
  fprintf(stderr, "\t-I nnn: limit the # isomorphic relations to nnn\n");
  fprintf(stderr, "    If file1.ctf or both file1.ctf & file2.ctf specified,\n");
  fprintf(stderr, "    only this/these files will be compared against.\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  uint8_t *key;			/* Key retrieved from the database */
  Ctfhashnode *node;		/* Array of results from the database */
  Ctfhashnode **copynode = NULL;/* Copy of nodes from the array, as needed */
  int numvals;			/* Size of the node[] array */
  uint32_t srcendoffset, dstendoffset;	/* Results passed back from */
  uint32_t srcendline, dstendline;	/* compare_runs() */
  struct run *runnode;		/* A run after its evaluation as real */
  uint32_t runlength;		/* Length of the run in tokens */
  AVL_IX_REC pe;		/* Above run as it is inserted into AVL tree */
  int file1_id = 0, file2_id = 0;	/* Specific fileids to ignore */
  int ch, i, j;
#ifdef NO_INTRA_TREE_COMPARISONS
  int firstfile, has_many_files;
#endif

  if (argc < 3)
    {
      usage();
      exit(1);
    }

  output = fopen(argv[1], "w");

  // XXX[patch to use a specific db file]
  char* dbname = argv[2];

  FILEID_DB = malloc(strlen(dbname) + 1 + 2 + 1 + 2 + 1);
  sprintf(FILEID_DB, "%s-id.db", dbname);

  CTFNODE_DB = malloc(strlen(dbname) + 1 + 5 + 1 + 2 + 1);
  sprintf(CTFNODE_DB, "%s-nodes.db", dbname);

  argc -= 2;
  argv += 2;

  /* Process options */
  while ((ch = getopt(argc, argv, "n:iI:rstx")) != -1) {
    switch (ch) {
    case 'I':
      /*
       * isomorph_count_threshold is always doubled because we always have a
       * 2-way relation.
       */
      i = 2 * atoi(optarg);
      if (i < 2) {
	fprintf(stderr, "Bad value for -I, must be 1 or greater\n");
      } else
	isomorph_count_threshold = i;
    case 'i':
      do_isomorph_comparison = 1;
      break;
    case 'r':
#ifndef QUIET
      sort_results = 1;
#endif
      break;
    case 't':
      if (crosscompare) {
	fprintf(stderr, "-t and -x are mutually exclusive\n");
	crosscompare = 0;
      }
      showtokens = 1;
      break;
    case 's':
      side_by_side = 1;
    case 'x':
      if (showtokens) {
	fprintf(stderr, "-t and -x are mutually exclusive\n");
	showtokens = 0;
      }
      crosscompare = 1;
      break;
    case 'n':
      i = atoi(optarg);
      if (i < 16) {
	fprintf(stderr, "Bad value for -n, must be 16 or greater\n");
      } else
	threshold = i;
      break;
    default:
      usage();
    }
  }
  argc -= optind;
  argv += optind;

  if (argc > 2)
    usage();

  /*
   * Get the names and fileids of the CTF files in the database.
   */
  numfileids = get_fileidnames(&namearray);

  ctf_handle = malloc(numfileids * sizeof(ctfhandle *));
  if (ctf_handle == NULL) {
    fprintf(stderr, "Cannot allocate %d ctf handles\n", numfileids);
    exit(1);
  }
  /* Open up all of the CTF files. */
  for (i = 1; i < numfileids; i++)
    ctf_handle[i] = ctfopen(namearray[i]);

  /* Determine if any ctf filter files exists in the database. */
  if (argc > 0) {
    for (i = 1; i < numfileids; i++) {
      if (!strcmp(argv[0], namearray[i])) {
	file1_id = i;
	break;
      }
    }
    if (file1_id == 0) {
      fprintf(stderr, "%s not represented in the database\n", argv[0]);
      exit(1);
    }
  }
  if (argc > 1) {
    for (i = 1; i < numfileids; i++) {
      if (!strcmp(argv[1], namearray[i])) {
	file2_id = i;
	break;
      }
    }
    if (file2_id == file1_id) {
      fprintf(stderr, "%s not represented in the database\n", argv[1]);
      exit(1);
    }
  }
  /* Create the potential AVL tree */
  avl_create_index(&potential);

  /* Get first tuple key from the external database */
  if (get_firstkey(&key, &node, &numvals) == 0) {
    printf("Cannot get first key\n");
    exit(1);
  }
  /* Loop while we have a database key */
  while (1) {

    /* Skip values which are size 1 */
    if (numvals < 2) {
      goto loopback;		/* Yuk, a goto! */
    }

#ifdef NO_INTRA_TREE_COMPARISONS
    /* Skip values which are all for the same CTF file */
    has_many_files = 0;
    firstfile = node[0].fileid;
    for (i = 1; i < numvals; i++) {
      if (firstfile != node[i].fileid) {
	has_many_files = 1;
	break;
      }
    }

    if (has_many_files == 0) {
      goto loopback;		/* Yuk, a goto! */
    }
#endif

#ifdef STATS
    multi_count++;		/* We found one with a mult-file collision */
    multi_hashcount += numvals;	/* Also count the number of ctfhashnodes */
#endif

    /*
     * We have to malloc/copy some of the node[] entries below. The problem
     * is, we may malloc/copy some of them multiple times. To avoid this, we
     * cache each copy found. This helps to reduce the program's memory
     * footprint.
     */

    /* Recreate copynode and empty it */
    copynode = realloc(copynode, numvals * sizeof(Ctfhashnode));
    if (copynode == NULL)
      break;
    for (i = 0; i < numvals; i++)
      copynode[i] = NULL;

    /* Time to do a full run length analysis of the combinations */
    for (i = 0; i < numvals; i++) {

      /* Skip if both filter files on and no match against either */
      if (file1_id && file2_id
	  && (node[i].fileid != file1_id) && (node[i].fileid != file2_id))
	continue;

      for (j = i; j < numvals; j++) {

#ifdef NO_INTRA_TREE_COMPARISONS
	/* Don't compare within the same CTF file */
	if (node[i].fileid == node[j].fileid)
	  continue;
#else
	/* Don't compare within the same source file in a CTF file */
	if ((node[i].fileid == node[j].fileid) &&
	    (node[i].name_offset == node[j].name_offset))
	  continue;
#endif

	/* If no file1_id match, and none this time, skip */
	if (file1_id && (node[i].fileid != file1_id) &&
	    (node[j].fileid != file1_id))
	  continue;

	/* Check against file2_id if it exists */
	if (file2_id) {
	  /*
	   * If there was a prev match against file1_id, skip if no
	   * file2_match
	   */
	  if ((node[i].fileid == file1_id) && (node[j].fileid != file2_id))
	    continue;
	  /* And vice versa */
	  if ((node[i].fileid == file2_id) && (node[j].fileid != file1_id))
	    continue;
	}

	/* We can finally do the comparison between nodes */
	runlength = compare_runs(&node[i], &node[j],
				 &srcendoffset, &dstendoffset,
				 &srcendline, &dstendline);
	if (runlength >= threshold) {

	  /* Save this run into the AVL tree of potential runs */

	  /* Create a runnode for each entry in the database result */
	  runnode = malloc(sizeof(struct run));
	  if (runnode == NULL)
	    break;
	  runnode->runlength = runlength;

	  /* Make copies of the nodes if we don't already have them */
	  if (copynode[i] == NULL) {
	    copynode[i] = malloc(sizeof(Ctfhashnode));
	    if (copynode[i] == NULL)
	      break;
	    memcpy(copynode[i], &node[i], sizeof(Ctfhashnode));
	  }
	  if (copynode[j] == NULL) {
	    copynode[j] = malloc(sizeof(Ctfhashnode));
	    if (copynode[j] == NULL)
	      break;
	    memcpy(copynode[j], &node[j], sizeof(Ctfhashnode));
	  }

	  /*
	   * Point the run at copies of the two nodes that we found. We
	   * ensure that the lowest fileid is the source node.
	   */
	  if (node[i].fileid < node[j].fileid) {
	    runnode->src = copynode[i];
	    runnode->src_end_offset = srcendoffset;
	    runnode->src_last_line_number = srcendline;
	    runnode->dst = copynode[j];
	    runnode->dst_end_offset = dstendoffset;
	    runnode->dst_last_line_number = dstendline;
	  } else {
	    runnode->dst = copynode[i];
	    runnode->dst_end_offset = srcendoffset;
	    runnode->dst_last_line_number = srcendline;
	    runnode->src = copynode[j];
	    runnode->src_end_offset = dstendoffset;
	    runnode->src_last_line_number = dstendline;
	  }

#ifdef LISTDEBUG
	  printf(" Got length %d\n", runlength);
	  printf("\tsource %d-%d, dest %d-%d\n",
		 runnode->src->offset, runnode->src_end_offset,
		 runnode->dst->offset, runnode->dst_end_offset);
#endif

	  /* Add the run to the potential AVL tree */
	  pe.key = runnode->src->offset;
	  pe.recptr = (void *)runnode;
	  avl_add_key(&pe, &potential);
	}
      }
    }

loopback:
    if (get_nextkey(&key, &node, &numvals) == 0)
      break;
  }
  close_ctfnodedb();		/* For HCTF, frees the 64M array */

  /*
   * Now traverse the potential AVL tree. We cross-compare potential runs
   * with those in the actual lists, to eliminate subsets.
   */
  avl_first_key(&potential);
  while (avl_next_key(&pe, &potential) == AVL_IX_OK) {
    runnode = (struct run *)pe.recptr;
    check_run(runnode);
#ifdef FREE_MEMORY
    free(runnode);		/* We normally don't do this as it kills
				 * performance */
#endif
  }

  /* All comparisons done, print out the final list of runs */
  print_listruns();
#ifdef STATS
  fprintf(stderr, "Number of multi-file keys:       %lld\n", multi_count);
  fprintf(stderr, "Number of multi ctfhashnodes:    %lld\n", multi_hashcount);
  fprintf(stderr, "Number of potential runs found:  %lld\n", potential_count);
  fprintf(stderr, "Number of actual runs found:     %lld\n", actual_count);
  fprintf(stderr, "Number of actual comparisons:    %lld\n", comp_count);
  fprintf(stderr, "Number of fileid skips:          %lld\n", skip_count);
  fprintf(stderr, "Number of final runs found:      %lld\n", final_count);
#endif

  return (0);
}
