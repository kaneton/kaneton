/*
 * db4files: Functions that deal with Berkeley DB files (version >= 4.1) and
 * the nodes contained in them.
 * Copyright (c) Warren Toomey, under the GPL3 license.
 * Copyright (c) 2008 Michael Stefaniuc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <db.h>
#include <limits.h>
#include "dbfiles.h"

/***** CHANGE THE FOLLOWING TO ALTER THE DATABASE RAM CACHE ******/
#define DB_CACHE_SIZE	300	/* Measured in megabytes */

/* Static variables, used to hold state here. */

static DB *ctf_dbf = NULL;	/* File handle for ctfnode.db file */
static DBC *ctf_cursor = NULL;	/* Cursor for ctfnode.db file */

/*** Fileid functions ***/

/*
 * Given an open db file handle, return the highest fileid in the database.
 * Also check the existence of the given filename.
 */
static int get_highest_fileid(DB * dbf, char *name)
{
  DBC *dbc;
  static DBT key, val;
  int *id, fileid = 0;		/* Highest fileid found */

  dbf->cursor(dbf, NULL, &dbc, 0);
  /* Find all the keys in the database: they are the file-ids */
  while (dbc->c_get(dbc, &key, &val, DB_NEXT) == 0) {
    id = (int *)key.data;

    /* Ensure the ctf filename is not in there already */
    if (name != NULL) {
      if (val.data && (!strcmp((char *)val.data, name))) {
	fprintf(stderr,
		"ctf file %s is already in the database as id %d, exiting\n",
		name, *id);
	dbc->c_close(dbc);
	dbf->close(dbf, 0);
	exit(1);
      }
    }

    /* Find and update the highest fileid found */
    /* printf("Found fileid %d: %s\n", *id, val.dptr); */
    if (*id > fileid)
      fileid = *id;
  }

  /* Close the cursor and return the fileid */
  dbc->c_close(dbc);
  return (fileid);
}

/*
 * Given a filename, add it to the fileid database and return the fileid. On
 * failure, return the value 0.
 */
int add_fileidname(char *name)
{
  DB *dbf;			/* File handle */
  int fileid;			/* New fileid to add to the database */
  static DBT key;		/* Key retrieved and added */
  static DBT val;		/* Value added */
  int result;

  /* Reject NULL or empty strings */
  if (name == NULL || strlen(name) == 0)
    return (0);

  /* Open up the database for read/write */
  if ((result = db_create(&dbf, NULL, 0)) != 0 ||
      (result = dbf->open(dbf, NULL, FILEID_DB, NULL, DB_HASH, DB_CREATE,
			  0666)) != 0) {
    fprintf(stderr, "Unable to open the %s database: %s\n", FILEID_DB,
	    db_strerror(result));
    exit(1);
  }

  /* Now write a new fileid to the database */
  fileid = get_highest_fileid(dbf, name) + 1;
  key.data = (char *)&fileid;
  key.size = sizeof(fileid);
  val.data = name;
  val.size = strlen(name) + 1;	/* Ensure we add the \0 */

  if ((result = dbf->put(dbf, NULL, &key, &val, 0)) != 0) {
    fprintf(stderr, "Unable to insert %d:%s into the %s database: %s\n",
	    fileid, name, FILEID_DB, db_strerror(result));
    dbf->close(dbf, 0);
    exit(1);
  }
  dbf->close(dbf, 0);
  return (fileid);
}

/*
 * Return a pointer to an array of filenames in the database ordered by their
 * fileid, and the number of fileids (including the unused zero one).
 */
int get_fileidnames(char ***outarray)
{
  DB *dbf;			/* File handle */
  char **namearray;
  static DBT key, val;
  int fileid, numfileids, result;

  if (outarray == NULL)
    return (0);

  /* Open up the database for reading */
  if ((result = db_create(&dbf, NULL, 0)) != 0 ||
      (result = dbf->open(dbf, NULL, FILEID_DB, NULL, DB_HASH, DB_RDONLY,
			  0666)) != 0) {
    fprintf(stderr, "Unable to open the %s database: %s\n", FILEID_DB,
	    db_strerror(result));
    exit(1);
  }

  /* Get the number of fileids in the database */
  numfileids = get_highest_fileid(dbf, NULL) + 1;

  /* Allocate an array to hold all the file names */
  namearray = (char **)malloc(numfileids * sizeof(char *));

  /* Now read in each of the names */
  key.data = (char *)&fileid;
  key.size = sizeof(fileid);
  for (fileid = 1; fileid < numfileids; fileid++) {
    dbf->get(dbf, NULL, &key, &val, 0);
    namearray[fileid] = strdup(val.data);
  }

  *outarray = namearray;
  dbf->close(dbf, 0);
  return (numfileids);
}

/*
 * Open the ctfnode.db with the given parameter, and save the filehandle in
 * the global variable. On failure, print out an error message and kill the
 * program.
 */
