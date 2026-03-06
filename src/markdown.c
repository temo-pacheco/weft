
#line 117 "literate/architecture.weft"
/* {8: literate/architecture.weft:117} */
#include "global.h"
static int scraps = 1;
/* {:8} */

#line 17 "literate/markdown-output.weft"
/* {146: literate/markdown-output.weft:17} */
static void md_copy_scrap(FILE *file, int prefix, Name *name);
static void md_print_scrap_numbers(FILE *md_file, Scrap_Node *scraps);
static void md_format_entry(Name *name, FILE *md_file, unsigned char sector);
static void md_format_file_entry(Name *name, FILE *md_file);
static void md_format_user_entry(Name *name, FILE *md_file, unsigned char sector);
static void md_write_arg(FILE *md_file, char *p);
static void md_write_literal(FILE *md_file, char *p);
static void md_write_ArglistElement(FILE *file, Arglist *args, char **params);
static void md_format_uses_refs(FILE *md_file, int scrap);
static void md_format_defs_refs(FILE *md_file, int scrap);
/* {:146} */

#line 33 "literate/markdown-output.weft"
/* {147: literate/markdown-output.weft:33} */
void write_md(char *file_name, char *md_name, unsigned char sector)
{
  (void)sector;
  FILE *md_file = fopen(md_name, "w");
  if (md_file) {
    if (verbose_flag)
      fprintf(stderr, "writing %s\n", md_name);
    source_open(file_name);
    /* Copy \verb|source_file| into \verb|md_file| */
    
#line 57 "literate/markdown-output.weft"
    /* {148: literate/markdown-output.weft:57} */
{
      int c = source_get();
      int at_line_start = TRUE;
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret md at-sequence */
          
#line 106 "literate/markdown-output.weft"
          /* {150: literate/markdown-output.weft:106} */
{
            int big_definition = FALSE;
            c = source_get();
            switch (c) {
              case 'r':
                    c = source_get();
                    nw_char = c;
                    update_delimit_scrap();
                    break;
              case 'O': big_definition = TRUE;
                        FALLTHROUGH;
              case 'o': 
#line 206 "literate/markdown-output.weft"
                        /* {154: literate/markdown-output.weft:206} */
{
                          Name *name = collect_file_name();
                          (void)big_definition;
                          fputs("\n<a id=\"weft", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("\"></a>\n", md_file);
                          fprintf(md_file, "**\"%s\"**", name->spelling);
                          fputs(" **", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("** =\n", md_file);
                          /* Fill in the md scrap */
                          
#line 283 "literate/markdown-output.weft"
                          /* {158: literate/markdown-output.weft:283} */
{
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                          }/* {:158} */

#line 216 "literate/markdown-output.weft"

                          /* Write md file cross-refs */
                          
#line 313 "literate/markdown-output.weft"
                          /* {161: literate/markdown-output.weft:313} */
{
                            if ( scrap_flag ) {
                              /* Write md file defs */
                              
#line 322 "literate/markdown-output.weft"
                              /* {162: literate/markdown-output.weft:322} */
{
                                if (name->defs) {
                                  if (name->defs->next) {
                                    fputs("> *File defined by* ", md_file);
                                    md_print_scrap_numbers(md_file, name->defs);
                                  }
                                } else {
                                  fprintf(stderr,
                                          "would have crashed in 'Write md file defs' for '%s'\n",
                                           name->spelling);
                                }
                              }/* {:162} */

#line 315 "literate/markdown-output.weft"

                            }
                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }/* {:161} */

#line 217 "literate/markdown-output.weft"

                          /* Finish md scrap */
                          
#line 295 "literate/markdown-output.weft"
                          /* {159: literate/markdown-output.weft:295} */
{
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }/* {:159} */

#line 218 "literate/markdown-output.weft"

                        }/* {:154} */

#line 117 "literate/markdown-output.weft"

                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
                        FALLTHROUGH;
              case 'q':
              case 'd': 
#line 223 "literate/markdown-output.weft"
                        /* {155: literate/markdown-output.weft:223} */
{
                          Name *name = collect_macro_name();
                          (void)big_definition;
                          fputs("\n<a id=\"weft", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("\"></a>\n", md_file);
                          fputs("**&lt;*", md_file);
                          /* Write the md macro's name */
                          
#line 240 "literate/markdown-output.weft"
                          /* {156: literate/markdown-output.weft:240} */
{
                            char * p = name->spelling;
                            int i = 0;

                            while (*p != '\000') {
                              if (*p == ARG_CHR) {
                                md_write_arg(md_file, name->arg[i++]);
                                p++;
                              }
                              else
                                 fputc(*p++, md_file);
                            }
                          }/* {:156} */

#line 230 "literate/markdown-output.weft"

                          fputs("* ", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("&gt;** =\n", md_file);
                          /* Fill in the md scrap */
                          
#line 283 "literate/markdown-output.weft"
                          /* {158: literate/markdown-output.weft:283} */
{
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                          }/* {:158} */

#line 234 "literate/markdown-output.weft"

                          /* Write md macro cross-refs */
                          
#line 336 "literate/markdown-output.weft"
                          /* {163: literate/markdown-output.weft:336} */
{
                            /* Write md macro defs */
                            
#line 344 "literate/markdown-output.weft"
                            /* {164: literate/markdown-output.weft:344} */
{
                              if (name->defs->next) {
                                fputs("> *Fragment defined by* ", md_file);
                                md_print_scrap_numbers(md_file, name->defs);
                              }
                            }/* {:164} */

#line 337 "literate/markdown-output.weft"

                            /* Write md macro refs */
                            
#line 352 "literate/markdown-output.weft"
                            /* {165: literate/markdown-output.weft:352} */
{
                              if (name->uses) {
                                if (name->uses->next) {
                                  fputs("> *Fragment referenced in* ", md_file);
                                  md_print_scrap_numbers(md_file, name->uses);
                                }
                                else {
                                  fputs("> *Fragment referenced in* ", md_file);
                                  fputs("<a href=\"#weft", md_file);
                                  write_single_scrap_ref(md_file, name->uses->scrap);
                                  fputs("\">", md_file);
                                  write_single_scrap_ref(md_file, name->uses->scrap);
                                  fputs("</a>", md_file);
                                  fputs(".\n", md_file);
                                }
                              }
                              else {
                                fputs("> *Fragment never referenced*.\n", md_file);
                                fprintf(stderr, "%s: <%s> never referenced.\n",
                                        command_name, name->spelling);
                              }
                            }/* {:165} */

#line 338 "literate/markdown-output.weft"

                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }/* {:163} */

#line 235 "literate/markdown-output.weft"

                          /* Finish md scrap */
                          
#line 295 "literate/markdown-output.weft"
                          /* {159: literate/markdown-output.weft:295} */
{
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }/* {:159} */

#line 236 "literate/markdown-output.weft"

                        }/* {:155} */

#line 123 "literate/markdown-output.weft"

                        break;
              case 's':
                        /* Step to next sector */
                        
#line 170 "literate/parser.weft"
/* {47: literate/parser.weft:170} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:47} */

#line 126 "literate/markdown-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 177 "literate/parser.weft"
                        /* {48: literate/parser.weft:177} */
current_sector = 1;
                        c = source_get();
                        /* {:48} */

#line 129 "literate/markdown-output.weft"

                        break;
              case '{':
              case '[':
              case '(': 
#line 304 "literate/markdown-output.weft"
                        /* {160: literate/markdown-output.weft:304} */
md_copy_scrap(md_file, FALSE, NULL);
                        c = source_get();
                        /* {:160} */

#line 133 "literate/markdown-output.weft"

                        break;
              case '<': 
#line 170 "literate/markdown-output.weft"
                        /* {153: literate/markdown-output.weft:170} */
{
                           Parameters local_parameters = 0;
                           int changed;
                           char indent_chars[MAX_INDENT];
                           Arglist *a;
                           Name *name;
                           Arglist * args;
                           a = collect_scrap_name(0);
                           name = a->name;
                           args = instance(a->args, NULL, NULL, &changed);
                           name->mark = TRUE;
                           write_scraps(md_file, md_name, name->defs, 0, indent_chars, 0, 0, 1, 0, 0,
                                 args, name->arg, local_parameters, md_name);
                           name->mark = FALSE;
                           c = source_get();
                        }
                        /* {:153} */

#line 135 "literate/markdown-output.weft"

                        break;
              case 'x': 
#line 734 "literate/latex-output.weft"
                        /* {119: literate/latex-output.weft:734} */
{
                           /* Get label from */
                           
#line 591 "literate/search-labels.weft"
                           /* {386: literate/search-labels.weft:591} */
char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = 
#line 735 "literate/latex-output.weft"
                                      /* {120: literate/latex-output.weft:735} */
source_get()/* {:120} */
, c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = 
#line 735 "literate/latex-output.weft"
                               /* {120: literate/latex-output.weft:735} */
source_get()/* {:120} */
;
                           /* {:386} */

#line 735 "literate/latex-output.weft"

                           write_label(label_name, 
#line 137 "literate/markdown-output.weft"
                                                   /* {151: literate/markdown-output.weft:137} */
md_file/* {:151} */
);
                        }/* {:119} */

#line 137 "literate/markdown-output.weft"

                        c = source_get();
                        break;
              case 'c': /* Block comments: no special handling in Markdown */
                        c = source_get();
                        break;
              case 'f': 
#line 670 "literate/markdown-output.weft"
                        /* {178: literate/markdown-output.weft:670} */
{
                          if (file_names) {
                            fputs("\n", md_file);
                            md_format_file_entry(file_names, md_file);
                            fputs("\n", md_file);
                          }
                          c = source_get();
                        }/* {:178} */

#line 143 "literate/markdown-output.weft"

                        break;
              case 'm': 
#line 713 "literate/markdown-output.weft"
                        /* {182: literate/markdown-output.weft:713} */
{
                          unsigned char sector = current_sector;
                          int c = source_get();
                          if (c == '+')
                             sector = 0;
                          else
                             source_ungetc(&c);
                          if (has_sector(macro_names, sector)) {
                            fputs("\n", md_file);
                            md_format_entry(macro_names, md_file, sector);
                            fputs("\n", md_file);
                          } else {
                            fputs("None.\n", md_file);
                          }
                        }
                        c = source_get();
                        /* {:182} */

#line 145 "literate/markdown-output.weft"

                        break;
              case 'u': 
#line 819 "literate/markdown-output.weft"
                        /* {188: literate/markdown-output.weft:819} */
{
                            unsigned char sector = current_sector;
                            c = source_get();
                            if (c == '+') {
                               sector = 0;
                               c = source_get();
                            }
                            if (has_sector(user_names, sector)) {
                              fputs("\n", md_file);
                              md_format_user_entry(user_names, md_file, sector);
                              fputs("\n", md_file);
                            }
                        }/* {:188} */

#line 147 "literate/markdown-output.weft"

                        break;
              case 'v': 
#line 165 "literate/markdown-output.weft"
                        /* {152: literate/markdown-output.weft:165} */
fputs(version_string, md_file);
                        c = source_get();
                        /* {:152} */

#line 149 "literate/markdown-output.weft"

                        break;
              case 'l':
              case 'L': 
#line 102 "literate/parser.weft"
                        /* {45: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:45} */

#line 152 "literate/markdown-output.weft"

                        break;
              case 'W': 
#line 102 "literate/parser.weft"
                        /* {45: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:45} */

#line 154 "literate/markdown-output.weft"

                        break;
              default:
                    if (c==nw_char)
                      putc(c, md_file);
                    c = source_get();
                        break;
            }
          }/* {:150} */

#line 63 "literate/markdown-output.weft"

          }
        else if (c == '`' && at_line_start) {
          /* Check for cosmetic fence line */
          
#line 82 "literate/markdown-output.weft"
          /* {149: literate/markdown-output.weft:82} */
{
            int c2 = source_get();
            int c3 = (c2 == '`') ? source_get() : 0;
            if (c2 == '`' && c3 == '`') {
              /* Skip rest of fence line */
              int fc = source_get();
              while (fc != '\n' && fc != EOF)
                fc = source_get();
              c = source_get();
              at_line_start = TRUE;
            } else {
              /* Not a fence, output what we consumed */
              putc('`', md_file);
              if (c2 == '`') {
                putc('`', md_file);
                c = c3;
              } else {
                c = c2;
              }
              at_line_start = FALSE;
            }
          }/* {:149} */

#line 66 "literate/markdown-output.weft"

        }
        else {
          if (c == '\n') at_line_start = TRUE;
          else at_line_start = FALSE;
          putc(c, md_file);
          c = source_get();
        }
      }
    }/* {:148} */

#line 41 "literate/markdown-output.weft"

    fclose(md_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, md_name);
}
/* {:147} */

#line 255 "literate/markdown-output.weft"
/* {157: literate/markdown-output.weft:255} */
static void md_write_arg(FILE * md_file, char * p)
{
   fputs("<i>", md_file);
   while (*p)
   {
      switch (*p)
      {
      case '<':
         fputs("&lt;", md_file);
         break;
      case '>':
         fputs("&gt;", md_file);
         break;
      case '&':
         fputs("&amp;", md_file);
         break;
      default:
         fputc(*p, md_file);
         break;
      }
      p++;
   }
   fputs("</i>", md_file);
}
/* {:157} */

#line 376 "literate/markdown-output.weft"
/* {166: literate/markdown-output.weft:376} */
static void md_print_scrap_numbers(FILE *md_file, Scrap_Node *scraps)
{
  int page;
  fputs("<a href=\"#weft", md_file);
  write_scrap_ref(md_file, scraps->scrap, -1, &page);
  fputs("\">", md_file);
  write_scrap_ref(md_file, scraps->scrap, TRUE, &page);
  fputs("</a>", md_file);
  scraps = scraps->next;
  while (scraps) {
    fputs("<a href=\"#weft", md_file);
    write_scrap_ref(md_file, scraps->scrap, -1, &page);
    fputs("\">", md_file);
    write_scrap_ref(md_file, scraps->scrap, FALSE, &page);
    fputs("</a>", md_file);
    scraps = scraps->next;
  }
  fputs(".\n", md_file);
}
/* {:166} */

#line 405 "literate/markdown-output.weft"
/* {167: literate/markdown-output.weft:405} */
static int md_scrap_type = 0;

static void md_copy_scrap(FILE *file, int prefix, Name *name)
{
  int indent = 0;
  int c;
  char ** params = name ? name->arg : NULL;
  if (source_last == '{') md_scrap_type = 0;
  if (source_last == '[') md_scrap_type = 1;
  if (source_last == '(') md_scrap_type = 2;
  c = source_get();
  while (1) {
    switch (c) {
      case '\n': fputs("\n", file);
                 indent = 0;
                 break;
      case '\t': 
#line 451 "literate/markdown-output.weft"
                 /* {169: literate/markdown-output.weft:451} */
{
                   int delta = 8 - (indent % 8);
                   indent += delta;
                   while (delta > 0) {
                     putc(' ', file);
                     delta--;
                   }
                 }/* {:169} */

#line 421 "literate/markdown-output.weft"

                 break;
      default:
         if (c==nw_char)
           {
             /* Check md at-sequence for end-of-scrap */
             
#line 461 "literate/markdown-output.weft"
             /* {170: literate/markdown-output.weft:461} */
{
               c = source_get();
               switch (c) {
                 case 'c': 
#line 519 "literate/markdown-output.weft"
                           /* {172: literate/markdown-output.weft:519} */
{
                             fputs("<i>(Comment)</i>", file);
                           }
                           /* {:172} */

#line 464 "literate/markdown-output.weft"

                           break;
                 case 'x': 
#line 734 "literate/latex-output.weft"
                           /* {119: literate/latex-output.weft:734} */
{
                              /* Get label from */
                              
#line 591 "literate/search-labels.weft"
                              /* {386: literate/search-labels.weft:591} */
char  label_name[MAX_NAME_LEN];
                              char * p = label_name;
                              while (c = 
#line 735 "literate/latex-output.weft"
                                         /* {120: literate/latex-output.weft:735} */
source_get()/* {:120} */
, c != nw_char) /* Here is ?-01 */
                                 *p++ = c;
                              *p = '\0';
                              c = 
#line 735 "literate/latex-output.weft"
                                  /* {120: literate/latex-output.weft:735} */
source_get()/* {:120} */
;
                              /* {:386} */

#line 735 "literate/latex-output.weft"

                              write_label(label_name, 
#line 466 "literate/markdown-output.weft"
                                                      /* {171: literate/markdown-output.weft:466} */
file/* {:171} */
);
                           }/* {:119} */

#line 466 "literate/markdown-output.weft"

                           break;
                 case 'v': 
#line 730 "literate/latex-output.weft"
                           /* {118: literate/latex-output.weft:730} */
fputs(version_string, file);
                           /* {:118} */

#line 468 "literate/markdown-output.weft"

                 case 's':
                           break;
                 case '+':
                 case '-':
                 case '*':
                 case '|': 
#line 743 "literate/latex-output.weft"
                           /* {121: literate/latex-output.weft:743} */
{
                             do {
                               do
                                 c = source_get();
                               while (c != nw_char);
                               c = source_get();
                             } while (c != '}' && c != ']' && c != ')' );
                           }/* {:121} */

#line 474 "literate/markdown-output.weft"

                 case ',':
                 case ')':
                 case ']':
                 case '}': return;
                 case '<': 
#line 545 "literate/markdown-output.weft"
                           /* {175: literate/markdown-output.weft:545} */
{
                             Arglist *args = collect_scrap_name(-1);
                             Name *name = args->name;
                             char * p = name->spelling;
                             Arglist *q = args->args;
                             int narg = 0;

                             fputs("&lt;<a href=\"#weft", file);
                             if (name->defs) {
                               write_single_scrap_ref(file, name->defs->scrap);
                             } else {
                               putc('?', file);
                             }
                             fputs("\"><i>", file);
                             while (*p != '\000') {
                               if (*p == ARG_CHR) {
                                 if (q == NULL) {
                                    md_write_literal(file, name->arg[narg]);
                                 }
                                 else {
                                   md_write_ArglistElement(file, q, params);
                                   q = q->next;
                                 }
                                 p++;
                                 narg++;
                               }
                               else {
                                 if (*p == '<') fputs("&lt;", file);
                                 else if (*p == '>') fputs("&gt;", file);
                                 else if (*p == '&') fputs("&amp;", file);
                                 else fputc(*p, file);
                                 p++;
                               }
                             }
                             fputs("</i> ", file);
                             if (name->defs) {
                               Scrap_Node *dp = name->defs;
                               write_single_scrap_ref(file, dp->scrap);
                               dp = dp->next;
                               if (dp)
                                 fputs(", ...", file);
                             }
                             else {
                               putc('?', file);
                               fprintf(stderr, "%s: never defined <%s>\n",
                                       command_name, name->spelling);
                             }
                             fputs("</a>&gt;", file);
                           }/* {:175} */

#line 479 "literate/markdown-output.weft"

                           break;
                 case '%': 
#line 753 "literate/latex-output.weft"
                           /* {122: literate/latex-output.weft:753} */
{
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }/* {:122} */

#line 481 "literate/markdown-output.weft"

                           break;
                 case '_': 
#line 525 "literate/markdown-output.weft"
                           /* {173: literate/markdown-output.weft:525} */
{
                             fprintf(file, "<b>");
                             c = source_get();
                             do {
                                 if (c == '<') fputs("&lt;", file);
                                 else if (c == '>') fputs("&gt;", file);
                                 else if (c == '&') fputs("&amp;", file);
                                 else fputc(c, file);
                                 c = source_get();
                             } while (c != nw_char);
                             c = source_get();
                             fprintf(file, "</b>");
                           }/* {:173} */

#line 483 "literate/markdown-output.weft"

                           break;
                 case 't': 
#line 540 "literate/markdown-output.weft"
                           /* {174: literate/markdown-output.weft:540} */
{
                             fprintf(file, "<i>fragment title</i>");
                           }/* {:174} */

#line 485 "literate/markdown-output.weft"

                           break;
                 case 'f': 
#line 540 "literate/markdown-output.weft"
                           /* {174: literate/markdown-output.weft:540} */
{
                             fprintf(file, "<i>file name</i>");
                           }/* {:174} */

#line 487 "literate/markdown-output.weft"

                           break;
                 case '1': case '2': case '3':
                 case '4': case '5': case '6':
                 case '7': case '8': case '9':
                           if (name == NULL
                               || name->arg[c - '1'] == NULL) {
                             if (md_scrap_type == 0 && prefix)
                               fputs("&amp;", file);
                             else
                               putc(nw_char, file);
                             fputc(c,   file);
                           }
                           else {
                             md_write_arg(file, name->arg[c - '1']);
                           }
                           break;
                 default:
                       if (c==nw_char)
                         {
                           if (md_scrap_type == 0 && prefix)
                             fputs("&amp;", file);
                           else
                             putc(nw_char, file);
                           break;
                         }
                       /* ignore these since pass1 will have warned about them */
                           break;
               }
             }/* {:170} */

#line 426 "literate/markdown-output.weft"

             break;
           }
         if (md_scrap_type == 0 && prefix) {
           /* HTML-escape character c */
           
#line 443 "literate/markdown-output.weft"
           /* {168: literate/markdown-output.weft:443} */
switch (c) {
             case '<': fputs("&lt;", file); break;
             case '>': fputs("&gt;", file); break;
             case '&': fputs("&amp;", file); break;
             default:  putc(c, file); break;
           }/* {:168} */

#line 430 "literate/markdown-output.weft"

         } else {
           putc(c, file);
         }
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:167} */

#line 597 "literate/markdown-output.weft"
/* {176: literate/markdown-output.weft:597} */
static void md_write_literal(FILE * md_file, char * p)
{
   while (*p!= '\000') {
     if (*p == '<') fputs("&lt;", md_file);
     else if (*p == '>') fputs("&gt;", md_file);
     else if (*p == '&') fputs("&amp;", md_file);
     else fputc(*p, md_file);
     p++;
   }
}
/* {:176} */

#line 610 "literate/markdown-output.weft"
/* {177: literate/markdown-output.weft:610} */
static void
md_write_ArglistElement(FILE * file, Arglist * args, char ** params)
{
  Name *name = args->name;
  Arglist *q = args->args;

  if (name == NULL) {
    char * p = (char*)q;

    if (p[0] == ARG_CHR) {
       md_write_arg(file, params[p[1] - '1']);
    } else {
       md_write_literal(file, (char *)q);
    }
  } else if (name == (Name *)1) {
    Scrap_Node * qq = (Scrap_Node *)q;
    qq->quoted = TRUE;
    write_scraps(file, "", qq,
                 -1, "", 0, 0, 0, 0, 0,
                 NULL, params, 0, "");
    extra_scraps++;
    qq->quoted = FALSE;
  } else {
    char * p = name->spelling;
    fputs("&lt;<i>", file);
    while (*p != '\000') {
      if (*p == ARG_CHR) {
        md_write_ArglistElement(file, q, params);
        q = q->next;
        p++;
      }
      else {
        if (*p == '<') fputs("&lt;", file);
        else if (*p == '>') fputs("&gt;", file);
        else if (*p == '&') fputs("&amp;", file);
        else fputc(*p, file);
        p++;
      }
    }
    fputs("</i> ", file);
    if (name->defs) {
      Scrap_Node *dp = name->defs;
      write_single_scrap_ref(file, dp->scrap);
      dp = dp->next;
      if (dp)
        fputs(", ...", file);
    }
    else {
      putc('?', file);
      fprintf(stderr, "%s: never defined <%s>\n",
              command_name, name->spelling);
    }
    fputs("&gt;", file);
  }
}
/* {:177} */

#line 680 "literate/markdown-output.weft"
/* {179: literate/markdown-output.weft:680} */
static void md_format_file_entry(Name *name, FILE *md_file)
{
  while (name) {
    md_format_file_entry(name->llink, md_file);
    /* Format a md file index entry */
    
#line 691 "literate/markdown-output.weft"
    /* {180: literate/markdown-output.weft:691} */
fputs("- ", md_file);
    fprintf(md_file, "`\"%s\"` ", name->spelling);
    /* Write md file's defining scrap numbers */

#line 697 "literate/markdown-output.weft"
    /* {181: literate/markdown-output.weft:697} */
    {
      Scrap_Node *p = name->defs;
      fputs("*Defined by* ", md_file);
      if (p->next) {
        md_print_scrap_numbers(md_file, p);
      }
      else {
        fputs("<a href=\"#weft", md_file);
        write_single_scrap_ref(md_file, p->scrap);
        fputs("\">", md_file);
        write_single_scrap_ref(md_file, p->scrap);
        fputs("</a>.", md_file);
      }
    }/* {:181} */

#line 693 "literate/markdown-output.weft"

    putc('\n', md_file);/* {:180} */

#line 684 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:179} */

#line 732 "literate/markdown-output.weft"
/* {183: literate/markdown-output.weft:732} */
static int md_load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = md_load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:183} */

#line 744 "literate/markdown-output.weft"
/* {184: literate/markdown-output.weft:744} */
static void md_format_entry(Name *name, FILE *md_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = md_load_entry(name, nms, 0);
  int i;

  /* Sort 'nms' of size 'n' for <Rob's ordering> */
  
#line 1003 "literate/latex-output.weft"
  /* {136: literate/latex-output.weft:1003} */
int j;
  for (j = 1; j < n; j++)
  {
     int i = j - 1;
     Name * kj = nms[j];

     do
     {
        Name * ki = nms[i];

        if (
#line 1000 "literate/latex-output.weft"
            /* {135: literate/latex-output.weft:1000} */
robs_strcmp(ki->spelling, kj->spelling) < 0/* {:135} */
)
           break;
        nms[i + 1] = ki;
        i -= 1;
     } while (i >= 0);
     nms[i + 1] = kj;
  }
  /* {:136} */

#line 750 "literate/markdown-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format a md index entry */
     
#line 762 "literate/markdown-output.weft"
     /* {185: literate/markdown-output.weft:762} */
if (name->sector == sector){
       fputs("- ", md_file);
       fputs("&lt;*", md_file);
       /* Write the md macro's name */
       
#line 240 "literate/markdown-output.weft"
       /* {156: literate/markdown-output.weft:240} */
{
         char * p = name->spelling;
         int i = 0;

         while (*p != '\000') {
           if (*p == ARG_CHR) {
             md_write_arg(md_file, name->arg[i++]);
             p++;
           }
           else
              fputc(*p++, md_file);
         }
       }/* {:156} */

#line 765 "literate/markdown-output.weft"

       fputs("* ", md_file);
       /* Write md defining scrap numbers */
       
#line 774 "literate/markdown-output.weft"
       /* {186: literate/markdown-output.weft:774} */
{
         Scrap_Node *p = name->defs;
         if (p) {
           int page;
           fputs("<a href=\"#weft", md_file);
           write_scrap_ref(md_file, p->scrap, -1, &page);
           fputs("\">", md_file);
           write_scrap_ref(md_file, p->scrap, TRUE, &page);
           fputs("</a>", md_file);
           p = p->next;
           while (p) {
             fputs("<a href=\"#weft", md_file);
             write_scrap_ref(md_file, p->scrap, -1, &page);
             fputs("\">", md_file);
             write_scrap_ref(md_file, p->scrap, FALSE, &page);
             fputs("</a>", md_file);
             p = p->next;
           }
         }
         else
           putc('?', md_file);
       }/* {:186} */

#line 767 "literate/markdown-output.weft"

       fputs("&gt; ", md_file);
       /* Write md referencing scrap numbers */
       
#line 798 "literate/markdown-output.weft"
       /* {187: literate/markdown-output.weft:798} */
{
         Scrap_Node *p = name->uses;
         if (p) {
           fputs("*Referenced in* ", md_file);
           if (p->next) {
             md_print_scrap_numbers(md_file, p);
           }
           else {
             fputs("<a href=\"#weft", md_file);
             write_single_scrap_ref(md_file, p->scrap);
             fputs("\">", md_file);
             write_single_scrap_ref(md_file, p->scrap);
             fputs("</a>.", md_file);
           }
         }
         else
           fputs("*Not referenced*.", md_file);
       }/* {:187} */

#line 769 "literate/markdown-output.weft"

       putc('\n', md_file);
     }/* {:185} */

#line 755 "literate/markdown-output.weft"

  }
  free(nms);
}
/* {:184} */

#line 835 "literate/markdown-output.weft"
/* {189: literate/markdown-output.weft:835} */
static void md_format_user_entry(Name *name, FILE *md_file, unsigned char sector)
{
  while (name) {
    md_format_user_entry(name->llink, md_file, sector);
    /* Format a md user index entry */
    
#line 847 "literate/markdown-output.weft"
    /* {190: literate/markdown-output.weft:847} */
if (name->sector == sector){
      Scrap_Node *uses = name->uses;
      if ( uses || dangling_flag ) {
        int page;
        Scrap_Node *defs = name->defs;
        fprintf(md_file, "- `%s`: ", name->spelling);
        if (!uses) {
            fputs("(**", md_file);
            fputs("<a href=\"#weft", md_file);
            write_single_scrap_ref(md_file, defs->scrap);
            fputs("\">", md_file);
            write_single_scrap_ref(md_file, defs->scrap);
            fputs("</a>**)", md_file);
            page = -2;
            defs = defs->next;
        }
        else
          if (!defs || uses->scrap < defs->scrap) {
          fputs("<a href=\"#weft", md_file);
          write_scrap_ref(md_file, uses->scrap, -1, &page);
          fputs("\">", md_file);
          write_scrap_ref(md_file, uses->scrap, TRUE, &page);
          fputs("</a>", md_file);
          uses = uses->next;
        }
        else {
          if (defs->scrap == uses->scrap)
            uses = uses->next;
          fputs("**", md_file);

          fputs("<a href=\"#weft", md_file);
          write_single_scrap_ref(md_file, defs->scrap);
          fputs("\">", md_file);
          write_single_scrap_ref(md_file, defs->scrap);
          fputs("</a>**", md_file);
          page = -2;
          defs = defs->next;
        }
        while (uses || defs) {
          if (uses && (!defs || uses->scrap < defs->scrap)) {
            fputs("<a href=\"#weft", md_file);
            write_scrap_ref(md_file, uses->scrap, -1, &page);
            fputs("\">", md_file);
            write_scrap_ref(md_file, uses->scrap, FALSE, &page);
            fputs("</a>", md_file);
            uses = uses->next;
          }
          else {
            if (uses && defs->scrap == uses->scrap)
              uses = uses->next;
            fputs(", **", md_file);

            fputs("<a href=\"#weft", md_file);
            write_single_scrap_ref(md_file, defs->scrap);
            fputs("\">", md_file);
            write_single_scrap_ref(md_file, defs->scrap);
            fputs("</a>**", md_file);
            page = -2;
            defs = defs->next;
          }
        }
        fputs(".\n", md_file);
      }
    }/* {:190} */

#line 839 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:189} */

#line 919 "literate/markdown-output.weft"
/* {191: literate/markdown-output.weft:919} */

static void
md_format_uses_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_uses(scrap);
  if (p != NULL)
    /* Write md uses references */
    
#line 930 "literate/markdown-output.weft"
    /* {192: literate/markdown-output.weft:930} */
{
      char join = ' ';
      fputs("> *Uses:*", md_file);
      do {
        /* Write one md use reference */
        
#line 942 "literate/markdown-output.weft"
        /* {193: literate/markdown-output.weft:942} */
Name * name = p->defn;
        Scrap_Node *defs = name->defs;
        int first = TRUE, page = -1;
        fprintf(md_file,
                "%c `%s` ",
                join, name->spelling);
        if (defs)
        {
          do {
            /* Write one md referenced scrap */
            
#line 963 "literate/markdown-output.weft"
            /* {194: literate/markdown-output.weft:963} */
fputs("<a href=\"#weft", md_file);
            write_scrap_ref(md_file, defs->scrap, -1, &page);
            fputs("\">", md_file);
            write_scrap_ref(md_file, defs->scrap, first, &page);
            fputs("</a>", md_file);/* {:194} */

#line 951 "literate/markdown-output.weft"

            first = FALSE;
            defs = defs->next;
          }while (defs!= NULL);
        }
        else
        {
          fputs("*(not defined globally)*", md_file);
        }
        /* {:193} */

#line 934 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:192} */

#line 925 "literate/markdown-output.weft"

}
/* {:191} */

#line 971 "literate/markdown-output.weft"
/* {195: literate/markdown-output.weft:971} */

static void
md_format_defs_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_defs(scrap);
  if (p != NULL)
    /* Write md defs references */
    
#line 982 "literate/markdown-output.weft"
    /* {196: literate/markdown-output.weft:982} */
{
      char join = ' ';
      fputs("> *Defines:*", md_file);
      do {
        /* Write one md def reference */
        
#line 994 "literate/markdown-output.weft"
        /* {197: literate/markdown-output.weft:994} */
Name * name = p->defn;
        Scrap_Node *defs = name->uses;
        int first = TRUE, page = -1;
        fprintf(md_file,
                "%c `%s` ",
                join, name->spelling);
        if (defs == NULL
            || (defs->scrap == scrap && defs->next == NULL)) {
          fputs("*Never used*", md_file);
        }
        else {
          do {
            if (defs->scrap != scrap) {
               /* Write one md referenced scrap */
               
#line 963 "literate/markdown-output.weft"
               /* {194: literate/markdown-output.weft:963} */
fputs("<a href=\"#weft", md_file);
               write_scrap_ref(md_file, defs->scrap, -1, &page);
               fputs("\">", md_file);
               write_scrap_ref(md_file, defs->scrap, first, &page);
               fputs("</a>", md_file);/* {:194} */

#line 1007 "literate/markdown-output.weft"

               first = FALSE;
            }
            defs = defs->next;
          }while (defs!= NULL);
        }
        /* {:197} */

#line 986 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:196} */

#line 977 "literate/markdown-output.weft"

}
/* {:195} */
