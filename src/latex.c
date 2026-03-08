
#line 110 "literate/architecture.weft"
/* {7: literate/architecture.weft:110} */
#include "global.h"
static int scraps = 1;
/* {:7} */

#line 22 "literate/latex-output.weft"
/* {81: literate/latex-output.weft:22} */
static void copy_scrap(FILE *file, int prefix, Name *name);             /* formats the body of a scrap */
static void print_scrap_numbers(FILE *tex_file, Scrap_Node *scraps);      /* formats a list of scrap numbers */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector);             /* formats an index entry */
static void format_file_entry(Name *name, FILE *tex_file);        /* formats a file index entry */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector);
static void write_arg(FILE *tex_file, char *p);
static void write_literal(FILE *tex_file, char *p, int mode);
static void write_ArglistElement(FILE *file, Arglist *args, char **params);
/* {:81} */

#line 36 "literate/latex-output.weft"
/* {82: literate/latex-output.weft:36} */
void write_tex(char *file_name, char *tex_name, unsigned char sector)
{
  (void)sector;
  FILE *tex_file = fopen(tex_name, "w");
  if (tex_file) {
    if (verbose_flag)
      fprintf(stderr, "writing %s\n", tex_name);
    source_open(file_name);
    /* Write LaTeX limbo definitions */
    
#line 61 "literate/latex-output.weft"
    /* {83: literate/latex-output.weft:61} */
if (hyperref_flag) {
       fputs("\\newcommand{\\NWtarget}[2]{\\hypertarget{#1}{#2}}\n", tex_file);
       fputs("\\newcommand{\\NWlink}[2]{\\hyperlink{#1}{#2}}\n", tex_file);
    } else {
       fputs("\\newcommand{\\NWtarget}[2]{#2}\n", tex_file);
       fputs("\\newcommand{\\NWlink}[2]{#2}\n", tex_file);
    }
    fputs("\\newcommand{\\NWtxtMacroDefBy}{Fragment defined by}\n", tex_file);
    fputs("\\newcommand{\\NWtxtMacroRefIn}{Fragment referenced in}\n", tex_file);
    fputs("\\newcommand{\\NWtxtMacroNoRef}{Fragment never referenced}\n", tex_file);
    fputs("\\newcommand{\\NWtxtDefBy}{Defined by}\n", tex_file);
    fputs("\\newcommand{\\NWtxtRefIn}{Referenced in}\n", tex_file);
    fputs("\\newcommand{\\NWtxtNoRef}{Not referenced}\n", tex_file);
    fputs("\\newcommand{\\NWtxtFileDefBy}{File defined by}\n", tex_file);
    fputs("\\newcommand{\\NWtxtIdentsUsed}{Uses:}\n", tex_file);
    fputs("\\newcommand{\\NWtxtIdentsNotUsed}{Never used}\n", tex_file);
    fputs("\\newcommand{\\NWtxtIdentsDefed}{Defines:}\n", tex_file);
    fputs("\\newcommand{\\NWsep}{${\\diamond}$}\n", tex_file);
    fputs("\\newcommand{\\NWnotglobal}{(not defined globally)}\n", tex_file);
    fputs("\\newcommand{\\NWuseHyperlinks}{", tex_file);
    if (hyperoptions[0] != '\0')
    {
       /* Write the hyperlink usage macro */
       
#line 89 "literate/latex-output.weft"
       /* {84: literate/latex-output.weft:89} */
fprintf(tex_file, "\\usepackage[%s]{hyperref}", hyperoptions);/* {:84} */

#line 83 "literate/latex-output.weft"

    }
    fputs("}\n", tex_file);
    /* {:83} */

#line 44 "literate/latex-output.weft"

    /* Copy \verb|source_file| into \verb|tex_file| */
    
#line 96 "literate/latex-output.weft"
    /* {85: literate/latex-output.weft:96} */
{
      int inBlock = FALSE;
      int c = source_get();
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret at-sequence */
          
#line 112 "literate/latex-output.weft"
          /* {86: literate/latex-output.weft:112} */
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
#line 243 "literate/latex-output.weft"
                        /* {90: literate/latex-output.weft:243} */
{
                          Name *name = collect_file_name();
                          /* Begin the scrap environment */
                          
#line 330 "literate/latex-output.weft"
                          /* {94: literate/latex-output.weft:330} */
{
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 /* End block */
                                 
#line 361 "literate/latex-output.weft"
                                 /* {97: literate/latex-output.weft:361} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                                 fputs("\\end{flushleft}\n", tex_file);
                                 inBlock = FALSE;/* {:97} */

#line 335 "literate/latex-output.weft"

                              }
                              fputs("\\begin{flushleft} \\small", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 /* Switch block */
                                 
#line 358 "literate/latex-output.weft"
                                 /* {96: literate/latex-output.weft:358} */
fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);/* {:96} */

#line 343 "literate/latex-output.weft"

                              }
                              else
                              {
                                 /* Start block */
                                 
#line 354 "literate/latex-output.weft"
                                 /* {95: literate/latex-output.weft:354} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                                 inBlock = TRUE;/* {:95} */

#line 347 "literate/latex-output.weft"

                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\\raggedright\\small\n", scraps);
                          }/* {:94} */

#line 245 "literate/latex-output.weft"

                          fputs("\\NWtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} ", tex_file);
                          fprintf(tex_file, "\\verb%c\"%s\"%c\\nobreak\\ {\\footnotesize {", nw_char, name->spelling, nw_char);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\equiv$\n", tex_file);
                          /* Fill in the middle of the scrap environment */
                          
#line 371 "literate/latex-output.weft"
                          /* {98: literate/latex-output.weft:371} */
{
                            fputs("\\vspace{-1ex}\n\\begin{list}{}{} \\item\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\NWsep}\n\\end{list}\n", tex_file);
                          }/* {:98} */

#line 252 "literate/latex-output.weft"

                          /* Begin the cross-reference environment */
                          
#line 417 "literate/latex-output.weft"
                          /* {103: literate/latex-output.weft:417} */
{
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          /* {:103} */

#line 253 "literate/latex-output.weft"

                          if ( scrap_flag ) {
                            /* Write file defs */
                            
#line 436 "literate/latex-output.weft"
                            /* {105: literate/latex-output.weft:436} */
{
                              if (name->defs) {
                                if (name->defs->next) {
                                  fputs("\\item \\NWtxtFileDefBy\\ ", tex_file);
                                  print_scrap_numbers(tex_file, name->defs);
                                }
                              } else {
                                fprintf(stderr,
                                        "would have crashed in 'Write file defs' for '%s'\n",
                                         name->spelling);
                              }
                            }/* {:105} */

#line 255 "literate/latex-output.weft"

                          }
                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          /* Finish the cross-reference environment */
                          
#line 430 "literate/latex-output.weft"
                          /* {104: literate/latex-output.weft:430} */
{
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }/* {:104} */

#line 259 "literate/latex-output.weft"

                          /* Finish the scrap environment */
                          
#line 388 "literate/latex-output.weft"
                          /* {99: literate/latex-output.weft:388} */
{
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\vspace{4ex}\n\\end{flushleft}\n", tex_file);
                            else
                            {
                               /* End block */
                               
#line 361 "literate/latex-output.weft"
                               /* {97: literate/latex-output.weft:361} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                               fputs("\\end{flushleft}\n", tex_file);
                               inBlock = FALSE;/* {:97} */

#line 394 "literate/latex-output.weft"

                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }/* {:99} */

#line 260 "literate/latex-output.weft"

                        }/* {:90} */

#line 123 "literate/latex-output.weft"

                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
                        FALLTHROUGH;
              case 'q':
              case 'd': 
#line 268 "literate/latex-output.weft"
                        /* {91: literate/latex-output.weft:268} */
{
                          Name *name = collect_macro_name();

                          /* Begin the scrap environment */
                          
#line 330 "literate/latex-output.weft"
                          /* {94: literate/latex-output.weft:330} */
{
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 /* End block */
                                 
#line 361 "literate/latex-output.weft"
                                 /* {97: literate/latex-output.weft:361} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                                 fputs("\\end{flushleft}\n", tex_file);
                                 inBlock = FALSE;/* {:97} */

#line 335 "literate/latex-output.weft"

                              }
                              fputs("\\begin{flushleft} \\small", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 /* Switch block */
                                 
#line 358 "literate/latex-output.weft"
                                 /* {96: literate/latex-output.weft:358} */
fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);/* {:96} */

#line 343 "literate/latex-output.weft"

                              }
                              else
                              {
                                 /* Start block */
                                 
#line 354 "literate/latex-output.weft"
                                 /* {95: literate/latex-output.weft:354} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                                 inBlock = TRUE;/* {:95} */

#line 347 "literate/latex-output.weft"

                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\\raggedright\\small\n", scraps);
                          }/* {:94} */

#line 271 "literate/latex-output.weft"

                          fputs("\\NWtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} $\\langle\\,${\\itshape ", tex_file);
                          /* Write the macro's name */
                          
#line 290 "literate/latex-output.weft"
                          /* {92: literate/latex-output.weft:290} */
{
                            char * p = name->spelling;
                            int i = 0;

                            while (*p != '\000') {
                              if (*p == ARG_CHR) {
                                write_arg(tex_file, name->arg[i++]);
                                p++;
                              }
                              else
                                 fputc(*p++, tex_file);
                            }
                          }/* {:92} */

#line 275 "literate/latex-output.weft"

                          fputs("}\\nobreak\\ {\\footnotesize {", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\,\\rangle\\equiv$\n", tex_file);
                          /* Fill in the middle of the scrap environment */
                          
#line 371 "literate/latex-output.weft"
                          /* {98: literate/latex-output.weft:371} */
{
                            fputs("\\vspace{-1ex}\n\\begin{list}{}{} \\item\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\NWsep}\n\\end{list}\n", tex_file);
                          }/* {:98} */

#line 279 "literate/latex-output.weft"

                          /* Begin the cross-reference environment */
                          
#line 417 "literate/latex-output.weft"
                          /* {103: literate/latex-output.weft:417} */
{
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          /* {:103} */

#line 280 "literate/latex-output.weft"

                          /* Write macro defs */
                          
#line 450 "literate/latex-output.weft"
                          /* {106: literate/latex-output.weft:450} */
{
                            if (name->defs->next) {
                              fputs("\\item \\NWtxtMacroDefBy\\ ", tex_file);
                              print_scrap_numbers(tex_file, name->defs);
                            }
                          }/* {:106} */

#line 281 "literate/latex-output.weft"

                          /* Write macro refs */
                          
#line 458 "literate/latex-output.weft"
                          /* {107: literate/latex-output.weft:458} */
{
                            if (name->uses) {
                              if (name->uses->next) {
                                fputs("\\item \\NWtxtMacroRefIn\\ ", tex_file);
                                print_scrap_numbers(tex_file, name->uses);
                              }
                              else {
                                fputs("\\item \\NWtxtMacroRefIn\\ ", tex_file);
                                fputs("\\NWlink{weft", tex_file);
                                write_single_scrap_ref(tex_file, name->uses->scrap);
                                fputs("}{", tex_file);
                                write_single_scrap_ref(tex_file, name->uses->scrap);
                                fputs("}", tex_file);
                                fputs(".\n", tex_file);
                              }
                            }
                            else {
                              fputs("\\item {\\NWtxtMacroNoRef}.\n", tex_file);
                              fprintf(stderr, "%s: <%s> never referenced.\n",
                                      command_name, name->spelling);
                            }
                          }/* {:107} */

#line 282 "literate/latex-output.weft"

                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          /* Finish the cross-reference environment */
                          
#line 430 "literate/latex-output.weft"
                          /* {104: literate/latex-output.weft:430} */
{
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }/* {:104} */

#line 285 "literate/latex-output.weft"

                          /* Finish the scrap environment */
                          
#line 388 "literate/latex-output.weft"
                          /* {99: literate/latex-output.weft:388} */
{
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\vspace{4ex}\n\\end{flushleft}\n", tex_file);
                            else
                            {
                               /* End block */
                               
#line 361 "literate/latex-output.weft"
                               /* {97: literate/latex-output.weft:361} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                               fputs("\\end{flushleft}\n", tex_file);
                               inBlock = FALSE;/* {:97} */

#line 394 "literate/latex-output.weft"

                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }/* {:99} */

#line 286 "literate/latex-output.weft"

                        }/* {:91} */

#line 129 "literate/latex-output.weft"

                        break;
              case 's':
                        /* Step to next sector */
                        
#line 170 "literate/parser.weft"
/* {49: literate/parser.weft:170} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:49} */

#line 132 "literate/latex-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 177 "literate/parser.weft"
                        /* {50: literate/parser.weft:177} */
current_sector = 1;
                        c = source_get();
                        /* {:50} */

#line 135 "literate/latex-output.weft"

                        break;
              case '{':
              case '[':
              case '(': 
#line 410 "literate/latex-output.weft"
                        /* {102: literate/latex-output.weft:410} */
copy_scrap(tex_file, FALSE, NULL);
                        c = source_get();
                        /* {:102} */

#line 139 "literate/latex-output.weft"

                        break;
              case '<': 
#line 176 "literate/latex-output.weft"
                        /* {89: literate/latex-output.weft:176} */
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
                           write_scraps(tex_file, tex_name, name->defs, 0, indent_chars, 0, 0, 1, 0, 0,
                                 args, name->arg, local_parameters, tex_name);
                           name->mark = FALSE;
                           c = source_get();
                        }
                        /* {:89} */

#line 141 "literate/latex-output.weft"

                        break;
              case 'x': 
#line 734 "literate/latex-output.weft"
                        /* {121: literate/latex-output.weft:734} */
{
                           /* Get label from */
                           
#line 591 "literate/search-labels.weft"
                           /* {388: literate/search-labels.weft:591} */
char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = 
#line 735 "literate/latex-output.weft"
                                      /* {122: literate/latex-output.weft:735} */
source_get()/* {:122} */
, c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = 
#line 735 "literate/latex-output.weft"
                               /* {122: literate/latex-output.weft:735} */
source_get()/* {:122} */
;
                           /* {:388} */

#line 735 "literate/latex-output.weft"

                           write_label(label_name, 
#line 143 "literate/latex-output.weft"
                                                   /* {87: literate/latex-output.weft:143} */
tex_file/* {:87} */
);
                        }/* {:121} */

#line 143 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'c': 
#line 462 "literate/parser.weft"
                        /* {73: literate/parser.weft:462} */
if (inBlock)
                        {
                           /* End block */
                           
#line 361 "literate/latex-output.weft"
                           /* {97: literate/latex-output.weft:361} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                           fputs("\\end{flushleft}\n", tex_file);
                           inBlock = FALSE;/* {:97} */

#line 464 "literate/parser.weft"

                        }
                        else
                        {
                           /* Start block */
                           
#line 354 "literate/latex-output.weft"
                           /* {95: literate/latex-output.weft:354} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                           inBlock = TRUE;/* {:95} */

#line 468 "literate/parser.weft"

                        }/* {:73} */

#line 146 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'f': 
#line 900 "literate/latex-output.weft"
                        /* {130: literate/latex-output.weft:900} */
{
                          if (file_names) {
                            fputs("\n{\\small\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                                  tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);
                            format_file_entry(file_names, tex_file);
                            fputs("\\end{list}}", tex_file);
                          }
                          c = source_get();
                        }/* {:130} */

#line 149 "literate/latex-output.weft"

                        break;
              case 'm': 
#line 950 "literate/latex-output.weft"
                        /* {134: literate/latex-output.weft:950} */
{
                          unsigned char sector = current_sector;
                          int c = source_get();
                          if (c == '+')
                             sector = 0;
                          else
                             source_ungetc(&c);
                          if (has_sector(macro_names, sector)) {
                            fputs("\n{\\small\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                                  tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);
                            format_entry(macro_names, tex_file, sector);
                            fputs("\\end{list}}", tex_file);
                          } else {
                            fputs("None.\n", tex_file);
                          }
                        }
                        c = source_get();
                        /* {:134} */

#line 151 "literate/latex-output.weft"

                        break;
              case 'u': 
#line 1103 "literate/latex-output.weft"
                        /* {144: literate/latex-output.weft:1103} */
{
                            unsigned char sector = current_sector;
                            c = source_get();
                            if (c == '+') {
                               sector = 0;
                               c = source_get();
                            }
                            if (has_sector(user_names, sector)) {
                              fputs("\n{\\small\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                                    tex_file);
                              fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);
                              format_user_entry(user_names, tex_file, sector);
                              fputs("\\end{list}}", tex_file);
                            }
                        }/* {:144} */

#line 153 "literate/latex-output.weft"

                        break;
              case 'v': 
#line 171 "literate/latex-output.weft"
                        /* {88: literate/latex-output.weft:171} */
fputs(version_string, tex_file);
                        c = source_get();
                        /* {:88} */

#line 155 "literate/latex-output.weft"

                        break;
              case 'l':
              case 'L': 
#line 102 "literate/parser.weft"
                        /* {47: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:47} */

#line 158 "literate/latex-output.weft"

                        break;
              case 'W': 
#line 102 "literate/parser.weft"
                        /* {47: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:47} */

#line 160 "literate/latex-output.weft"

                        break;
              default:
                    if (c==nw_char)
                      putc(c, tex_file);
                    c = source_get();
                        break;
            }
          }/* {:86} */

#line 102 "literate/latex-output.weft"

          }
        else {
          putc(c, tex_file);
          c = source_get();
        }
      }
    }/* {:85} */

#line 45 "literate/latex-output.weft"

    fclose(tex_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, tex_name);
}
/* {:82} */

#line 305 "literate/latex-output.weft"
/* {93: literate/latex-output.weft:305} */
static void write_arg(FILE * tex_file, char * p)
{
   fputs("\\hbox{\\slshape\\sffamily ", tex_file);
   while (*p)
   {
      switch (*p)
      {
      case '$':
      case '_':
      case '^':
      case '#':
         fputc('\\', tex_file);
         break;
      default:
         break;
      }
      fputc(*p, tex_file);
      p++;
   }

   fputs("\\/}", tex_file);
}
/* {:93} */

#line 482 "literate/latex-output.weft"
/* {108: literate/latex-output.weft:482} */
static void print_scrap_numbers(FILE *tex_file, Scrap_Node *scraps)
{
  int page;
  fputs("\\NWlink{weft", tex_file);
  write_scrap_ref(tex_file, scraps->scrap, -1, &page);
  fputs("}{", tex_file);
  write_scrap_ref(tex_file, scraps->scrap, TRUE, &page);
  fputs("}", tex_file);
  scraps = scraps->next;
  while (scraps) {
    fputs("\\NWlink{weft", tex_file);
    write_scrap_ref(tex_file, scraps->scrap, -1, &page);
    fputs("}{", tex_file);
    write_scrap_ref(tex_file, scraps->scrap, FALSE, &page);
    scraps = scraps->next;
    fputs("}", tex_file);
  }
  fputs(".\n", tex_file);
}
/* {:108} */

#line 519 "literate/latex-output.weft"
/* {109: literate/latex-output.weft:519} */
static char *orig_delimit_scrap[3][5] = {
  /* {} mode: begin, end, insert nw_char, prefix, suffix */
  { "\\verb@", "@", "@{\\tt @}\\verb@", "\\mbox{}", "\\\\" },
  /* [] mode: begin, end, insert nw_char, prefix, suffix */
  { "", "", "@", "", "" },
  /* () mode: begin, end, insert nw_char, prefix, suffix */
  { "$", "$", "@", "", "" },
};

static char *delimit_scrap[3][5];
/* {:109} */

#line 538 "literate/latex-output.weft"
/* {110: literate/latex-output.weft:538} */
void initialise_delimit_scrap_array(void) {
  int i,j;
  for(i = 0; i < 3; i++) {
    for(j = 0; j < 5; j++) {
      if((delimit_scrap[i][j] = strdup(orig_delimit_scrap[i][j])) == NULL) {
        fprintf(stderr, "Not enough memory for string allocation\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  /* replace verb by lstinline */
  if (listings_flag) {
    free(delimit_scrap[0][0]);
    if((delimit_scrap[0][0]=strdup("\\lstinline@")) == NULL) {
      fprintf(stderr, "Not enough memory for string allocation\n");
      exit(EXIT_FAILURE);
    }
    free(delimit_scrap[0][2]);
    if((delimit_scrap[0][2]=strdup("@{\\tt @}\\lstinline@")) == NULL) {
      fprintf(stderr, "Not enough memory for string allocation\n");
      exit(EXIT_FAILURE);
    }
  }
}
/* {:110} */

#line 570 "literate/latex-output.weft"
/* {112: literate/latex-output.weft:570} */
int scrap_type = 0;
/* {:112} */

#line 574 "literate/latex-output.weft"
/* {113: literate/latex-output.weft:574} */
static void write_literal(FILE * tex_file, char * p, int mode)
{
   fputs(delimit_scrap[mode][0], tex_file);
   while (*p!= '\000') {
     if (*p == nw_char)
       fputs(delimit_scrap[mode][2], tex_file);
     else
       fputc(*p, tex_file);
     p++;
   }
   fputs(delimit_scrap[mode][1], tex_file);
}
/* {:113} */

#line 589 "literate/latex-output.weft"
/* {114: literate/latex-output.weft:589} */
static void copy_scrap(FILE *file, int prefix, Name *name)
{
  int indent = 0;
  int c;
  char ** params = name->arg;
  if (source_last == '{') scrap_type = 0;
  if (source_last == '[') scrap_type = 1;
  if (source_last == '(') scrap_type = 2;
  c = source_get();
  if (prefix) fputs(delimit_scrap[scrap_type][3], file);
  fputs(delimit_scrap[scrap_type][0], file);
  while (1) {
    switch (c) {
      case '\n': fputs(delimit_scrap[scrap_type][1], file);
                 if (prefix) fputs(delimit_scrap[scrap_type][4], file);
                 fputs("\n", file);
                 if (prefix) fputs(delimit_scrap[scrap_type][3], file);
                 fputs(delimit_scrap[scrap_type][0], file);
                 indent = 0;
                 break;
      case '\t': 
#line 664 "literate/latex-output.weft"
                 /* {117: literate/latex-output.weft:664} */
{
                   int delta = 8 - (indent % 8);
                   indent += delta;
                   while (delta > 0) {
                     putc(' ', file);
                     delta--;
                   }
                 }/* {:117} */

#line 609 "literate/latex-output.weft"

                 break;
      default:
         if (c==nw_char)
           {
             /* Check at-sequence for end-of-scrap */
             
#line 674 "literate/latex-output.weft"
             /* {118: literate/latex-output.weft:674} */
{
               c = source_get();
               switch (c) {
                 case 'c': 
#line 391 "literate/parser.weft"
                           /* {67: literate/parser.weft:391} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape (Comment)}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
                           /* {:67} */

#line 677 "literate/latex-output.weft"

                           break;
                 case 'x': 
#line 734 "literate/latex-output.weft"
                           /* {121: literate/latex-output.weft:734} */
{
                              /* Get label from */
                              
#line 591 "literate/search-labels.weft"
                              /* {388: literate/search-labels.weft:591} */
char  label_name[MAX_NAME_LEN];
                              char * p = label_name;
                              while (c = 
#line 735 "literate/latex-output.weft"
                                         /* {122: literate/latex-output.weft:735} */
source_get()/* {:122} */
, c != nw_char) /* Here is ?-01 */
                                 *p++ = c;
                              *p = '\0';
                              c = 
#line 735 "literate/latex-output.weft"
                                  /* {122: literate/latex-output.weft:735} */
source_get()/* {:122} */
;
                              /* {:388} */

#line 735 "literate/latex-output.weft"

                              write_label(label_name, 
#line 679 "literate/latex-output.weft"
                                                      /* {119: literate/latex-output.weft:679} */
file/* {:119} */
);
                           }/* {:121} */

#line 679 "literate/latex-output.weft"

                           break;
                 case 'v': 
#line 730 "literate/latex-output.weft"
                           /* {120: literate/latex-output.weft:730} */
fputs(version_string, file);
                           /* {:120} */

#line 681 "literate/latex-output.weft"

                 case 's':
                           break;
                 case '+':
                 case '-':
                 case '*':
                 case '|': 
#line 743 "literate/latex-output.weft"
                           /* {123: literate/latex-output.weft:743} */
{
                             do {
                               do
                                 c = source_get();
                               while (c != nw_char);
                               c = source_get();
                             } while (c != '}' && c != ']' && c != ')' );
                           }/* {:123} */

#line 687 "literate/latex-output.weft"

                           FALLTHROUGH;
                 case ',':
                 case ')':
                 case ']':
                 case '}': fputs(delimit_scrap[scrap_type][1], file);
                           return;
                 case '<': 
#line 783 "literate/latex-output.weft"
                           /* {127: literate/latex-output.weft:783} */
{
                             Arglist *args = collect_scrap_name(-1);
                             Name *name = args->name;
                             char * p = name->spelling;
                             Arglist *q = args->args;
                             int narg = 0;

                             fputs(delimit_scrap[scrap_type][1],file);
                             if (prefix)
                               fputs("\\hbox{", file);
                             fputs("$\\langle\\,${\\itshape ", file);
                             while (*p != '\000') {
                               if (*p == ARG_CHR) {
                                 if (q == NULL) {
                                    write_literal(file, name->arg[narg], scrap_type);
                                 }
                                 else {
                                   write_ArglistElement(file, q, params);
                                   q = q->next;
                                 }
                                 p++;
                                 narg++;
                               }
                               else
                                  fputc(*p++, file);
                             }
                             fputs("}\\nobreak\\ ", file);
                             if (scrap_name_has_parameters) {
                               /* Format macro parameters */
                               
#line 641 "literate/parser.weft"
/* {79: literate/parser.weft:641} */

                                  char sep;

                                  sep = '(';
                                  do {
                                    fputc(sep,file);

                                    fputs("{\\footnotesize ", file);
                                    write_single_scrap_ref(file, scraps + 1);
                                    fprintf(file, "\\label{scrap%d}\n", scraps + 1);
                                    fputs(" }", file);

                                    source_last = '{';
                                    copy_scrap(file, TRUE, NULL);

                                    ++scraps;

                                    sep = ',';
                                  } while ( source_last != ')' && source_last != EOF );
                                  fputs(" ) ",file);
                                  do
                                    c = source_get();
                                  while(c != nw_char && c != EOF);
                                  if (c == nw_char) {
                                    c = source_get();
                                  }
                               /* {:79} */

#line 811 "literate/latex-output.weft"

                             }
                             fprintf(file, "{\\footnotesize ");
                             if (name->defs)
                               /* Write abbreviated definition list */
                               
#line 829 "literate/latex-output.weft"
                               /* {128: literate/latex-output.weft:829} */
{
                                 Scrap_Node *p = name->defs;
                                 fputs("\\NWlink{weft", file);
                                 write_single_scrap_ref(file, p->scrap);
                                 fputs("}{", file);
                                 write_single_scrap_ref(file, p->scrap);
                                 fputs("}", file);
                                 p = p->next;
                                 if (p)
                                   fputs(", \\ldots\\ ", file);
                               }/* {:128} */

#line 815 "literate/latex-output.weft"

                             else {
                               putc('?', file);
                               fprintf(stderr, "%s: never defined <%s>\n",
                                       command_name, name->spelling);
                             }
                             fputs("}$\\,\\rangle$", file);
                             if (prefix)
                                fputs("}", file);
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:127} */

#line 694 "literate/latex-output.weft"

                           break;
                 case '%': 
#line 753 "literate/latex-output.weft"
                           /* {124: literate/latex-output.weft:753} */
{
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }/* {:124} */

#line 696 "literate/latex-output.weft"

                           break;
                 case '_': 
#line 762 "literate/latex-output.weft"
                           /* {125: literate/latex-output.weft:762} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\bfseries ");
                             c = source_get();
                             do {
                                 fputc(c, file);
                                 c = source_get();
                             } while (c != nw_char);
                             c = source_get();
                             fprintf(file, "}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:125} */

#line 698 "literate/latex-output.weft"

                           break;
                 case 't': 
#line 776 "literate/latex-output.weft"
                           /* {126: literate/latex-output.weft:776} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape fragment title}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:126} */

#line 700 "literate/latex-output.weft"

                           break;
                 case 'f': 
#line 776 "literate/latex-output.weft"
                           /* {126: literate/latex-output.weft:776} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape file name}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:126} */

#line 702 "literate/latex-output.weft"

                           break;
                 case '1': case '2': case '3':
                 case '4': case '5': case '6':
                 case '7': case '8': case '9':
                           if (name == NULL
                               || name->arg[c - '1'] == NULL) {
                             fputs(delimit_scrap[scrap_type][2], file);
                             fputc(c,   file);
                           }
                           else {
                             fputs(delimit_scrap[scrap_type][1], file);
                             write_arg(file, name->arg[c - '1']);
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
                           break;
                 default:
                       if (c==nw_char)
                         {
                           fputs(delimit_scrap[scrap_type][2], file);
                           break;
                         }
                       /* ignore these since pass1 will have warned about them */
                           break;
               }
             }/* {:118} */

#line 614 "literate/latex-output.weft"

             break;
           }
         putc(c, file);
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:114} */

#line 630 "literate/latex-output.weft"
/* {115: literate/latex-output.weft:630} */
void update_delimit_scrap(void)
{
  /* {}-mode begin */
  if (listings_flag) {
    delimit_scrap[0][0][10] = nw_char;
  } else {
    delimit_scrap[0][0][5] = nw_char;
  }
  /* {}-mode end */
  delimit_scrap[0][1][0] = nw_char;
  /* {}-mode insert nw_char */

  delimit_scrap[0][2][0] = nw_char;
  delimit_scrap[0][2][6] = nw_char;

  if (listings_flag) {
    delimit_scrap[0][2][18] = nw_char;
  } else {
    delimit_scrap[0][2][13] = nw_char;
  }

  /* []-mode insert nw_char */
  delimit_scrap[1][2][0] = nw_char;

  /* ()-mode insert nw_char */
  delimit_scrap[2][2][0] = nw_char;
}
/* {:115} */

#line 842 "literate/latex-output.weft"
/* {129: literate/latex-output.weft:842} */
static void
write_ArglistElement(FILE * file, Arglist * args, char ** params)
{
  Name *name = args->name;
  Arglist *q = args->args;

  if (name == NULL) {
    char * p = (char*)q;

    if (p[0] == ARG_CHR) {
       write_arg(file, params[p[1] - '1']);
    } else {
       write_literal(file, (char *)q, 0);
    }
  } else if (name == (Name *)1) {
    Scrap_Node * qq = (Scrap_Node *)q;
    qq->quoted = TRUE;
    fputs(delimit_scrap[scrap_type][0], file);
    write_scraps(file, "", qq,
                 -1, "", 0, 0, 0, 0, 0,
                 NULL, params, 0, "");
    fputs(delimit_scrap[scrap_type][1], file);
    extra_scraps++;
    qq->quoted = FALSE;
  } else {
    char * p = name->spelling;
    fputs("$\\langle\\,${\\itshape ", file);
      while (*p != '\000') {
      if (*p == ARG_CHR) {
        write_ArglistElement(file, q, params);
        q = q->next;
        p++;
      }
      else
         fputc(*p++, file);
    }
    fputs("}\\nobreak\\ ", file);
    if (scrap_name_has_parameters) {
      int c;

      /* Format macro parameters */
      
#line 641 "literate/parser.weft"
/* {79: literate/parser.weft:641} */

         char sep;

         sep = '(';
         do {
           fputc(sep,file);

           fputs("{\\footnotesize ", file);
           write_single_scrap_ref(file, scraps + 1);
           fprintf(file, "\\label{scrap%d}\n", scraps + 1);
           fputs(" }", file);

           source_last = '{';
           copy_scrap(file, TRUE, NULL);

           ++scraps;

           sep = ',';
         } while ( source_last != ')' && source_last != EOF );
         fputs(" ) ",file);
         do
           c = source_get();
         while(c != nw_char && c != EOF);
         if (c == nw_char) {
           c = source_get();
         }
      /* {:79} */

#line 882 "literate/latex-output.weft"

    }
    fprintf(file, "{\\footnotesize ");
    if (name->defs)
      /* Write abbreviated definition list */
      
#line 829 "literate/latex-output.weft"
      /* {128: literate/latex-output.weft:829} */
{
        Scrap_Node *p = name->defs;
        fputs("\\NWlink{weft", file);
        write_single_scrap_ref(file, p->scrap);
        fputs("}{", file);
        write_single_scrap_ref(file, p->scrap);
        fputs("}", file);
        p = p->next;
        if (p)
          fputs(", \\ldots\\ ", file);
      }/* {:128} */

#line 886 "literate/latex-output.weft"

    else {
      putc('?', file);
      fprintf(stderr, "%s: never defined <%s>\n",
              command_name, name->spelling);
    }
    fputs("}$\\,\\rangle$", file);
  }
}
/* {:129} */

#line 912 "literate/latex-output.weft"
/* {131: literate/latex-output.weft:912} */
static void format_file_entry(Name *name, FILE *tex_file)
{
  while (name) {
    format_file_entry(name->llink, tex_file);
    /* Format a file index entry */
    
#line 923 "literate/latex-output.weft"
    /* {132: literate/latex-output.weft:923} */
fputs("\\item ", tex_file);
    fprintf(tex_file, "\\verb%c\"%s\"%c ", nw_char, name->spelling, nw_char);
    /* Write file's defining scrap numbers */

#line 929 "literate/latex-output.weft"
    /* {133: literate/latex-output.weft:929} */
    {
      Scrap_Node *p = name->defs;
      fputs("{\\footnotesize {\\NWtxtDefBy}", tex_file);
      if (p->next) {
        /* fputs("s ", tex_file); */
          putc(' ', tex_file);
        print_scrap_numbers(tex_file, p);
      }
      else {
        putc(' ', tex_file);
        fputs("\\NWlink{weft", tex_file);
        write_single_scrap_ref(tex_file, p->scrap);
        fputs("}{", tex_file);
        write_single_scrap_ref(tex_file, p->scrap);
        fputs("}", tex_file);
        putc('.', tex_file);
      }
      putc('}', tex_file);
    }/* {:133} */

#line 925 "literate/latex-output.weft"

    putc('\n', tex_file);/* {:132} */

#line 916 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:131} */

#line 971 "literate/latex-output.weft"
/* {135: literate/latex-output.weft:971} */
static int load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:135} */

#line 983 "literate/latex-output.weft"
/* {136: literate/latex-output.weft:983} */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = load_entry(name, nms, 0);
  int i;

  /* Sort 'nms' of size 'n' for <Rob's ordering> */
  
#line 1003 "literate/latex-output.weft"
  /* {138: literate/latex-output.weft:1003} */
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
            /* {137: literate/latex-output.weft:1000} */
robs_strcmp(ki->spelling, kj->spelling) < 0/* {:137} */
)
           break;
        nms[i + 1] = ki;
        i -= 1;
     } while (i >= 0);
     nms[i + 1] = kj;
  }
  /* {:138} */

#line 989 "literate/latex-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format an index entry */
     
#line 1023 "literate/latex-output.weft"
     /* {139: literate/latex-output.weft:1023} */
if (name->sector == sector){
       fputs("\\item ", tex_file);
       fputs("$\\langle\\,$", tex_file);
       /* Write the macro's name */
       
#line 290 "literate/latex-output.weft"
       /* {92: literate/latex-output.weft:290} */
{
         char * p = name->spelling;
         int i = 0;

         while (*p != '\000') {
           if (*p == ARG_CHR) {
             write_arg(tex_file, name->arg[i++]);
             p++;
           }
           else
              fputc(*p++, tex_file);
         }
       }/* {:92} */

#line 1026 "literate/latex-output.weft"

       fputs("\\nobreak\\ {\\footnotesize ", tex_file);
       /* Write defining scrap numbers */
       
#line 1035 "literate/latex-output.weft"
       /* {140: literate/latex-output.weft:1035} */
{
         Scrap_Node *p = name->defs;
         if (p) {
           int page;
           fputs("\\NWlink{weft", tex_file);
           write_scrap_ref(tex_file, p->scrap, -1, &page);
           fputs("}{", tex_file);
           write_scrap_ref(tex_file, p->scrap, TRUE, &page);
           fputs("}", tex_file);
           p = p->next;
           while (p) {
             fputs("\\NWlink{weft", tex_file);
             write_scrap_ref(tex_file, p->scrap, -1, &page);
             fputs("}{", tex_file);
             write_scrap_ref(tex_file, p->scrap, FALSE, &page);
             fputs("}", tex_file);
             p = p->next;
           }
         }
         else
           putc('?', tex_file);
       }/* {:140} */

#line 1028 "literate/latex-output.weft"

       fputs("}$\\,\\rangle$ ", tex_file);
       /* Write referencing scrap numbers */
       
#line 1059 "literate/latex-output.weft"
       /* {141: literate/latex-output.weft:1059} */
{
         Scrap_Node *p = name->uses;
         fputs("{\\footnotesize ", tex_file);
         if (p) {
           fputs("{\\NWtxtRefIn}", tex_file);
           if (p->next) {
             /* fputs("s ", tex_file); */
             putc(' ', tex_file);
             print_scrap_numbers(tex_file, p);
           }
           else {
             putc(' ', tex_file);
             fputs("\\NWlink{weft", tex_file);
             write_single_scrap_ref(tex_file, p->scrap);
             fputs("}{", tex_file);
             write_single_scrap_ref(tex_file, p->scrap);
             fputs("}", tex_file);
             putc('.', tex_file);
           }
         }
         else
           fputs("{\\NWtxtNoRef}.", tex_file);
         putc('}', tex_file);
       }/* {:141} */

#line 1030 "literate/latex-output.weft"

       putc('\n', tex_file);
     }/* {:139} */

#line 994 "literate/latex-output.weft"

  }
}
/* {:136} */

#line 1089 "literate/latex-output.weft"
/* {143: literate/latex-output.weft:1089} */
int has_sector(Name * name, unsigned char sector)
{
  while(name) {
    if (name->sector == sector)
       return TRUE;
    if (has_sector(name->llink, sector))
       return TRUE;
     name = name->rlink;
   }
   return FALSE;
}
/* {:143} */

#line 1121 "literate/latex-output.weft"
/* {145: literate/latex-output.weft:1121} */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  while (name) {
    format_user_entry(name->llink, tex_file, sector);
    /* Format a user index entry */
    
#line 1133 "literate/latex-output.weft"
    /* {146: literate/latex-output.weft:1133} */
if (name->sector == sector){
      Scrap_Node *uses = name->uses;
      if ( uses || dangling_flag ) {
        int page;
        Scrap_Node *defs = name->defs;
        fprintf(tex_file, "\\item \\verb%c%s%c: ", nw_char,name->spelling,nw_char);
        if (!uses) {
            fputs("(\\underline{", tex_file);
            fputs("\\NWlink{weft", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("}{", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("})}", tex_file);
            page = -2;
            defs = defs->next;
        }
        else
          if (!defs || uses->scrap < defs->scrap) {
          fputs("\\NWlink{weft", tex_file);
          write_scrap_ref(tex_file, uses->scrap, -1, &page);
          fputs("}{", tex_file);
          write_scrap_ref(tex_file, uses->scrap, TRUE, &page);
          fputs("}", tex_file);
          uses = uses->next;
        }
        else {
          if (defs->scrap == uses->scrap)
            uses = uses->next;
          fputs("\\underline{", tex_file);

          fputs("\\NWlink{weft", tex_file);
          write_single_scrap_ref(tex_file, defs->scrap);
          fputs("}{", tex_file);
          write_single_scrap_ref(tex_file, defs->scrap);
          fputs("}}", tex_file);
          page = -2;
          defs = defs->next;
        }
        while (uses || defs) {
          if (uses && (!defs || uses->scrap < defs->scrap)) {
            fputs("\\NWlink{weft", tex_file);
            write_scrap_ref(tex_file, uses->scrap, -1, &page);
            fputs("}{", tex_file);
            write_scrap_ref(tex_file, uses->scrap, FALSE, &page);
            fputs("}", tex_file);
            uses = uses->next;
          }
          else {
            if (uses && defs->scrap == uses->scrap)
              uses = uses->next;
            fputs(", \\underline{", tex_file);

            fputs("\\NWlink{weft", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("}{", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("}", tex_file);

            putc('}', tex_file);
            page = -2;
            defs = defs->next;
          }
        }
        fputs(".\n", tex_file);
      }
    }/* {:146} */

#line 1125 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:145} */
