
#line 128 "literate/architecture.weft"
/* {9: literate/architecture.weft:128} */
#include "global.h"
static int scraps = 1;
/* {:9} */

#line 30 "literate/markdown-output.weft"
/* {162: literate/markdown-output.weft:30} */
static void md_format_entry(Name *name, FILE *md_file, unsigned char sector);
static void md_format_file_entry(Name *name, FILE *md_file);
static void md_format_user_entry(Name *name, FILE *md_file, unsigned char sector);
static void md_write_arg(FILE *md_file, char *p);
static void md_write_literal(FILE *md_file, char *p);
static void md_write_ArglistElement(FILE *file, Arglist *args, char **params);
static void md_format_uses_refs(FILE *md_file, int scrap);
static void md_format_defs_refs(FILE *md_file, int scrap);
/* {:162} */

#line 44 "literate/markdown-output.weft"
/* {163: literate/markdown-output.weft:44} */
void write_md(char *file_name, char *md_name, unsigned char sector)
{
  (void)sector;
  FILE *md_file = fopen(md_name, "w");
  if (md_file) {
    if (verbose_flag)
      fprintf(stderr, "writing %s\n", md_name);
    source_open(file_name);
    /* Copy \verb|source_file| into \verb|md_file| */
    
#line 68 "literate/markdown-output.weft"
    /* {164: literate/markdown-output.weft:68} */
{
      int c = source_get();
      int at_line_start = TRUE;
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret md at-sequence */
          
#line 117 "literate/markdown-output.weft"
          /* {166: literate/markdown-output.weft:117} */
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
              case 'o': {
                          Name *name = collect_file_name();
                          (void)big_definition;
                          fputs("\n<a id=\"weft", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("\"></a>\n", md_file);
                          fprintf(md_file, "**\"%s\"**", name->spelling);
                          fputs(" **", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("** =\n", md_file);
                          {
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                            /* end-of-scrap tombstone: a left-aligned square closing the code, the
                               Markdown twin of the LaTeX \WEFTsep and the HTML .weft-end.  We use the
                               GitHub-safe <div align> since a Markdown file carries no stylesheet */
                            fputs("<div align=\"left\">&#9633;</div>\n", md_file);
                          }
                          {
                            if ( scrap_flag ) {
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
                              }
                            }
                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }
                          {
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }
                        }
                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
                        FALLTHROUGH;
              case 'q':
              case 'd': {
                          Name *name = collect_macro_name();
                          (void)big_definition;
                          fputs("\n<a id=\"weft", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("\"></a>\n", md_file);
                          fputs("**&lt;*", md_file);
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
                          }
                          fputs("* ", md_file);
                          write_single_scrap_ref(md_file, scraps);
                          fputs("&gt;** =\n", md_file);
                          {
                            if (name->lang[0] != '\0')
                              fprintf(md_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", md_file);
                            extra_scraps = 0;
                            md_copy_scrap(md_file, TRUE, name);
                            fputs("</code></pre>\n", md_file);
                            /* end-of-scrap tombstone: a left-aligned square closing the code, the
                               Markdown twin of the LaTeX \WEFTsep and the HTML .weft-end.  We use the
                               GitHub-safe <div align> since a Markdown file carries no stylesheet */
                            fputs("<div align=\"left\">&#9633;</div>\n", md_file);
                          }
                          {
                            {
                              if (name->defs->next) {
                                fputs("> *Fragment defined by* ", md_file);
                                md_print_scrap_numbers(md_file, name->defs);
                              }
                            }
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
                            }
                            md_format_defs_refs(md_file, scraps);
                            md_format_uses_refs(md_file, scraps++);
                          }
                          {
                            scraps += extra_scraps;
                            do
                              c = source_get();
                            while (isspace(c));
                            at_line_start = TRUE;
                          }
                        }
                        break;
              case 's':
                        /* Step to next sector */
                        
#line 202 "literate/parser.weft"
/* {54: literate/parser.weft:202} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:54} */

#line 137 "literate/markdown-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 209 "literate/parser.weft"
                        /* {55: literate/parser.weft:209} */
current_sector = 1;
                        c = source_get();
                        /* {:55} */

#line 140 "literate/markdown-output.weft"

                        break;
              case '{':
              case '[':
              case '(': md_copy_scrap(md_file, FALSE, NULL);
                        c = source_get();
                        
                        break;
              case '<': {
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
                        
                        break;
              case 'x': {
                           char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = source_get(), c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = source_get();
                           
                           write_label(label_name, md_file);
                        }
                        c = source_get();
                        break;
              case 'c': /* Block comments: no special handling in Markdown */
                        c = source_get();
                        break;
              case 'f': {
                          if (file_names) {
                            fputs("\n", md_file);
                            md_format_file_entry(file_names, md_file);
                            fputs("\n", md_file);
                          }
                          c = source_get();
                        }
                        break;
              case 'm': {
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
                        
                        break;
              case 'u': {
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
                        }
                        break;
              case 'v': fputs(version_string, md_file);
                        c = source_get();
                        
                        break;
              case 'l':
              case 'L': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();
                        break;
              case 'W': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();
                        break;
              default:
                    if (c==nw_char)
                      putc(c, md_file);
                    c = source_get();
                        break;
            }
          }/* {:166} */

#line 74 "literate/markdown-output.weft"

          }
        else if (c == '`' && at_line_start) {
          /* Check for cosmetic fence line */
          
#line 93 "literate/markdown-output.weft"
          /* {165: literate/markdown-output.weft:93} */
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
          }/* {:165} */

#line 77 "literate/markdown-output.weft"

        }
        else {
          if (c == '\n') at_line_start = TRUE;
          else at_line_start = FALSE;
          putc(c, md_file);
          c = source_get();
        }
      }
    }/* {:164} */

