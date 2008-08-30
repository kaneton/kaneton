/*
 * ctcompare: Compare two C tokens files for similarities.
 * Copyright (c) Warren Toomey, under the GPL license.
 * 
 * $Revision: 1.1.1.1 $
 */
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tokens.h"
#define ISOMORPH_CODE		/* Turn on to enable isomorphic code compares */
#define OPT_OVERLAP		/* Turn on to enable overlap optimisations */
#undef DEBUG

#define STARTFILE1 0xfe		/* Token id for start of each srcfile */
#define STARTFILE2 0xff		/* Token id for start of each dstfile */
#define BUF_SIZE 1024

void paste_files(char *file1, int start1, int num1,
		 char *file2, int start2, int num2);

/*
 * Each token is represented by the following structure. Each file in a ctf
 * file is represented as a linked list following the next pointer. The value
 * holds the token's value for two token types: identifiers and numbers.
 */
struct token {
  char *file;			/* File this token comes from */
  unsigned int linenum;		/* Line # on which token appeared */
  struct token *next;
  unsigned short value;
  unsigned char type;
};

/*
 * When we find a token run in each source file, we keep a
 * record of the run in a linked list of the following nodes.
 */
struct tokrun {
  int sstart, send;
  int dstart, dend;
  struct tokrun *next;
};

struct tokrun *Runhead = NULL;
struct tokrun *Runtail = NULL;


/*
 * We use the Rabin-Karp string comparison algorithm to compare the two token
 * lists in O(n+m) time. We keep just the basic token values in two char
 * arrays, and once we detect a possible match we then apply further tests to
 * accept or reject it.
 * 
 * For each of the two char token arrays, we keep a matching array of struct
 * token pointers, so we can find the filename and linenumber for each token.
 */
unsigned char *Toksrcarray;		/* Array of source tokens */
unsigned char *Tokdstarray;		/* Array of destination tokens */
int Toksrcsize;				/* # of src tokens */
int Tokdstsize;				/* # of destination tokens */
struct token **Toksrcref;
struct token **Tokdstref;

/* Run-time parameters */
int threshold = 20;		/* Threshold above which we report a run */
int showtokens = 0;		/* If 1, print out tokens in a run */
int crosscompare = 0;		/* If 1, print out actual code in a run */
int progress = 0;		/* Print filenames to stderr as we process */
int skip_cppstuff = 1;		/* Skip #defines, #includes etc. */

/* Array used to print out a run of tokens */
unsigned char *tokstring[] = {
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",
  "ERR ", "ERR ", "ERR ", "\n", "ERR ", "ERR ", ">>= ", "ERR ", "ERR ",
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ",
  "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "ERR ", "/= ", "! ",
  "\"string", "->", "++ ", "% ", "& ", "'c", "( ", ") ", "* ", "+ ", ", ", "- ",
  ". ", "/ ", "-- ", "&& ", "|| ", "++= ", "%= ", "-= ", "&= ", "*= ", "|= ",
  "NUM", ": ", "; ", "< ", "= ", "> ", "? ", "!= ", "<= ", "case ", "char ",
  "const ", "continue ", "default ", "do ", "...", "double ", "else ",
  "enum ", "extern ", "float ", "for ", "goto ", "if ", "int ", "long ",
  "register ", "return", "short ", "signed ", "sizeof ", "static ", "struct ",
  "switch ", "[ ", "\\", "] ", "^ ", "id", "label", "typedef ", "union ",
  "unsigned ", "void ", "volatile ", "while ", "#define ", "#elif ",
  "#else ", "#endif ", "#error ", "#ifdef ", "#if ", "#ifndef ", "#include ",
  "#line ", "#pragma ", "#undef ", "#warning ", "~= ", "== ", "break ", ">= ",
  "<< ", ">> ", "<<= ", "{ ", "| ", "} ", "~ ", "ERR "};

/*
 * <kaneton patch>
 */
#define KANETON

typedef struct		s_exclude
{
  char*			file;

  struct s_exclude*	next;
}			t_exclude;

typedef struct		s_detail
{
  char*			file;

  unsigned int		from;
  unsigned int		to;

  unsigned int		slines;
  unsigned int		dlines;

  unsigned int		run;

  struct s_detail*	next;
}			t_detail;

