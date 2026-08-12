
#line 152 "literate/architecture.weft"
/* {11: literate/architecture.weft:152} */
#include "global.h"
/* {:11} */

#line 5 "literate/scraps.weft"
/* {246: literate/scraps.weft:5} */
#define SLAB_SIZE 1024

typedef struct slab {
  struct slab *next;
  char chars[SLAB_SIZE];
} Slab;
/* {:246} */

#line 14 "literate/scraps.weft"
/* {247: literate/scraps.weft:14} */
typedef struct {
  char *file_name;
  Slab *slab;
  struct uses *uses;
  struct uses *defs;
  int file_line;
  int end_line;
  int page;
  char letter;
  unsigned char sector;
  char *prose;
  struct name *owner;
} ScrapEntry;
/* {:247} */

#line 32 "literate/scraps.weft"
/* {248: literate/scraps.weft:32} */

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

#line 691 "literate/scraps.weft"
/* {294: literate/scraps.weft:691} */
int delayed_indent = 0;
/* {:294} */

#line 695 "literate/scraps.weft"
/* {295: literate/scraps.weft:695} */

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
/* {:295} */

#line 1040 "literate/scraps.weft"
/* {314: literate/scraps.weft:1040} */
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
     
#line 1060 "literate/scraps.weft"
     /* {315: literate/scraps.weft:1060} */
Embed_Node * e = (Embed_Node *)q;
     fputc('{', file);
     write_scraps(file, "", e->defs, -1, "", 0, 0, 0, 0, 0, e->args, NULL, NULL, "");
     fputc('}', file);/* {:315} */

#line 1052 "literate/scraps.weft"

  } else {
     /* Include a fragment use in comment */
     
#line 1066 "literate/scraps.weft"
     /* {316: literate/scraps.weft:1066} */
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
     fputc('>', file);/* {:316} */

#line 1054 "literate/scraps.weft"

  }
}
/* {:314} */

#line 1070 "literate/names.weft"
/* {347: literate/names.weft:1070} */
char * comment_begin[6] = { "", "/* ", "// ", "# ", "-- ", "<!-- "};
char * comment_mid[6] = { "", " * ", "// ", "# ", "-- ", "     "};
char * comment_end[6] = { "", " */", "", "", "", " -->"};
/* {:347} */

#line 394 "literate/search-labels.weft"
/* {392: literate/search-labels.weft:394} */

static void add_uses(Uses **root, Name *name);
static int scrap_is_in(Scrap_Node *list, int i);
/* {:392} */

#line 46 "literate/scraps.weft"

/* {:248} */

#line 63 "literate/scraps.weft"
/* {250: literate/scraps.weft:63} */
void init_scraps(void)
{
  scraps = 1;
  SCRAP[0] = (ScrapEntry *) arena_getmem(SCRAP_SIZE * sizeof(ScrapEntry));
}
/* {:250} */

#line 71 "literate/scraps.weft"
/* {251: literate/scraps.weft:71} */
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
    
#line 152 "literate/scraps.weft"
    /* {261: literate/scraps.weft:152} */
{
      if (!already_warned) {
        fprintf(stderr, "%s: you'll need to rerun weft after running latex\n",
                command_name);
        already_warned = TRUE;
      }
    }/* {:261} */

#line 86 "literate/scraps.weft"

  }
  if (first>=0)
  *page = scrap_array(num).page;
}
/* {:251} */

#line 94 "literate/scraps.weft"
/* {252: literate/scraps.weft:94} */
void write_single_scrap_ref(FILE *file, int num)
{
  int page;
  write_scrap_ref(file, num, TRUE, &page);
}
/* {:252} */

#line 102 "literate/scraps.weft"
/* {253: literate/scraps.weft:102} */
Uses * get_scrap_uses(int scrap)
{
  return scrap_array(scrap).uses;
}
/* {:253} */

#line 109 "literate/scraps.weft"
/* {254: literate/scraps.weft:109} */
Uses * get_scrap_defs(int scrap)
{
  return scrap_array(scrap).defs;
}
/* {:254} */

#line 132 "literate/scraps.weft"
/* {256: literate/scraps.weft:132} */
const char *scrap_file_name(int i) { return scrap_array(i).file_name; }
/* {:256} */

#line 136 "literate/scraps.weft"
/* {257: literate/scraps.weft:136} */
int scrap_file_line(int i) { return scrap_array(i).file_line; }
/* {:257} */

#line 140 "literate/scraps.weft"
/* {258: literate/scraps.weft:140} */
int scrap_end_line(int i) { return scrap_array(i).end_line; }
/* {:258} */

#line 144 "literate/scraps.weft"
/* {259: literate/scraps.weft:144} */
const char *scrap_prose(int i) { return scrap_array(i).prose; }
/* {:259} */

#line 148 "literate/scraps.weft"
/* {260: literate/scraps.weft:148} */
Name *scrap_owner(int i) { return scrap_array(i).owner; }
/* {:260} */

