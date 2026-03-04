
#line 90 "literate/architecture.weft"
/* {7: literate/architecture.weft:90} */
#include "global.h"
static int scraps = 1;
/* {:7} */

#line 22 "literate/latex-output.weft"
/* {77: literate/latex-output.weft:22} */
static void copy_scrap(FILE *file, int prefix, Name *name);             /* formats the body of a scrap */
static void print_scrap_numbers(FILE *tex_file, Scrap_Node *scraps);      /* formats a list of scrap numbers */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector);             /* formats an index entry */
static void format_file_entry(Name *name, FILE *tex_file);        /* formats a file index entry */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector);
static void write_arg(FILE *tex_file, char *p);
static void write_literal(FILE *tex_file, char *p, int mode);
static void write_ArglistElement(FILE *file, Arglist *args, char **params);
/* {:77} */

#line 36 "literate/latex-output.weft"
/* {78: literate/latex-output.weft:36} */
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
    /* {79: literate/latex-output.weft:61} */
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
       /* {80: literate/latex-output.weft:89} */
fprintf(tex_file, "\\usepackage[%s]{hyperref}", hyperoptions);/* {:80} */

#line 83 "literate/latex-output.weft"

    }
    fputs("}\n", tex_file);
    /* {:79} */

#line 44 "literate/latex-output.weft"

    /* Copy \verb|source_file| into \verb|tex_file| */
    
#line 96 "literate/latex-output.weft"
    /* {81: literate/latex-output.weft:96} */
{
      int inBlock = FALSE;
      int c = source_get();
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret at-sequence */
          
#line 112 "literate/latex-output.weft"
          /* {82: literate/latex-output.weft:112} */
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
#line 241 "literate/latex-output.weft"
                        /* {86: literate/latex-output.weft:241} */
{
                          Name *name = collect_file_name();
                          /* Begin the scrap environment */
                          
#line 328 "literate/latex-output.weft"
                          /* {90: literate/latex-output.weft:328} */
{
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 /* End block */
                                 
#line 359 "literate/latex-output.weft"
                                 /* {93: literate/latex-output.weft:359} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                                 fputs("\\end{flushleft}\n", tex_file);
                                 inBlock = FALSE;/* {:93} */

#line 333 "literate/latex-output.weft"

                              }
                              fputs("\\begin{flushleft} \\small", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 /* Switch block */
                                 
#line 356 "literate/latex-output.weft"
                                 /* {92: literate/latex-output.weft:356} */
fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);/* {:92} */

#line 341 "literate/latex-output.weft"

                              }
                              else
                              {
                                 /* Start block */
                                 
#line 352 "literate/latex-output.weft"
                                 /* {91: literate/latex-output.weft:352} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                                 inBlock = TRUE;/* {:91} */

#line 345 "literate/latex-output.weft"

                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\\raggedright\\small\n", scraps);
                          }/* {:90} */

#line 243 "literate/latex-output.weft"

                          fputs("\\NWtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} ", tex_file);
                          fprintf(tex_file, "\\verb%c\"%s\"%c\\nobreak\\ {\\footnotesize {", nw_char, name->spelling, nw_char);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\equiv$\n", tex_file);
                          /* Fill in the middle of the scrap environment */
                          
#line 369 "literate/latex-output.weft"
                          /* {94: literate/latex-output.weft:369} */
{
                            fputs("\\vspace{-1ex}\n\\begin{list}{}{} \\item\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\NWsep}\n\\end{list}\n", tex_file);
                          }/* {:94} */

#line 250 "literate/latex-output.weft"

                          /* Begin the cross-reference environment */
                          
#line 415 "literate/latex-output.weft"
                          /* {99: literate/latex-output.weft:415} */
{
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          /* {:99} */

#line 251 "literate/latex-output.weft"

                          if ( scrap_flag ) {
                            /* Write file defs */
                            
#line 434 "literate/latex-output.weft"
                            /* {101: literate/latex-output.weft:434} */
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
                            }/* {:101} */

#line 253 "literate/latex-output.weft"

                          }
                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          /* Finish the cross-reference environment */
                          
#line 428 "literate/latex-output.weft"
                          /* {100: literate/latex-output.weft:428} */
{
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }/* {:100} */

#line 257 "literate/latex-output.weft"

                          /* Finish the scrap environment */
                          
#line 386 "literate/latex-output.weft"
                          /* {95: literate/latex-output.weft:386} */
{
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\vspace{4ex}\n\\end{flushleft}\n", tex_file);
                            else
                            {
                               /* End block */
                               
#line 359 "literate/latex-output.weft"
                               /* {93: literate/latex-output.weft:359} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                               fputs("\\end{flushleft}\n", tex_file);
                               inBlock = FALSE;/* {:93} */

#line 392 "literate/latex-output.weft"

                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }/* {:95} */

#line 258 "literate/latex-output.weft"

                        }/* {:86} */

#line 122 "literate/latex-output.weft"

                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
              case 'q':
              case 'd': 
#line 266 "literate/latex-output.weft"
                        /* {87: literate/latex-output.weft:266} */
{
                          Name *name = collect_macro_name();

                          /* Begin the scrap environment */
                          
#line 328 "literate/latex-output.weft"
                          /* {90: literate/latex-output.weft:328} */
{
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 /* End block */
                                 
#line 359 "literate/latex-output.weft"
                                 /* {93: literate/latex-output.weft:359} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                                 fputs("\\end{flushleft}\n", tex_file);
                                 inBlock = FALSE;/* {:93} */

#line 333 "literate/latex-output.weft"

                              }
                              fputs("\\begin{flushleft} \\small", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 /* Switch block */
                                 
#line 356 "literate/latex-output.weft"
                                 /* {92: literate/latex-output.weft:356} */
fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);/* {:92} */

#line 341 "literate/latex-output.weft"

                              }
                              else
                              {
                                 /* Start block */
                                 
#line 352 "literate/latex-output.weft"
                                 /* {91: literate/latex-output.weft:352} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                                 inBlock = TRUE;/* {:91} */

#line 345 "literate/latex-output.weft"

                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\\raggedright\\small\n", scraps);
                          }/* {:90} */

#line 269 "literate/latex-output.weft"

                          fputs("\\NWtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} $\\langle\\,${\\itshape ", tex_file);
                          /* Write the macro's name */
                          
#line 288 "literate/latex-output.weft"
                          /* {88: literate/latex-output.weft:288} */
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
                          }/* {:88} */

