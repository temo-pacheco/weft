
#line 121 "literate/architecture.weft"
/* {7: literate/architecture.weft:121} */
#include "global.h"
static int scraps = 1;
/* {:7} */

#line 22 "literate/latex-output.weft"
/* {84: literate/latex-output.weft:22} */
static void copy_scrap(FILE *file, int prefix, Name *name);             /* formats the body of a scrap */
static void latex_code_putc(FILE *file, int c);                          /* escapes one code character for LaTeX */
static void print_scrap_numbers(FILE *tex_file, Scrap_Node *scraps);      /* formats a list of scrap numbers */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector);             /* formats an index entry */
static void format_file_entry(Name *name, FILE *tex_file);        /* formats a file index entry */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector);
static void write_arg(FILE *tex_file, char *p);
static void write_literal(FILE *tex_file, char *p, int mode);
static void write_ArglistElement(FILE *file, Arglist *args, char **params);
/* {:84} */

#line 37 "literate/latex-output.weft"
/* {85: literate/latex-output.weft:37} */
void write_tex(char *file_name, char *tex_name, unsigned char sector)
{
  (void)sector;
  FILE *tex_file = fopen(tex_name, "w");
  if (tex_file) {
    if (verbose_flag)
      fprintf(stderr, "writing %s\n", tex_name);
    source_open(file_name);
    /* Write LaTeX limbo definitions */
    
#line 73 "literate/latex-output.weft"
    /* {86: literate/latex-output.weft:73} */
if (hyperref_flag) {
       fputs("\\newcommand{\\WEFTtarget}[2]{\\hypertarget{#1}{#2}}\n", tex_file);
       fputs("\\newcommand{\\WEFTlink}[2]{\\hyperlink{#1}{#2}}\n", tex_file);
    } else {
       fputs("\\newcommand{\\WEFTtarget}[2]{#2}\n", tex_file);
       fputs("\\newcommand{\\WEFTlink}[2]{#2}\n", tex_file);
    }
    fputs("\\newcommand{\\WEFTtxtMacroDefBy}{Fragment defined by}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtMacroRefIn}{Fragment referenced in}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtMacroNoRef}{Fragment never referenced}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtDefBy}{Defined by}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtRefIn}{Referenced in}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtNoRef}{Not referenced}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtFileDefBy}{File defined by}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtIdentsUsed}{Uses:}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtIdentsNotUsed}{Never used}\n", tex_file);
    fputs("\\newcommand{\\WEFTtxtIdentsDefed}{Defines:}\n", tex_file);
    fputs("\\newcommand{\\WEFTsep}{${\\diamond}$}\n", tex_file);
    fputs("\\newcommand{\\WEFTnotglobal}{(not defined globally)}\n", tex_file);
    /* Write the scrap formatting macros */

#line 116 "literate/latex-output.weft"
    /* {87: literate/latex-output.weft:116} */
    fputs("\\newcommand{\\WEFTbreakpenalty}{500}\n", tex_file);
    fputs("\\newcommand{\\WEFTsp}{\\hskip\\fontdimen2\\font\\relax}\n", tex_file);
    fputs("\\newcommand{\\WEFTbrk}{\\discretionary{}{}{}}\n", tex_file);
    fputs("\\newlength{\\WEFTindent}\\setlength{\\WEFTindent}{1.5em}\n", tex_file);
    fputs("\\newlength{\\WEFThang}\\setlength{\\WEFThang}{2em}\n", tex_file);
    fputs("\\newcommand{\\WEFTbegin}{\\par\\addvspace{2.3ex plus .6ex}"
          "\\begingroup\\small\\raggedright}\n", tex_file);
    fputs("\\newcommand{\\WEFTend}{\\par\\endgroup"
          "\\addvspace{2.3ex plus .6ex}}\n", tex_file);
    fputs("\\newcommand{\\WEFTcode}{\\par\\nobreak\\vspace{-.5ex}\\begingroup"
          "\\ttfamily\\small\\parindent0pt\\parskip0pt\\raggedright"
          "\\leftskip\\WEFTindent\\hangindent\\WEFThang\\hangafter1\\relax"
          "\\everypar{\\hangindent\\WEFThang\\hangafter1\\relax}}\n", tex_file);
    fputs("\\newcommand{\\WEFTendcode}{\\par\\endgroup}\n", tex_file);
    fputs("\\newcommand{\\WEFTeol}{\\par\\penalty\\WEFTbreakpenalty\\relax}\n",
          tex_file);
    /* {:87} */

#line 92 "literate/latex-output.weft"

    fputs("\\newcommand{\\WEFTuseHyperlinks}{", tex_file);
    if (hyperoptions[0] != '\0')
    {
       /* Write the hyperlink usage macro */
       
#line 135 "literate/latex-output.weft"
       /* {88: literate/latex-output.weft:135} */
fprintf(tex_file, "\\usepackage[%s]{hyperref}", hyperoptions);/* {:88} */

#line 96 "literate/latex-output.weft"

    }
    fputs("}\n", tex_file);
    /* {:86} */

#line 45 "literate/latex-output.weft"

    /* Copy \verb|source_file| into \verb|tex_file| */
    
#line 142 "literate/latex-output.weft"
    /* {89: literate/latex-output.weft:142} */
{
      int inBlock = FALSE;
      int c = source_get();
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret at-sequence */
          
#line 158 "literate/latex-output.weft"
          /* {90: literate/latex-output.weft:158} */
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
                          {
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 fputs("\\WEFTend\n", tex_file);
                                 inBlock = FALSE;
                              }
                              fputs("\\WEFTbegin", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);
                              }
                              else
                              {
                                 fputs("\\WEFTbegin\n", tex_file);
                                 inBlock = TRUE;
                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\n", scraps);
                          }
                          fputs("\\WEFTtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} ", tex_file);
                          fprintf(tex_file, "\\verb%c\"%s\"%c\\nobreak\\ {\\footnotesize {", nw_char, name->spelling, nw_char);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\equiv$\n", tex_file);
                          {
                            fputs("\\WEFTcode\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\WEFTsep}\\WEFTendcode\n", tex_file);
                          }
                          {
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          
                          if ( scrap_flag ) {
                            {
                              if (name->defs) {
                                if (name->defs->next) {
                                  fputs("\\item \\WEFTtxtFileDefBy\\ ", tex_file);
                                  print_scrap_numbers(tex_file, name->defs);
                                }
                              } else {
                                fprintf(stderr,
                                        "would have crashed in 'Write file defs' for '%s'\n",
                                         name->spelling);
                              }
                            }
                          }
                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          {
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }
                          {
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\WEFTend\n", tex_file);
                            else
                            {
                               fputs("\\WEFTend\n", tex_file);
                               inBlock = FALSE;
                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }
                        }
#line 169 "literate/latex-output.weft"

                        break;
              case 'Q':
              case 'D': big_definition = TRUE;
                        FALLTHROUGH;
              case 'q':
              case 'd': {
                          Name *name = collect_macro_name();

                          {
                            if (big_definition)
                            {
                              if (inBlock)
                              {
                                 fputs("\\WEFTend\n", tex_file);
                                 inBlock = FALSE;
                              }
                              fputs("\\WEFTbegin", tex_file);
                            }
                            else
                            {
                              if (inBlock)
                              {
                                 fputs("\\par\\vspace{\\baselineskip}\n",  tex_file);
                              }
                              else
                              {
                                 fputs("\\WEFTbegin\n", tex_file);
                                 inBlock = TRUE;
                              }
                            }
                            fprintf(tex_file, "\\label{scrap%d}\n", scraps);
                          }
                          fputs("\\WEFTtarget{weft", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}{} $\\langle\\,${\\itshape ", tex_file);
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
                          }
                          fputs("}\\nobreak\\ {\\footnotesize {", tex_file);
                          write_single_scrap_ref(tex_file, scraps);
                          fputs("}}$\\,\\rangle\\equiv$\n", tex_file);
                          {
                            fputs("\\WEFTcode\n", tex_file);
                            extra_scraps = 0;
                            copy_scrap(tex_file, TRUE, name);
                            fputs("{\\WEFTsep}\\WEFTendcode\n", tex_file);
                          }
                          {
                            fputs("\\vspace{-1.5ex}\n", tex_file);
                            fputs("\\footnotesize\n", tex_file);
                            fputs("\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                              tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);}
                          
                          {
                            if (name->defs->next) {
                              fputs("\\item \\WEFTtxtMacroDefBy\\ ", tex_file);
                              print_scrap_numbers(tex_file, name->defs);
                            }
                          }
                          {
                            if (name->uses) {
                              if (name->uses->next) {
                                fputs("\\item \\WEFTtxtMacroRefIn\\ ", tex_file);
                                print_scrap_numbers(tex_file, name->uses);
                              }
                              else {
                                fputs("\\item \\WEFTtxtMacroRefIn\\ ", tex_file);
                                fputs("\\WEFTlink{weft", tex_file);
                                write_single_scrap_ref(tex_file, name->uses->scrap);
                                fputs("}{", tex_file);
                                write_single_scrap_ref(tex_file, name->uses->scrap);
                                fputs("}", tex_file);
                                fputs(".\n", tex_file);
                              }
                            }
                            else {
                              fputs("\\item {\\WEFTtxtMacroNoRef}.\n", tex_file);
                              fprintf(stderr, "%s: <%s> never referenced.\n",
                                      command_name, name->spelling);
                            }
                          }
                          format_defs_refs(tex_file, scraps);
                          format_uses_refs(tex_file, scraps++);
                          {
                            fputs("\n\\item{}", tex_file);
                            fputs("\n\\end{list}\n", tex_file);
                          }
                          {
                            scraps += extra_scraps;
                            if (big_definition)
                              fputs("\\WEFTend\n", tex_file);
                            else
                            {
                               fputs("\\WEFTend\n", tex_file);
                               inBlock = FALSE;
                            }
                            do
                              c = source_get();
                            while (isspace(c));
                          }
                        }
#line 175 "literate/latex-output.weft"

                        break;
              case 's':
                        /* Step to next sector */
                        
#line 202 "literate/parser.weft"
/* {52: literate/parser.weft:202} */

                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                        /* {:52} */

#line 178 "literate/latex-output.weft"

                        break;
              case 'S':
                        /* Close the current sector */
                        
#line 209 "literate/parser.weft"
                        /* {53: literate/parser.weft:209} */
current_sector = 1;
                        c = source_get();
                        /* {:53} */

#line 181 "literate/latex-output.weft"

                        break;
              case '{':
              case '[':
              case '(': copy_scrap(tex_file, FALSE, NULL);
                        c = source_get();
                        
#line 185 "literate/latex-output.weft"

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
                           write_scraps(tex_file, tex_name, name->defs, 0, indent_chars, 0, 0, 1, 0, 0,
                                 args, name->arg, local_parameters, tex_name);
                           name->mark = FALSE;
                           c = source_get();
                        }
                        
#line 187 "literate/latex-output.weft"

                        break;
              case 'x': {
                           char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = source_get(), c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = source_get();
                           
                           write_label(label_name, tex_file);
                        }
#line 189 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'c': if (inBlock)
                        {
                           fputs("\\WEFTend\n", tex_file);
                           inBlock = FALSE;
                        }
                        else
                        {
                           fputs("\\WEFTbegin\n", tex_file);
                           inBlock = TRUE;
                        }
#line 192 "literate/latex-output.weft"

                        c = source_get();
                        break;
              case 'f': {
                          if (file_names) {
                            fputs("\n{\\small\\begin{list}{}{\\setlength{\\itemsep}{-\\parsep}",
                                  tex_file);
                            fputs("\\setlength{\\itemindent}{-\\leftmargin}}\n", tex_file);
                            format_file_entry(file_names, tex_file);
                            fputs("\\end{list}}", tex_file);
                          }
                          c = source_get();
                        }
#line 195 "literate/latex-output.weft"

                        break;
              case 'm': {
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
                        
#line 197 "literate/latex-output.weft"

                        break;
              case 'u': {
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
                        }
#line 199 "literate/latex-output.weft"

                        break;
              case 'v': fputs(version_string, tex_file);
                        c = source_get();
                        
#line 201 "literate/latex-output.weft"

                        break;
              case 'l':
              case 'L': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();
#line 204 "literate/latex-output.weft"

                        break;
              case 'W': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get();
#line 206 "literate/latex-output.weft"

                        break;
              default:
                    if (c==nw_char)
                      putc(c, tex_file);
                    c = source_get();
                        break;
            }
          }/* {:90} */

#line 148 "literate/latex-output.weft"

          }
        else {
          putc(c, tex_file);
          c = source_get();
        }
      }
    }/* {:89} */