#line 169 "literate/scraps.weft"
/* {264: literate/scraps.weft:169} */
typedef struct {
  Slab *scrap;
  Slab *prev;
  int index;
} Manager;
/* {:264} */

#line 179 "literate/scraps.weft"
/* {265: literate/scraps.weft:179} */
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
/* {:265} */

#line 195 "literate/scraps.weft"
/* {266: literate/scraps.weft:195} */
static void pushs(char *s, Manager *manager)
{
  while (*s)
    push(*s++, manager);
}
/* {:266} */

#line 203 "literate/scraps.weft"
/* {267: literate/scraps.weft:203} */
int collect_scrap(void)
{
  int current_scrap, lblseq = 0;
  int depth = 1;
  Manager writer;
  /* Create new scrap, managed by \verb|writer| */
  
#line 214 "literate/scraps.weft"
  /* {268: literate/scraps.weft:214} */
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
    scrap_array(scraps).end_line = source_line;
    scrap_array(scraps).prose = NULL;
    scrap_array(scraps).owner = NULL;
    writer.scrap = scrap;
    writer.index = 0;
    current_scrap = scraps++;
  }/* {:268} */

#line 208 "literate/scraps.weft"

  /* Accumulate scrap and return \verb|scraps++| */
  
#line 236 "literate/scraps.weft"
  /* {269: literate/scraps.weft:236} */
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
              
#line 258 "literate/scraps.weft"
              /* {270: literate/scraps.weft:258} */
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
                  case '|': {
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
                                       name = name_add(&user_names, new_name, sector);
                                       if (!name->uses || name->uses->scrap != current_scrap) {
                                         Scrap_Node *use = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         use->scrap = current_scrap;
                                         use->next = name->uses;
                                         name->uses = use;
                                         add_uses(&(scrap_array(current_scrap).uses), name);
                                       }
                                       break;
                                    case '-':
                                       name = name_add(&user_names, new_name, sector);
                                       if (!name->uses || name->uses->scrap != current_scrap) {
                                         Scrap_Node *use = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         use->scrap = current_scrap;
                                         use->next = name->uses;
                                         name->uses = use;
                                         add_uses(&(scrap_array(current_scrap).uses), name);
                                       }
                                       /* Fall through */
                                    case '|':
                                       sector = current_sector;
                                       /* Fall through */
                                    case '+':
                                       name = name_add(&user_names, new_name, sector);
                                       if (!name->defs || name->defs->scrap != current_scrap) {
                                         Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                                         def->scrap = current_scrap;
                                         def->next = name->defs;
                                         name->defs = def;
                                         add_uses(&(scrap_array(current_scrap).defs), name);
                                       }
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
                            }
#line 268 "literate/scraps.weft"

                            /* Fall through */
                  case ')':
                  case ']':
                  case '}': if (--depth > 0)
                              break;
                            /* else fall through */
                  case ',':
                            push('\0', &writer);
                            scrap_ended_with = c;
                            scrap_array(current_scrap).end_line = source_line;
                            return current_scrap;
                  case '<': {
                              Arglist * args = collect_scrap_name(current_scrap);
                              Name *name = args->name;
                              {
                                char buff[24];

                                push(nw_char, &writer);
                                push('<', &writer);
                                push(name->sector, &writer);
                                sprintf(buff, "%d", arglist_register(args));
                                pushs(buff, &writer);
                              }
                              add_to_use(name, current_scrap);
                              if (scrap_name_has_parameters) {
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
                                }
                              }
                              push(nw_char, &writer);
                              push('>', &writer);
                              c = source_get();
                            }
#line 280 "literate/scraps.weft"

                            break;
                  case '%': {
                                    do
                                            c = source_get();
                                    while (c != '\n');
                            }
#line 282 "literate/scraps.weft"

                            /* emit line break to the output file to keep #line in sync. */
                            push('\n', &writer);
                            c = source_get();
                            break;
                  case 'x': {
                               char  label_name[MAX_NAME_LEN];
                               char * p = label_name;
                               while (c = source_get(), c != nw_char) /* Here is ?-01 */
                                  *p++ = c;
                               *p = '\0';
                               c = source_get();
                               
                               if (label_name[0])
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
                                           fprintf(stderr, "Duplicate label %s.\n", label_name);
                                           break;
                                        }
                                     }
                                     else
                                     {
                                         lbl = (label_node *)arena_getmem(sizeof(label_node) + (p - label_name));
                                         lbl->left = lbl->right = NULL;
                                         strcpy(lbl->name, label_name);
                                         lbl->scrap = current_scrap;
                                         lbl->seq = ++lblseq;
                                         *plbl = lbl;
                                         break;
                                     }
                                  }
                               }
                               
                               else
                               {
                                  fprintf(stderr, "Empty label.\n");
                               }
                               push(nw_char, &writer);
                               push('x', &writer);
                               pushs(label_name, &writer);
                               push(nw_char, &writer);
                            }