#line 273 "literate/latex-output.weft"

                          fputs("}\\nobreak\\ {\\footnotesize {", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\,\\rangle\\equiv$\n", tex_file);
                          /* Fill in the middle of the scrap environment */
                          
#line 369 "literate/latex-output.weft"
                          /* {94: literate/latex-output.weft:369} */
{
                            fputs("\\vspace{-1ex}\n\\begin{list}{}{} \\item\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\NWsep}\n\\end{list}\n", tex_file);
                          }/* {:94} */

#line 277 "literate/latex-output.weft"

                          /* Begin the cross-reference environment */
                          
#line 415 "literate/latex-output.weft"
                          /* {99: literate/latex-output.weft:415} */
{
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          /* {:99} */

#line 278 "literate/latex-output.weft"

                          /* Write macro defs */
                          
#line 448 "literate/latex-output.weft"
                          /* {102: literate/latex-output.weft:448} */
{
                            if (name->defs->next) {
                              fputs("\\item \\NWtxtMacroDefBy\\ ", tex_file);
                              print_scrap_numbers(tex_file, name->defs);
                            }
                          }/* {:102} */

#line 279 "literate/latex-output.weft"

                          /* Write macro refs */
                          
#line 456 "literate/latex-output.weft"
                          /* {103: literate/latex-output.weft:456} */
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
                          }/* {:103} */

#line 280 "literate/latex-output.weft"

                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          /* Finish the cross-reference environment */
                          
#line 428 "literate/latex-output.weft"
                          /* {100: literate/latex-output.weft:428} */
{
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }/* {:100} */

#line 283 "literate/latex-output.weft"

                          /* Finish the scrap environment */
                          
#line 386 "literate/latex-output.weft"
                          /* {95: literate/latex-output.weft:386} */
{
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\vspace{4ex}\n\\end{flushleft}\n", tex_file);
                            else
                            {
                               /* End block */
                               
#line 359 "literate/latex-output.weft"
                               /* {93: literate/latex-output.weft:359} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                               fputs("\\end{flushleft}\n", tex_file);
                               inBlock = FALSE;/* {:93} */

#line 392 "literate/latex-output.weft"

                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }/* {:95} */

#line 284 "literate/latex-output.weft"

                        }/* {:87} */

#line 127 "literate/latex-output.weft"

                        break;
              case 's':
                        /* Step to next sector */
                        
#line 169 "literate/parser.weft"
/* {45: literate/parser.weft:169} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:45} */

#line 130 "literate/latex-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 176 "literate/parser.weft"
                        /* {46: literate/parser.weft:176} */
current_sector = 1;
                        c = source_get();
                        /* {:46} */

#line 133 "literate/latex-output.weft"

                        break;
              case '{':
              case '[':
              case '(': 
#line 408 "literate/latex-output.weft"
                        /* {98: literate/latex-output.weft:408} */
copy_scrap(tex_file, FALSE, NULL);
                        c = source_get();
                        /* {:98} */

#line 137 "literate/latex-output.weft"

                        break;
              case '<': 
#line 174 "literate/latex-output.weft"
                        /* {85: literate/latex-output.weft:174} */
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
                        /* {:85} */

#line 139 "literate/latex-output.weft"

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
#line 141 "literate/latex-output.weft"
                                                   /* {83: literate/latex-output.weft:141} */
tex_file/* {:83} */
);
                        }/* {:117} */

