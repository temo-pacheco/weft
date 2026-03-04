
#line 121 "literate/architecture.weft"
/* {11: literate/architecture.weft:121} */
#include "global.h"
/* {:11} */

#line 5 "literate/scraps.weft"
/* {225: literate/scraps.weft:5} */
#define SLAB_SIZE 1024

typedef struct slab {
  struct slab *next;
  char chars[SLAB_SIZE];
} Slab;
/* {:225} */

#line 14 "literate/scraps.weft"
/* {226: literate/scraps.weft:14} */
typedef struct {
  char *file_name;
  Slab *slab;
  struct uses *uses;
  struct uses *defs;
  int file_line;
  int page;
  char letter;
  unsigned char sector;
} ScrapEntry;
/* {:226} */

#line 29 "literate/scraps.weft"
/* {227: literate/scraps.weft:29} */

#define SCRAP_BITS 10
#define SCRAP_SIZE (1<<SCRAP_BITS)
#define SCRAP_MASK (SCRAP_SIZE - 1)
#define SCRAP_SHIFT SCRAP_BITS
static ScrapEntry *SCRAP[SCRAP_SIZE];

#define scrap_array(i) SCRAP[(i) >> SCRAP_SHIFT][(i) & SCRAP_MASK]

static int scraps;
int num_scraps(void)
{
   return scraps;
}
/* Forward declarations for scraps.c */

#line 624 "literate/scraps.weft"
/* {267: literate/scraps.weft:624} */
int delayed_indent = 0;
/* {:267} */

#line 628 "literate/scraps.weft"
/* {268: literate/scraps.weft:628} */

#define MAX_ARGLISTS 4096
static Arglist *arglist_table[MAX_ARGLISTS];
static int arglist_count = 0;

void arglist_reset(void)
{
  arglist_count = 0;
}

static int arglist_register(Arglist *a)
{
  if (arglist_count >= MAX_ARGLISTS) {
    fprintf(stderr, "%s: too many macro argument lists (max %d)\n",
            command_name, MAX_ARGLISTS);
    exit(EXIT_FAILURE);
  }
  arglist_table[arglist_count] = a;
  return arglist_count++;
}

static Arglist *arglist_lookup(int idx)
{
  if (idx < 0 || idx >= arglist_count) {
    fprintf(stderr, "%s: invalid arglist index %d\n", command_name, idx);
    exit(EXIT_FAILURE);
  }
  return arglist_table[idx];
}
/* {:268} */

#line 956 "literate/scraps.weft"
/* {287: literate/scraps.weft:956} */
static void
comment_ArglistElement(FILE * file, Arglist * args, int quote)
{
  Name *name = args->name;
  Arglist *q = args->args;

  if (name == NULL) {
    if (quote)
       fprintf(file, "%c'%s%c'", nw_char, (char *)q, nw_char);
    else
       fprintf(file, "'%s'", (char *)q);
  } else if (name == (Name *)1) {
     /* Include an embedded scrap in comment */
     
#line 976 "literate/scraps.weft"
     /* {288: literate/scraps.weft:976} */
Embed_Node * e = (Embed_Node *)q;
     fputc('{', file);
     write_scraps(file, "", e->defs, -1, "", 0, 0, 0, 0, 0, e->args, NULL, NULL, "");
     fputc('}', file);/* {:288} */

#line 968 "literate/scraps.weft"

  } else {
     /* Include a fragment use in comment */
     
#line 982 "literate/scraps.weft"
     /* {289: literate/scraps.weft:982} */
char * p = name->spelling;
     if (quote)
        fputc(nw_char, file);
     fputc('<', file);
     if (quote && name->sector == 0)
        fputc('+', file);
     while (*p != '\000') {
       if (*p == ARG_CHR) {
         comment_ArglistElement(file, q, quote);
         q = q->next;
         p++;
       }
       else
          fputc(*p++, file);
     }
     if (quote)
        fputc(nw_char, file);
     fputc('>', file);/* {:289} */

#line 970 "literate/scraps.weft"

  }
}
/* {:287} */

#line 1059 "literate/names.weft"
/* {320: literate/names.weft:1059} */
char * comment_begin[6] = { "", "/* ", "// ", "# ", "-- ", "<!-- "};
char * comment_mid[6] = { "", " * ", "// ", "# ", "-- ", "     "};
char * comment_end[6] = { "", " */", "", "", "", " -->"};
/* {:320} */

#line 389 "literate/search-labels.weft"
/* {365: literate/search-labels.weft:389} */

static void add_uses(Uses **root, Name *name);
static int scrap_is_in(Scrap_Node *list, int i);
/* {:365} */

#line 43 "literate/scraps.weft"

/* {:227} */

#line 60 "literate/scraps.weft"
/* {229: literate/scraps.weft:60} */
void init_scraps(void)
{
  scraps = 1;
  SCRAP[0] = (ScrapEntry *) arena_getmem(SCRAP_SIZE * sizeof(ScrapEntry));
}
/* {:229} */

#line 68 "literate/scraps.weft"
/* {230: literate/scraps.weft:68} */
void write_scrap_ref(FILE *file, int num, int first, int *page)
{
  if (scrap_array(num).page >= 0) {
    if (first!=0)
      fprintf(file, "%d", scrap_array(num).page);
    else if (scrap_array(num).page != *page)
      fprintf(file, ", %d", scrap_array(num).page);
    if (scrap_array(num).letter > 0)
      fputc(scrap_array(num).letter, file);
  }
  else {
    if (first!=0)
      putc('?', file);
    else
      fputs(", ?", file);
    /* Warn (only once) about needing to rerun after Latex */
    
#line 131 "literate/scraps.weft"
    /* {237: literate/scraps.weft:131} */
{
      if (!already_warned) {
        fprintf(stderr, "%s: you'll need to rerun weft after running latex\n",
                command_name);
        already_warned = TRUE;
      }
    }/* {:237} */

#line 83 "literate/scraps.weft"

  }
  if (first>=0)
  *page = scrap_array(num).page;
}
/* {:230} */

#line 91 "literate/scraps.weft"
/* {231: literate/scraps.weft:91} */
void write_single_scrap_ref(FILE *file, int num)
{
  int page;
  write_scrap_ref(file, num, TRUE, &page);
}
/* {:231} */

#line 99 "literate/scraps.weft"
/* {232: literate/scraps.weft:99} */
Uses * get_scrap_uses(int scrap)
{
  return scrap_array(scrap).uses;
}
/* {:232} */

#line 106 "literate/scraps.weft"
/* {233: literate/scraps.weft:106} */
Uses * get_scrap_defs(int scrap)
{
  return scrap_array(scrap).defs;
}
/* {:233} */

#line 123 "literate/scraps.weft"
/* {235: literate/scraps.weft:123} */
const char *scrap_file_name(int i) { return scrap_array(i).file_name; }
/* {:235} */

#line 127 "literate/scraps.weft"
/* {236: literate/scraps.weft:127} */
int scrap_file_line(int i) { return scrap_array(i).file_line; }
/* {:236} */

#line 148 "literate/scraps.weft"
/* {240: literate/scraps.weft:148} */
typedef struct {
  Slab *scrap;
  Slab *prev;
  int index;
} Manager;
/* {:240} */

#line 158 "literate/scraps.weft"
/* {241: literate/scraps.weft:158} */
static void push(char c, Manager *manager)
{
  Slab *scrap = manager->scrap;
  int index = manager->index;
  scrap->chars[index++] = c;
  if (index == SLAB_SIZE) {
    Slab *new_slab = (Slab *) arena_getmem(sizeof(Slab));
    scrap->next = new_slab;
    manager->scrap = new_slab;
    index = 0;
  }
  manager->index = index;
}
/* {:241} */

#line 174 "literate/scraps.weft"
/* {242: literate/scraps.weft:174} */
static void pushs(char *s, Manager *manager)
{
  while (*s)
    push(*s++, manager);
}
/* {:242} */

