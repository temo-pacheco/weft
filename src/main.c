
#line 95 "literate/architecture.weft"
/* {5: literate/architecture.weft:95} */
#include "global.h"
/* {:5} */

#line 7 "literate/main.weft"
/* {15: literate/main.weft:7} */

#include <stdlib.h>
int main(int argc, char **argv)
{
  int arg = 1;
  /* Interpret command-line arguments */
  
#line 184 "literate/main.weft"
  /* {23: literate/main.weft:184} */
command_name = argv[0];
  /* {:23} */

#line 190 "literate/main.weft"
  /* {24: literate/main.weft:190} */
while (arg < argc) {
    char *s = argv[arg];
    if (*s++ == '-') {
      /* Interpret the argument string \verb|s| */
      
#line 212 "literate/main.weft"
      /* {25: literate/main.weft:212} */
{
        char c = *s++;
        while (c) {
          switch (c) {
            case 'c': compare_flag = FALSE;
                      break;
            case 'd': dangling_flag = TRUE;
                      break;
            case 'h': hyperopt_flag = TRUE;
                      goto HasValue;
            case 'I': includepath_flag = TRUE;
                      goto HasValue;
            case 'l': listings_flag = TRUE;
                      break;
            case 'n': number_flag = TRUE;
                      break;
            case 'o': output_flag = FALSE;
                      break;
            case 'p': prepend_flag = TRUE;
                      goto HasValue;
            case 'r': hyperref_flag = TRUE;
                      break;
            case 's': scrap_flag = FALSE;
                      break;
            case 't': tex_flag = FALSE;
                      break;
            case 'e': extract_flag = TRUE;
                      goto HasValue;
            case 'R': reverse_map_flag = TRUE;
                      goto HasValue;
            case 'm': map_flag = TRUE;
                      break;
            case 'v': verbose_flag = TRUE;
                      break;
            case 'V': version_info_flag = TRUE;
                      goto HasValue;
            case 'w': goto HasWeaveFormat;
            case 'x': xref_flag = TRUE;
                      break;
            default:  fprintf(stderr, "%s: unexpected argument ignored.  ",
                              command_name);
                      fprintf(stderr, "Usage is: %s [-cdmnostvx] "
                            "[-e name] [-R file[:line]] [-I path] [-V version] "
                            "[-w [format]] [-h options] [-p path] file...\n",
                              command_name);
                      break;
          }
          c = *s++;
        }
        goto HasValue;
HasWeaveFormat:
        {
          char *fmt = NULL;
          weave_flag = TRUE;
          if (*s != '\0')
            fmt = s;
          else if (arg + 1 < argc && argv[arg + 1][0] != '-') {
            /* Peek: only consume next arg if it looks like a format token */
            char *candidate = argv[arg + 1];
            if (strcmp(candidate, "md") == 0 || strcmp(candidate, "tex") == 0) {
              fmt = candidate;
              ++arg;
            }
          }
          if (fmt != NULL) {
            if (strcmp(fmt, "md") == 0) {
              weave_format = 2;
              markdown_flag = TRUE;
              weave_format_from_cli = TRUE;
            } else if (strcmp(fmt, "tex") == 0) {
              weave_format = 1;
              weave_format_from_cli = TRUE;
            } else {
              fprintf(stderr, "%s: unknown weave format '%s'. Use 'md' or 'tex'.\n",
                      command_name, fmt);
            }
          }
          /* If fmt is NULL, -w alone: format will come from @W */
          s = "";
        }
HasValue:;
      }/* {:25} */

#line 193 "literate/main.weft"

      arg++;
      /* Perhaps get the prepend path */
      
#line 297 "literate/main.weft"
      /* {26: literate/main.weft:297} */
if (prepend_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        dirpath = s;
        prepend_flag = FALSE;
      }
      /* {:26} */

#line 195 "literate/main.weft"

      /* Perhaps get the version info string */
      
#line 325 "literate/main.weft"
      /* {28: literate/main.weft:325} */
if (version_info_flag)
      {
         if (*s == '\0')
           s = argv[arg++];
         version_string = s;
         version_info_flag = FALSE;
      }
      /* {:28} */

#line 196 "literate/main.weft"

      /* Perhaps get the hyperref options */
      
#line 335 "literate/main.weft"
      /* {29: literate/main.weft:335} */
if (hyperopt_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        hyperoptions = s;
        hyperopt_flag = FALSE;
        hyperref_flag = TRUE;
      }
      /* {:29} */

#line 197 "literate/main.weft"

      /* Perhaps add an include path */
      
#line 307 "literate/main.weft"
      /* {27: literate/main.weft:307} */
if (includepath_flag)
      {
         struct incl * le
            = (struct incl *)arena_getmem(sizeof(struct incl));
         struct incl ** p = &include_list;

         if (*s == '\0')
           s = argv[arg++];
         le->name = save_string(s);
         le->next = NULL;
         while (*p != NULL)
            p = &((*p)->next);
         *p = le;
         includepath_flag = FALSE;
      }
      /* {:27} */

#line 198 "literate/main.weft"

      /* Perhaps get the extract name */
      
#line 346 "literate/main.weft"
      /* {30: literate/main.weft:346} */
if (extract_flag) {
        extract_name = (*s != '\0') ? s : argv[arg++];
        extract_flag = FALSE;
      }/* {:30} */

#line 199 "literate/main.weft"

      /* Perhaps get the reverse map argument */
      
#line 352 "literate/main.weft"
      /* {31: literate/main.weft:352} */
if (reverse_map_flag) {
        reverse_map_arg = (*s != '\0') ? s : argv[arg++];
        reverse_map_flag = FALSE;
      }/* {:31} */

#line 200 "literate/main.weft"

    }
    else break;
  }/* {:24} */

#line 12 "literate/main.weft"

  /* Set locale information */
  
#line 372 "literate/main.weft"
/* {33: literate/main.weft:372} */

  {
    /* try to get locale information */
    char *s=getenv("LC_CTYPE");
    if (s==NULL) s=getenv("LC_ALL");

    /* set it */
    if (s!=NULL)
      if(setlocale(LC_CTYPE, s)==NULL)
        fprintf(stderr, "Setting locale failed\n");
  }
  /* {:33} */

#line 13 "literate/main.weft"

  /* Handle reverse map mode */
  
#line 361 "literate/main.weft"
  /* {32: literate/main.weft:361} */
if (reverse_map_arg) {
    write_reverse_map(reverse_map_arg);
    exit(0);
  }/* {:32} */

#line 14 "literate/main.weft"

  initialise_delimit_scrap_array();
  /* Process the remaining arguments (file names) */
  
#line 391 "literate/main.weft"
  /* {34: literate/main.weft:391} */
{
    if (arg >= argc) {
      fprintf(stderr, "%s: expected a file name.  ", command_name);
      fprintf(stderr, "Usage is: %s [-cdmnostvx] [-e name] [-R file[:line]] [-w [format]] [-p path] file-name...\n", command_name);
      exit(-1);
    }
    do {
      /* Handle the file name in \verb|argv[arg]| */
      
#line 413 "literate/main.weft"
      /* {35: literate/main.weft:413} */
{
        char source_name[FILENAME_MAX];
        char tex_name[FILENAME_MAX];
        char aux_name[FILENAME_MAX];
        char *trim;
        char *dot;
        /* Build \verb|source_name| and \verb|tex_name| */
        
#line 437 "literate/main.weft"
        /* {36: literate/main.weft:437} */
{
          char *p = argv[arg];
          char *q = source_name;
          char c;
          trim = q;
          dot = NULL;
          c = *p++;
          while (c) {
            *q++ = c;
            if (PATH_SEP(c)) {
              trim = q;
              dot = NULL;
            }
            else if (c == '.')
              dot = q - 1;
            c = *p++;
          }
          /* Add the source path to the include path list */
          
#line 483 "literate/main.weft"
          /* {37: literate/main.weft:483} */
if (trim != source_name) {
             struct incl * le
                = (struct incl *)arena_getmem(sizeof(struct incl));
             struct incl ** p = &include_list;
             char sv = *trim;

             *trim = '\0';
             le->name = save_string(source_name);
             le->next = NULL;
             while (*p != NULL)
                p = &((*p)->next);
             *p = le;
             *trim = sv;
          }
          /* {:37} */

#line 454 "literate/main.weft"

          *q = '\0';
          if (!dot) {
            *q++ = '.';
            *q++ = 'w';
            *q++ = 'e';
            *q++ = 'f';
            *q++ = 't';
            *q = '\0';
          }
          /* Find trim again for tex_name computation later */
          {
            char *t = source_name;
            trim = t;
            dot = NULL;
            while (*t) {
              if (PATH_SEP(*t)) { trim = t + 1; dot = NULL; }
              else if (*t == '.') dot = t;
              t++;
            }
          }
        }/* {:36} */

#line 419 "literate/main.weft"

        /* Process a file */
        
#line 515 "literate/main.weft"
        /* {38: literate/main.weft:515} */
{
          int do_weave = FALSE;
          int effective_format = 0;

          /* Reset weave_format for this file unless CLI locked it */
          if (!weave_format_from_cli)
            weave_format = 0;

          pass1(source_name);
          current_sector = 1;
          prev_sector = 1;

          if (map_flag) {
            write_map(file_names, macro_names);
            /* skip weave and tangle */
          } else if (extract_name) {
            write_extract(extract_name, file_names, macro_names);
            /* skip weave and tangle */
          } else {
            /* Determine whether to weave and with which format */
            if (weave_flag && !tex_flag) {
              /* -t suppresses weave: tex_flag==FALSE means user passed -t */
            } else if (weave_flag) {
              effective_format = weave_format;
              if (effective_format == 0) {
                fprintf(stderr, "%s: -w requires a weave format.\n"
                        "  Use -w md, -w tex, or add %cW md/%cW tex to the source file.\n",
                        command_name, nw_char, nw_char);
                exit(-1);
              }
              do_weave = TRUE;
            }

            if (do_weave) {
              /* Compute weave output name now that format is known */
              const char *weave_ext = (effective_format == 2) ? ".md" : ".tex";
              if (dot) {
                *dot = '\0';
                sprintf(tex_name, "%s%s%s%s", dirpath, path_sep, trim, weave_ext);
                sprintf(aux_name, "%s%s%s.aux", dirpath, path_sep, trim);
                *dot = '.';
              } else {
                sprintf(tex_name, "%s%s%s%s", dirpath, path_sep, trim, weave_ext);
                sprintf(aux_name, "%s%s%s.aux", dirpath, path_sep, trim);
              }

              if (effective_format == 2) {
                markdown_flag = TRUE;
                number_flag = TRUE;
                collect_numbers(aux_name);
                write_md(source_name, tex_name, 0);
              } else {
                collect_numbers(aux_name);
                write_tex(source_name, tex_name, 0);
              }
            }

            if (output_flag)
              write_files(file_names);
          }
          arena_free();
        }/* {:38} */

#line 420 "literate/main.weft"

      }/* {:35} */

#line 398 "literate/main.weft"

      arg++;
    } while (arg < argc);
  }/* {:34} */

#line 16 "literate/main.weft"

  exit(0);
}
/* {:15} */