#line 287 "literate/scraps.weft"

                            break;
                  case 'c': {
                               char * p = blockBuff;

                               push(nw_char, &writer);
                               do
                               {
                                  push(c, &writer);
                                  c = *p++;
                               } while (c != '\0');
                            }
#line 289 "literate/scraps.weft"

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
              }/* {:270} */

#line 247 "literate/scraps.weft"

                  break;
            }
          push(c, &writer);
                  c = source_get();
                  break;
      }
    }
  }/* {:269} */

#line 209 "literate/scraps.weft"

}
/* {:267} */

#line 416 "literate/scraps.weft"
/* {278: literate/scraps.weft:416} */
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
/* {:278} */

#line 432 "literate/scraps.weft"
/* {280: literate/scraps.weft:432} */
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
/* {:280} */

#line 454 "literate/scraps.weft"
/* {281: literate/scraps.weft:454} */
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
/* {:281} */

#line 513 "literate/scraps.weft"
/* {282: literate/scraps.weft:513} */
void dump_scrap_json(FILE *f, int scrap_idx)
{
  FILE *tmp = tmpfile();
  int ch;
  if (!tmp) { fputs("\"\"", f); return; }
  dump_scrap_text(tmp, scrap_idx);
  fflush(tmp);
  fseek(tmp, 0, SEEK_SET);
  fputc('"', f);
  while ((ch = fgetc(tmp)) != EOF) {
    switch (ch) {
      case '"':  fputs("\\\"", f); break;
      case '\\': fputs("\\\\", f); break;
      case '\n': fputs("\\n", f);  break;
      case '\r': fputs("\\r", f);  break;
      case '\t': fputs("\\t", f);  break;
      default:
        if (ch < 0x20)
          fprintf(f, "\\u%04x", ch);
        else
          fputc(ch, f);
    }
  }
  fputc('"', f);
  fclose(tmp);
}
/* {:282} */

#line 542 "literate/scraps.weft"
/* {283: literate/scraps.weft:542} */
void set_scrap_prose(int i, char *p) { scrap_array(i).prose = p; }
/* {:283} */

#line 546 "literate/scraps.weft"
/* {284: literate/scraps.weft:546} */
void set_scrap_owner(int i, Name *n) { scrap_array(i).owner = n; }
/* {:284} */

#line 550 "literate/scraps.weft"
/* {285: literate/scraps.weft:550} */
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
/* {:285} */

#line 565 "literate/scraps.weft"
/* {286: literate/scraps.weft:565} */
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
/* {:286} */

#line 587 "literate/scraps.weft"
/* {287: literate/scraps.weft:587} */
Arglist * instance(Arglist * a, Arglist * par, char * arg[9], int * ch)
{
   if (a != NULL) {
      int changed = 0;
      Arglist *args, *next;
      Name* name;
      /* Set up name, args and next */
      
#line 609 "literate/scraps.weft"
      /* {289: literate/scraps.weft:609} */
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
      }/* {:289} */

#line 593 "literate/scraps.weft"

      if (changed){
        /* Build a new arglist */
        
#line 631 "literate/scraps.weft"
        /* {290: literate/scraps.weft:631} */
a = (Arglist *)arena_getmem(sizeof(Arglist));
        a->name = name;
        a->args = args;
        a->next = next;/* {:290} */

#line 595 "literate/scraps.weft"

        *ch = 1;
      }
   }

   return a;
}
/* {:287} */

#line 637 "literate/scraps.weft"
/* {291: literate/scraps.weft:637} */
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
  
#line 666 "literate/scraps.weft"
  /* {292: literate/scraps.weft:666} */
{
    c = pop(manager);
    /* Check for macro parameters */
    
#line 647 "literate/parser.weft"
/* {81: literate/parser.weft:647} */

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
    /* {:81} */

#line 668 "literate/scraps.weft"

  }/* {:292} */

#line 659 "literate/scraps.weft"

  return args;
}
/* {:291} */

#line 672 "literate/scraps.weft"
/* {293: literate/scraps.weft:672} */
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
    
#line 727 "literate/scraps.weft"
    /* {296: literate/scraps.weft:727} */
{
      char c;
      Manager reader;
      Parameters local_parameters = 0;
      int line_number = scrap_array(defs->scrap).file_line;
      reader.scrap = scrap_array(defs->scrap).slab;
      reader.prev = NULL;
      reader.index = 0;
      /* Insert debugging information if required */
      
#line 792 "literate/scraps.weft"
      /* {298: literate/scraps.weft:792} */
if (debug_flag) {
        fprintf(file, "\n#line %d \"%s\"\n",
                line_number, scrap_array(defs->scrap).file_name);
        /* Insert appropriate indentation */
        
#line 823 "literate/scraps.weft"
        /* {301: literate/scraps.weft:823} */
{
          char c1 = pop(&reader);
          char c2 = pop(&reader);

          if (indent_flag && !(c1 == '\n'
                               || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))
#line 827 "literate/scraps.weft"
        )) {
            /* Put out the indent */
            
#line 835 "literate/scraps.weft"
            /* {302: literate/scraps.weft:835} */
if (tab_flag)
                for (indent=0; indent<global_indent; indent++)
                  putc(' ', file);
              else
                for (indent=0; indent<global_indent; indent++)
                  putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
            /* {:302} */

#line 828 "literate/scraps.weft"

          }
          indent = 0;
          backup(2, &reader);
        }/* {:301} */