#line 182 "literate/scraps.weft"
/* {243: literate/scraps.weft:182} */
int collect_scrap(void)
{
  int current_scrap, lblseq = 0;
  int depth = 1;
  Manager writer;
  /* Create new scrap, managed by \verb|writer| */
  
#line 193 "literate/scraps.weft"
  /* {244: literate/scraps.weft:193} */
{
    Slab *scrap = (Slab *) arena_getmem(sizeof(Slab));
    if ((scraps & SCRAP_MASK) == 0)
      SCRAP[scraps >> SCRAP_SHIFT] = (ScrapEntry *) arena_getmem(SCRAP_SIZE * sizeof(ScrapEntry));
    scrap_array(scraps).slab = scrap;
    scrap_array(scraps).file_name = save_string(source_name);
    scrap_array(scraps).file_line = source_line;
    scrap_array(scraps).page = -1;
    scrap_array(scraps).letter = 0;
    scrap_array(scraps).uses = NULL;
    scrap_array(scraps).defs = NULL;
    scrap_array(scraps).sector = current_sector;
    writer.scrap = scrap;
    writer.index = 0;
    current_scrap = scraps++;
  }/* {:244} */

#line 187 "literate/scraps.weft"

  /* Accumulate scrap and return \verb|scraps++| */
  
#line 212 "literate/scraps.weft"
  /* {245: literate/scraps.weft:212} */
{
    int c = source_get();
    while (1) {
      switch (c) {
        case EOF: fprintf(stderr, "%s: unexpect EOF in (%s, %d)\n",
                          command_name, scrap_array(current_scrap).file_name,
                          scrap_array(current_scrap).file_line);
                  exit(-1);
        default:
          if (c==nw_char)
            {
              /* Handle at-sign during scrap accumulation */
              
#line 234 "literate/scraps.weft"
              /* {246: literate/scraps.weft:234} */
{
                c = source_get();
                switch (c) {
                  case '(':
                  case '[':
                  case '{': depth++;
                            break;
                  case '+':
                  case '-':
                  case '*':
                  case '|': 
#line 301 "literate/scraps.weft"
                            /* {249: literate/scraps.weft:301} */
{
                              do {
                                int type = c;
                                do {
                                  char new_name[MAX_NAME_LEN];
                                  char *p = new_name;
                                  unsigned int sector = 0;
                                  do
                                    c = source_get();
                                  while (isspace(c));
                                  if (c != nw_char) {
                                    Name *name;
                                    do {
                                      *p++ = c;
                                      c = source_get();
                                    } while (c != nw_char && !isspace(c));
                                    *p = '\0';
                                    switch (type) {
                                    case '*':
                                       sector = current_sector;
                                       /* Add user identifier use */
                                       
#line 345 "literate/scraps.weft"
                                       /* {250: literate/scraps.weft:345} */
name = name_add(&user_names, new_name, sector);
                                       if (!name->uses || name->uses->scrap != current_scrap) {
                                         Scrap_Node *use = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         use->scrap = current_scrap;
                                         use->next = name->uses;
                                         name->uses = use;
                                         add_uses(&(scrap_array(current_scrap).uses), name);
                                       }/* {:250} */

#line 321 "literate/scraps.weft"

                                       break;
                                    case '-':
                                       /* Add user identifier use */
                                       
#line 345 "literate/scraps.weft"
                                       /* {250: literate/scraps.weft:345} */
name = name_add(&user_names, new_name, sector);
                                       if (!name->uses || name->uses->scrap != current_scrap) {
                                         Scrap_Node *use = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         use->scrap = current_scrap;
                                         use->next = name->uses;
                                         name->uses = use;
                                         add_uses(&(scrap_array(current_scrap).uses), name);
                                       }/* {:250} */

#line 324 "literate/scraps.weft"

                                       /* Fall through */
                                    case '|':
                                       sector = current_sector;
                                       /* Fall through */
                                    case '+':
                                       /* Add user identifier definition */
                                       
#line 355 "literate/scraps.weft"
                                       /* {251: literate/scraps.weft:355} */
name = name_add(&user_names, new_name, sector);
                                       if (!name->defs || name->defs->scrap != current_scrap) {
                                         Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         def->scrap = current_scrap;
                                         def->next = name->defs;
                                         name->defs = def;
                                         add_uses(&(scrap_array(current_scrap).defs), name);
                                       }/* {:251} */

#line 330 "literate/scraps.weft"

                                       break;
                                    }
                                  }
                                } while (c != nw_char);
                                c = source_get();
                              }while (c == '|' || c == '*' || c == '-' || c == '+');
                              if (c != '}' && c != ']' && c != ')') {
                                fprintf(stderr, "%s: unexpected %c%c in index entry (%s, %d)\n",
                                        command_name, nw_char, c, source_name, source_line);
                                exit(-1);
                              }
                            }/* {:249} */

#line 244 "literate/scraps.weft"

                            /* Fall through */
                  case ')':
                  case ']':
                  case '}': if (--depth > 0)
                              break;
                            /* else fall through */
                  case ',':
                            push('\0', &writer);
                            scrap_ended_with = c;
                            return current_scrap;
                  case '<': 
#line 365 "literate/scraps.weft"
                            /* {252: literate/scraps.weft:365} */
{
                              Arglist * args = collect_scrap_name(current_scrap);
                              Name *name = args->name;
                              /* Save macro name */
                              
#line 380 "literate/scraps.weft"
                              /* {253: literate/scraps.weft:380} */
{
                                char buff[24];

                                push(nw_char, &writer);
                                push('<', &writer);
                                push(name->sector, &writer);
                                sprintf(buff, "%d", arglist_register(args));
                                pushs(buff, &writer);
                              }/* {:253} */

#line 368 "literate/scraps.weft"

                              add_to_use(name, current_scrap);
                              if (scrap_name_has_parameters) {
                                /* Save macro parameters */
                                
#line 573 "literate/parser.weft"
                                /* {73: literate/parser.weft:573} */
{
                                  int param_scrap;
                                  char param_buf[10];

                                  push(nw_char, &writer);
                                  push('(', &writer);
                                  do {
                                     param_scrap = collect_scrap();
                                     sprintf(param_buf, "%d", param_scrap);
                                     pushs(param_buf, &writer);
                                     push(nw_char, &writer);
                                     push(scrap_ended_with, &writer);
                                     add_to_use(name, current_scrap);
                                  } while( scrap_ended_with == ',' );
                                  do
                                    c = source_get();
                                  while( ' ' == c );
                                  if (c == nw_char) {
                                    c = source_get();
                                  }
                                  if (c != '>') {
                                    /* ZZZ print error */;
                                  }
                                }/* {:73} */

#line 371 "literate/scraps.weft"

                              }
                              push(nw_char, &writer);
                              push('>', &writer);
                              c = source_get();
                            }/* {:252} */

#line 255 "literate/scraps.weft"

                            break;
                  case '%': 
#line 750 "literate/latex-output.weft"
                            /* {120: literate/latex-output.weft:750} */
{
                                    do
                                            c = source_get();
                                    while (c != '\n');
                            }/* {:120} */

#line 257 "literate/scraps.weft"

                            /* emit line break to the output file to keep #line in sync. */
                            push('\n', &writer);
                            c = source_get();
                            break;
                  case 'x': 
#line 290 "literate/scraps.weft"
                            /* {247: literate/scraps.weft:290} */
{
                               /* Get label from */
                               
#line 586 "literate/search-labels.weft"
                               /* {380: literate/search-labels.weft:586} */
char  label_name[MAX_NAME_LEN];
                               char * p = label_name;
                               while (c = 
#line 291 "literate/scraps.weft"
                                          /* {248: literate/scraps.weft:291} */
source_get()/* {:248} */
, c != nw_char) /* Here is ?-01 */
                                  *p++ = c;
                               *p = '\0';
                               c = 
#line 291 "literate/scraps.weft"
                                   /* {248: literate/scraps.weft:291} */
source_get()/* {:248} */
;
                               /* {:380} */

#line 291 "literate/scraps.weft"

                               /* Save label to label store */
                               
#line 612 "literate/search-labels.weft"
                               /* {385: literate/search-labels.weft:612} */
if (label_name[0])
                               /* Search for label(<Complain about duplicate labels>,<Create a new label entry>) */

#line 634 "literate/search-labels.weft"
                               /* {389: literate/search-labels.weft:634} */
                               {
                                  label_node * * plbl = &label_tab;
                                  for (;;)
                                  {
                                     label_node * lbl = *plbl;

                                     if (lbl)
                                     {
                                        int cmp = label_name[0] - lbl->name[0];

                                        if (cmp == 0)
                                           cmp = strcmp(label_name + 1, lbl->name + 1);
                                        if (cmp < 0)
                                           plbl = &lbl->left;
                                        else if (cmp > 0)
                                           plbl = &lbl->right;
                                        else
                                        {
                                           /* Complain about duplicate labels */
                                           
#line 628 "literate/search-labels.weft"
                                           /* {387: literate/search-labels.weft:628} */
fprintf(stderr, "Duplicate label %s.\n", label_name);/* {:387} */

                                           break;
                                        }
                                     }
                                     else
                                     {
                                         /* Create a new label entry */
                                         
#line 620 "literate/search-labels.weft"
                                         /* {386: literate/search-labels.weft:620} */
lbl = (label_node *)arena_getmem(sizeof(label_node) + (p - label_name));
                                         lbl->left = lbl->right = NULL;
                                         strcpy(lbl->name, label_name);
                                         lbl->scrap = current_scrap;
                                         lbl->seq = ++lblseq;
                                         *plbl = lbl;/* {:386} */

                                         break;
                                     }
                                  }
                               }
                               /* {:389} */

#line 613 "literate/search-labels.weft"

                               else
                               {
                                  /* Complain about empty label */
                                  
#line 631 "literate/search-labels.weft"
                                  /* {388: literate/search-labels.weft:631} */
fprintf(stderr, "Empty label.\n");/* {:388} */

#line 616 "literate/search-labels.weft"

                               }/* {:385} */

#line 292 "literate/scraps.weft"

                               push(nw_char, &writer);
                               push('x', &writer);
                               pushs(label_name, &writer);
                               push(nw_char, &writer);
                            }/* {:247} */

#line 262 "literate/scraps.weft"

                            break;
                  case 'c': 
#line 402 "literate/parser.weft"
                            /* {64: literate/parser.weft:402} */
{
                               char * p = blockBuff;

                               push(nw_char, &writer);
                               do
                               {
                                  push(c, &writer);
                                  c = *p++;
                               } while (c != '\0');
                            }/* {:64} */

#line 264 "literate/scraps.weft"

                            break;
                  case '1': case '2': case '3':
                  case '4': case '5': case '6':
                  case '7': case '8': case '9':
                  case 'f': case '#': case 'v':
                  case 't': case 's':
                            push(nw_char, &writer);
                            break;
                  case '_': c = source_get();
                            break;
                  default :
                        if (c==nw_char)
                          {
                            push(nw_char, &writer);
                            push(nw_char, &writer);
                            c = source_get();
                            break;
                          }
                        fprintf(stderr, "%s: unexpected %c%c in scrap (%s, %d)\n",
                                    command_name, nw_char, c, source_name, source_line);
                            exit(-1);
                }
              }/* {:246} */

#line 223 "literate/scraps.weft"

                  break;
            }
          push(c, &writer);
                  c = source_get();
                  break;
      }
    }
  }/* {:245} */

#line 188 "literate/scraps.weft"

}
/* {:243} */

