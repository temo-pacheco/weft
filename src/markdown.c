
#line 97 "literate/architecture.weft"
/* {8: literate/architecture.weft:97} */
#include "global.h"
static int scraps = 1;
/* {:8} */

#line 17 "literate/markdown-output.weft"
/* {144: literate/markdown-output.weft:17} */
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
/* {:144} */

#line 33 "literate/markdown-output.weft"
/* {145: literate/markdown-output.weft:33} */
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
    /* {146: literate/markdown-output.weft:57} */
{
      int c = source_get();
      int at_line_start = TRUE;
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret md at-sequence */
          
#line 106 "literate/markdown-output.weft"
          /* {148: literate/markdown-output.weft:106} */
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
              case 'o': 
#line 204 "literate/markdown-output.weft"
                        /* {152: literate/markdown-output.weft:204} */
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
                          
#line 281 "literate/markdown-output.weft"
                          /* {156: literate/markdown-output.weft:281} */
{
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                          }/* {:156} */

#line 214 "literate/markdown-output.weft"

                          /* Write md file cross-refs */
                          
#line 311 "literate/markdown-output.weft"
                          /* {159: literate/markdown-output.weft:311} */
{
                            if ( scrap_flag ) {
                              /* Write md file defs */
                              
#line 320 "literate/markdown-output.weft"
                              /* {160: literate/markdown-output.weft:320} */
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
                              }/* {:160} */

#line 313 "literate/markdown-output.weft"

                            }
                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }/* {:159} */

#line 215 "literate/markdown-output.weft"

                          /* Finish md scrap */
                          
#line 293 "literate/markdown-output.weft"
                          /* {157: literate/markdown-output.weft:293} */
{
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }/* {:157} */

#line 216 "literate/markdown-output.weft"

                        }/* {:152} */

#line 116 "literate/markdown-output.weft"

                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
              case 'q':
              case 'd': 
#line 221 "literate/markdown-output.weft"
                        /* {153: literate/markdown-output.weft:221} */
{
                          Name *name = collect_macro_name();
                          (void)big_definition;
                          fputs("\n<a id=\"weft", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("\"></a>\n", md_file);
                          fputs("**&lt;*", md_file);
                          /* Write the md macro's name */
                          
#line 238 "literate/markdown-output.weft"
                          /* {154: literate/markdown-output.weft:238} */
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
                          }/* {:154} */

#line 228 "literate/markdown-output.weft"

                          fputs("* ", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("&gt;** =\n", md_file);
                          /* Fill in the md scrap */
                          
#line 281 "literate/markdown-output.weft"
                          /* {156: literate/markdown-output.weft:281} */
{
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                          }/* {:156} */

#line 232 "literate/markdown-output.weft"

                          /* Write md macro cross-refs */
                          
#line 334 "literate/markdown-output.weft"
                          /* {161: literate/markdown-output.weft:334} */
{
                            /* Write md macro defs */
                            
#line 342 "literate/markdown-output.weft"
                            /* {162: literate/markdown-output.weft:342} */
{
                              if (name->defs->next) {
                                fputs("> *Fragment defined by* ", md_file);
                                md_print_scrap_numbers(md_file, name->defs);
                              }
                            }/* {:162} */

#line 335 "literate/markdown-output.weft"

                            /* Write md macro refs */
                            
#line 350 "literate/markdown-output.weft"
                            /* {163: literate/markdown-output.weft:350} */
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
                            }/* {:163} */

#line 336 "literate/markdown-output.weft"

                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }/* {:161} */

#line 233 "literate/markdown-output.weft"

                          /* Finish md scrap */
                          
#line 293 "literate/markdown-output.weft"
                          /* {157: literate/markdown-output.weft:293} */
{
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }/* {:157} */

#line 234 "literate/markdown-output.weft"

                        }/* {:153} */

#line 121 "literate/markdown-output.weft"

                        break;
              case 's':
                        /* Step to next sector */
                        
#line 169 "literate/parser.weft"
/* {45: literate/parser.weft:169} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:45} */

#line 124 "literate/markdown-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 176 "literate/parser.weft"
                        /* {46: literate/parser.weft:176} */
current_sector = 1;
                        c = source_get();
                        /* {:46} */

#line 127 "literate/markdown-output.weft"

                        break;
              case '{':
              case '[':
              case '(': 
#line 302 "literate/markdown-output.weft"
                        /* {158: literate/markdown-output.weft:302} */
md_copy_scrap(md_file, FALSE, NULL);
                        c = source_get();
                        /* {:158} */

#line 131 "literate/markdown-output.weft"

                        break;
              case '<': 
#line 168 "literate/markdown-output.weft"
                        /* {151: literate/markdown-output.weft:168} */
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
                        /* {:151} */

