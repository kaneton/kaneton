/* NOTE: This is a modified version of avltree, originally written by
 * Gregory Tseytin. The code was modified to have an integer key.
 * There does not seem to be a web site for the project: I obtained
 * the code from the FreeBSD ports area:
 *      http://www.gtlib.gatech.edu/pub/FreeBSD/distfiles/
 *
 * Warren
 */

#include <stdint.h>
#include "dbfiles.h"

/*
 * We keep an AVL tree of confirmed token runs found so far, to ensure that
 * we don't print multiple run subsets or overlaps.
 */
struct run {
  Ctfhashnode *src;		/* Details of the src run start, line start */
  Ctfhashnode *dst;		/* Details of the dst run start, line start */
  uint32_t runlength;		/* Length of the run in tokens */
  uint32_t src_end_offset;	/* End offset of the run in the src tree */
  uint32_t dst_end_offset;	/* End offset of the run in the dst tree */
  uint32_t src_last_line_number; /* Last line number of the run in src tree */
  uint32_t dst_last_line_number; /* Last line number of the run in dst tree */
  struct run *next;
};

typedef struct {
  struct run *recptr;
  unsigned int count;
  uint32_t key;
} rectype;

typedef rectype AVL_IX_REC;

typedef struct {
  void *root;
}   AVL_IX_DESC;

/* return codes  */
#define AVL_IX_OK	1
#define AVL_IX_FAIL	0
#define AVL_EOIX	(-2)

extern void avl_create_index(AVL_IX_DESC * pix);
extern void avl_destroy_index(AVL_IX_DESC * pix);
extern int avl_find_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern int avl_locate_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern int avl_add_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern int avl_delete_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern void avl_first_key(AVL_IX_DESC * pix);
extern void avl_last_key(AVL_IX_DESC * pix);
extern int avl_next_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern int avl_prev_key(AVL_IX_REC * pe, AVL_IX_DESC * pix);
extern int avl_find_exact(AVL_IX_REC * pe, AVL_IX_DESC * pix);