#line 391 "literate/scraps.weft"
/* {254: literate/scraps.weft:391} */
void
add_to_use(Name * name, int current_scrap)
{
  if (!name->uses || name->uses->scrap != current_scrap) {
    Scrap_Node *use = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
    use->scrap = current_scrap;
    use->next = name->uses;
    name->uses = use;
  }
}
/* {:254} */

#line 407 "literate/scraps.weft"
/* {256: literate/scraps.weft:407} */
static char pop(Manager *manager)
{
  Slab *scrap = manager->scrap;
  int index = manager->index;
  char c = scrap->chars[index++];
  if (index == SLAB_SIZE) {
    manager->prev = scrap;
    manager->scrap = scrap->next;
    index = 0;
  }
  manager->index = index;
  return c;
}
/* {:256} */

#line 429 "literate/scraps.weft"
/* {257: literate/scraps.weft:429} */
void dump_scrap_text(FILE *file, int scrap_idx)
{
  Manager reader;
  char c;
  reader.scrap = scrap_array(scrap_idx).slab;
  reader.prev = NULL;
  reader.index = 0;
  c = pop(&reader);
  while (c) {
    if (c == nw_char) {
      /* handle internal control sequences */
      c = pop(&reader);
      if (c == '<') {
        /* fragment reference encoded as: sector_byte + decimal_index
           followed by nw_char + '>' */
        char idx_buf[MAX_NAME_LEN];
        char *p = idx_buf;
        int idx;
        (void)pop(&reader); /* sector byte, discard */
        c = pop(&reader);
        while (c != nw_char && c != '\0') {
          *p++ = c;
          c = pop(&reader);
        }
        *p = '\0';
        if (sscanf(idx_buf, "%d", &idx) == 1) {
          Arglist *a = arglist_lookup(idx);
          if (a && a->name) {
            fprintf(file, "%c<%s%c>", nw_char, a->name->spelling, nw_char);
          }
        }
        /* skip past closing '>' marker */
        if (c == nw_char)
          c = pop(&reader);
        /* skip optional parameter block */
        if (c == '(') {
          while (c && c != nw_char)
            c = pop(&reader);
          if (c == nw_char)
            c = pop(&reader);
        }
      } else {
        /* other control seq: skip */
      }
      c = pop(&reader);
      continue;
    }
    fputc(c, file);
    c = pop(&reader);
  }
}
/* {:257} */

#line 483 "literate/scraps.weft"
/* {258: literate/scraps.weft:483} */
static void backup(int n, Manager *manager)
{
  int index = manager->index;
  if (n > index
      && manager->prev != NULL)
  {
     manager->scrap = manager->prev;
     manager->prev = NULL;
     index += SLAB_SIZE;
  }
  manager->index = (n <= index ? index - n : 0);
}
/* {:258} */

#line 498 "literate/scraps.weft"
/* {259: literate/scraps.weft:498} */
void
lookup(int n, Arglist * par, char * arg[9], Name **name, Arglist ** args)
{
  int i;
  Arglist * p = par;

  for (i = 0; i < n && p != NULL; i++)
    p = p->next;
  if (p == NULL) {
    char * a = arg[n];

    *name = NULL;
    *args = (Arglist *)a;
  }
  else {
    *name = p->name;
    *args = p->args;
  }
}
/* {:259} */

#line 520 "literate/scraps.weft"
/* {260: literate/scraps.weft:520} */
Arglist * instance(Arglist * a, Arglist * par, char * arg[9], int * ch)
{
   if (a != NULL) {
      int changed = 0;
      Arglist *args, *next;
      Name* name;
      /* Set up name, args and next */
      
#line 542 "literate/scraps.weft"
      /* {262: literate/scraps.weft:542} */
next = instance(a->next, par, arg, &changed);
      name = a->name;
      if (name == (Name *)1) {
         Embed_Node * q = (Embed_Node *)arena_getmem(sizeof(Embed_Node));
         q->defs = (Scrap_Node *)a->args;
         q->args = par;
         args = (Arglist *)q;
         changed = 1;
      } else if (name != NULL)
        args = instance(a->args, par, arg, &changed);
      else {
        char * p = (char *)a->args;
         if (p[0] == ARG_CHR) {
            lookup(p[1] - '1', par, arg, &name, &args);
            changed = 1;
         }
         else {
            args = a->args;
         }
      }/* {:262} */

#line 526 "literate/scraps.weft"

      if (changed){
        /* Build a new arglist */
        
#line 564 "literate/scraps.weft"
        /* {263: literate/scraps.weft:564} */
a = (Arglist *)arena_getmem(sizeof(Arglist));
        a->name = name;
        a->args = args;
        a->next = next;/* {:263} */

#line 528 "literate/scraps.weft"

        *ch = 1;
      }
   }

   return a;
}
/* {:260} */

#line 570 "literate/scraps.weft"
/* {264: literate/scraps.weft:570} */
static Arglist *pop_scrap_name(Manager *manager, Parameters *parameters)
{
  char name[MAX_NAME_LEN];
  char *p = name;
  Arglist * args;
  int c;
  int idx;

  (void)pop(manager); /* not sure why we have to pop twice */
  c = pop(manager);

  while (c != nw_char) {
    *p++ = c;
    c = pop(manager);
  }
  *p = '\000';
  if (sscanf(name, "%d", &idx) != 1)
  {
    fprintf(stderr,  "%s: found an internal problem (2)\n", command_name);
    exit(-1);
  }
  args = arglist_lookup(idx);
  /* Check for end of scrap name */
  
#line 599 "literate/scraps.weft"
  /* {265: literate/scraps.weft:599} */
{
    c = pop(manager);
    /* Check for macro parameters */
    
#line 603 "literate/parser.weft"
/* {74: literate/parser.weft:603} */

      if (c == '(') {
        Parameters res = arena_getmem(10 * sizeof(int));
        int *p2 = res;
        int count = 0;
        int scrapnum;

        while( c && c != ')' ) {
          scrapnum = 0;
          c = pop(manager);
          while( '0' <= c && c <= '9' ) {
            scrapnum = scrapnum  * 10 + c - '0';
            c = pop(manager);
          }
          if ( c == nw_char ) {
            c = pop(manager);
          }
          *p2++ = scrapnum;
        }
        while (count < 10) {
          *p2++ = 0;
          count++;
        }
        while( c && c != nw_char ) {
            c = pop(manager);
        }
        if ( c == nw_char ) {
          c = pop(manager);
        }
        *parameters = res;
      }
    /* {:74} */

#line 601 "literate/scraps.weft"

  }/* {:265} */

#line 592 "literate/scraps.weft"

  return args;
}
/* {:264} */