typedef struct		s_summary
{
  char*			source;
  char*			destination;

  unsigned int		run;
  double		match;

  unsigned int		records;

  struct s_summary*	next;
}			t_summary;

typedef struct		s_file
{
  char*			file;
  unsigned int		ntokens;

  t_detail*		details;
  t_detail*		_tail;

  struct s_file*	next;
}			t_file;

t_exclude*		excludes = NULL;
t_file*			files = NULL;
t_summary*		summaries = NULL;

#define DISPLAY_NONE		0x0
#define DISPLAY_SUMMARY		0x1
#define DISPLAY_DETAIL		0x2

int			display = DISPLAY_NONE;

void		kaneton_exclude(char*		name)
{
  t_exclude*	new;

  if ((new = malloc(sizeof(t_exclude))) == NULL)
    return;

  new->file = name;
  new->next = excludes;

  excludes = new;
}

int		kaneton_include(char*		name)
{
  t_exclude*	tmp;

  for (tmp = excludes; tmp != NULL; tmp = tmp->next)
    if (strstr(name, tmp->file) != NULL)
      return (-1);

  return (0);
}

void		kaneton_file(char*		name)
{
  t_file*	new;

  if ((new = malloc(sizeof(t_file))) == NULL)
    return;

  new->file = name;
  new->ntokens = 0;
  new->details = NULL;
  new->_tail = NULL;

  new->next = files;

  files = new;
}

t_file*		kaneton_retrieve(char*		name)
{
  t_file*	tmp;

  for (tmp = files; tmp != NULL; tmp = tmp->next)
    if (strcmp(name, tmp->file) == 0)
      return (tmp);

  return (NULL);
}

void		kaneton_dump(void)
{
  t_file*	tmp;

  for (tmp = files; tmp != NULL; tmp = tmp->next)
    printf("[%s] %u tokens\n",
	   tmp->file,
	   tmp->ntokens);
}

void		kaneton_token(char*		name,
			      struct token*	token)
{
  t_file*	file;

  if ((file = kaneton_retrieve(name)) == NULL)
    return;

  file->ntokens++;
}

void		kaneton_insert(t_summary*	s)
{
  t_summary*	t;
  t_summary*	p;

  s->next = NULL;

  for (p = NULL, t = summaries; t != NULL; p = t, t = t->next)
    if (s->match > t->match)
      {
	s->next = t;

	if (p != NULL)
	  p->next = s;
	else
	  summaries = s;

	return;
      }

  if (p != NULL)
    p->next = s;
  else
    summaries = s;
}

int		kaneton_filter(t_file*		file,
			       unsigned int	from,
			       char*		destination,
			       unsigned int	to,
			       unsigned int	run)
{
  t_detail*	d;

  for (d = file->details; d != NULL; d = d->next)
    {
      if (strcmp(destination, d->file) == 0)
	{
	  if ((from == d->from) && (run <= d->run))
	    return (-1);

	  if ((to == d->to) && (run <= d->run))
	    return (-1);
	}
    }

  return (0);
}

void		kaneton_record(char*		source,
			       unsigned int	from,
			       unsigned int	slines,
			       char*		destination,
			       unsigned int	to,
			       unsigned int	dlines,
			       unsigned int	run)
{
  t_summary*	summary;
  t_detail*	detail;
  t_file*	file;
  t_summary*	p;
  t_summary*	s;

  if ((kaneton_include(source) != 0) || (kaneton_include(destination) != 0))
    return;

  if ((file = kaneton_retrieve(source)) == NULL)
    return;

  if (kaneton_filter(file, from, destination, to, run) != 0)
    return;

  if ((detail = malloc(sizeof(t_detail))) == NULL)
    return;

  detail->file = destination;
  detail->from = from;
  detail->to = to;
  detail->slines = slines;
  detail->dlines = dlines;
  detail->run = run;
  detail->next = NULL;

  if ((file->details == NULL) && (file->_tail == NULL))
    {
      file->details = detail;
      file->_tail = detail;
    }
  else
    {
      file->_tail->next = detail;

      file->_tail = detail;
    }

  for (p = NULL, s = summaries; s != NULL; p = s, s = s->next)
    if ((strcmp(source, s->source) == 0) &&
	(strcmp(destination, s->destination) == 0))
      {
	s->run += run;
	s->records += 1;
	s->match =
	  ((double)s->run) * 100.0 / ((double)file->ntokens);

	if (p != NULL)
	  p->next = s->next;
	else
	  summaries = s->next;

	kaneton_insert(s);

	return;
      }

  if ((summary = malloc(sizeof(t_summary))) == NULL)
    return;

  summary->source = source;
  summary->destination = destination;
  summary->run = run;
  summary->match =
    ((double)summary->run) * 100.0 / ((double)file->ntokens);
  summary->records = 1;

  kaneton_insert(summary);
}