#line 133 "literate/markdown-output.weft"

                        break;
              case 'x': 
#line 731 "literate/latex-output.weft"
                        /* {117: literate/latex-output.weft:731} */
{
                           /* Get label from */
                           
#line 586 "literate/search-labels.weft"
                           /* {380: literate/search-labels.weft:586} */
char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = 
#line 732 "literate/latex-output.weft"
                                      /* {118: literate/latex-output.weft:732} */
source_get()/* {:118} */
, c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = 
#line 732 "literate/latex-output.weft"
                               /* {118: literate/latex-output.weft:732} */
source_get()/* {:118} */
;
                           /* {:380} */

#line 732 "literate/latex-output.weft"

                           write_label(label_name, 
#line 135 "literate/markdown-output.weft"
                                                   /* {149: literate/markdown-output.weft:135} */
md_file/* {:149} */
);
                        }/* {:117} */

#line 135 "literate/markdown-output.weft"

                        c = source_get();
                        break;
              case 'c': /* Block comments: no special handling in Markdown */
                        c = source_get();
                        break;
              case 'f': 
#line 668 "literate/markdown-output.weft"
                        /* {176: literate/markdown-output.weft:668} */
{
                          if (file_names) {
                            fputs("\n", md_file);
                            md_format_file_entry(file_names, md_file);
                            fputs("\n", md_file);
                          }
                          c = source_get();
                        }/* {:176} */

#line 141 "literate/markdown-output.weft"

                        break;
              case 'm': 
#line 711 "literate/markdown-output.weft"
                        /* {180: literate/markdown-output.weft:711} */
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
                        /* {:180} */

#line 143 "literate/markdown-output.weft"

                        break;
              case 'u': 
#line 817 "literate/markdown-output.weft"
                        /* {186: literate/markdown-output.weft:817} */
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
                        }/* {:186} */

#line 145 "literate/markdown-output.weft"

                        break;
              case 'v': 
#line 163 "literate/markdown-output.weft"
                        /* {150: literate/markdown-output.weft:163} */
fputs(version_string, md_file);
                        c = source_get();
                        /* {:150} */

#line 147 "literate/markdown-output.weft"

                        break;
              case 'l':
              case 'L': 
#line 102 "literate/parser.weft"
                        /* {43: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:43} */

#line 150 "literate/markdown-output.weft"

                        break;
              case 'W': 
#line 102 "literate/parser.weft"
                        /* {43: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:43} */

#line 152 "literate/markdown-output.weft"

                        break;
              default:
                    if (c==nw_char)
                      putc(c, md_file);
                    c = source_get();
                        break;
            }
          }/* {:148} */

#line 63 "literate/markdown-output.weft"

          }
        else if (c == '`' && at_line_start) {
          /* Check for cosmetic fence line */
          
#line 82 "literate/markdown-output.weft"
          /* {147: literate/markdown-output.weft:82} */
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
          }/* {:147} */

#line 66 "literate/markdown-output.weft"

        }
        else {
          if (c == '\n') at_line_start = TRUE;
          else at_line_start = FALSE;
          putc(c, md_file);
          c = source_get();
        }
      }
    }/* {:146} */

#line 41 "literate/markdown-output.weft"

    fclose(md_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, md_name);
}
/* {:145} */

#line 253 "literate/markdown-output.weft"
/* {155: literate/markdown-output.weft:253} */
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
/* {:155} */

#line 374 "literate/markdown-output.weft"
/* {164: literate/markdown-output.weft:374} */
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
/* {:164} */

#line 403 "literate/markdown-output.weft"
/* {165: literate/markdown-output.weft:403} */
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
#line 449 "literate/markdown-output.weft"
                 /* {167: literate/markdown-output.weft:449} */
{
                   int delta = 8 - (indent % 8);
                   indent += delta;
                   while (delta > 0) {
                     putc(' ', file);
                     delta--;
                   }
                 }/* {:167} */