#line 141 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'c': 
#line 461 "literate/parser.weft"
                        /* {69: literate/parser.weft:461} */
if (inBlock)
                        {
                           /* End block */
                           
#line 359 "literate/latex-output.weft"
                           /* {93: literate/latex-output.weft:359} */
fputs("\\end{minipage}\\vspace{4ex}\n",  tex_file);
                           fputs("\\end{flushleft}\n", tex_file);
                           inBlock = FALSE;/* {:93} */

#line 463 "literate/parser.weft"

                        }
                        else
                        {
                           /* Start block */
                           
#line 352 "literate/latex-output.weft"
                           /* {91: literate/latex-output.weft:352} */
fputs("\\begin{flushleft} \\small\n\\begin{minipage}{\\linewidth}", tex_file);
                           inBlock = TRUE;/* {:91} */

#line 467 "literate/parser.weft"

                        }/* {:69} */

#line 144 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'f': 
#line 897 "literate/latex-output.weft"
                        /* {126: literate/latex-output.weft:897} */
{
                          if (file_names) {
                            fputs("\n{\\small\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                                  tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);
                            format_file_entry(file_names, tex_file);
                            fputs("\\end{list}}", tex_file);
                          }
                          c = source_get();
                        }/* {:126} */

#line 147 "literate/latex-output.weft"

                        break;
              case 'm': 
#line 947 "literate/latex-output.weft"
                        /* {130: literate/latex-output.weft:947} */
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
                        /* {:130} */

#line 149 "literate/latex-output.weft"

                        break;
              case 'u': 
#line 1100 "literate/latex-output.weft"
                        /* {140: literate/latex-output.weft:1100} */
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
                        }/* {:140} */

#line 151 "literate/latex-output.weft"

                        break;
              case 'v': 
#line 169 "literate/latex-output.weft"
                        /* {84: literate/latex-output.weft:169} */
fputs(version_string, tex_file);
                        c = source_get();
                        /* {:84} */

#line 153 "literate/latex-output.weft"

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

#line 156 "literate/latex-output.weft"

                        break;
              case 'W': 
#line 102 "literate/parser.weft"
                        /* {43: literate/parser.weft:102} */
{ int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();/* {:43} */

#line 158 "literate/latex-output.weft"

                        break;
              default:
                    if (c==nw_char)
                      putc(c, tex_file);
                    c = source_get();
                        break;
            }
          }/* {:82} */

#line 102 "literate/latex-output.weft"

          }
        else {
          putc(c, tex_file);
          c = source_get();
        }
      }
    }/* {:81} */