#line 605 "literate/scraps.weft"
/* {266: literate/scraps.weft:605} */
int write_scraps(FILE *file, char *spelling, Scrap_Node *defs,
                   int global_indent, char *indent_chars,
                   char debug_flag, char tab_flag, char indent_flag,
                   unsigned char comment_flag, char location_flag,
                   Arglist *inArgs,
                   char *inParams[9], Parameters parameters, char *title)
{
  /* This is in file scraps.c */
  int indent = 0;
  int newline = 1;
  while (defs) {
    /* Copy \verb|defs->scrap| to \verb|file| */
    
#line 660 "literate/scraps.weft"
    /* {269: literate/scraps.weft:660} */
{
      char c;
      Manager reader;
      Parameters local_parameters = 0;
      int line_number = scrap_array(defs->scrap).file_line;
      reader.scrap = scrap_array(defs->scrap).slab;
      reader.index = 0;
      /* Insert debugging information if required */
      
#line 724 "literate/scraps.weft"
      /* {271: literate/scraps.weft:724} */
if (debug_flag) {
        fprintf(file, "\n#line %d \"%s\"\n",
                line_number, scrap_array(defs->scrap).file_name);
        /* Insert appropriate indentation */
        
#line 755 "literate/scraps.weft"
        /* {274: literate/scraps.weft:755} */
{
          char c1 = pop(&reader);
          char c2 = pop(&reader);

          if (indent_flag && !(
#line 781 "literate/scraps.weft"
                               /* {276: literate/scraps.weft:781} */
c1 == '\n'
                               || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))/* {:276} */

#line 759 "literate/scraps.weft"
        )) {
            /* Put out the indent */
            
#line 767 "literate/scraps.weft"
            /* {275: literate/scraps.weft:767} */
if (tab_flag)
                for (indent=0; indent<global_indent; indent++)
                  putc(' ', file);
              else
                for (indent=0; indent<global_indent; indent++)
                  putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
            /* {:275} */

#line 760 "literate/scraps.weft"

          }
          indent = 0;
          backup(2, &reader);
        }/* {:274} */

#line 727 "literate/scraps.weft"

      }/* {:271} */

#line 667 "literate/scraps.weft"

      /* Insert section opening marker */
      
#line 736 "literate/scraps.weft"
      /* {272: literate/scraps.weft:736} */
if (location_flag && comment_flag) {
        fprintf(file, "%s{%d: %s:%d}%s\n",
                comment_begin[comment_flag],
                defs->scrap,
                scrap_array(defs->scrap).file_name,
                line_number,
                comment_end[comment_flag]);
      }/* {:272} */

#line 668 "literate/scraps.weft"

      if (delayed_indent)
      {
        /* Insert appropriate indentation */
        
#line 755 "literate/scraps.weft"
        /* {274: literate/scraps.weft:755} */
{
          char c1 = pop(&reader);
          char c2 = pop(&reader);

          if (indent_flag && !(
#line 781 "literate/scraps.weft"
                               /* {276: literate/scraps.weft:781} */
c1 == '\n'
                               || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))/* {:276} */

#line 759 "literate/scraps.weft"
        )) {
            /* Put out the indent */
            
#line 767 "literate/scraps.weft"
            /* {275: literate/scraps.weft:767} */
if (tab_flag)
                for (indent=0; indent<global_indent; indent++)
                  putc(' ', file);
              else
                for (indent=0; indent<global_indent; indent++)
                  putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
            /* {:275} */

#line 760 "literate/scraps.weft"

          }
          indent = 0;
          backup(2, &reader);
        }/* {:274} */

#line 671 "literate/scraps.weft"

      }
      c = pop(&reader);
      while (c) {
        switch (c) {
          case '\n':
             if (global_indent >= 0) {
               putc(c, file);
               line_number++;
               newline = 1;
               delayed_indent = 0;
               /* Insert appropriate indentation */
               
#line 755 "literate/scraps.weft"
               /* {274: literate/scraps.weft:755} */
{
                 char c1 = pop(&reader);
                 char c2 = pop(&reader);

                 if (indent_flag && !(
#line 781 "literate/scraps.weft"
                                      /* {276: literate/scraps.weft:781} */
c1 == '\n'
                                      || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))/* {:276} */

#line 759 "literate/scraps.weft"
               )) {
                   /* Put out the indent */
                   
#line 767 "literate/scraps.weft"
                   /* {275: literate/scraps.weft:767} */
if (tab_flag)
                       for (indent=0; indent<global_indent; indent++)
                         putc(' ', file);
                     else
                       for (indent=0; indent<global_indent; indent++)
                         putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                   /* {:275} */

#line 760 "literate/scraps.weft"

                 }
                 indent = 0;
                 backup(2, &reader);
               }/* {:274} */

#line 682 "literate/scraps.weft"

               break;
             } else {
               /* Don't show newlines in embedded fragmants */
               fputs(". . .", file);
               return 0;
             }
          case '\t': 
#line 785 "literate/scraps.weft"
                     /* {277: literate/scraps.weft:785} */
{
                       if (tab_flag)
                         /* Expand tab into spaces */
                         
#line 662 "literate/latex-output.weft"
                         /* {113: literate/latex-output.weft:662} */
{
                           int delta = 8 - (indent % 8);
                           indent += delta;
                           while (delta > 0) {
                             putc(' ', file);
                             delta--;
                           }
                         }/* {:113} */

#line 787 "literate/scraps.weft"

                       else {
                         putc('\t', file);
                         if (global_indent >= 0) {
                           /* Add more indentation ''\t'' */
                           
#line 717 "literate/scraps.weft"
                           /* {270: literate/scraps.weft:717} */
{
                             if (global_indent + indent < MAX_INDENT)
                               indent_chars[global_indent + indent] = '\t';
                           }/* {:270} */

#line 791 "literate/scraps.weft"

                         }
                         indent++;
                       }
                     }/* {:277} */