#line 795 "literate/scraps.weft"

      }/* {:298} */

#line 735 "literate/scraps.weft"

      /* Insert section opening marker */
      
#line 804 "literate/scraps.weft"
      /* {299: literate/scraps.weft:804} */
if (location_flag && comment_flag) {
        fprintf(file, "%s{%d: %s:%d}%s\n",
                comment_begin[comment_flag],
                defs->scrap,
                scrap_array(defs->scrap).file_name,
                line_number,
                comment_end[comment_flag]);
      }/* {:299} */

#line 736 "literate/scraps.weft"

      if (delayed_indent)
      {
        /* Insert appropriate indentation */
        
#line 823 "literate/scraps.weft"
        /* {301: literate/scraps.weft:823} */
{
          char c1 = pop(&reader);
          char c2 = pop(&reader);

          if (indent_flag && !(c1 == '\n'
                               || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))
#line 827 "literate/scraps.weft"
        )) {
            /* Put out the indent */
            
#line 835 "literate/scraps.weft"
            /* {302: literate/scraps.weft:835} */
if (tab_flag)
                for (indent=0; indent<global_indent; indent++)
                  putc(' ', file);
              else
                for (indent=0; indent<global_indent; indent++)
                  putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
            /* {:302} */

#line 828 "literate/scraps.weft"

          }
          indent = 0;
          backup(2, &reader);
        }/* {:301} */

#line 739 "literate/scraps.weft"

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
               
#line 823 "literate/scraps.weft"
               /* {301: literate/scraps.weft:823} */
{
                 char c1 = pop(&reader);
                 char c2 = pop(&reader);

                 if (indent_flag && !(c1 == '\n'
                                      || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))
#line 827 "literate/scraps.weft"
               )) {
                   /* Put out the indent */
                   
#line 835 "literate/scraps.weft"
                   /* {302: literate/scraps.weft:835} */
if (tab_flag)
                       for (indent=0; indent<global_indent; indent++)
                         putc(' ', file);
                     else
                       for (indent=0; indent<global_indent; indent++)
                         putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                   /* {:302} */

#line 828 "literate/scraps.weft"

                 }
                 indent = 0;
                 backup(2, &reader);
               }/* {:301} */

#line 750 "literate/scraps.weft"

               break;
             } else {
               /* Don't show newlines in embedded fragmants */
               fputs(". . .", file);
               return 0;
             }
          case '\t': {
                       if (tab_flag)
                         {
                           int delta = 8 - (indent % 8);
                           indent += delta;
                           while (delta > 0) {
                             putc(' ', file);
                             delta--;
                           }
                         }
                       else {
                         putc('\t', file);
                         if (global_indent >= 0) {
                           {
                             if (global_indent + indent < MAX_INDENT)
                               indent_chars[global_indent + indent] = '\t';
                           }
                         }
                         indent++;
                       }
                     }