#line 45 "literate/latex-output.weft"

    fclose(tex_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, tex_name);
}
/* {:78} */

#line 303 "literate/latex-output.weft"
/* {89: literate/latex-output.weft:303} */
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
/* {:89} */

#line 480 "literate/latex-output.weft"
/* {104: literate/latex-output.weft:480} */
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
/* {:104} */

#line 517 "literate/latex-output.weft"
/* {105: literate/latex-output.weft:517} */
static char *orig_delimit_scrap[3][5] = {
  /* {} mode: begin, end, insert nw_char, prefix, suffix */
  { "\\verb@", "@", "@{\\tt @}\\verb@", "\\mbox{}", "\\\\" },
  /* [] mode: begin, end, insert nw_char, prefix, suffix */
  { "", "", "@", "", "" },
  /* () mode: begin, end, insert nw_char, prefix, suffix */
  { "$", "$", "@", "", "" },
};

static char *delimit_scrap[3][5];
/* {:105} */

#line 536 "literate/latex-output.weft"
/* {106: literate/latex-output.weft:536} */
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
/* {:106} */

#line 568 "literate/latex-output.weft"
/* {108: literate/latex-output.weft:568} */
int scrap_type = 0;
/* {:108} */

#line 572 "literate/latex-output.weft"
/* {109: literate/latex-output.weft:572} */
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
/* {:109} */

#line 587 "literate/latex-output.weft"
/* {110: literate/latex-output.weft:587} */
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

#line 607 "literate/latex-output.weft"

                 break;
      default:
         if (c==nw_char)
           {
             /* Check at-sequence for end-of-scrap */
             
#line 672 "literate/latex-output.weft"
             /* {114: literate/latex-output.weft:672} */
{
               c = source_get();
               switch (c) {
                 case 'c': 
#line 390 "literate/parser.weft"
                           /* {63: literate/parser.weft:390} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape (Comment)}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
                           /* {:63} */

#line 675 "literate/latex-output.weft"

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
#line 677 "literate/latex-output.weft"
                                                      /* {115: literate/latex-output.weft:677} */
file/* {:115} */
);
                           }/* {:117} */

#line 677 "literate/latex-output.weft"

                           break;
                 case 'v': 
#line 727 "literate/latex-output.weft"
                           /* {116: literate/latex-output.weft:727} */
fputs(version_string, file);
                           /* {:116} */

#line 679 "literate/latex-output.weft"

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

#line 685 "literate/latex-output.weft"

                 case ',':
                 case ')':
                 case ']':
                 case '}': fputs(delimit_scrap[scrap_type][1], file);
                           return;
                 case '<': 
#line 780 "literate/latex-output.weft"
                           /* {123: literate/latex-output.weft:780} */
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
                               
#line 640 "literate/parser.weft"
/* {75: literate/parser.weft:640} */

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
                               /* {:75} */

#line 808 "literate/latex-output.weft"

                             }
                             fprintf(file, "{\\footnotesize ");
                             if (name->defs)
                               /* Write abbreviated definition list */
                               
#line 826 "literate/latex-output.weft"
                               /* {124: literate/latex-output.weft:826} */
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
                               }/* {:124} */

#line 812 "literate/latex-output.weft"

                             else {
                               putc('?', file);
                               fprintf(stderr, "%s: never defined <%s>\n",
                                       command_name, name->spelling);
                             }
                             fputs("}$\\,\\rangle$", file);
                             if (prefix)
                                fputs("}", file);
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:123} */

#line 691 "literate/latex-output.weft"

                           break;
                 case '%': 
#line 750 "literate/latex-output.weft"
                           /* {120: literate/latex-output.weft:750} */
{
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }/* {:120} */

#line 693 "literate/latex-output.weft"

                           break;
                 case '_': 
#line 759 "literate/latex-output.weft"
                           /* {121: literate/latex-output.weft:759} */
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
                           }/* {:121} */

#line 695 "literate/latex-output.weft"

                           break;
                 case 't': 
#line 773 "literate/latex-output.weft"
                           /* {122: literate/latex-output.weft:773} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape fragment title}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:122} */