void		kaneton_summaries(void)
{
  t_summary*	s;

  printf("\n\n\n\t\t\t\t\t\t\t\t----------[ summary ]----------"
	 "\n\n\n");

  for (s = summaries; s != NULL; s = s->next)
    {
      t_file*	file;

      if ((file = kaneton_retrieve(s->source)) == NULL)
	continue;

      printf("-----[%.2f%%]     # %u #\n"
	     "                                  %s\n"
	     "                                  %s\n"
	     "\n",
	     s->match,
	     s->records,
	     s->source,
	     s->destination);
    }
}

void		kaneton_details(void)
{
  t_file*	f;
  t_detail*	d;

  printf("\n\n\n\n\n\t\t\t\t\t\t\t\t----------[ details ]----------"
	 "\n\n\n\n\n");

  for (f = files; f != NULL; f = f->next)
    {
      if (f->details != NULL)
	{
	  for (d = f->details; d != NULL; d = d->next)
	    {
	      printf("-----{%u / %u}\n"
		     "                                  %s:%u\n"
		     "                                  %s:%u\n"
		     "\n",
		     d->run,
		     f->ntokens,
		     f->file,
		     d->from,
		     d->file,
		     d->to);

	      if (crosscompare)
		{
		  paste_files(f->file, d->from, d->slines,
			      d->file, d->to, d->dlines);
		}

	      printf("\n\n\n\n\n");
	    }
	}
    }
}
/*
 * </kaneton patch>
 */

/* The next two functions take the linked list of
 * tokens and build corresponding char token arrays.
 */
int
build_srcchararray(struct token * head, int numtokens)
{
  struct token *T;
  int i;

  /*
   * We add 2 extra tokens on the end plus a NUL to ensure that the ends of
   * both char arrays will be different.
   */
  numtokens += 4;

  Toksrcsize = numtokens;
  Toksrcarray = (char *)malloc(numtokens * sizeof(char) + 1);
  if (Toksrcarray == NULL) {
    fprintf(stderr, "Toksrcarray malloc error in build_srcchararray\n");
    return (0);
  }
  Toksrcref = (struct token **) malloc(numtokens * sizeof(struct token *));
  if (Toksrcref == NULL) {
    fprintf(stderr, "Toksrcref malloc error in build_srcchararray\n");
    return (0);
  }

  for (i = 0, T = head; T != NULL; i++, T = T->next) {
    Toksrcarray[i] = T->type;
    Toksrcref[i] = T;
  }

  /* Now add that stuff at the end */
  Toksrcarray[i] = 0x01;
  Toksrcref[i++] = head;	/* Any ptr to a struct token :-) */
  Toksrcarray[i] = 0x01;
  Toksrcref[i++] = head;	/* Any ptr to a struct token :-) */
  Toksrcarray[i] = 0x0;
  Toksrcref[i++] = head;	/* Any ptr to a struct token :-) */

  return (1);
}

