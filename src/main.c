
#line 75 "literate/architecture.weft"
/* {5: literate/architecture.weft:75} */
#include "global.h"
/* {:5} */

#line 7 "literate/main.weft"
/* {15: literate/main.weft:7} */

#include <stdlib.h>
int main(int argc, char **argv)
{
  int arg = 1;
  /* Interpret command-line arguments */
  
#line 179 "literate/main.weft"
  /* {23: literate/main.weft:179} */
command_name = argv[0];
  /* {:23} */

#line 185 "literate/main.weft"
  /* {24: literate/main.weft:185} */
while (arg < argc) {
    char *s = argv[arg];
    if (*s++ == '-') {
      /* Interpret the argument string \verb|s| */
      
#line 206 "literate/main.weft"
      /* {25: literate/main.weft:206} */
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
                            "[-e name] [-I path] [-V version] "
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

#line 188 "literate/main.weft"

      arg++;
      /* Perhaps get the prepend path */
      
#line 289 "literate/main.weft"
      /* {26: literate/main.weft:289} */
if (prepend_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        dirpath = s;
        prepend_flag = FALSE;
      }
      /* {:26} */

#line 190 "literate/main.weft"

      /* Perhaps get the version info string */
      
#line 317 "literate/main.weft"
      /* {28: literate/main.weft:317} */
if (version_info_flag)
      {
         if (*s == '\0')
           s = argv[arg++];
         version_string = s;
         version_info_flag = FALSE;
      }
      /* {:28} */

#line 191 "literate/main.weft"

      /* Perhaps get the hyperref options */
      
#line 327 "literate/main.weft"
      /* {29: literate/main.weft:327} */
if (hyperopt_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        hyperoptions = s;
        hyperopt_flag = FALSE;
        hyperref_flag = TRUE;
      }
      /* {:29} */

#line 192 "literate/main.weft"

      /* Perhaps add an include path */
      
#line 299 "literate/main.weft"
      /* {27: literate/main.weft:299} */
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

#line 193 "literate/main.weft"

      /* Perhaps get the extract name */
      
#line 338 "literate/main.weft"
      /* {30: literate/main.weft:338} */
if (extract_flag) {
        extract_name = (*s != '\0') ? s : argv[arg++];
        extract_flag = FALSE;
      }/* {:30} */

#line 194 "literate/main.weft"

    }
    else break;
  }/* {:24} */

#line 12 "literate/main.weft"

  /* Set locale information */
  
#line 349 "literate/main.weft"
/* {31: literate/main.weft:349} */

  {
    /* try to get locale information */
    char *s=getenv("LC_CTYPE");
    if (s==NULL) s=getenv("LC_ALL");

    /* set it */
    if (s!=NULL)
      if(setlocale(LC_CTYPE, s)==NULL)
        fprintf(stderr, "Setting locale failed\n");
  }
  /* {:31} */

#line 13 "literate/main.weft"

  initialise_delimit_scrap_array();
  /* Process the remaining arguments (file names) */
  
#line 368 "literate/main.weft"
  /* {32: literate/main.weft:368} */
{
    if (arg >= argc) {
      fprintf(stderr, "%s: expected a file name.  ", command_name);
      fprintf(stderr, "Usage is: %s [-cdmnostvx] [-e name] [-w [format]] [-p path] file-name...\n", command_name);
      exit(-1);
    }
    do {
      /* Handle the file name in \verb|argv[arg]| */
      
#line 390 "literate/main.weft"
      /* {33: literate/main.weft:390} */
{
        char source_name[FILENAME_MAX];
        char tex_name[FILENAME_MAX];
        char aux_name[FILENAME_MAX];
        char *trim;
        char *dot;
        /* Build \verb|source_name| and \verb|tex_name| */
        
#line 414 "literate/main.weft"
        /* {34: literate/main.weft:414} */
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
          
#line 460 "literate/main.weft"
          /* {35: literate/main.weft:460} */
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
          /* {:35} */

#line 431 "literate/main.weft"

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
        }/* {:34} */

#line 396 "literate/main.weft"

        /* Process a file */
        
#line 492 "literate/main.weft"
        /* {36: literate/main.weft:492} */
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
        }/* {:36} */

#line 397 "literate/main.weft"

      }/* {:33} */

#line 375 "literate/main.weft"

      arg++;
    } while (arg < argc);
  }/* {:32} */

#line 15 "literate/main.weft"

  exit(0);
}
/* {:15} */
