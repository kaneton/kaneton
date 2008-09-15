/* Each key entry in the database file holds
 * a number of these records. Each record
 * indicates where a ctf file matches the
 * given rolling hash key.
 */
typedef struct {
  uint32_t  offset;	 /* Offset where this tuple of tokens occurs */
  uint32_t  name_offset; /* Offset of the filename which has this tuple */
  uint32_t  fileid;	 /* File identifier: external LUT gives CTF filename */
  uint32_t  line_number; /* Line number where first token in tuple occurs */
} Ctfhashnode;

/* Names of the database files we create */
extern char* FILEID_DB;
extern char* CTFNODE_DB;

#define TUPLE_SIZE 16

/* Prototypes */
int add_fileidname(char *name);
int get_fileidnames(char ***outarray);
int add_ctfnode(uint8_t * tuple, Ctfhashnode * value);
void close_ctfnodedb(void);
int get_firstkey(uint8_t ** tuple, Ctfhashnode ** value, int *size);
int get_nextkey(uint8_t ** tuple, Ctfhashnode ** value, int *size);