int
build_dstchararray(struct token * head, int numtokens)
{
  struct token *T;
  int i;

  /*
   * We add 2 extra tokens on the end plus a NUL to ensure that the ends of
   * both char arrays will be different.
   */
  numtokens += 4;

  Tokdstsize = numtokens;
  Tokdstarray = (char *)malloc(numtokens * sizeof(char) + 1);
  if (Tokdstarray == NULL) {
    fprintf(stderr, "Tokdstarray malloc error in build_dstchararray\n");
    return (0);
  }
  Tokdstref = (struct token **) malloc(numtokens * sizeof(struct token *));
  if (Tokdstref == NULL) {
    fprintf(stderr, "Tokdstref malloc error in build_dstchararray\n");
    return (0);
  }

  for (i = 0, T = head; T != NULL; i++, T = T->next) {
    Tokdstarray[i] = T->type;
    Tokdstref[i] = T;
  }

  /* Now add that stuff at the end */
  Tokdstarray[i] = 0x02;
  Tokdstref[i++] = head;	/* Any ptr to a struct token :-) */
  Tokdstarray[i] = 0x02;
  Tokdstref[i++] = head;	/* Any ptr to a struct token :-) */
  Tokdstarray[i] = 0x0;
  Tokdstref[i++] = head;	/* Any ptr to a struct token :-) */

  return (1);
}


/*
 * Load the ctf file, building token nodes and putting them into a linked
 * list using the next pointer. The end of each file is represented by an
 * ENDFILE node. Returns a pointer to a linked list of nodes, or NULL if
 * something goes wrong.
 */
struct token *
load_ctffile(char *filename, int issecond)
{
  FILE *zin;
  char buf[BUF_SIZE];
  unsigned char type;
  int i, ch, linenumber = 0;
  int numtokens = 0;
  unsigned short value;
  char *fname = NULL;
  int inside_cpp_line = 0;

  struct token *this = NULL, *last = NULL, *head = NULL;

  /* Open the file */
  zin = fopen(filename, "r");
  if (zin == NULL) {
    fprintf(stderr, "Can't open %s\n", filename);
    return (NULL);
  }

  /* Check the ctf header */
  if ((fgetc(zin) != 'c') || (fgetc(zin) != 't') || (fgetc(zin) != 'f') ||
      (fgetc(zin) != 0x1) || (fgetc(zin) != 0x2)) {
    fprintf(stderr, "File %s is not a valid 1.2 ctf file\n", filename);
    exit(1);
  }

  /* Now loop reading in the file */
  while ((ch = fgetc(zin)) != EOF) {

    switch (ch) {
    case IDENTIFIER:		/* We need to extract the */
    case INTVAL:		/* values of these 2 types */
      value = (fgetc(zin) & 0xff) << 8;
      value += (fgetc(zin) & 0xff);
      break;
    default:
      value = 0;
    }

    switch (ch) {
    case LINE:
      linenumber++;
      inside_cpp_line = 0;
      break;

    case HASHdefine:		/* Ignore C preprocessor tokens */
    case HASHelif:		/* if skip_cppstuff==1 */
    case HASHelse:
    case HASHendif:
    case HASHerror:
    case HASHifdef:
    case HASHif:
    case HASHifndef:
    case HASHinclude:
    case HASHline:
    case HASHpragma:
    case HASHundef:
    case HASHwarning:
      inside_cpp_line = 1;

    default:
      if ((skip_cppstuff == 1) && (inside_cpp_line == 1))
	break;

#ifdef DEBUG
      printf("Got token %s\n", tokstring[ch]);
#endif

      /* Build a token node */
      last = this;
      if (head == NULL)
	head = this;
      this = (struct token *) malloc(sizeof(struct token));
      if (this == NULL) {
        fprintf(stderr, "token node malloc error in load_ctffile\n");
	return (NULL);
      }

      if (last != NULL)
	last->next = this;
      this->next = NULL;

      /* If this is a FILE token, read the filename in */
      if (ch == FILENAME) {
	for (i = 0; i < BUF_SIZE - 1; i++) {
	  if (((ch = fgetc(zin)) == EOF) || (ch == '\0'))
	    break;
	  buf[i] = ch;
	}
	buf[i] = '\0';
	fname = strdup(buf);
#ifdef DEBUG
	printf("Just found filename %s\n", fname);
#endif
	if (issecond)
	  this->type = STARTFILE2;
	else
	  this->type = STARTFILE1;
	linenumber = 1;
	value = 0;

#ifdef KANETON
	/*
	 * kaneton patch
	 */
	kaneton_file(fname);
#endif
      } else {
	this->type = type = (unsigned char)ch;
      }

      this->value = value;
      this->file = fname;
      this->linenum = linenumber;
      last = this;
      numtokens++;

#ifdef KANETON
      /*
       * kaneton patch
       */
      kaneton_token(fname, this);
#endif
    }
  }

  /* The file's finished, close it */
  fclose(zin);

  /* Build an int array for quick comparisons */
  if (issecond) {
    if (build_dstchararray(head, numtokens) == 0)
      return (NULL);
  } else {
    if (build_srcchararray(head, numtokens) == 0)
      return (NULL);
  }
  return (head);
}