#line 697 "literate/latex-output.weft"

                           break;
                 case 'f': 
#line 773 "literate/latex-output.weft"
                           /* {122: literate/latex-output.weft:773} */
{
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape file name}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }/* {:122} */

#line 699 "literate/latex-output.weft"

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
             }/* {:114} */

#line 612 "literate/latex-output.weft"

             break;
           }
         putc(c, file);
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:110} */

#line 628 "literate/latex-output.weft"
/* {111: literate/latex-output.weft:628} */
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
/* {:111} */

#line 839 "literate/latex-output.weft"
/* {125: literate/latex-output.weft:839} */
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
      
#line 640 "literate/parser.weft"
/* {75: literate/parser.weft:640} */

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
      /* {:75} */

#line 879 "literate/latex-output.weft"

    }
    fprintf(file, "{\\footnotesize ");
    if (name->defs)
      /* Write abbreviated definition list */
      
#line 826 "literate/latex-output.weft"
      /* {124: literate/latex-output.weft:826} */
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
      }/* {:124} */

#line 883 "literate/latex-output.weft"

    else {
      putc('?', file);
      fprintf(stderr, "%s: never defined <%s>\n",
              command_name, name->spelling);
    }
    fputs("}$\\,\\rangle$", file);
  }
}
/* {:125} */

#line 909 "literate/latex-output.weft"
/* {127: literate/latex-output.weft:909} */
static void format_file_entry(Name *name, FILE *tex_file)
{
  while (name) {
    format_file_entry(name->llink, tex_file);
    /* Format a file index entry */
    
#line 920 "literate/latex-output.weft"
    /* {128: literate/latex-output.weft:920} */
fputs("\\item ", tex_file);
    fprintf(tex_file, "\\verb%c\"%s\"%c ", nw_char, name->spelling, nw_char);
    /* Write file's defining scrap numbers */

#line 926 "literate/latex-output.weft"
    /* {129: literate/latex-output.weft:926} */
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
    }/* {:129} */

#line 922 "literate/latex-output.weft"

    putc('\n', tex_file);/* {:128} */

#line 913 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:127} */

#line 968 "literate/latex-output.weft"
/* {131: literate/latex-output.weft:968} */
static int load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:131} */

#line 980 "literate/latex-output.weft"
/* {132: literate/latex-output.weft:980} */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = load_entry(name, nms, 0);
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

#line 986 "literate/latex-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format an index entry */
     
#line 1020 "literate/latex-output.weft"
     /* {135: literate/latex-output.weft:1020} */
if (name->sector == sector){
       fputs("\\item ", tex_file);
       fputs("$\\langle\\,$", tex_file);
       /* Write the macro's name */
       
#line 288 "literate/latex-output.weft"
       /* {88: literate/latex-output.weft:288} */
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
       }/* {:88} */

#line 1023 "literate/latex-output.weft"

       fputs("\\nobreak\\ {\\footnotesize ", tex_file);
       /* Write defining scrap numbers */
       
#line 1032 "literate/latex-output.weft"
       /* {136: literate/latex-output.weft:1032} */
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
       }/* {:136} */

#line 1025 "literate/latex-output.weft"

       fputs("}$\\,\\rangle$ ", tex_file);
       /* Write referencing scrap numbers */
       
#line 1056 "literate/latex-output.weft"
       /* {137: literate/latex-output.weft:1056} */
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
       }/* {:137} */

#line 1027 "literate/latex-output.weft"

       putc('\n', tex_file);
     }/* {:135} */

#line 991 "literate/latex-output.weft"

  }
}
/* {:132} */

#line 1086 "literate/latex-output.weft"
/* {139: literate/latex-output.weft:1086} */
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
/* {:139} */

#line 1118 "literate/latex-output.weft"
/* {141: literate/latex-output.weft:1118} */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  while (name) {
    format_user_entry(name->llink, tex_file, sector);
    /* Format a user index entry */
    
#line 1130 "literate/latex-output.weft"
    /* {142: literate/latex-output.weft:1130} */
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
    }/* {:142} */

#line 1122 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:141} */