#line 757 "literate/scraps.weft"

                     delayed_indent = 0;
                     break;
          default:
             if (c==nw_char)
               {
                 /* Check for macro invocation in scrap */
                 
#line 867 "literate/scraps.weft"
                 /* {305: literate/scraps.weft:867} */
{
                   int oldin = indent;
                   char oldcf = comment_flag;
                   c = pop(&reader);
                   switch (c) {
                     case 't': {
                                  char * p = title;
                                  Arglist *q = inArgs;
                                  int narg;

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
                                  }
                                  if (xref_flag) {
                                     putc(' ', file);
                                     write_single_scrap_ref(file, defs->scrap);
                                  }
                               }
#line 872 "literate/scraps.weft"

                               break;
                     case 'c': {
                                  int bgn = indent + global_indent;
                                  int posn = bgn + strlen(comment_begin[comment_flag]);
                                  int i;

                                  if (delayed_indent)
                                     for (i = indent + global_indent; --i >= 0; )
                                        putc(' ', file);
                                  
                                  c = pop(&reader);
                                  fputs(comment_begin[comment_flag], file);
                                  while (c != '\0')
                                  {
                                     do
                                     {
                                        putc(c, file);
                                        posn += 1;
                                        c = pop(&reader);
                                     } while (c > ' ');
                                     
                                     if (c == '\n' || (c == ' ' && posn > 60))
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
                               
#line 874 "literate/scraps.weft"

                               break;
                     case 'f': if (defs->quoted)
                                  fprintf(file, "%cf", nw_char);
                               else
                                  fputs(spelling, file);
                               
#line 876 "literate/scraps.weft"

                               break;
                     case 'x': {
                                  char  label_name[MAX_NAME_LEN];
                                  char * p = label_name;
                                  while (c = pop(&reader), c != nw_char) /* Here is ?-01 */
                                     *p++ = c;
                                  *p = '\0';
                                  c = pop(&reader);
                                  
                                  write_label(label_name, file);
                               }
#line 878 "literate/scraps.weft"

                     case '_': break;
                     case 'v': fputs(version_string, file);
                               
#line 880 "literate/scraps.weft"

                               break;
                     case 's': indent = -global_indent;
                               comment_flag = 0;
                               break;
                     case '<': {
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
                                   unsigned char saved_comment = comment_flag;
                                   char saved_location = location_flag;
                                   char saved_debug = debug_flag;
                                   if (name->lang[0] != '\0') {
                                     comment_flag = name->comment_flag;
                                     location_flag = name->location_flag;
                                     debug_flag = name->debug_flag;
                                   }
                                   if (!newline) {
                                     comment_flag = 0;
                                     location_flag = FALSE;
                                     debug_flag = FALSE;
                                   }
                                   if (comment_flag && newline) {
                                      if (delayed_indent)
                                         for (i = indent + global_indent; --i >= 0; )
                                            putc(' ', file);
                                      
                                      fputs(comment_begin[comment_flag], file);
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
                                      }
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
                                   
                                   name->mark = TRUE;
                                   indent = write_scraps(file, spelling, name->defs, global_indent + indent,
                                                         indent_chars, debug_flag, tab_flag, indent_flag,
                                                         comment_flag, location_flag, args, name->arg,
                                                         local_parameters, name->spelling);
                                   comment_flag = saved_comment;
                                   location_flag = saved_location;
                                   debug_flag = saved_debug;
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
                                   }
                                   fprintf(file, "%c>",  nw_char);
                                   if (!defs->quoted && !tex_flag)
                                     fprintf(stderr, "%s: macro never defined <%s>\n",
                                           command_name, name->spelling);
                                 }
                               }
#line 885 "literate/scraps.weft"

                               /* Insert debugging information if required */
                               
#line 792 "literate/scraps.weft"
                               /* {298: literate/scraps.weft:792} */
if (debug_flag) {
                                 fprintf(file, "\n#line %d \"%s\"\n",
                                         line_number, scrap_array(defs->scrap).file_name);
                                 /* Insert appropriate indentation */
                                 
#line 823 "literate/scraps.weft"
                                 /* {301: literate/scraps.weft:823} */
{
                                   char c1 = pop(&reader);
                                   char c2 = pop(&reader);

                                   if (indent_flag && !(c1 == '\n'
                                                        || (c1 == nw_char && (c2 == '#' || (delayed_indent |= (c2 == '<'))))
#line 827 "literate/scraps.weft"
                                 )) {
                                     /* Put out the indent */
                                     
#line 835 "literate/scraps.weft"
                                     /* {302: literate/scraps.weft:835} */
if (tab_flag)
                                         for (indent=0; indent<global_indent; indent++)
                                           putc(' ', file);
                                       else
                                         for (indent=0; indent<global_indent; indent++)
                                           putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                                     /* {:302} */

#line 828 "literate/scraps.weft"

                                   }
                                   indent = 0;
                                   backup(2, &reader);
                                 }/* {:301} */

#line 795 "literate/scraps.weft"

                               }/* {:298} */

#line 886 "literate/scraps.weft"

                               indent = oldin;
                               comment_flag = oldcf;
                               break;
                     /* Handle macro parameter substitution */
                     
#line 545 "literate/parser.weft"
/* {79: literate/parser.weft:545} */

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
                            
#line 983 "literate/scraps.weft"
                            /* {310: literate/scraps.weft:983} */
if (comment_flag && newline) {
                               /* Perhaps put a delayed indent */
                               
#line 1000 "literate/scraps.weft"
                               /* {311: literate/scraps.weft:1000} */
if (delayed_indent)
                                  for (i = indent + global_indent; --i >= 0; )
                                     putc(' ', file);
                               /* {:311} */

#line 984 "literate/scraps.weft"

                               fputs(comment_begin[comment_flag], file);
                               /* Comment this macro use */
                               
#line 1019 "literate/scraps.weft"
                               /* {313: literate/scraps.weft:1019} */
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
                               }/* {:313} */

#line 986 "literate/scraps.weft"

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
                            /* {:310} */

#line 570 "literate/parser.weft"

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
                              
#line 835 "literate/scraps.weft"
                              /* {302: literate/scraps.weft:835} */
if (tab_flag)
                                  for (indent=0; indent<global_indent; indent++)
                                    putc(' ', file);
                                else
                                  for (indent=0; indent<global_indent; indent++)
                                    putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                              /* {:302} */

#line 581 "literate/parser.weft"

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
                           
#line 835 "literate/scraps.weft"
                           /* {302: literate/scraps.weft:835} */
if (tab_flag)
                               for (indent=0; indent<global_indent; indent++)
                                 putc(' ', file);
                             else
                               for (indent=0; indent<global_indent; indent++)
                                 putc(indent < MAX_INDENT ? indent_chars[indent] : ' ', file);
                           /* {:302} */

#line 596 "literate/parser.weft"

                         }
                       }
                     /* {:79} */