/*
 * Print out lines of two files side-by-side. This is best viewed with a
 * terminal window 160 characters wide! If neither file exists, the
 * function simply returns with no error message.
 */
void
paste_files(char *file1, int start1, int num1,
	    char *file2, int start2, int num2)
{
  FILE *f1in, *f2in;
  char buf[1024];
  int count_to_eighty = 0;
  int i, bptr, maxlines;
  int tab_upto = 0;

  f1in = fopen(file1, "r");
  if (f1in == NULL)
    return;
  f2in = fopen(file2, "r");
  if (f2in == NULL) {
    fclose(f1in);
    return;
  }

  printf("------------------------------------------------------------"
	 "------------------------------------------------------------"
	 "---------------------------------------\n\n");

  /* Get maximum number of lines */
  maxlines = (num1 > num2) ? num1 : num2;
  start1--;
  start2--;

  /* Read up to line start1 in file1 */
  while (start1--)
    fgets(buf, 1020, f1in);

  /* Read up to line start2 in file2 */
  while (start2--)
    fgets(buf, 1020, f2in);

  /* Now print out the lines */
  for (i = 0; i < maxlines; i++, num1--, num2--) {

    /* Print the left-hand line */
    if (num1 > 0) {
      fgets(buf, 1020, f1in);
      buf[strlen(buf) - 1] = '\0';	/* Remove newline */

      /*
       * I used to printf("%-80s",buf); here, but it never did what I wanted,
       * so I now print out exactly 80 chars by hand. This is because we have
       * to deal with tabs, dammit!
       */
      tab_upto = 0;
      bptr = 0;
      for (count_to_eighty = 0; count_to_eighty < 80; count_to_eighty++) {
	if (tab_upto > count_to_eighty) {
	  fputc(' ', stdout);
	  continue;
	}
	if (buf[bptr] == '\0')
	  break;
	if (buf[bptr] == '\t') {

	  /* Calculate the next highest multiple of 8 */
	  tab_upto = (count_to_eighty + 8) & ~7;
	  if (tab_upto >= 80)
	    break;
	  fputc(' ', stdout);
	  bptr++;
	  continue;
	}
	fputc(buf[bptr++], stdout);
      }
      for (; count_to_eighty < 80; count_to_eighty++) {
	fputc(' ', stdout);
      }
      printf(" ");
    } else
      printf("%81s", " ");

    /* Print the right-hand line */
    if (num2 > 0) {
      fgets(buf, 1020, f2in);
      fputs(buf, stdout);
    } else
      printf("\n");
  }
  printf("\n");
  if (num1 > num2)
    printf("\n");
  fclose(f1in);
  fclose(f2in);

  printf("------------------------------------------------------------"
	 "------------------------------------------------------------"
	 "---------------------------------------\n");
}


#ifdef ISOMORPH_CODE
/*
 * Here are some comments on the isomorph[] arrays. We would like to identify
 * code fragments that are similar, even if variables have been renamed or
 * even declared in a different order. For example, the following two
 * functions are essentially identical.
 * 
 * int maxofthree(int x, int y, int z)    int bigtriple(int b, int a, int c)
 * {                                      {
 *   if ((x>y) && (x>z)) return(x);          if ((a>b) && (a>c)) return(a);
 *   if (y>z) return(y);                     if (b>c) return(b);
 *   return(z);                              return(c);
 * }                                      }
 * 
 * The tokenised forms of these functions are obviously different
 * 
 * int id0(int id1,int id2,int id3)         int id0(int id1,int id2,int id3)
 * {                                        {
 *  if ((id1>id2)&&(id1>id3))                if ((id2>id1)&&(id2>id3))
 *    return(id1);                             return(id2);
 *  if(id2>id3) return(id2);                 if (id1>id3) return(id1);
 *  return(id3);                             return(id3);
 * }                                        }
 *
 * 
 * However, it is possible to compare these and show that they are identical. If
 * we start with the first "if" statement, a straight lexical comparison
 * shows that they are different. But at the first difference (id1 cf. id2),
 * we put an entry into the isodtos[] array so that isodtos[2]= 1, i.e. the
 * identifier #2 on the RHS is isomorphic to identifier #1 on the LHS. Thus
 * when we get to the first "return" statements, we can see that they are
 * also isomorphic. We can thus conclude that everything from the "if" down
 * to the "}" is isomorphic and thus identical.
 * 
 */