#line 46 "literate/latex-output.weft"

    fclose(tex_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, tex_name);
}
/* {:85} */

#line 346 "literate/latex-output.weft"
/* {97: literate/latex-output.weft:346} */
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
/* {:97} */

#line 528 "literate/latex-output.weft"
/* {112: literate/latex-output.weft:528} */
static void print_scrap_numbers(FILE *tex_file, Scrap_Node *scraps)
{
  int page;
  fputs("\\WEFTlink{weft", tex_file);
  write_scrap_ref(tex_file, scraps->scrap, -1, &page);
  fputs("}{", tex_file);
  write_scrap_ref(tex_file, scraps->scrap, TRUE, &page);
  fputs("}", tex_file);
  scraps = scraps->next;
  while (scraps) {
    fputs("\\WEFTlink{weft", tex_file);
    write_scrap_ref(tex_file, scraps->scrap, -1, &page);
    fputs("}{", tex_file);
    write_scrap_ref(tex_file, scraps->scrap, FALSE, &page);
    scraps = scraps->next;
    fputs("}", tex_file);
  }
  fputs(".\n", tex_file);
}
/* {:112} */

#line 571 "literate/latex-output.weft"
/* {113: literate/latex-output.weft:571} */
static char *orig_delimit_scrap[3][5] = {
  /* {} mode (code): begin, end, insert nw_char, prefix, suffix */
  { "", "", "@", "\\mbox{\\strut}", "\\WEFTeol" },
  /* [] mode: begin, end, insert nw_char, prefix, suffix */
  { "", "", "@", "", "" },
  /* () mode: begin, end, insert nw_char, prefix, suffix */
  { "$", "$", "@", "", "" },
};