#line 419 "literate/markdown-output.weft"

                 break;
      default:
         if (c==nw_char)
           {
             /* Check md at-sequence for end-of-scrap */
             
#line 459 "literate/markdown-output.weft"
             /* {168: literate/markdown-output.weft:459} */
{
               c = source_get();
               switch (c) {
                 case 'c': 
#line 517 "literate/markdown-output.weft"
                           /* {170: literate/markdown-output.weft:517} */
{
                             fputs("<i>(Comment)</i>", file);
                           }
                           /* {:170} */

#line 462 "literate/markdown-output.weft"

                           break;
                 case 'x': 
#line 731 "literate/latex-output.weft"
                           /* {117: literate/latex-output.weft:731} */
{
                              /* Get label from */
                              
#line 586 "literate/search-labels.weft"
                              /* {380: literate/search-labels.weft:586} */
char  label_name[MAX_NAME_LEN];
                              char * p = label_name;
                              while (c = 
#line 732 "literate/latex-output.weft"
                                         /* {118: literate/latex-output.weft:732} */
source_get()/* {:118} */
, c != nw_char) /* Here is ?-01 */
                                 *p++ = c;
                              *p = '\0';
                              c = 
#line 732 "literate/latex-output.weft"
                                  /* {118: literate/latex-output.weft:732} */
source_get()/* {:118} */
;
                              /* {:380} */

#line 732 "literate/latex-output.weft"

                              write_label(label_name, 
#line 464 "literate/markdown-output.weft"
                                                      /* {169: literate/markdown-output.weft:464} */
file/* {:169} */
);
                           }/* {:117} */

#line 464 "literate/markdown-output.weft"

                           break;
                 case 'v': 
#line 727 "literate/latex-output.weft"
                           /* {116: literate/latex-output.weft:727} */
fputs(version_string, file);
                           /* {:116} */

#line 466 "literate/markdown-output.weft"

                 case 's':
                           break;
                 case '+':
                 case '-':
                 case '*':
                 case '|': 
#line 740 "literate/latex-output.weft"
                           /* {119: literate/latex-output.weft:740} */
{
                             do {
                               do
                                 c = source_get();
                               while (c != nw_char);
                               c = source_get();
                             } while (c != '}' && c != ']' && c != ')' );
                           }/* {:119} */

#line 472 "literate/markdown-output.weft"

                 case ',':
                 case ')':
                 case ']':
                 case '}': return;
                 case '<': 
#line 543 "literate/markdown-output.weft"
                           /* {173: literate/markdown-output.weft:543} */
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
                           }/* {:173} */

#line 477 "literate/markdown-output.weft"

                           break;
                 case '%': 
#line 750 "literate/latex-output.weft"
                           /* {120: literate/latex-output.weft:750} */
{
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }/* {:120} */

#line 479 "literate/markdown-output.weft"

                           break;
                 case '_': 
#line 523 "literate/markdown-output.weft"
                           /* {171: literate/markdown-output.weft:523} */
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
                           }/* {:171} */

#line 481 "literate/markdown-output.weft"

                           break;
                 case 't': 
#line 538 "literate/markdown-output.weft"
                           /* {172: literate/markdown-output.weft:538} */
{
                             fprintf(file, "<i>fragment title</i>");
                           }/* {:172} */

#line 483 "literate/markdown-output.weft"

                           break;
                 case 'f': 
#line 538 "literate/markdown-output.weft"
                           /* {172: literate/markdown-output.weft:538} */
{
                             fprintf(file, "<i>file name</i>");
                           }/* {:172} */

#line 485 "literate/markdown-output.weft"

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
             }/* {:168} */

#line 424 "literate/markdown-output.weft"

             break;
           }
         if (md_scrap_type == 0 && prefix) {
           /* HTML-escape character c */
           
#line 441 "literate/markdown-output.weft"
           /* {166: literate/markdown-output.weft:441} */
switch (c) {
             case '<': fputs("&lt;", file); break;
             case '>': fputs("&gt;", file); break;
             case '&': fputs("&amp;", file); break;
             default:  putc(c, file); break;
           }/* {:166} */

#line 428 "literate/markdown-output.weft"

         } else {
           putc(c, file);
         }
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:165} */

#line 595 "literate/markdown-output.weft"
/* {174: literate/markdown-output.weft:595} */
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
/* {:174} */

#line 608 "literate/markdown-output.weft"
/* {175: literate/markdown-output.weft:608} */
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
/* {:175} */

#line 678 "literate/markdown-output.weft"
/* {177: literate/markdown-output.weft:678} */
static void md_format_file_entry(Name *name, FILE *md_file)
{
  while (name) {
    md_format_file_entry(name->llink, md_file);
    /* Format a md file index entry */
    
#line 689 "literate/markdown-output.weft"
    /* {178: literate/markdown-output.weft:689} */
fputs("- ", md_file);
    fprintf(md_file, "`\"%s\"` ", name->spelling);
    /* Write md file's defining scrap numbers */

#line 695 "literate/markdown-output.weft"
    /* {179: literate/markdown-output.weft:695} */
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
    }/* {:179} */

#line 691 "literate/markdown-output.weft"

    putc('\n', md_file);/* {:178} */

#line 682 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:177} */

#line 730 "literate/markdown-output.weft"
/* {181: literate/markdown-output.weft:730} */
static int md_load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = md_load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:181} */