#line 890 "literate/scraps.weft"

                               indent = oldin;
                               break;
                     default:
                           if(c==nw_char)
                             {
                               putc(c, file);
                               if (global_indent >= 0) {
                                  /* Add more indentation '' '' */
                                  
#line 785 "literate/scraps.weft"
                                  /* {297: literate/scraps.weft:785} */
{
                                    if (global_indent + indent < MAX_INDENT)
                                      indent_chars[global_indent + indent] = ' ';
                                  }/* {:297} */

#line 898 "literate/scraps.weft"

                               }
                               indent++;
                               break;
                             }
                           /* ignore, since we should already have a warning */
                               break;
                   }
                 }/* {:305} */

#line 763 "literate/scraps.weft"

                 break;
               }
             putc(c, file);
             if (global_indent >= 0) {
               /* Add more indentation '' '' */
               
#line 785 "literate/scraps.weft"
               /* {297: literate/scraps.weft:785} */
{
                 if (global_indent + indent < MAX_INDENT)
                   indent_chars[global_indent + indent] = ' ';
               }/* {:297} */

#line 768 "literate/scraps.weft"

             }
             indent++;
             if (c > ' ') newline = 0;
             delayed_indent = 0;
             break;
        }
        c = pop(&reader);
      }
      /* Insert section closing marker */
      
#line 814 "literate/scraps.weft"
      /* {300: literate/scraps.weft:814} */
if (location_flag && comment_flag) {
        fprintf(file, "%s{:%d}%s\n",
                comment_begin[comment_flag],
                defs->scrap,
                comment_end[comment_flag]);
      }/* {:300} */

#line 777 "literate/scraps.weft"

    }/* {:296} */

#line 683 "literate/scraps.weft"

    defs = defs->next;
  }
  return indent + global_indent;
}
/* {:293} */

#line 1092 "literate/scraps.weft"
/* {318: literate/scraps.weft:1092} */
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
        
#line 1118 "literate/scraps.weft"
/* {319: literate/scraps.weft:1118} */

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
              
#line 152 "literate/scraps.weft"
              /* {261: literate/scraps.weft:152} */
{
                if (!already_warned) {
                  fprintf(stderr, "%s: you'll need to rerun weft after running latex\n",
                          command_name);
                  already_warned = TRUE;
                }
              }/* {:261} */

#line 1138 "literate/scraps.weft"

          }
        }
        /* {:319} */

#line 1105 "literate/scraps.weft"

      }
      fclose(aux_file);
      /* Add letters to scraps with duplicate page numbers */
      
#line 1145 "literate/scraps.weft"
      /* {320: literate/scraps.weft:1145} */
{
         int i = 0;

         /* Step 'i' to the next valid scrap */
         
#line 1159 "literate/scraps.weft"
         /* {321: literate/scraps.weft:1159} */
do
            i++;
         while (i < scraps && scrap_array(i).page == -1);
         /* {:321} */

#line 1148 "literate/scraps.weft"

         /* For all remaining scraps */
         
#line 1165 "literate/scraps.weft"
         /* {322: literate/scraps.weft:1165} */
while (i < scraps)/* {:322} */

#line 1149 "literate/scraps.weft"
       {
            int j = i;
            /* Step 'j' to the next valid scrap */
            
#line 1159 "literate/scraps.weft"
            /* {321: literate/scraps.weft:1159} */
do
               j++;
            while (j < scraps && scrap_array(j).page == -1);
            /* {:321} */

#line 1151 "literate/scraps.weft"

            /* Perhaps add letters to the page numbers */
            
#line 1168 "literate/scraps.weft"
            /* {323: literate/scraps.weft:1168} */
if (scrap_array(i).page == scrap_array(j).page) {
               if (scrap_array(i).letter == 0)
                  scrap_array(i).letter = 'a';
               scrap_array(j).letter = scrap_array(i).letter + 1;
            }
            /* {:323} */

#line 1152 "literate/scraps.weft"

            i = j;
         }
      }
      /* {:320} */

#line 1108 "literate/scraps.weft"

    }
  }
}
/* {:318} */

#line 11 "literate/search-labels.weft"
/* {369: literate/search-labels.weft:11} */
typedef struct name_node {
  struct name_node *next;
  Name *name;
} Name_Node;
/* {:369} */

#line 18 "literate/search-labels.weft"
/* {370: literate/search-labels.weft:18} */
typedef struct goto_node {
  Name_Node *output;            /* list of words ending in this state */
  struct move_node *moves;      /* list of possible moves */
  struct goto_node *fail;       /* and where to go when no move fits */
  struct goto_node *next;       /* next goto node with same depth */
} Goto_Node;
/* {:370} */