/*
 * isomorphic_compare(): compare two runs of identical tokens to see if they
 * are also isomorphic w.r.t the token values. Returns the number of
 * isomorphic tokens in the run starting at s and d.
 */
int
isomorphic_compare(struct token * s, struct token * d, int runlength)
{
  /* We need two tables: one to record the match from a dst value */
  /* to a src value, and another to record the src to dst value match */
  unsigned short isodtos[65536];
  unsigned short isostod[65536];
  int runcount;

  /* Clear the identifer isomorph table for this run */
  memset(isodtos, 0, sizeof(isodtos));
  memset(isostod, 0, sizeof(isostod));

  /* Walk the token runs from start to end */
  for (runcount = 0; runcount < runlength; runcount++) {
    if (s->type != d->type) {
      printf("Assert error! token mismatch %d vs %d!!\n", s->type, d->type);
      return (runcount);
    }
    switch (s->type) {
    case IDENTIFIER:

      /* First thing, record a mapping each way if there is none */
      if (isodtos[d->value] == 0)
	isodtos[d->value] = s->value;
      if (isostod[s->value] == 0)
	isostod[s->value] = d->value;

      /* Now reject if the mappings fail in either direction */
      if (isodtos[d->value] != s->value)
	return (runcount);
      if (isostod[s->value] != d->value)
	return (runcount);
      break;

    case INTVAL:
      if (d->value != s->value)
	return (runcount);
      break;
    }
    s = s->next;
    d = d->next;
  }
  return (runcount);
}
#endif


/* We have a possible run starting at srcstart and dststart */
/* but this could not be a run due to Rabin-Karp hashing or */
/* for other reasons. Verify that this is indeed a run. */

