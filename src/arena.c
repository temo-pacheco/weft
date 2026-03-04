
#line 134 "literate/architecture.weft"
/* {13: literate/architecture.weft:134} */
#include "global.h"
/* {:13} */

#line 37 "literate/arena.weft"
/* {394: literate/arena.weft:37} */
typedef struct chunk {
  struct chunk *next;
  char *limit;
  char *avail;
} Chunk;
/* {:394} */

#line 51 "literate/arena.weft"
/* {395: literate/arena.weft:51} */
static Chunk first = { NULL, NULL, NULL };
static Chunk *arena = &first;
/* {:395} */

#line 65 "literate/arena.weft"
/* {396: literate/arena.weft:65} */
void *arena_getmem(size_t n)
{
  char *q;
  char *p = arena->avail;
  n = (n + 7) & ~7;             /* ensuring alignment to 8 bytes */
  q = p + n;
  if (q <= arena->limit) {
    arena->avail = q;
    return p;
  }
  /* Find a new chunk of memory */
  
#line 86 "literate/arena.weft"
  /* {397: literate/arena.weft:86} */
{
    Chunk *ap = arena;
    Chunk *np = ap->next;
    while (np) {
      char *v = sizeof(Chunk) + (char *) np;
      if (v + n <= np->limit) {
        np->avail = v + n;
        arena = np;
        return v;
      }
      ap = np;
      np = ap->next;
    }
    /* Allocate a new chunk of memory */
    
#line 106 "literate/arena.weft"
    /* {398: literate/arena.weft:106} */
{
      size_t m = n + 10000;
      np = (Chunk *) malloc(m);
      np->limit = m + (char *) np;
      np->avail = n + sizeof(Chunk) + (char *) np;
      np->next = NULL;
      ap->next = np;
      arena = np;
      return sizeof(Chunk) + (char *) np;
    }/* {:398} */

#line 99 "literate/arena.weft"

  }/* {:397} */

#line 75 "literate/arena.weft"

}
/* {:396} */

#line 123 "literate/arena.weft"
/* {399: literate/arena.weft:123} */
void arena_free(void)
{
  arena = &first;
  arglist_reset();
}
/* {:399} */