#line 689 "literate/scraps.weft"

                     delayed_indent = 0;
                     break;
          default:
             if (c==nw_char)
               {
                 /* Check for macro invocation in scrap */
                 
#line 799 "literate/scraps.weft"
                 /* {278: literate/scraps.weft:799} */
{
                   int oldin = indent;
                   char oldcf = comment_flag;
                   c = pop(&reader);
                   switch (c) {
                     case 't': 
#line 922 "literate/scraps.weft"
                               /* {285: literate/scraps.weft:922} */
{
                                  char * p = title;
                                  Arglist *q = inArgs;
                                  int narg;

                                  /* Comment this macro use */
                                  
#line 935 "literate/scraps.weft"
                                  /* {286: literate/scraps.weft:935} */
narg = 0;
                                  while (*p != '\000') {
                                    if (*p == ARG_CHR) {
                                      if (q == NULL) {
                                         if (defs->quoted)
                                            fprintf(file, "%c'%s%c'", nw_char, inParams[narg], nw_char);
                                         else
                                            fprintf(file, "'%s'", inParams[narg]);
                                      }
                                      else {
                                        comment_ArglistElement(file, q, defs->quoted);
                                        q = q->next;
                                      }
                                      p++;
                                      narg++;
                                    }
                                    else
                                       fputc(*p++, file);
                                  }/* {:286} */

#line 927 "literate/scraps.weft"

                                  if (xref_flag) {
                                     putc(' ', file);
                                     write_single_scrap_ref(file, defs->scrap);
                                  }
                               }/* {:285} */

#line 804 "literate/scraps.weft"

                               break;
                     case 'c': 
#line 414 "literate/parser.weft"
                               /* {65: literate/parser.weft:414} */
{
                                  int bgn = indent + global_indent;
                                  int posn = bgn + strlen(comment_begin[comment_flag]);
                                  int i;

                                  /* Perhaps put a delayed indent */
                                  
#line 916 "literate/scraps.weft"
                                  /* {284: literate/scraps.weft:916} */
if (delayed_indent)
                                     for (i = indent + global_indent; --i >= 0; )
                                        putc(' ', file);
                                  /* {:284} */

#line 419 "literate/parser.weft"

                                  c = pop(&reader);
                                  fputs(comment_begin[comment_flag], file);
                                  while (c != '\0')
                                  {
                                     /* Move a word to the file */
                                     
#line 443 "literate/parser.weft"
                                     /* {66: literate/parser.weft:443} */
do
                                     {
                                        putc(c, file);
                                        posn += 1;
                                        c = pop(&reader);
                                     } while (c > ' ');
                                     /* {:66} */

#line 424 "literate/parser.weft"

                                     /* If we break the line at this word */
                                     
#line 452 "literate/parser.weft"
                                     /* {67: literate/parser.weft:452} */
if (c == '\n' || (c == ' ' && posn > 60))/* {:67} */

#line 425 "literate/parser.weft"

                                     {
                                        putc('\n', file);
                                        for (i = 0; i < bgn ; i++)
                                           putc(' ', file);
                                        c = pop(&reader);
                                        if (c != '\0')
                                        {
                                           posn = bgn + strlen(comment_mid[comment_flag]);
                                           fputs(comment_mid[comment_flag], file);
                                        }
                                     }
                                  }
                                  fputs(comment_end[comment_flag], file);
                               }
                               /* {:65} */

#line 806 "literate/scraps.weft"

                               break;
                     case 'f': 
#line 847 "literate/scraps.weft"
                               /* {281: literate/scraps.weft:847} */
if (defs->quoted)
                                  fprintf(file, "%cf", nw_char);
                               else
                                  fputs(spelling, file);
                               /* {:281} */

#line 808 "literate/scraps.weft"

                               break;
                     case 'x': 
#line 841 "literate/scraps.weft"
                               /* {279: literate/scraps.weft:841} */
{
                                  /* Get label from */
                                  
#line 586 "literate/search-labels.weft"
                                  /* {380: literate/search-labels.weft:586} */
char  label_name[MAX_NAME_LEN];
                                  char * p = label_name;
                                  while (c = 
#line 842 "literate/scraps.weft"
                                             /* {280: literate/scraps.weft:842} */
pop(&reader)/* {:280} */
, c != nw_char) /* Here is ?-01 */
                                     *p++ = c;
                                  *p = '\0';
                                  c = 
#line 842 "literate/scraps.weft"
                                      /* {280: literate/scraps.weft:842} */
pop(&reader)/* {:280} */
;
                                  /* {:380} */

#line 842 "literate/scraps.weft"

                                  write_label(label_name, file);
                               }/* {:279} */

#line 810 "literate/scraps.weft"

                     case '_': break;
                     case 'v': 
#line 727 "literate/latex-output.weft"
                               /* {116: literate/latex-output.weft:727} */
fputs(version_string, file);
                               /* {:116} */

#line 812 "literate/scraps.weft"

                               break;
                     case 's': indent = -global_indent;
                               comment_flag = 0;
                               break;
                     case '<': 
#line 854 "literate/scraps.weft"
                               /* {282: literate/scraps.weft:854} */
{
                                 Arglist *a = pop_scrap_name(&reader, &local_parameters);
                                 Name *name = a->name;
                                 int changed;
                                 Arglist * args = instance(a->args, inArgs, inParams, &changed);
                                 int i, narg;
                                 char * p = name->spelling;
                                 char * * inParams = name->arg;
                                 Arglist *q = args;

                                 if (name->mark) {
                                   fprintf(stderr, "%s: recursive macro discovered involving <%s>\n",
                                           command_name, name->spelling);
                                   exit(-1);
                                 }
                                 if (name->defs && !defs->quoted) {
                                   /* Perhaps comment this macro */
                                   
#line 899 "literate/scraps.weft"
                                   /* {283: literate/scraps.weft:899} */
if (comment_flag && newline) {
                                      /* Perhaps put a delayed indent */
                                      
#line 916 "literate/scraps.weft"
                                      /* {284: literate/scraps.weft:916} */
if (delayed_indent)
                                         for (i = indent + global_indent; --i >= 0; )
                                            putc(' ', file);
                                      /* {:284} */

#line 900 "literate/scraps.weft"

                                      fputs(comment_begin[comment_flag], file);
                                      /* Comment this macro use */
                                      
#line 935 "literate/scraps.weft"
                                      /* {286: literate/scraps.weft:935} */
narg = 0;
                                      while (*p != '\000') {
                                        if (*p == ARG_CHR) {
                                          if (q == NULL) {
                                             if (defs->quoted)
                                                fprintf(file, "%c'%s%c'", nw_char, inParams[narg], nw_char);
                                             else
                                                fprintf(file, "'%s'", inParams[narg]);
                                          }
                                          else {
                                            comment_ArglistElement(file, q, defs->quoted);
                                            q = q->next;
                                          }
                                          p++;
                                          narg++;
                                        }
                                        else
                                           fputc(*p++, file);
                                      }/* {:286} */

#line 902 "literate/scraps.weft"

                                      if (xref_flag) {
                                         putc(' ', file);
                                         write_single_scrap_ref(file, name->defs->scrap);
                                      }
                                      fputs(comment_end[comment_flag], file);
                                      putc('\n', file);
                                      if (!delayed_indent)
                                         for (i = indent + global_indent; --i >= 0; )
                                            putc(' ', file);
                                   }
                                   /* {:283} */

#line 870 "literate/scraps.weft"

                                   name->mark = TRUE;
                                   indent = write_scraps(file, spelling, name->defs, global_indent + indent,
                                                         indent_chars, debug_flag, tab_flag, indent_flag,
                                                         comment_flag, location_flag, args, name->arg,
                                                         local_parameters, name->spelling);
                                   indent -= global_indent;
                                   name->mark = FALSE;
                                 }
                                 else
                                 {
                                   if (delayed_indent)
                                   {
                                     for (i = indent + global_indent; --i >= 0; )
                                        putc(' ', file);
                                   }

                                   fprintf(file, "%c<",  nw_char);
                                   if (name->sector == 0)
                                      fputc('+', file);
                                   /* Comment this macro use */
                                   
#line 935 "literate/scraps.weft"
                                   /* {286: literate/scraps.weft:935} */
narg = 0;
                                   while (*p != '\000') {
                                     if (*p == ARG_CHR) {
                                       if (q == NULL) {
                                          if (defs->quoted)
                                             fprintf(file, "%c'%s%c'", nw_char, inParams[narg], nw_char);
                                          else
                                             fprintf(file, "'%s'", inParams[narg]);
                                       }
                                       else {
                                         comment_ArglistElement(file, q, defs->quoted);
                                         q = q->next;
                                       }
                                       p++;
                                       narg++;
                                     }
                                     else
                                        fputc(*p++, file);
                                   }/* {:286} */

#line 890 "literate/scraps.weft"

                                   fprintf(file, "%c>",  nw_char);
                                   if (!defs->quoted && !tex_flag)
                                     fprintf(stderr, "%s: macro never defined <%s>\n",
                                           command_name, name->spelling);
                                 }
                               }/* {:282} */

#line 817 "literate/scraps.weft"

                               /* Insert debugging information if required */
                               
#line 724 "literate/scraps.weft"
                               /* {271: literate/scraps.weft:724} */
if (debug_flag) {
                                 fprintf(file, "\n#line %d \"%s\"\n",
                                         line_number, scrap_array(defs->scrap).file_name);
                                 /* Insert appropriate indentation */
                                 
#line 755 "literate/scraps.weft"
                                 /* {274: literate/scraps.weft:755} */
{
                                   char c1 = pop(&reader);
                                   char c2 = pop(&reader);

                                   if (indent_flag && !(
#line 781 "literate/scraps.weft"
                                                        /* {276: literate/scraps.weft:781} */
c1 == '\n'
                                                        || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))/* {:276} */

#line 759 "literate/scraps.weft"
                                 )) {
                                     /* Put out the indent */
                                     
#line 767 "literate/scraps.weft"
                                     /* {275: literate/scraps.weft:767} */
if (tab_flag)
                                         for (indent=0; indent<global_indent; indent++)
                                           putc(' ', file);
                                       else
                                         for (indent=0; indent<global_indent; indent++)
                                           putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                                     /* {:275} */

#line 760 "literate/scraps.weft"

                                   }
                                   indent = 0;
                                   backup(2, &reader);
                                 }/* {:274} */

#line 727 "literate/scraps.weft"

                               }/* {:271} */

#line 818 "literate/scraps.weft"

                               indent = oldin;
                               comment_flag = oldcf;
                               break;
                     /* Handle macro parameter substitution */
                     
#line 501 "literate/parser.weft"
/* {72: literate/parser.weft:501} */

                     case '1': case '2': case '3':
                     case '4': case '5': case '6':
                     case '7': case '8': case '9':
                       {
                         Arglist * args;
                         Name * name;

                         lookup(c - '1', inArgs, inParams, &name, &args);

                         if (name == (Name *)1) {
                           Embed_Node * q = (Embed_Node *)args;
                           indent = write_scraps(file, spelling, q->defs,
                                                 global_indent + indent,
                                                 indent_chars, debug_flag,
                                                 tab_flag, indent_flag,
                                                 0, 0,
                                                 q->args, inParams,
                                                 local_parameters, "");
                         }
                         else if (name != NULL) {
                            int i, narg;
                            char * p = name->spelling;
                            Arglist *q = args;

                            /* Perhaps comment this macro */
                            
#line 899 "literate/scraps.weft"
                            /* {283: literate/scraps.weft:899} */
if (comment_flag && newline) {
                               /* Perhaps put a delayed indent */
                               
#line 916 "literate/scraps.weft"
                               /* {284: literate/scraps.weft:916} */
if (delayed_indent)
                                  for (i = indent + global_indent; --i >= 0; )
                                     putc(' ', file);
                               /* {:284} */

#line 900 "literate/scraps.weft"

                               fputs(comment_begin[comment_flag], file);
                               /* Comment this macro use */
                               
#line 935 "literate/scraps.weft"
                               /* {286: literate/scraps.weft:935} */
narg = 0;
                               while (*p != '\000') {
                                 if (*p == ARG_CHR) {
                                   if (q == NULL) {
                                      if (defs->quoted)
                                         fprintf(file, "%c'%s%c'", nw_char, inParams[narg], nw_char);
                                      else
                                         fprintf(file, "'%s'", inParams[narg]);
                                   }
                                   else {
                                     comment_ArglistElement(file, q, defs->quoted);
                                     q = q->next;
                                   }
                                   p++;
                                   narg++;
                                 }
                                 else
                                    fputc(*p++, file);
                               }/* {:286} */

#line 902 "literate/scraps.weft"

                               if (xref_flag) {
                                  putc(' ', file);
                                  write_single_scrap_ref(file, name->defs->scrap);
                               }
                               fputs(comment_end[comment_flag], file);
                               putc('\n', file);
                               if (!delayed_indent)
                                  for (i = indent + global_indent; --i >= 0; )
                                     putc(' ', file);
                            }
                            /* {:283} */

#line 526 "literate/parser.weft"

                            indent = write_scraps(file, spelling, name->defs,
                                                  global_indent + indent,
                                                  indent_chars, debug_flag,
                                                  tab_flag, indent_flag,
                                                  comment_flag, location_flag,
                                                  args, name->arg,
                                                  local_parameters, p);
                         }
                         else if (args != NULL) {
                            if (delayed_indent) {
                              /* Put out the indent */
                              
#line 767 "literate/scraps.weft"
                              /* {275: literate/scraps.weft:767} */
if (tab_flag)
                                  for (indent=0; indent<global_indent; indent++)
                                    putc(' ', file);
                                else
                                  for (indent=0; indent<global_indent; indent++)
                                    putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                              /* {:275} */

#line 537 "literate/parser.weft"

                            }
                            fputs((char *)args, file);
                         }
                         else if ( parameters && parameters[c - '1'] ) {
                           Scrap_Node param_defs;
                           param_defs.scrap = parameters[c - '1'];
                           param_defs.next = 0;
                           write_scraps(file, spelling, &param_defs,
                                        global_indent + indent,
                                        indent_chars, debug_flag,
                                        tab_flag, indent_flag,
                                        comment_flag, location_flag,
                                        NULL, NULL, 0, "");
                         } else if (delayed_indent) {
                           /* Put out the indent */
                           
#line 767 "literate/scraps.weft"
                           /* {275: literate/scraps.weft:767} */
if (tab_flag)
                               for (indent=0; indent<global_indent; indent++)
                                 putc(' ', file);
                             else
                               for (indent=0; indent<global_indent; indent++)
                                 putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                           /* {:275} */

#line 552 "literate/parser.weft"

                         }
                       }
                     /* {:72} */

#line 822 "literate/scraps.weft"

                               indent = oldin;
                               break;
                     default:
                           if(c==nw_char)
                             {
                               putc(c, file);
                               if (global_indent >= 0) {
                                  /* Add more indentation '' '' */
                                  
#line 717 "literate/scraps.weft"
                                  /* {270: literate/scraps.weft:717} */
{
                                    if (global_indent + indent < MAX_INDENT)
                                      indent_chars[global_indent + indent] = ' ';
                                  }/* {:270} */

#line 830 "literate/scraps.weft"

                               }
                               indent++;
                               break;
                             }
                           /* ignore, since we should already have a warning */
                               break;
                   }
                 }/* {:278} */