static char *delimit_scrap[3][5];
/* {:113} */

#line 587 "literate/latex-output.weft"
/* {114: literate/latex-output.weft:587} */
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
}
/* {:114} */

#line 610 "literate/latex-output.weft"
/* {115: literate/latex-output.weft:610} */
static void latex_code_putc(FILE *file, int c)
{
  switch (c) {
    case ' ':  fputs("\\WEFTsp ", file);          break;
    case '\\': fputs("\\textbackslash{}", file);  break;
    case '{':  fputs("\\{", file);                break;
    case '}':  fputs("\\}", file);                break;
    case '$':  fputs("\\$", file);                break;
    case '&':  fputs("\\&", file);                break;
    case '#':  fputs("\\#", file);                break;
    case '%':  fputs("\\%", file);                break;
    case '^':  fputs("\\textasciicircum{}", file);break;
    case '~':  fputs("\\textasciitilde{}", file); break;
    case '_':  fputs("\\_\\WEFTbrk ", file);      break;
    /* escape-free punctuation that is a good place to break a long line */
    case ',': case ';': case ':': case '.':
    case '(': case ')': case '[': case ']':
    case '/': case '|': case '=': case '<': case '>':
    case '+': case '-':
      putc(c, file); fputs("\\WEFTbrk ", file);   break;
    default:
      putc(c, file);                              break;
  }
}
/* {:115} */

