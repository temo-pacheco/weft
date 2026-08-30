
#line 113 "literate/architecture.weft"
/* {7: literate/architecture.weft:113} */
#include "global.h"
/* {:7} */

#line 21 "literate/parser.weft"
/* {43: literate/parser.weft:21} */
static char prose_buf[4096];
static int prose_len = 0;
static char *pending_prose = NULL;
/* {:43} */

#line 27 "literate/parser.weft"
/* {44: literate/parser.weft:27} */
void pass1(char *file_name)
{
  if (verbose_flag)
    fprintf(stderr, "reading %s\n", file_name);
  source_open(file_name);
  init_scraps();
  macro_names = NULL;
  file_names = NULL;
  user_names = NULL;
  prose_len = 0;
  pending_prose = NULL;
  /* Scan the source file, looking for at-sequences */
  
#line 49 "literate/parser.weft"
  /* {45: literate/parser.weft:49} */
{
    int c = source_get();
    while (c != EOF) {
      if (c == nw_char)
        /* Scan at-sequence */
        
#line 148 "literate/parser.weft"
        /* {52: literate/parser.weft:148} */
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
            case 'o': {
                        Name *name;
                        int scrap;
                        if (prose_flag && prose_len > 0) {
                          /* trim trailing whitespace */
                          while (prose_len > 0 && (prose_buf[prose_len-1] == ' '
                                 || prose_buf[prose_len-1] == '\n'
                                 || prose_buf[prose_len-1] == '\t'
                                 || prose_buf[prose_len-1] == '\r'))
                            prose_len--;
                          prose_buf[prose_len] = '\0';
                          pending_prose = save_string(prose_buf);
                          prose_len = 0;
                        } else {
                          pending_prose = NULL;
                          prose_len = 0;
                        }
                        name = collect_file_name(); /* returns a pointer to the name entry */
                        scrap = collect_scrap();      /* returns an index to the scrap */
                        if (pending_prose) set_scrap_prose(scrap, pending_prose);
                        pending_prose = NULL;
                        {
                          Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                          def->scrap = scrap;
                          def->quoted = quoted;
                          def->next = name->defs;
                          name->defs = def;
                          set_scrap_owner(scrap, name);
                        }
                      }
                      break;
            case 'Q':
            case 'q': quoted = 1;
                      FALLTHROUGH;
            case 'D':
            case 'd': {
                        Name *name;
                        int scrap;
                        if (prose_flag && prose_len > 0) {
                          /* trim trailing whitespace */
                          while (prose_len > 0 && (prose_buf[prose_len-1] == ' '
                                 || prose_buf[prose_len-1] == '\n'
                                 || prose_buf[prose_len-1] == '\t'
                                 || prose_buf[prose_len-1] == '\r'))
                            prose_len--;
                          prose_buf[prose_len] = '\0';
                          pending_prose = save_string(prose_buf);
                          prose_len = 0;
                        } else {
                          pending_prose = NULL;
                          prose_len = 0;
                        }
                        name = collect_macro_name();
                        scrap = collect_scrap();
                        if (pending_prose) set_scrap_prose(scrap, pending_prose);
                        pending_prose = NULL;
                        {
                          Scrap_Node *def = (Scrap_Node *) arena_getmem(sizeof(Scrap_Node));
                          def->scrap = scrap;
                          def->quoted = quoted;
                          def->next = name->defs;
                          name->defs = def;
                          set_scrap_owner(scrap, name);
                        }
                      }
                      break;
            case 's':
                      /* Step to next sector */
                      
#line 202 "literate/parser.weft"
/* {53: literate/parser.weft:202} */

                      prev_sector += 1;
                      current_sector = prev_sector;
                      c = source_get();
                      /* {:53} */

#line 168 "literate/parser.weft"

                      break;
            case 'S':
                      /* Close the current sector */
                      
#line 209 "literate/parser.weft"
                      /* {54: literate/parser.weft:209} */
current_sector = 1;
                      c = source_get();
                      /* {:54} */

#line 171 "literate/parser.weft"

                      break;
            case '<':
            case '(':
            case '[':
            case '{': 
                      {
                         int c;
                         int depth = 1;
                         while ((c = source_get()) != EOF) {
                            if (c == nw_char)
                               
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
                               
                         }
                         fprintf(stderr, "%s: unexpected EOF in text at (%s, %d)\n",
                                          command_name, source_name, source_line);
                         exit(-1);

                      skipped:  ;
                      }
                      
                      break;
            case 'c': {
                         char * p = blockBuff;
                         char * e = blockBuff + (sizeof(blockBuff)/sizeof(blockBuff[0])) - 1;

                         while (source_peek == ' '
                                || source_peek == '\t'
                                || source_peek == '\n')
                            (void)source_get();
                         
                         while (p < e)
                         {
                            int c = source_get();

                            if (c == nw_char)
                            {
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
                               
                            }
                            else if (c == EOF)
                            {
                               source_ungetc(&c);
                               break;
                            }
                            else
                            {
                               
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
                                  
                                  *p++ = c;
                               
                            }
                            
                         }
                         if (p == e)
                         {
                            int c;

                            while ((c = source_get()), c != nw_char && c != EOF)/* Skip */
                            source_ungetc(&c);
                         }
                         *p = '\000';
                      }
                      
                      break;
            case 'l': {
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
                      }
                      break;
            case 'L': collect_lang_def();
                      break;
            case 'W': collect_weave_format();
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
        }/* {:52} */

#line 53 "literate/parser.weft"

      else if (prose_flag) {
        /* Accumulate prose character */
        
#line 62 "literate/parser.weft"
        /* {46: literate/parser.weft:62} */
if (prose_len < (int)sizeof(prose_buf) - 1)
          prose_buf[prose_len++] = c;
        /* {:46} */

#line 55 "literate/parser.weft"

      }
      c = source_get();
    }
  }/* {:45} */

#line 38 "literate/parser.weft"

  if (tex_flag)
    search();
  /* Reverse cross-reference lists */
  
#line 522 "literate/parser.weft"
  /* {78: literate/parser.weft:522} */
{
    reverse_lists(file_names);
    reverse_lists(macro_names);
    reverse_lists(user_names);
  }/* {:78} */

#line 41 "literate/parser.weft"

}
/* {:44} */