#line 27 "literate/search-labels.weft"
/* {371: literate/search-labels.weft:27} */
typedef struct move_node {
  struct move_node *next;
  Goto_Node *state;
  char c;
} Move_Node;
/* {:371} */

#line 35 "literate/search-labels.weft"
/* {372: literate/search-labels.weft:35} */
static Goto_Node *root[256];
static int max_depth;
static Goto_Node **depths;
/* {:372} */

#line 42 "literate/search-labels.weft"
/* {373: literate/search-labels.weft:42} */
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
/* {:373} */

#line 57 "literate/search-labels.weft"
/* {374: literate/search-labels.weft:57} */
typedef struct ArgMgr_s
{
   char * pv;
   char * bgn;
   Arglist * arg;
   struct ArgMgr_s * old;
} ArgMgr;
/* {:374} */

#line 67 "literate/search-labels.weft"
/* {375: literate/search-labels.weft:67} */
typedef struct ArgManager_s
{
   Manager * m;
   ArgMgr * a;
} ArgManager;
/* {:375} */

#line 75 "literate/search-labels.weft"
/* {376: literate/search-labels.weft:75} */
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
/* {:376} */

#line 93 "literate/search-labels.weft"
/* {377: literate/search-labels.weft:93} */
static char argpop(ArgManager * mgr)
{
   while (mgr->a != NULL)
   {
      ArgMgr * a = mgr->a;

      /* Perhaps |return| a character from the current arg */
      
#line 111 "literate/search-labels.weft"
      /* {378: literate/search-labels.weft:111} */
if (a->pv != NULL)
      {
         char c = *a->pv++;

         if (c != '\0')
            return c;
         a->pv = NULL;
         return ' ';
      }
      /* {:378} */

#line 99 "literate/search-labels.weft"

      /* Perhaps start a new arg */
      
#line 127 "literate/search-labels.weft"
      /* {379: literate/search-labels.weft:127} */
if (a->arg) {
         Arglist * b = a->arg;

         a->arg = b->next;
         if (b->name == NULL) {
            a->bgn = a->pv = (char *)b->args;
         } else if (b->name == (Name *)1) {
            a->bgn = a->pv = "{Embedded Scrap}";
         } else {
            pushArglist(mgr, b->args);
         }/* {:379} */

#line 100 "literate/search-labels.weft"

      /* Otherwise pop the current arg */
      
#line 140 "literate/search-labels.weft"
      /* {380: literate/search-labels.weft:140} */
} else {
         mgr->a = a->old;
         free(a);
      }/* {:380} */

#line 101 "literate/search-labels.weft"

   }

   return (pop(mgr->m));
}
/* {:377} */

#line 146 "literate/search-labels.weft"
/* {381: literate/search-labels.weft:146} */
static char
prev_char(ArgManager * mgr, int n)
{
   char c = '\0';
   ArgMgr * a = mgr->a;
   Manager * m = mgr->m;

   if (a != NULL) {
      /* Get the nth previous character from an argument */
      
#line 164 "literate/search-labels.weft"
      /* {382: literate/search-labels.weft:164} */
if (a->pv && a->pv - n >= a->bgn)
         c = *a->pv;
      else if (a->bgn) {
         int j = strlen(a->bgn) + 1;

         if (n >= j)
            c = a->bgn[j - n];
         else
            c = ' ';
      }
      /* {:382} */

#line 154 "literate/search-labels.weft"

   } else {
      /* Get the nth previous character from a scrap */
      
#line 182 "literate/search-labels.weft"
      /* {383: literate/search-labels.weft:182} */
int k = m->index - n - 2;

      if (k >= 0)
         c = m->scrap->chars[k];
      else if (m->prev)
         c = m->prev->chars[SLAB_SIZE + k];
      /* {:383} */

#line 156 "literate/search-labels.weft"

   }

   return c;
}
/* {:381} */

#line 197 "literate/search-labels.weft"
/* {385: literate/search-labels.weft:197} */
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
  
#line 281 "literate/search-labels.weft"
  /* {388: literate/search-labels.weft:281} */
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
  }/* {:388} */

#line 210 "literate/search-labels.weft"

  /* Search scraps */
  
#line 318 "literate/search-labels.weft"
  /* {389: literate/search-labels.weft:318} */
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
        
#line 365 "literate/search-labels.weft"
        /* {390: literate/search-labels.weft:365} */
if (last == nw_char && c == nw_char)
        {
           last = '\0';
           c = argpop(&reader);
        }
        /* {:390} */

#line 337 "literate/search-labels.weft"

        /* Skip over a scrap use */
        
#line 373 "literate/search-labels.weft"
        /* {391: literate/search-labels.weft:373} */
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
        }/* {:391} */

#line 338 "literate/search-labels.weft"

        /* Skip over a block comment */
        
#line 499 "literate/parser.weft"
        /* {75: literate/parser.weft:499} */
if (last == nw_char && c == 'c')
           while ((c = pop(reader.m)) != '\0')
              /* Skip */;
        /* {:75} */