#line 641 "literate/latex-output.weft"
/* {117: literate/latex-output.weft:641} */
int scrap_type = 0;
/* {:117} */

#line 645 "literate/latex-output.weft"
/* {118: literate/latex-output.weft:645} */
static void write_literal(FILE * tex_file, char * p, int mode)
{
   fputs(delimit_scrap[mode][0], tex_file);
   while (*p!= '\000') {
     if (*p == nw_char)
       fputs(delimit_scrap[mode][2], tex_file);
     else if (mode == 0)
       latex_code_putc(tex_file, (unsigned char)*p);
     else
       fputc(*p, tex_file);
     p++;
   }
   fputs(delimit_scrap[mode][1], tex_file);
}
/* {:118} */

#line 662 "literate/latex-output.weft"
/* {119: literate/latex-output.weft:662} */
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
      case '\t': {
                   int delta = 8 - (indent % 8);
                   indent += delta;
                   while (delta > 0) {
                     if (scrap_type == 0)
                       latex_code_putc(file, ' ');
                     else
                       putc(' ', file);
                     delta--;
                   }
                 }
#line 682 "literate/latex-output.weft"

                 break;
      default:
         if (c==nw_char)
           {
             /* Check at-sequence for end-of-scrap */
             
#line 750 "literate/latex-output.weft"
             /* {124: literate/latex-output.weft:750} */
{
               c = source_get();
               switch (c) {
                 case 'c': {
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape (Comment)}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
                           
#line 753 "literate/latex-output.weft"

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
#line 755 "literate/latex-output.weft"

                           break;
                 case 'v': fputs(version_string, file);
                           
#line 757 "literate/latex-output.weft"

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
#line 763 "literate/latex-output.weft"

                           FALLTHROUGH;
                 case ',':
                 case ')':
                 case ']':
                 case '}': fputs(delimit_scrap[scrap_type][1], file);
                           return;
                 case '<': {
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
                               
                             }
                             fprintf(file, "{\\footnotesize ");
                             if (name->defs)
                               {
                                 Scrap_Node *p = name->defs;
                                 fputs("\\WEFTlink{weft", file);
                                 write_single_scrap_ref(file, p->scrap);
                                 fputs("}{", file);
                                 write_single_scrap_ref(file, p->scrap);
                                 fputs("}", file);
                                 p = p->next;
                                 if (p)
                                   fputs(", \\ldots\\ ", file);
                               }
                             else {
                               putc('?', file);
                               fprintf(stderr, "%s: never defined <%s>\n",
                                       command_name, name->spelling);
                             }
                             fputs("}$\\,\\rangle$", file);
                             if (prefix)
                                fputs("}", file);
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
#line 770 "literate/latex-output.weft"

                           break;
                 case '%': {
                                   do
                                           c = source_get();
                                   while (c != '\n');
                           }
#line 772 "literate/latex-output.weft"

                           break;
                 case '_': {
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
                           }
#line 774 "literate/latex-output.weft"

                           break;
                 case 't': {
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape fragment title}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
#line 776 "literate/latex-output.weft"

                           break;
                 case 'f': {
                             fputs(delimit_scrap[scrap_type][1],file);
                             fprintf(file, "\\hbox{\\sffamily\\slshape file name}");
                             fputs(delimit_scrap[scrap_type][0], file);
                           }
#line 778 "literate/latex-output.weft"

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
             }/* {:124} */

#line 687 "literate/latex-output.weft"

             break;
           }
         if (scrap_type == 0)
           latex_code_putc(file, c);
         else
           putc(c, file);
                 indent++;
                 break;
    }
    c = source_get();
  }
}
/* {:119} */