void 
verify_run(int srcstart, int dststart)
{
  int i, s, d;
  int runcount, isoruncount;
  int lastline;
  struct token *T;
#ifdef OPT_OVERLAP
  struct tokrun *this;
  int found;
#endif

  /* We must now manually walk both arrays to get a real count */
  /* of the length of the token run. */
  s = srcstart;
  d = dststart;

  for (runcount = 0; Toksrcarray[s] == Tokdstarray[d]; runcount++) {
    s++; d++;
  }

  if (runcount < threshold)
    return;

#ifdef OPT_OVERLAP
  /* First check and ignore this if it's already recorded */
  found = 0;
  for (this = Runhead; this != NULL; this = this->next) {
    if ((srcstart >= this->sstart) && (s <= this->send) &&
	(dststart >= this->dstart) && (d <= this->dend)) {
      /* printf("Found an existing tokrun!\n"); */
      found = 1;
      break;
    }
  }
  if (found == 1) return;

  /* We record the run in the tokrun list */
  this = (struct tokrun *) malloc(sizeof(struct tokrun));
  if (this != NULL) {
    this->sstart = srcstart;
    this->send = s;
    this->dstart = dststart;
    this->dend = d;
    this->next = NULL;

    /* Link into the list */
    /* printf("Adding tokrun node %d-%d  %d-%d\n",srcstart,s,dststart,d); */

    if (Runhead == NULL) {
      Runhead = Runtail = this;
    } else {
      Runtail->next = this;
      Runtail = this;
    }
  }
#endif

  /* At this point we have a possible token run of at least */
  /* threshold in length starting at dststart. We now need to */
  /* apply some filtering to weed out lots of false positives. */

#ifdef ISOMORPH_CODE
  isoruncount = isomorphic_compare(Toksrcref[srcstart],
				   Tokdstref[dststart], runcount);

  if (isoruncount < threshold)
    return;

  /* isomorphic_compare() might have found a run shorter than that */
  /* which we found in the beginning. Modify runcount to reflect this. */
  runcount = isoruncount;
#endif

#ifdef KANETON
  /*
   * kaneton patch
   */
  kaneton_record(Toksrcref[srcstart]->file, Toksrcref[srcstart]->linenum,
		 Toksrcref[s]->linenum - Toksrcref[srcstart]->linenum + 1,
		 Tokdstref[dststart]->file, Tokdstref[dststart]->linenum,
		 Tokdstref[d]->linenum - Tokdstref[dststart]->linenum + 1,
		 runcount);
#else
  printf("%d  %s:%d  %s:%d\n",
         runcount,
         Toksrcref[srcstart]->file,
         Toksrcref[srcstart]->linenum,
         Tokdstref[dststart]->file,
         Tokdstref[dststart]->linenum);
#endif

  if (showtokens) {
    /* Now print out the list of tokens */
    lastline = Toksrcref[srcstart]->linenum;
    for (s = srcstart, i = 0; i < runcount; i++, s++) {
      T = Toksrcref[s];
      if (Toksrcref[s]->linenum > lastline) {
	lastline = Toksrcref[s]->linenum;
	fputc('\n', stdout);
      }
      fputs(tokstring[T->type], stdout);
      switch (T->type) {
      case IDENTIFIER:
      case INTVAL:
	printf("%d ", T->value);
	break;
      }
    }
    fputc('\n', stdout);
    fputc('\n', stdout);
  }

#ifndef KANETON
  if (crosscompare) {
    paste_files(Toksrcref[srcstart]->file, Toksrcref[srcstart]->linenum,
		Toksrcref[s]->linenum - Toksrcref[srcstart]->linenum + 1,
		Tokdstref[dststart]->file, Tokdstref[dststart]->linenum,
		Tokdstref[d]->linenum - Tokdstref[dststart]->linenum + 1);
  }
#endif
}

/* We now use the Rabin-Karp algorithm to find potential
 * matches on the string starting at srcstart and which
 * are at least threshold tokens long. See this URL:
 * http://www-igm.univ-mlv.fr/~lecroq/string/node5.html
 * Thanks also to Roger Willcocks <roger@rops.org> who
 * pointed the algorithm out to me.
 */

#define REHASH(a, b, h) ((((h) - (a)*d) << 1) + (b))

/* Because the Tokdstarray never changes, we can precompute an
 * array of hy hashes instead of doing it inside the KR loop.
 */
int *HY;

int
precompute_hyarray(void)
{
  int d, hy, i, j;
  unsigned char *y;

  y = Tokdstarray;
  HY = (int *)malloc(Tokdstsize * sizeof(int));
  if (HY == NULL) {
    fprintf(stderr, "HY malloc error in precompute_hyarray\n");
    return (0);
  }

  /* Preprocessing */
  /*
   * computes d = 2^(m-1) with the left-shift operator
   */
  for (d = i = 1; i < threshold; ++i)
    d = (d << 1);

  /* Build the first hash value */
  for (hy = i = 0; i < threshold; ++i) {
    hy = ((hy << 1) + y[i]);
  }
  HY[0] = hy;

  /* And then build the progressive hashes */
  /* for consecutive token runs */
  j = 0;
  while (j <= Tokdstsize - threshold) {
    hy = REHASH(y[j], y[j + threshold], hy);
    j++;
    HY[j] = hy;
  }
  return (1);
}



/*
 * Compare the two source trees. We walk the linked token list we are given,
 * and then call another function one token at a time to find matches.
 */