#line 339 "literate/search-labels.weft"

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
  }/* {:389} */

#line 211 "literate/search-labels.weft"

}
/* {:385} */

#line 218 "literate/search-labels.weft"
/* {386: literate/search-labels.weft:218} */
static void build_gotos(Name *tree)
{
  while (tree) {
    /* Extend goto graph with \verb|tree->spelling| */
    
#line 229 "literate/search-labels.weft"
    /* {387: literate/search-labels.weft:229} */
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
    }/* {:387} */

#line 221 "literate/search-labels.weft"

    build_gotos(tree->rlink);
    tree = tree->llink;
  }
}
/* {:386} */

#line 400 "literate/search-labels.weft"
/* {393: literate/search-labels.weft:400} */

static int scrap_is_in(Scrap_Node * list, int i)
{
  while (list != NULL) {
    if (list->scrap == i)
      return TRUE;
    list = list->next;
  }
  return FALSE;
}
/* {:393} */

#line 413 "literate/search-labels.weft"
/* {394: literate/search-labels.weft:413} */

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
/* {:394} */

#line 444 "literate/search-labels.weft"
/* {397: literate/search-labels.weft:444} */

void
format_uses_refs(FILE * tex_file, int scrap)
{
  Uses * p = scrap_array(scrap).uses;
  if (p != NULL)
    /* Write uses references */
    
#line 455 "literate/search-labels.weft"
    /* {398: literate/search-labels.weft:455} */
{
      char join = ' ';
      fputs("\\item \\NWtxtIdentsUsed\\nobreak\\", tex_file);
      do {
        /* Write one use reference */
        
#line 467 "literate/search-labels.weft"
        /* {399: literate/search-labels.weft:467} */
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
            
#line 488 "literate/search-labels.weft"
            /* {400: literate/search-labels.weft:488} */
fputs("\\NWlink{weft", tex_file);
            write_scrap_ref(tex_file, defs->scrap, -1, &page);
            fputs("}{", tex_file);
            write_scrap_ref(tex_file, defs->scrap, first, &page);
            fputs("}", tex_file);/* {:400} */

#line 476 "literate/search-labels.weft"

            first = FALSE;
            defs = defs->next;
          }while (defs!= NULL);
        }
        else
        {
          fputs("\\NWnotglobal", tex_file);
        }
        /* {:399} */

#line 459 "literate/search-labels.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".", tex_file);
    }/* {:398} */

#line 450 "literate/search-labels.weft"

}
/* {:397} */

#line 499 "literate/search-labels.weft"
/* {402: literate/search-labels.weft:499} */

void
format_defs_refs(FILE * tex_file, int scrap)
{
  Uses * p = scrap_array(scrap).defs;
  if (p != NULL)
    /* Write defs references */
    
#line 510 "literate/search-labels.weft"
    /* {403: literate/search-labels.weft:510} */
{
      char join = ' ';
      fputs("\\item \\NWtxtIdentsDefed\\nobreak\\", tex_file);
      do {
        /* Write one def reference */
        
#line 522 "literate/search-labels.weft"
        /* {404: literate/search-labels.weft:522} */
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
               
#line 488 "literate/search-labels.weft"
               /* {400: literate/search-labels.weft:488} */
fputs("\\NWlink{weft", tex_file);
               write_scrap_ref(tex_file, defs->scrap, -1, &page);
               fputs("}{", tex_file);
               write_scrap_ref(tex_file, defs->scrap, first, &page);
               fputs("}", tex_file);/* {:400} */

#line 535 "literate/search-labels.weft"

               first = FALSE;
            }
            defs = defs->next;
          }while (defs!= NULL);
        }
        /* {:404} */

#line 514 "literate/search-labels.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".", tex_file);
    }/* {:403} */

#line 505 "literate/search-labels.weft"

}
/* {:402} */

#line 554 "literate/search-labels.weft"
/* {405: literate/search-labels.weft:554} */
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
/* {:405} */

#line 570 "literate/search-labels.weft"
/* {406: literate/search-labels.weft:570} */
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
/* {:406} */

#line 600 "literate/search-labels.weft"
/* {408: literate/search-labels.weft:600} */
void
write_label(char label_name[], FILE * file)
/* Search for label(<Write the label to file>,<Complain about missing label>) */

#line 639 "literate/search-labels.weft"
/* {416: literate/search-labels.weft:639} */
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
            
#line 610 "literate/search-labels.weft"
            /* {410: literate/search-labels.weft:610} */
write_single_scrap_ref(file, lbl->scrap);
            fprintf(file, "-%02d", lbl->seq);/* {:410} */

            break;
         }
      }
      else
      {
          /* Complain about missing label */
          
#line 614 "literate/search-labels.weft"
          /* {411: literate/search-labels.weft:614} */
fprintf(stderr, "Can't find label %s.\n", label_name);/* {:411} */

          break;
      }
   }
}
/* {:416} */

#line 602 "literate/search-labels.weft"

/* {:408} */