#line 706 "literate/latex-output.weft"
/* {120: literate/latex-output.weft:706} */
void update_delimit_scrap(void)
{
  /* Every mode's "insert nw_char" string is the single character that
     stands in for the weft character inside a scrap; keep them in sync
     with the (possibly changed) weft character. */
  delimit_scrap[0][2][0] = nw_char;
  delimit_scrap[1][2][0] = nw_char;
  delimit_scrap[2][2][0] = nw_char;
}
/* {:120} */

#line 918 "literate/latex-output.weft"
/* {135: literate/latex-output.weft:918} */
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
      
#line 684 "literate/parser.weft"
/* {82: literate/parser.weft:684} */

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
      /* {:82} */

#line 958 "literate/latex-output.weft"

    }
    fprintf(file, "{\\footnotesize ");
    if (name->defs)
      /* Write abbreviated definition list */
      
#line 905 "literate/latex-output.weft"
      /* {134: literate/latex-output.weft:905} */
{
        Scrap_Node *p = name->defs;
        fputs("\\WEFTlink{weft", file);
        write_single_scrap_ref(file, p->scrap);
        fputs("}{", file);
        write_single_scrap_ref(file, p->scrap);
        fputs("}", file);
        p = p->next;
        if (p)
          fputs(", \\ldots\\ ", file);
      }/* {:134} */