#line 742 "literate/markdown-output.weft"
/* {182: literate/markdown-output.weft:742} */
static void md_format_entry(Name *name, FILE *md_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = md_load_entry(name, nms, 0);
  int i;

  /* Sort 'nms' of size 'n' for <Rob's ordering> */
  
#line 1000 "literate/latex-output.weft"
  /* {134: literate/latex-output.weft:1000} */
int j;
  for (j = 1; j < n; j++)
  {
     int i = j - 1;
     Name * kj = nms[j];

     do
     {
        Name * ki = nms[i];

        if (
#line 997 "literate/latex-output.weft"
            /* {133: literate/latex-output.weft:997} */
robs_strcmp(ki->spelling, kj->spelling) < 0/* {:133} */
)
           break;
        nms[i + 1] = ki;
        i -= 1;
     } while (i >= 0);
     nms[i + 1] = kj;
  }
  /* {:134} */

#line 748 "literate/markdown-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format a md index entry */
     
#line 760 "literate/markdown-output.weft"
     /* {183: literate/markdown-output.weft:760} */
if (name->sector == sector){
       fputs("- ", md_file);
       fputs("&lt;*", md_file);
       /* Write the md macro's name */
       
#line 238 "literate/markdown-output.weft"
       /* {154: literate/markdown-output.weft:238} */
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
       }/* {:154} */

#line 763 "literate/markdown-output.weft"

       fputs("* ", md_file);
       /* Write md defining scrap numbers */
       
#line 772 "literate/markdown-output.weft"
       /* {184: literate/markdown-output.weft:772} */
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
       }/* {:184} */

#line 765 "literate/markdown-output.weft"

       fputs("&gt; ", md_file);
       /* Write md referencing scrap numbers */
       
#line 796 "literate/markdown-output.weft"
       /* {185: literate/markdown-output.weft:796} */
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
       }/* {:185} */

#line 767 "literate/markdown-output.weft"

       putc('\n', md_file);
     }/* {:183} */

#line 753 "literate/markdown-output.weft"

  }
  free(nms);
}
/* {:182} */

#line 833 "literate/markdown-output.weft"
/* {187: literate/markdown-output.weft:833} */
static void md_format_user_entry(Name *name, FILE *md_file, unsigned char sector)
{
  while (name) {
    md_format_user_entry(name->llink, md_file, sector);
    /* Format a md user index entry */
    
#line 845 "literate/markdown-output.weft"
    /* {188: literate/markdown-output.weft:845} */
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
    }/* {:188} */

#line 837 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:187} */

#line 917 "literate/markdown-output.weft"
/* {189: literate/markdown-output.weft:917} */

static void
md_format_uses_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_uses(scrap);
  if (p != NULL)
    /* Write md uses references */
    
#line 928 "literate/markdown-output.weft"
    /* {190: literate/markdown-output.weft:928} */
{
      char join = ' ';
      fputs("> *Uses:*", md_file);
      do {
        /* Write one md use reference */
        
#line 940 "literate/markdown-output.weft"
        /* {191: literate/markdown-output.weft:940} */
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
            
#line 961 "literate/markdown-output.weft"
            /* {192: literate/markdown-output.weft:961} */
fputs("<a href=\"#weft", md_file);
            write_scrap_ref(md_file, defs->scrap, -1, &page);
            fputs("\">", md_file);
            write_scrap_ref(md_file, defs->scrap, first, &page);
            fputs("</a>", md_file);/* {:192} */

#line 949 "literate/markdown-output.weft"

            first = FALSE;
            defs = defs->next;
          }while (defs!= NULL);
        }
        else
        {
          fputs("*(not defined globally)*", md_file);
        }
        /* {:191} */

#line 932 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:190} */

#line 923 "literate/markdown-output.weft"

}
/* {:189} */

#line 969 "literate/markdown-output.weft"
/* {193: literate/markdown-output.weft:969} */

static void
md_format_defs_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_defs(scrap);
  if (p != NULL)
    /* Write md defs references */
    
#line 980 "literate/markdown-output.weft"
    /* {194: literate/markdown-output.weft:980} */
{
      char join = ' ';
      fputs("> *Defines:*", md_file);
      do {
        /* Write one md def reference */
        
#line 992 "literate/markdown-output.weft"
        /* {195: literate/markdown-output.weft:992} */
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
               
#line 961 "literate/markdown-output.weft"
               /* {192: literate/markdown-output.weft:961} */
fputs("<a href=\"#weft", md_file);
               write_scrap_ref(md_file, defs->scrap, -1, &page);
               fputs("\">", md_file);
               write_scrap_ref(md_file, defs->scrap, first, &page);
               fputs("</a>", md_file);/* {:192} */

#line 1005 "literate/markdown-output.weft"

               first = FALSE;
            }
            defs = defs->next;
          }while (defs!= NULL);
        }
        /* {:195} */

#line 984 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:194} */

#line 975 "literate/markdown-output.weft"

}
/* {:193} */