void
compare_trees(void)
{
  unsigned int srcstart;
  unsigned int d, hx, i, j;

#ifdef OPT_OVERLAP
  struct tokrun *prev, *this;
#endif

  /* Precompute the value of d used by Rabin-Karp */
  /* d = 2^(threshold-1) */
  for (d = i = 1; i < threshold; ++i)
    d = (d << 1);

  /* Walk the Toksrcarray[] to search for strings */
  /* that start at srcstart. */

  for (srcstart = 0; srcstart < Toksrcsize - threshold; srcstart++) {

    if ((progress == 1) && (Toksrcref[srcstart]->type == STARTFILE1)) {
      fprintf(stderr, "Processing src file %s\n", Toksrcref[srcstart]->file);
      continue;
    }

#ifdef OPT_OVERLAP
    /* Clean out the old tokrun list if this is a new srcfile */
    if (Toksrcref[srcstart]->type == STARTFILE1) {
      for (this = Runhead; this != NULL; this = prev) {
	/* printf("Removing tokrun node 0x%x\n", (unsigned int)this); */
	prev = this->next;
	free(this);
      }
      Runhead = Runtail = NULL;
    }
#endif

    /* Build the Rabin-Karp hash of the source token array */
    /* starting at srcstart and going for threshold length */
    for (hx = i = 0; i < threshold; i++) {
      hx = ((hx << 1) + Toksrcarray[srcstart+i]);
    }

 
    /* Search for a matching hash for this run in the HY array. */
    /* If one is found, then proceed to verify it as a real run */
    /* which is isomorphically correct. */
    j = 0;
    for (j=0; j <= Tokdstsize - threshold; j++) {
      if (hx == HY[j])
      verify_run(srcstart, j);
    }
  }
}

void
usage(void)
{
  fprintf(stderr,
       "Usage: ctcompare [-p] [-t] [-x] [-i] [-n nnn] cft_file ctf_file\n");
  fprintf(stderr, "\t-p: print progress reports to stderr\n");
  fprintf(stderr, "\t-t: show matching tokens when match is found\n");
  fprintf(stderr, "\t-x: show matching source lines when match is found\n");
  fprintf(stderr, "\t-i: also match C preprocessor tokens (#include etc.)\n");
  fprintf(stderr, "\t-n nnn: set the minimum matching run length to nnn\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "\t-s: display results in a summary\n");
  fprintf(stderr, "\t-d: display detailed results\n");

  exit(1);
}


int
main(int argc, char *argv[])
{
  struct token *Srctoklist, *Dsttoklist;
  int ch;

  /* Process options */
  while ((ch = getopt(argc, argv, "ptxin:sde:")) != -1) {
    switch (ch) {
    case 'p':
      progress = 1;
      break;
    case 'i':
      skip_cppstuff = 0;
      break;
    case 't':
      if (crosscompare) {
	fprintf(stderr, "-t and -x are mutually exclusive\n");
	crosscompare = 0;
      }
      showtokens = 1;
      break;
    case 'x':
      if (showtokens) {
	fprintf(stderr, "-t and -x are mutually exclusive\n");
	showtokens = 0;
      }
      crosscompare = 1;
      break;
    case 'n':
      threshold = atoi(optarg);
      if (threshold <= 0) {
	fprintf(stderr, "Bad value for -n\n");
	threshold = 10;
      }
      break;

#ifdef KANETON
      /*
       * kaneton patch
       */
    case 's':
      display |= DISPLAY_SUMMARY;
      break;
    case 'd':
      display |= DISPLAY_DETAIL;
      break;
    case 'e':
      kaneton_exclude(optarg);
      break;
#endif

    default:
      usage();
    }
  }
  argc -= optind;
  argv += optind;

  if (argc != 2)
    usage();

  Srctoklist = load_ctffile(argv[0], 0);
  if (Srctoklist == NULL) {
    fprintf(stderr, "Error reading ctf file %s\n", argv[0]);
    exit(1);
  }
  Dsttoklist = load_ctffile(argv[1], 1);
  if (Dsttoklist == NULL) {
    fprintf(stderr, "Error reading ctf file %s\n", argv[1]);
    exit(1);
  }
  if (precompute_hyarray() == 0) {
    fprintf(stderr, "Error precomputing HY array\n");
    exit(1);
  }
  compare_trees();

#ifdef KANETON
  /*
   * kaneton patch
   */
  if (display & DISPLAY_SUMMARY)
    kaneton_summaries();
  if ((display & DISPLAY_DETAIL) || (display == DISPLAY_NONE))
    kaneton_details();
#endif

  exit(0);
}