#line 695 "literate/scraps.weft"

                 break;
               }
             putc(c, file);
             if (global_indent >= 0) {
               /* Add more indentation '' '' */
               
#line 717 "literate/scraps.weft"
               /* {270: literate/scraps.weft:717} */
{
                 if (global_indent + indent < MAX_INDENT)
                   indent_chars[global_indent + indent] = ' ';
               }/* {:270} */

#line 700 "literate/scraps.weft"

             }
             indent++;
             if (c > ' ') newline = 0;
             delayed_indent = 0;
             break;
        }
        c = pop(&reader);
      }
      /* Insert section closing marker */
      
#line 746 "literate/scraps.weft"
      /* {273: literate/scraps.weft:746} */
if (location_flag && comment_flag) {
        fprintf(file, "%s{:%d}%s\n",
                comment_begin[comment_flag],
                defs->scrap,
                comment_end[comment_flag]);
      }/* {:273} */

#line 709 "literate/scraps.weft"

    }/* {:269} */

#line 616 "literate/scraps.weft"

    defs = defs->next;
  }
  return indent + global_indent;
}
/* {:266} */

#line 1008 "literate/scraps.weft"
/* {291: literate/scraps.weft:1008} */
void collect_numbers(char *aux_name)
{
  if (number_flag) {
    int i;
    for (i=1; i<scraps; i++)
      scrap_array(i).page = i;
  }
  else {
    FILE *aux_file = fopen(aux_name, "r");
    already_warned = FALSE;
    if (aux_file) {
      char aux_line[500];
      while (fgets(aux_line, 500, aux_file)) {
        /* Read line in \verb|.aux| file */
        
#line 1034 "literate/scraps.weft"
/* {292: literate/scraps.weft:1034} */

        int scrap_number;
        int page_number;
        int i;
        int dummy_idx;
        int bracket_depth = 1;
        if (1 == sscanf(aux_line,
                        "\\newlabel{scrap%d}{{%n",
                        &scrap_number,
                        &dummy_idx)) {
          for (i = dummy_idx; i < (int)strlen(aux_line) && bracket_depth > 0; i++) {
            if (aux_line[i] == '{') bracket_depth++;
            else if (aux_line[i] == '}') bracket_depth--;
          }
          if (i > dummy_idx
              && i < (int)strlen(aux_line)
              && 1 == sscanf(aux_line+i, "{%d}" ,&page_number)) {
            if (scrap_number < scraps)
              scrap_array(scrap_number).page = page_number;
            else
              /* Warn (only once) about needing to rerun after Latex */
              
#line 131 "literate/scraps.weft"
              /* {237: literate/scraps.weft:131} */
{
                if (!already_warned) {
                  fprintf(stderr, "%s: you'll need to rerun weft after running latex\n",
                          command_name);
                  already_warned = TRUE;
                }
              }/* {:237} */

#line 1054 "literate/scraps.weft"

          }
        }
        /* {:292} */

#line 1021 "literate/scraps.weft"

      }
      fclose(aux_file);
      /* Add letters to scraps with duplicate page numbers */
      
#line 1061 "literate/scraps.weft"
      /* {293: literate/scraps.weft:1061} */
{
         int i = 0;

         /* Step 'i' to the next valid scrap */
         
#line 1075 "literate/scraps.weft"
         /* {294: literate/scraps.weft:1075} */
do
            i++;
         while (i < scraps && scrap_array(i).page == -1);
         /* {:294} */

#line 1064 "literate/scraps.weft"

         /* For all remaining scraps */
         
#line 1081 "literate/scraps.weft"
         /* {295: literate/scraps.weft:1081} */
while (i < scraps)/* {:295} */

#line 1065 "literate/scraps.weft"
       {
            int j = i;
            /* Step 'j' to the next valid scrap */
            
#line 1075 "literate/scraps.weft"
            /* {294: literate/scraps.weft:1075} */
do
               j++;
            while (j < scraps && scrap_array(j).page == -1);
            /* {:294} */

#line 1067 "literate/scraps.weft"

            /* Perhaps add letters to the page numbers */
            
#line 1084 "literate/scraps.weft"
            /* {296: literate/scraps.weft:1084} */
if (scrap_array(i).page == scrap_array(j).page) {
               if (scrap_array(i).letter == 0)
                  scrap_array(i).letter = 'a';
               scrap_array(j).letter = scrap_array(i).letter + 1;
            }
            /* {:296} */

#line 1068 "literate/scraps.weft"

            i = j;
         }
      }
      /* {:293} */

#line 1024 "literate/scraps.weft"

    }
  }
}
/* {:291} */

#line 11 "literate/search-labels.weft"
/* {342: literate/search-labels.weft:11} */
typedef struct name_node {
  struct name_node *next;
  Name *name;
} Name_Node;
/* {:342} */

#line 18 "literate/search-labels.weft"
/* {343: literate/search-labels.weft:18} */
typedef struct goto_node {
  Name_Node *output;            /* list of words ending in this state */
  struct move_node *moves;      /* list of possible moves */
  struct goto_node *fail;       /* and where to go when no move fits */
  struct goto_node *next;       /* next goto node with same depth */
} Goto_Node;
/* {:343} */