#line 52 "literate/markdown-output.weft"

    fclose(md_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, md_name);
}
/* {:163} */

#line 272 "literate/markdown-output.weft"
/* {173: literate/markdown-output.weft:272} */
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
/* {:173} */

#line 397 "literate/markdown-output.weft"
/* {182: literate/markdown-output.weft:397} */
void md_print_scrap_numbers(FILE *md_file, Scrap_Node *scraps)
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
/* {:182} */

#line 426 "literate/markdown-output.weft"
/* {183: literate/markdown-output.weft:426} */
static int md_scrap_type = 0;

void md_copy_scrap(FILE *file, int prefix, Name *name)
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
      case '\t': {
                   int delta = 8 - (indent % 8);
                   indent += delta;
                   while (delta > 0) {
                     putc(' ', file);
                     delta--;
                   }
                 }
                 break;
      default:
         if (c==nw_char)
           {
             /* Check md at-sequence for end-of-scrap */
             
#line 482 "literate/markdown-output.weft"
             /* {186: literate/markdown-output.weft:482} */
{
               c = source_get();
               switch (c) {
                 case 'c': {
                             fputs("<i>(Comment)</i>", file);
                           }
                           
                           break;
                 case 'x': {
                              char  label_name[MAX_NAME_LEN];
                              char * p = label_name;
                              while (c = source_get(), c != nw_char) /* Here is ?-01 */
                                 *p++ = c;
                              *p = '\0';
                              c = source_get();
                              
                              write_label(label_name, file);
                           }
                           break;
                 case 'v': fputs(version_string, file);
                           
                 case 's':
                           break;
                 case '+':
                 case '-':
                 case '*':
                 case '|': {
                             do {
                               do
                                 c = source_get();
                               while (c != nw_char);
                               c = source_get();
                             } while (c != '}' && c != ']' && c != ')' );
                           }
                 case ',':
                 case ')':
                 case ']':
                 case '}': return;
                 case '<': {
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
                           }
                           break;
                 case '%': {
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }
                           break;
                 case '_': {
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
                           }
                           break;
                 case 't': {
                             fprintf(file, "<i>fragment title</i>");
                           }
                           break;
                 case 'f': {
                             fprintf(file, "<i>file name</i>");
                           }
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
             }/* {:186} */

#line 447 "literate/markdown-output.weft"

             break;
           }
         if (md_scrap_type == 0 && prefix) {
           /* HTML-escape character c */
           
#line 464 "literate/markdown-output.weft"
           /* {184: literate/markdown-output.weft:464} */
switch (c) {
             case '<': fputs("&lt;", file); break;
             case '>': fputs("&gt;", file); break;
             case '&': fputs("&amp;", file); break;
             default:  putc(c, file); break;
           }/* {:184} */

#line 451 "literate/markdown-output.weft"

         } else {
           putc(c, file);
         }
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:183} */

#line 618 "literate/markdown-output.weft"
/* {192: literate/markdown-output.weft:618} */
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
/* {:192} */

#line 631 "literate/markdown-output.weft"
/* {193: literate/markdown-output.weft:631} */
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
/* {:193} */

#line 701 "literate/markdown-output.weft"
/* {195: literate/markdown-output.weft:701} */
static void md_format_file_entry(Name *name, FILE *md_file)
{
  while (name) {
    md_format_file_entry(name->llink, md_file);
    /* Format a md file index entry */
    
#line 712 "literate/markdown-output.weft"
    /* {196: literate/markdown-output.weft:712} */
fputs("- ", md_file);
    fprintf(md_file, "`\"%s\"` ", name->spelling);
    /* Write md file's defining scrap numbers */

#line 718 "literate/markdown-output.weft"
    /* {197: literate/markdown-output.weft:718} */
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
    }/* {:197} */

#line 714 "literate/markdown-output.weft"

    putc('\n', md_file);/* {:196} */

#line 705 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:195} */

#line 753 "literate/markdown-output.weft"
/* {199: literate/markdown-output.weft:753} */
static int md_load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = md_load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:199} */

