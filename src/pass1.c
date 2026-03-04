
#line 102 "literate/architecture.weft"
/* {6: literate/architecture.weft:102} */
#include "global.h"
/* {:6} */

#line 21 "literate/parser.weft"
/* {38: literate/parser.weft:21} */
void pass1(char *file_name)
{
  if (verbose_flag)
    fprintf(stderr, "reading %s\n", file_name);
  source_open(file_name);
  init_scraps();
  macro_names = NULL;
  file_names = NULL;
  user_names = NULL;
  /* Scan the source file, looking for at-sequences */
  
#line 41 "literate/parser.weft"
  /* {39: literate/parser.weft:41} */
{
    int c = source_get();
    while (c != EOF) {
      if (c == nw_char)
        /* Scan at-sequence */
        
#line 116 "literate/parser.weft"
        /* {44: literate/parser.weft:116} */
{
          char quoted = 0;

          c = source_get();
          switch (c) {
            case 'r':
                  c = source_get();
                  nw_char = c;
                  update_delimit_scrap();
                  break;
            case 'O':
            case 'o': 
#line 201 "literate/parser.weft"
                      /* {49: literate/parser.weft:201} */
{
                        Name *name = collect_file_name(); /* returns a pointer to the name entry */
                        int scrap = collect_scrap();      /* returns an index to the scrap */
                        /* Add \verb|scrap| to \verb|name|'s definition list */
                        
#line 220 "literate/parser.weft"
                        /* {51: literate/parser.weft:220} */
{
                          Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                          def->scrap = scrap;
                          def->quoted = quoted;
                          def->next = name->defs;
                          name->defs = def;
                        }/* {:51} */

#line 204 "literate/parser.weft"

                      }/* {:49} */

#line 127 "literate/parser.weft"

                      break;
            case 'Q':
            case 'q': quoted = 1;
                      FALLTHROUGH;
            case 'D':
            case 'd': 
#line 209 "literate/parser.weft"
                      /* {50: literate/parser.weft:209} */
{
                        Name *name = collect_macro_name();
                        int scrap = collect_scrap();
                        /* Add \verb|scrap| to \verb|name|'s definition list */
                        
#line 220 "literate/parser.weft"
                        /* {51: literate/parser.weft:220} */
{
                          Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                          def->scrap = scrap;
                          def->quoted = quoted;
                          def->next = name->defs;
                          name->defs = def;
                        }/* {:51} */

#line 212 "literate/parser.weft"

                      }/* {:50} */

#line 133 "literate/parser.weft"

                      break;
            case 's':
                      /* Step to next sector */
                      
#line 170 "literate/parser.weft"
/* {45: literate/parser.weft:170} */

                      prev_sector += 1;
                      current_sector = prev_sector;
                      c = source_get();
                      /* {:45} */

#line 136 "literate/parser.weft"

                      break;
            case 'S':
                      /* Close the current sector */
                      
#line 177 "literate/parser.weft"
                      /* {46: literate/parser.weft:177} */
current_sector = 1;
                      c = source_get();
                      /* {:46} */

#line 139 "literate/parser.weft"

                      break;
            case '<':
            case '(':
            case '[':
            case '{': 
#line 229 "literate/parser.weft"
/* {52: literate/parser.weft:229} */

                      {
                         int c;
                         int depth = 1;
                         while ((c = source_get()) != EOF) {
                            if (c == nw_char)
                               /* Skip over at-sign or go to skipped */
                               
#line 246 "literate/parser.weft"
/* {53: literate/parser.weft:246} */

                               {
                                  c = source_get();
                                  switch (c) {
                                    case '{': case '[': case '(': case '<':
                                       depth += 1;
                                       break;
                                    case '}': case ']': case ')': case '>':
                                       if (--depth == 0)
                                          goto skipped;
                                    case 'x': case '|': case ',':
                                    case '%': case '1': case '2':
                                    case '3': case '4': case '5': case '6':
                                    case '7': case '8': case '9': case '_':
                                    case 'f': case '#': case '+': case '-':
                                    case 'v': case '*': case 'c': case '\'':
                                    case 's':
                                       break;
                                    default:
                                       if (c != nw_char) {
                                          fprintf(stderr, "%s: unexpected %c%c in text at (%s, %d)\n",
                                                          command_name, nw_char, c, source_name, source_line);
                                          exit(-1);
                                       }
                                       break;
                                  }
                               }
                               /* {:53} */

#line 235 "literate/parser.weft"

                         }
                         fprintf(stderr, "%s: unexpected EOF in text at (%s, %d)\n",
                                          command_name, source_name, source_line);
                         exit(-1);

                      skipped:  ;
                      }
                      /* {:52} */

#line 144 "literate/parser.weft"

                      break;
            case 'c': 
#line 277 "literate/parser.weft"
                      /* {54: literate/parser.weft:277} */
{
                         char * p = blockBuff;
                         char * e = blockBuff + (sizeof(blockBuff)/sizeof(blockBuff[0])) - 1;

                         /* Skip whitespace */
                         
#line 295 "literate/parser.weft"
                         /* {55: literate/parser.weft:295} */
while (source_peek == ' '
                                || source_peek == '\t'
                                || source_peek == '\n')
                            (void)source_get();
                         /* {:55} */

#line 281 "literate/parser.weft"

                         while (p < e)
                         {
                            /* Add one char to the block buffer */
                            
#line 302 "literate/parser.weft"
                            /* {56: literate/parser.weft:302} */
int c = source_get();

                            if (c == nw_char)
                            {
                               /* Add an at character to the block or break */
                               
#line 320 "literate/parser.weft"
                               /* {57: literate/parser.weft:320} */
int cc = source_peek;

                               if (cc == 'c')
                               {
                                  do
                                     c = source_get();
                                  while (c <= ' ');

                                  break;
                               }
                               else if (cc == 'd'
                                        || cc == 'D'
                                        || cc == 'q'
                                        || cc == 'Q'
                                        || cc == 'o'
                                        || cc == 'O'
                                        || cc == EOF)
                               {
                                  source_ungetc(&c);
                                  break;
                               }
                               else
                               {
                                  *p++ = c;
                                  *p++ = source_get();
                               }
                               /* {:57} */

#line 306 "literate/parser.weft"

                            }
                            else if (c == EOF)
                            {
                               source_ungetc(&c);
                               break;
                            }
                            else
                            {
                               /* Add any other character to the block */
                               
#line 349 "literate/parser.weft"
/* {58: literate/parser.weft:349} */

                                  /* Perhaps skip white-space */
                                  
#line 355 "literate/parser.weft"
                                  /* {59: literate/parser.weft:355} */
if (c == ' ')
                                  {
                                     while (source_peek == ' ')
                                        c = source_get();
                                  }
                                  if (c == '\n')
                                  {
                                     if (source_peek == '\n')
                                     {
                                        do
                                           c = source_get();
                                        while (source_peek == '\n');
                                     }
                                     else
                                        c = ' ';
                                  }
                                  /* {:59} */

#line 350 "literate/parser.weft"

                                  *p++ = c;
                               /* {:58} */

#line 315 "literate/parser.weft"

                            }
                            /* {:56} */

#line 284 "literate/parser.weft"

                         }
                         if (p == e)
                         {
                            /* Skip to the next nw-char */
                            
#line 374 "literate/parser.weft"
                            /* {60: literate/parser.weft:374} */
int c;

                            while ((c = source_get()), c != nw_char && c != EOF)/* Skip */
                            source_ungetc(&c);/* {:60} */

#line 288 "literate/parser.weft"

                         }
                         *p = '\000';
                      }
                      /* {:54} */

#line 146 "literate/parser.weft"

                      break;
            case 'l': 
#line 65 "literate/parser.weft"
                      /* {40: literate/parser.weft:65} */
{
                        char lang[64];
                        char *lp = lang;
                        int lc = source_get();
                        /* skip optional whitespace */
                        while (lc == ' ' || lc == '\t')
                          lc = source_get();
                        /* read language tag */
                        while (lc != EOF && lc != '\n' && lc != ' ' && lc != '\t'
                               && (lp - lang) < 62) {
                          *lp++ = lc;
                          lc = source_get();
                        }
                        *lp = '\0';
                        if (lang[0] != '\0') {
                          strncpy(pending_lang, lang, 63);
                          pending_lang[63] = '\0';
                        }
                        /* consume rest of line */
                        while (lc != EOF && lc != '\n')
                          lc = source_get();
                      }/* {:40} */

#line 148 "literate/parser.weft"

                      break;
            case 'L': 
#line 96 "literate/parser.weft"
                      /* {41: literate/parser.weft:96} */
collect_lang_def();/* {:41} */

#line 150 "literate/parser.weft"

                      break;
            case 'W': 
#line 99 "literate/parser.weft"
                      /* {42: literate/parser.weft:99} */
collect_weave_format();/* {:42} */

#line 152 "literate/parser.weft"

                      break;
            case 'x':
            case 'v':
            case 'u':
            case 'm':
            case 'f': /* ignore during this pass */
                      break;
            default:  if (c==nw_char) /* ignore during this pass */
                        break;
                      fprintf(stderr,
                              "%s: unexpected %c sequence ignored (%s, line %d)\n",
                              command_name, nw_char, source_name, source_line);
                      break;
          }
        }/* {:44} */

#line 45 "literate/parser.weft"

      c = source_get();
    }
  }/* {:39} */

#line 30 "literate/parser.weft"

  if (tex_flag)
    search();
  /* Reverse cross-reference lists */
  
#line 479 "literate/parser.weft"
  /* {70: literate/parser.weft:479} */
{
    reverse_lists(file_names);
    reverse_lists(macro_names);
    reverse_lists(user_names);
  }/* {:70} */

#line 33 "literate/parser.weft"

}
/* {:38} */