#line 962 "literate/latex-output.weft"

    else {
      putc('?', file);
      fprintf(stderr, "%s: never defined <%s>\n",
              command_name, name->spelling);
    }
    fputs("}$\\,\\rangle$", file);
  }
}
/* {:135} */

#line 988 "literate/latex-output.weft"
/* {137: literate/latex-output.weft:988} */
static void format_file_entry(Name *name, FILE *tex_file)
{
  while (name) {
    format_file_entry(name->llink, tex_file);
    /* Format a file index entry */
    
#line 999 "literate/latex-output.weft"
    /* {138: literate/latex-output.weft:999} */
fputs("\\item ", tex_file);
    fprintf(tex_file, "\\verb%c\"%s\"%c ", nw_char, name->spelling, nw_char);
    /* Write file's defining scrap numbers */

#line 1005 "literate/latex-output.weft"
    /* {139: literate/latex-output.weft:1005} */
    {
      Scrap_Node *p = name->defs;
      fputs("{\\footnotesize {\\WEFTtxtDefBy}", tex_file);
      if (p->next) {
        /* fputs("s ", tex_file); */
          putc(' ', tex_file);
        print_scrap_numbers(tex_file, p);
      }
      else {
        putc(' ', tex_file);
        fputs("\\WEFTlink{weft", tex_file);
        write_single_scrap_ref(tex_file, p->scrap);
        fputs("}{", tex_file);
        write_single_scrap_ref(tex_file, p->scrap);
        fputs("}", tex_file);
        putc('.', tex_file);
      }
      putc('}', tex_file);
    }/* {:139} */

#line 1001 "literate/latex-output.weft"

    putc('\n', tex_file);/* {:138} */

#line 992 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:137} */

#line 1047 "literate/latex-output.weft"
/* {141: literate/latex-output.weft:1047} */
static int load_entry(Name * name, Name ** nms, int n)
{
   while (name) {
      n = load_entry(name->llink, nms, n);
      nms[n++] = name;
      name = name->rlink;
   }
   return n;
}
/* {:141} */

#line 1059 "literate/latex-output.weft"
/* {142: literate/latex-output.weft:1059} */
static void format_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  Name ** nms = malloc(num_scraps()*sizeof(Name *));
  int n = load_entry(name, nms, 0);
  int i;

  /* Sort 'nms' of size 'n' for <Rob's ordering> */
  
#line 1079 "literate/latex-output.weft"
  /* {144: literate/latex-output.weft:1079} */
int j;
  for (j = 1; j < n; j++)
  {
     int i = j - 1;
     Name * kj = nms[j];

     do
     {
        Name * ki = nms[i];

        if (
#line 1076 "literate/latex-output.weft"
            /* {143: literate/latex-output.weft:1076} */
robs_strcmp(ki->spelling, kj->spelling) < 0/* {:143} */
)
           break;
        nms[i + 1] = ki;
        i -= 1;
     } while (i >= 0);
     nms[i + 1] = kj;
  }
  /* {:144} */

#line 1065 "literate/latex-output.weft"

  for (i = 0; i < n; i++)
  {
     Name * name = nms[i];

     /* Format an index entry */
     
#line 1099 "literate/latex-output.weft"
     /* {145: literate/latex-output.weft:1099} */
if (name->sector == sector){
       fputs("\\item ", tex_file);
       fputs("$\\langle\\,$", tex_file);
       /* Write the macro's name */
       
#line 331 "literate/latex-output.weft"
       /* {96: literate/latex-output.weft:331} */
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
       }/* {:96} */

#line 1102 "literate/latex-output.weft"

       fputs("\\nobreak\\ {\\footnotesize ", tex_file);
       /* Write defining scrap numbers */
       
#line 1111 "literate/latex-output.weft"
       /* {146: literate/latex-output.weft:1111} */
{
         Scrap_Node *p = name->defs;
         if (p) {
           int page;
           fputs("\\WEFTlink{weft", tex_file);
           write_scrap_ref(tex_file, p->scrap, -1, &page);
           fputs("}{", tex_file);
           write_scrap_ref(tex_file, p->scrap, TRUE, &page);
           fputs("}", tex_file);
           p = p->next;
           while (p) {
             fputs("\\WEFTlink{weft", tex_file);
             write_scrap_ref(tex_file, p->scrap, -1, &page);
             fputs("}{", tex_file);
             write_scrap_ref(tex_file, p->scrap, FALSE, &page);
             fputs("}", tex_file);
             p = p->next;
           }
         }
         else
           putc('?', tex_file);
       }/* {:146} */

#line 1104 "literate/latex-output.weft"

       fputs("}$\\,\\rangle$ ", tex_file);
       /* Write referencing scrap numbers */
       
#line 1135 "literate/latex-output.weft"
       /* {147: literate/latex-output.weft:1135} */
{
         Scrap_Node *p = name->uses;
         fputs("{\\footnotesize ", tex_file);
         if (p) {
           fputs("{\\WEFTtxtRefIn}", tex_file);
           if (p->next) {
             /* fputs("s ", tex_file); */
             putc(' ', tex_file);
             print_scrap_numbers(tex_file, p);
           }
           else {
             putc(' ', tex_file);
             fputs("\\WEFTlink{weft", tex_file);
             write_single_scrap_ref(tex_file, p->scrap);
             fputs("}{", tex_file);
             write_single_scrap_ref(tex_file, p->scrap);
             fputs("}", tex_file);
             putc('.', tex_file);
           }
         }
         else
           fputs("{\\WEFTtxtNoRef}.", tex_file);
         putc('}', tex_file);
       }/* {:147} */

#line 1106 "literate/latex-output.weft"

       putc('\n', tex_file);
     }/* {:145} */

#line 1070 "literate/latex-output.weft"

  }
}
/* {:142} */