static void open_ctfnodedb(int flags)
{
  int result;

  if (ctf_dbf != NULL) {
    fprintf(stderr, "Cannot re-open already open file %s, exiting\n",
	    CTFNODE_DB);
    exit(1);
  }

  if ((result = db_create(&ctf_dbf, NULL, 0)) == 0) {
    /* Set the amount of RAM used for the cache */
    ctf_dbf->set_cachesize(ctf_dbf, 0, DB_CACHE_SIZE * 1024 * 1024, 1);
    result = ctf_dbf->open(ctf_dbf, NULL, CTFNODE_DB, NULL, DB_HASH,
			   flags, 0666);
  }

  if (result != 0) {
    fprintf(stderr, "Unable to open the %s database: %s\n", CTFNODE_DB,
	    db_strerror(result));
    exit(1);
  }
}

/*
 * Create and add a new ctf node into the ctfnode file. We need to check if
 * the key is already in use. If so, read the existing data, delete the old
 * node, augment the new node and write it back. Returns 1 if OK, 0 if fail.
 */
int add_ctfnode(uint8_t * tuple, Ctfhashnode * value)
{
  static DBT key, val, oldval;
  char *newptr = NULL, *insertposn;
  int result;

  /* Inputs must exist */
  if ((tuple == NULL) || (value == NULL))
    return (0);

  key.data = (char *)tuple;
  key.size = TUPLE_SIZE + sizeof(uint16_t);
  val.data = (char *)value;
  val.size = sizeof(Ctfhashnode);

  /* Open the ctfnode.db file */
  if (ctf_dbf == NULL)
    open_ctfnodedb(DB_CREATE);

  /* Get any old value */
  if (ctf_dbf->get(ctf_dbf, NULL, &key, &oldval, 0) == 0) {
#if 0
    int	i;
    uint8_t *k = (uint8_t *) key.data;
    printf("Found existing val for key ");
    for (i = 0; i < TUPLE_SIZE; i++)
      printf("%02x", k[i]);
    printf(", size %d\n", oldval.size);
#endif

    /* Malloc and append the val's Ctfhashnode to the end of oldval's data */
    /* XXX: It used to be realloc() with gdbm, how do we do it with Berk DB? */
    newptr = malloc(oldval.size + sizeof(Ctfhashnode));
    if (newptr == NULL) {
      return (0);
    }
    memcpy(newptr, oldval.data, oldval.size);
    insertposn = newptr + oldval.size;
    memcpy(insertposn, val.data, sizeof(Ctfhashnode));
    val.data = newptr;
    val.size = oldval.size + sizeof(Ctfhashnode);
  } else {
#if 0
    int	i;
    uint8_t *k = (uint8_t *) key.data;
    printf("Storing first val for key ");
    for (i = 0; i < TUPLE_SIZE; i++)
      printf("%02x", k[i]);
    printf("\n");
#endif
  }

  result = ctf_dbf->put(ctf_dbf, NULL, &key, &val, 0);
  if (newptr)
    free(newptr);
  if (result != 0)
    return (0);
  return (1);
}


/* Close the ctf database */
void close_ctfnodedb(void)
{
  if (ctf_cursor) {
    ctf_cursor->c_close(ctf_cursor);
    ctf_cursor = NULL;
  }
  if (ctf_dbf) {
    ctf_dbf->close(ctf_dbf, 0);
    ctf_dbf = NULL;
  }
}

/*
 * Get the first key in the database and its value. Size is # Ctfhashnodes in
 * the value. Returns 1 if OK, 0 otherwise.
 */
int get_firstkey(uint8_t ** tuple, Ctfhashnode ** value, int *size)
{
  static DBT key, val;
  int result;

  /* Inputs must exist */
  if ((tuple == NULL) || (value == NULL))
    return (0);

  /* Open the ctfnode.db file */
  if (ctf_dbf == NULL)
    open_ctfnodedb(DB_RDONLY);

  /* Create the cursor if it doesn't exist already */
  if (ctf_cursor == NULL)
    ctf_dbf->cursor(ctf_dbf, NULL, &ctf_cursor, 0);

  result = ctf_cursor->c_get(ctf_cursor, &key, &val, DB_FIRST);
  if (result != 0)
    return (0);

  *tuple = (uint8_t *) key.data;
  *value = (Ctfhashnode *) val.data;
  *size = val.size / sizeof(Ctfhashnode);
  return (1);
}

/*
 * Get the next key in the database and its value. Size is # Ctfhashnodes in
 * the value. Returns 1 if OK, 0 otherwise.
 */
int get_nextkey(uint8_t ** tuple, Ctfhashnode ** value, int *size)
{
  static DBT key, val;

  /* Inputs must exist */
  if ((tuple == NULL) || (value == NULL))
    return (0);

  /* Open the ctfnode.db file */
  if (ctf_dbf == NULL)
    open_ctfnodedb(DB_RDONLY);

  /* Create the cursor if it doesn't exist already */
  if (ctf_cursor == NULL)
    ctf_dbf->cursor(ctf_dbf, NULL, &ctf_cursor, 0);

  if (ctf_cursor->c_get(ctf_cursor, &key, &val, DB_NEXT) != 0)
    return (0);

  *tuple = (uint8_t *) key.data;
  *value = (Ctfhashnode *) val.data;
  *size = val.size / sizeof(Ctfhashnode);
  return (1);
}