#line 27 "literate/search-labels.weft"
/* {344: literate/search-labels.weft:27} */
typedef struct move_node {
  struct move_node *next;
  Goto_Node *state;
  char c;
} Move_Node;
/* {:344} */

#line 35 "literate/search-labels.weft"
/* {345: literate/search-labels.weft:35} */
static Goto_Node *root[256];
static int max_depth;
static Goto_Node **depths;
/* {:345} */

#line 42 "literate/search-labels.weft"
/* {346: literate/search-labels.weft:42} */
static Goto_Node *goto_lookup(char c, Goto_Node *g)
{
  Move_Node *m = g->moves;
  while (m && m->c != c)
    m = m->next;
  if (m)
    return m->state;
  else
    return NULL;
}
/* {:346} */

#line 57 "literate/search-labels.weft"
/* {347: literate/search-labels.weft:57} */
typedef struct ArgMgr_s
{
   char * pv;
   char * bgn;
   Arglist * arg;
   struct ArgMgr_s * old;
} ArgMgr;
/* {:347} */

#line 67 "literate/search-labels.weft"
/* {348: literate/search-labels.weft:67} */
typedef struct ArgManager_s
{
   Manager * m;
   ArgMgr * a;
} ArgManager;
/* {:348} */

#line 75 "literate/search-labels.weft"
/* {349: literate/search-labels.weft:75} */
static void
pushArglist(ArgManager * mgr, Arglist * a)
{
   ArgMgr * b = malloc(sizeof(ArgMgr));

   if (b == NULL)
   {
      fprintf(stderr, "Can't allocate space for an argument manager\n");
      exit(EXIT_FAILURE);
   }
   b->pv = b->bgn = NULL;
   b->arg = a;
   b->old = mgr->a;
   mgr->a = b;
}
/* {:349} */

#line 93 "literate/search-labels.weft"
/* {350: literate/search-labels.weft:93} */
static char argpop(ArgManager * mgr)
{
   while (mgr->a != NULL)
   {
      ArgMgr * a = mgr->a;

      /* Perhaps |return| a character from the current arg */
      
#line 111 "literate/search-labels.weft"
      /* {351: literate/search-labels.weft:111} */
if (a->pv != NULL)
      {
         char c = *a->pv++;

         if (c != '\0')
            return c;
         a->pv = NULL;
         return ' ';
      }
      /* {:351} */

#line 99 "literate/search-labels.weft"

      /* Perhaps start a new arg */
      
#line 127 "literate/search-labels.weft"
      /* {352: literate/search-labels.weft:127} */
if (a->arg) {
         Arglist * b = a->arg;

         a->arg = b->next;
         if (b->name == NULL) {
            a->bgn = a->pv = (char *)b->args;
         } else if (b->name == (Name *)1) {
            a->bgn = a->pv = "{Embedded Scrap}";
         } else {
            pushArglist(mgr, b->args);
         }/* {:352} */

#line 100 "literate/search-labels.weft"

      /* Otherwise pop the current arg */
      
#line 140 "literate/search-labels.weft"
      /* {353: literate/search-labels.weft:140} */
} else {
         mgr->a = a->old;
         free(a);
      }/* {:353} */

#line 101 "literate/search-labels.weft"

   }

   return (pop(mgr->m));
}
/* {:350} */

#line 146 "literate/search-labels.weft"
/* {354: literate/search-labels.weft:146} */
static char
prev_char(ArgManager * mgr, int n)
{
   char c = '\0';
   ArgMgr * a = mgr->a;
   Manager * m = mgr->m;

   if (a != NULL) {
      /* Get the nth previous character from an argument */
      
#line 164 "literate/search-labels.weft"
      /* {355: literate/search-labels.weft:164} */
if (a->pv && a->pv - n >= a->bgn)
         c = *a->pv;
      else if (a->bgn) {
         int j = strlen(a->bgn) + 1;

         if (n >= j)
            c = a->bgn[j - n];
         else
            c = ' ';
      }
      /* {:355} */

#line 154 "literate/search-labels.weft"

   } else {
      /* Get the nth previous character from a scrap */
      
#line 177 "literate/search-labels.weft"
      /* {356: literate/search-labels.weft:177} */
int k = m->index - n - 2;

      if (k >= 0)
         c = m->scrap->chars[k];
      else if (m->prev)
         c = m->prev->chars[SLAB_SIZE - k];
      /* {:356} */

#line 156 "literate/search-labels.weft"

   }

   return c;
}
/* {:354} */

#line 192 "literate/search-labels.weft"
/* {358: literate/search-labels.weft:192} */
static void build_gotos(Name *tree);
static int reject_match(Name *name, char post, ArgManager *reader);