#line 1165 "literate/latex-output.weft"
/* {149: literate/latex-output.weft:1165} */
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
/* {:149} */

#line 1197 "literate/latex-output.weft"
/* {151: literate/latex-output.weft:1197} */
static void format_user_entry(Name *name, FILE *tex_file, unsigned char sector)
{
  while (name) {
    format_user_entry(name->llink, tex_file, sector);
    /* Format a user index entry */
    
#line 1209 "literate/latex-output.weft"
    /* {152: literate/latex-output.weft:1209} */
if (name->sector == sector){
      Scrap_Node *uses = name->uses;
      if ( uses || dangling_flag ) {
        int page;
        Scrap_Node *defs = name->defs;
        fprintf(tex_file, "\\item \\verb%c%s%c: ", nw_char,name->spelling,nw_char);
        if (!uses) {
            fputs("(\\underline{", tex_file);
            fputs("\\WEFTlink{weft", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("}{", tex_file);
            write_single_scrap_ref(tex_file, defs->scrap);
            fputs("})}", tex_file);
            page = -2;
            defs = defs->next;
        }
        else
          if (!defs || uses->scrap < defs->scrap) {
          fputs("\\WEFTlink{weft", tex_file);
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

          fputs("\\WEFTlink{weft", tex_file);
          write_single_scrap_ref(tex_file, defs->scrap);
          fputs("}{", tex_file);
          write_single_scrap_ref(tex_file, defs->scrap);
          fputs("}}", tex_file);
          page = -2;
          defs = defs->next;
        }
        while (uses || defs) {
          if (uses && (!defs || uses->scrap < defs->scrap)) {
            fputs("\\WEFTlink{weft", tex_file);
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

            fputs("\\WEFTlink{weft", tex_file);
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
    }/* {:152} */

#line 1201 "literate/latex-output.weft"

    name = name->rlink;
  }
}
/* {:151} */