#line 765 "literate/markdown-output.weft"
/* {200: literate/markdown-output.weft:765} */
static void md_format_entry(Name *name, FILE *md_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = md_load_entry(name, nms, 0);
  int i;

  /* Sort 'nms' of size 'n' for <Rob's ordering> */
  
#line 1401 "literate/latex-output.weft"
  /* {151: literate/latex-output.weft:1401} */
int j;
  for (j = 1; j < n; j++)
  {
     int i = j - 1;
     Name * kj = nms[j];

     do
     {
        Name * ki = nms[i];

        if (
#line 1398 "literate/latex-output.weft"
            /* {150: literate/latex-output.weft:1398} */
robs_strcmp(ki->spelling, kj->spelling) < 0/* {:150} */
)
           break;
        nms[i + 1] = ki;
        i -= 1;
     } while (i >= 0);
     nms[i + 1] = kj;
  }
  /* {:151} */

#line 771 "literate/markdown-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format a md index entry */
     
#line 783 "literate/markdown-output.weft"
     /* {201: literate/markdown-output.weft:783} */
if (name->sector == sector){
       fputs("- ", md_file);
       fputs("&lt;*", md_file);
       /* Write the md macro's name */
       
#line 257 "literate/markdown-output.weft"
       /* {172: literate/markdown-output.weft:257} */
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
       }/* {:172} */

#line 786 "literate/markdown-output.weft"

       fputs("* ", md_file);
       /* Write md defining scrap numbers */
       
#line 795 "literate/markdown-output.weft"
       /* {202: literate/markdown-output.weft:795} */
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
       }/* {:202} */

#line 788 "literate/markdown-output.weft"

       fputs("&gt; ", md_file);
       /* Write md referencing scrap numbers */
       
#line 819 "literate/markdown-output.weft"
       /* {203: literate/markdown-output.weft:819} */
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
       }/* {:203} */

#line 790 "literate/markdown-output.weft"

       putc('\n', md_file);
     }/* {:201} */

#line 776 "literate/markdown-output.weft"

  }
  free(nms);
}
/* {:200} */

#line 856 "literate/markdown-output.weft"
/* {205: literate/markdown-output.weft:856} */
static void md_format_user_entry(Name *name, FILE *md_file, unsigned char sector)
{
  while (name) {
    md_format_user_entry(name->llink, md_file, sector);
    /* Format a md user index entry */
    
#line 868 "literate/markdown-output.weft"
    /* {206: literate/markdown-output.weft:868} */
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
    }/* {:206} */

#line 860 "literate/markdown-output.weft"

    name = name->rlink;
  }
}
/* {:205} */

#line 940 "literate/markdown-output.weft"
/* {207: literate/markdown-output.weft:940} */

static void
md_format_uses_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_uses(scrap);
  if (p != NULL)
    /* Write md uses references */
    
#line 951 "literate/markdown-output.weft"
    /* {208: literate/markdown-output.weft:951} */
{
      char join = ' ';
      fputs("> *Uses:*", md_file);
      do {
        /* Write one md use reference */
        
#line 963 "literate/markdown-output.weft"
        /* {209: literate/markdown-output.weft:963} */
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
            
#line 984 "literate/markdown-output.weft"
            /* {210: literate/markdown-output.weft:984} */
fputs("<a href=\"#weft", md_file);
            write_scrap_ref(md_file, defs->scrap, -1, &page);
            fputs("\">", md_file);
            write_scrap_ref(md_file, defs->scrap, first, &page);
            fputs("</a>", md_file);/* {:210} */

#line 972 "literate/markdown-output.weft"

            first = FALSE;
            defs = defs->next;
          }while (defs!= NULL);
        }
        else
        {
          fputs("*(not defined globally)*", md_file);
        }
        /* {:209} */

#line 955 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:208} */

#line 946 "literate/markdown-output.weft"

}
/* {:207} */

#line 992 "literate/markdown-output.weft"
/* {211: literate/markdown-output.weft:992} */

static void
md_format_defs_refs(FILE * md_file, int scrap)
{
  Uses * p = get_scrap_defs(scrap);
  if (p != NULL)
    /* Write md defs references */
    
#line 1003 "literate/markdown-output.weft"
    /* {212: literate/markdown-output.weft:1003} */
{
      char join = ' ';
      fputs("> *Defines:*", md_file);
      do {
        /* Write one md def reference */
        
#line 1015 "literate/markdown-output.weft"
        /* {213: literate/markdown-output.weft:1015} */
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
               
#line 984 "literate/markdown-output.weft"
               /* {210: literate/markdown-output.weft:984} */
fputs("<a href=\"#weft", md_file);
               write_scrap_ref(md_file, defs->scrap, -1, &page);
               fputs("\">", md_file);
               write_scrap_ref(md_file, defs->scrap, first, &page);
               fputs("</a>", md_file);/* {:210} */

#line 1028 "literate/markdown-output.weft"

               first = FALSE;
            }
            defs = defs->next;
          }while (defs!= NULL);
        }
        /* {:213} */

#line 1007 "literate/markdown-output.weft"

        join = ',';
        p = p->next;
      }while (p != NULL);
      fputs(".\n", md_file);
    }/* {:212} */

#line 998 "literate/markdown-output.weft"

}
/* {:211} */