void search(void)
{
  int i;
  for (i=0; i<256; i++)
    root[i] = NULL;
  max_depth = 10;
  depths = (Goto_Node **) arena_getmem(max_depth * sizeof(Goto_Node *));
  for (i=0; i<max_depth; i++)
    depths[i] = NULL;
  build_gotos(user_names);
  /* Build failure functions */
  
#line 276 "literate/search-labels.weft"
  /* {361: literate/search-labels.weft:276} */
{
    int depth;
    for (depth=1; depth<max_depth; depth++) {
      Goto_Node *r = depths[depth];
      while (r) {
        Move_Node *m = r->moves;
        while (m) {
          char a = m->c;
          Goto_Node *s = m->state;
          Goto_Node *state = r->fail;
          while (state && !goto_lookup(a, state))
            state = state->fail;
          if (state)
            s->fail = goto_lookup(a, state);
          else
            s->fail = root[(unsigned char)a];
          if (s->fail) {
            Name_Node *p = s->fail->output;
            while (p) {
              Name_Node *q = (Name_Node *) arena_getmem(sizeof(Name_Node));
              q->name = p->name;
              q->next = s->output;
              s->output = q;
              p = p->next;
            }
          }
          m = m->next;
        }
        r = r->next;
      }
    }
  }/* {:361} */

#line 205 "literate/search-labels.weft"

  /* Search scraps */
  
#line 313 "literate/search-labels.weft"
  /* {362: literate/search-labels.weft:313} */
{
    for (i=1; i<scraps; i++) {
      char c, last = '\0';
      Manager rd;
      ArgManager reader;
      Goto_Node *state = NULL;
      rd.prev = NULL;
      rd.scrap = scrap_array(i).slab;
      rd.index = 0;
      reader.m = &rd;
      reader.a = NULL;
      c = argpop(&reader);
      while (c) {
        while (state && !goto_lookup(c, state))
          state = state->fail;
        if (state)
          state = goto_lookup(c, state);
        else
          state = root[(unsigned char)c];
        /* Skip over at at */
        
#line 360 "literate/search-labels.weft"
        /* {363: literate/search-labels.weft:360} */
if (last == nw_char && c == nw_char)
        {
           last = '\0';
           c = argpop(&reader);
        }
        /* {:363} */

#line 332 "literate/search-labels.weft"

        /* Skip over a scrap use */
        
#line 368 "literate/search-labels.weft"
        /* {364: literate/search-labels.weft:368} */
if (last == nw_char && c == '<')
        {
           char buf[MAX_NAME_LEN];
           char * p = buf;
           Arglist * args;
           int idx;

           c = argpop(&reader);
           while ((c = argpop(&reader)) != nw_char)
              *p++ = c;
           c = argpop(&reader);
           *p = '\0';
           if (sscanf(buf, "%d", &idx) != 1) {
              fprintf(stderr,  "%s: found an internal problem (3)\n", command_name);
              exit(-1);
           }
           args = arglist_lookup(idx);
           pushArglist(&reader, args);
        }/* {:364} */

#line 333 "literate/search-labels.weft"

        /* Skip over a block comment */
        
#line 455 "literate/parser.weft"
        /* {68: literate/parser.weft:455} */
if (last == nw_char && c == 'c')
           while ((c = pop(reader.m)) != '\0')
              /* Skip */;
        /* {:68} */

#line 334 "literate/search-labels.weft"

        last = c;
        c = argpop(&reader);
        if (state && state->output) {
          Name_Node *p = state->output;
          do {
            Name *name = p->name;
            if (!reject_match(name, c, &reader) &&
                scrap_array(i).sector == name->sector &&
                (!name->uses || name->uses->scrap != i)) {
              Scrap_Node *new_use =
                  (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
              new_use->scrap = i;
              new_use->next = name->uses;
              name->uses = new_use;
              if (!scrap_is_in(name->defs, i))
                add_uses(&(scrap_array(i).uses), name);
            }
            p = p->next;
          } while (p);
        }
      }
    }
  }/* {:362} */

#line 206 "literate/search-labels.weft"

}
/* {:358} */

#line 213 "literate/search-labels.weft"
/* {359: literate/search-labels.weft:213} */
static void build_gotos(Name *tree)
{
  while (tree) {
    /* Extend goto graph with \verb|tree->spelling| */
    
#line 224 "literate/search-labels.weft"
    /* {360: literate/search-labels.weft:224} */
{
      int depth = 2;
      char *p = tree->spelling;
      char c = *p++;
      Goto_Node *q = root[(unsigned char)c];
      Name_Node * last;
      if (!q) {
        q = (Goto_Node *) arena_getmem(sizeof(Goto_Node));
        root[(unsigned char)c] = q;
        q->moves = NULL;
        q->fail = NULL;
        q->output = NULL;
        q->next = depths[1];
        depths[1] = q;
      }
      while ((c = *p++)) {
        Goto_Node *new_node = goto_lookup(c, q);
        if (!new_node) {
          Move_Node *new_move = (Move_Node *) arena_getmem(sizeof(Move_Node));
          new_node = (Goto_Node *) arena_getmem(sizeof(Goto_Node));
          new_node->moves = NULL;
          new_node->fail = NULL;
          new_node->output = NULL;
          new_move->state = new_node;
          new_move->c = c;
          new_move->next = q->moves;
          q->moves = new_move;
          if (depth == max_depth) {
            int i;
            Goto_Node **new_depths =
                (Goto_Node **) arena_getmem(2*depth*sizeof(Goto_Node *));
            max_depth = 2 * depth;
            for (i=0; i<depth; i++)
              new_depths[i] = depths[i];
            depths = new_depths;
            for (i=depth; i<max_depth; i++)
              depths[i] = NULL;
          }
          new_node->next = depths[depth];
          depths[depth] = new_node;
        }
        q = new_node;
        depth++;
      }
      last = q->output;
      q->output = (Name_Node *) arena_getmem(sizeof(Name_Node));
      q->output->next = last;
      q->output->name = tree;
    }/* {:360} */

#line 216 "literate/search-labels.weft"

    build_gotos(tree->rlink);
    tree = tree->llink;
  }
}
/* {:359} */

#line 395 "literate/search-labels.weft"
/* {366: literate/search-labels.weft:395} */

static int scrap_is_in(Scrap_Node * list, int i)
{
  while (list != NULL) {
    if (list->scrap == i)
      return TRUE;
    list = list->next;
  }
  return FALSE;
}
/* {:366} */

#line 408 "literate/search-labels.weft"
/* {367: literate/search-labels.weft:408} */

static void add_uses(Uses * * root, Name *name)
{
   int cmp;
   Uses *p, **q = root;

   while ((p = *q, p != NULL)
          && (cmp = robs_strcmp(p->defn->spelling, name->spelling)) < 0)
      q = &(p->next);
   if (p == NULL || cmp > 0)
   {
      Uses *new_use = arena_getmem(sizeof(Uses));
      new_use->next = p;
      new_use->defn = name;
      *q = new_use;
   }
}
/* {:367} */

#line 439 "literate/search-labels.weft"
/* {370: literate/search-labels.weft:439} */

void
format_uses_refs(FILE * tex_file, int scrap)
{
  Uses * p = scrap_array(scrap).uses;
  if (p != NULL)
    /* Write uses references */
    
#line 450 "literate/search-labels.weft"
    /* {371: literate/search-labels.weft:450} */
{
      char join = ' ';
      fputs("\\item \\NWtxtIdentsUsed\\nobreak\\", tex_file);
      do {
        /* Write one use reference */
        
#line 462 "literate/search-labels.weft"
        /* {372: literate/search-labels.weft:462} */
Name * name = p->defn;
        Scrap_Node *defs = name->defs;
        int first = TRUE, page = -1;
        fprintf(tex_file,
                "%c \\verb%c%s%c\\nobreak\\ ",
                join, nw_char, name->spelling, nw_char);
        if (defs)
        {
          do {
            /* Write one referenced scrap */
            
#line 483 "literate/search-labels.weft"
            /* {373: literate/search-labels.weft:483} */
fputs("\\NWlink{weft", tex_file);
            write_scrap_ref(tex_file, defs->scrap, -1, &page);
            fputs("}{", tex_file);
            write_scrap_ref(tex_file, defs->scrap, first, &page);
            fputs("}", tex_file);/* {:373} */

#line 471 "literate/search-labels.weft"

            first = FALSE;
            defs = defs->next;
          }while (defs!= NULL);
        }
        else
        {
          fputs("\\NWnotglobal", tex_file);
        }
        /* {:372} */

#line 454 "literate/search-labels.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".", tex_file);
    }/* {:371} */

#line 445 "literate/search-labels.weft"

}
/* {:370} */

#line 494 "literate/search-labels.weft"
/* {375: literate/search-labels.weft:494} */

void
format_defs_refs(FILE * tex_file, int scrap)
{
  Uses * p = scrap_array(scrap).defs;
  if (p != NULL)
    /* Write defs references */
    
#line 505 "literate/search-labels.weft"
    /* {376: literate/search-labels.weft:505} */
{
      char join = ' ';
      fputs("\\item \\NWtxtIdentsDefed\\nobreak\\", tex_file);
      do {
        /* Write one def reference */
        
#line 517 "literate/search-labels.weft"
        /* {377: literate/search-labels.weft:517} */
Name * name = p->defn;
        Scrap_Node *defs = name->uses;
        int first = TRUE, page = -1;
        fprintf(tex_file,
                "%c \\verb%c%s%c\\nobreak\\ ",
                join, nw_char, name->spelling, nw_char);
        if (defs == NULL
            || (defs->scrap == scrap && defs->next == NULL)) {
          fputs("\\NWtxtIdentsNotUsed", tex_file);
        }
        else {
          do {
            if (defs->scrap != scrap) {
               /* Write one referenced scrap */
               
#line 483 "literate/search-labels.weft"
               /* {373: literate/search-labels.weft:483} */
fputs("\\NWlink{weft", tex_file);
               write_scrap_ref(tex_file, defs->scrap, -1, &page);
               fputs("}{", tex_file);
               write_scrap_ref(tex_file, defs->scrap, first, &page);
               fputs("}", tex_file);/* {:373} */

#line 530 "literate/search-labels.weft"

               first = FALSE;
            }
            defs = defs->next;
          }while (defs!= NULL);
        }
        /* {:377} */

#line 509 "literate/search-labels.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".", tex_file);
    }/* {:376} */

#line 500 "literate/search-labels.weft"

}
/* {:375} */

#line 549 "literate/search-labels.weft"
/* {378: literate/search-labels.weft:549} */
#define sym_char(c) (isalnum(c) || (c) == '_')

static int op_char(char c)
{
  switch (c) {
    case '!':           case '#': case '%': case '$': case '^':
    case '&': case '*': case '-': case '+': case '=': case '/':
    case '|': case '~': case '<': case '>':
      return TRUE;
    default:
      return c==nw_char ? TRUE : FALSE;
  }
}
/* {:378} */

#line 565 "literate/search-labels.weft"
/* {379: literate/search-labels.weft:565} */
static int reject_match(Name *name, char post, ArgManager *reader)
{
  int len = strlen(name->spelling);
  char first = name->spelling[0];
  char last = name->spelling[len - 1];
  char prev = prev_char(reader, len);
  if (sym_char(last) && sym_char(post)) return TRUE;
  if (sym_char(first) && sym_char(prev)) return TRUE;
  if (op_char(last) && op_char(post)) return TRUE;
  if (op_char(first) && op_char(prev)) return TRUE;
  return FALSE; /* Here is ?-01 */
}
/* {:379} */

#line 595 "literate/search-labels.weft"
/* {381: literate/search-labels.weft:595} */
void
write_label(char label_name[], FILE * file)
/* Search for label(<Write the label to file>,<Complain about missing label>) */

#line 634 "literate/search-labels.weft"
/* {389: literate/search-labels.weft:634} */
{
   label_node * * plbl = &label_tab;
   for (;;)
   {
      label_node * lbl = *plbl;

      if (lbl)
      {
         int cmp = label_name[0] - lbl->name[0];

         if (cmp == 0)
            cmp = strcmp(label_name + 1, lbl->name + 1);
         if (cmp < 0)
            plbl = &lbl->left;
         else if (cmp > 0)
            plbl = &lbl->right;
         else
         {
            /* Write the label to file */
            
#line 605 "literate/search-labels.weft"
            /* {383: literate/search-labels.weft:605} */
write_single_scrap_ref(file, lbl->scrap);
            fprintf(file, "-%02d", lbl->seq);/* {:383} */

            break;
         }
      }
      else
      {
          /* Complain about missing label */
          
#line 609 "literate/search-labels.weft"
          /* {384: literate/search-labels.weft:609} */
fprintf(stderr, "Can't find label %s.\n", label_name);/* {:384} */

          break;
      }
   }
}
/* {:389} */

#line 597 "literate/search-labels.weft"

/* {:381} */
