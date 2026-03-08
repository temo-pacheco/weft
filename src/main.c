
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
      /* Check for long options */
      
#line 217 "literate/main.weft"
      /* {25: literate/main.weft:217} */
if (*s == '-') {
        if (strcmp(s + 1, "help") == 0) {
          /* Print help message */
          
#line 231 "literate/main.weft"
          /* {26: literate/main.weft:231} */
printf("weft -- A Modern Literate Programming Tool\n\n");
          printf("USAGE\n");
          printf("  %s [options] file...\n\n", command_name);
          printf("WHAT IS WEFT?\n");
          printf("  weft is a literate programming tool.  You write .weft files where\n");
          printf("  documentation and code live together as a single narrative.  weft\n");
          printf("  then TANGLES the .weft source into executable output files, and\n");
          printf("  optionally WEAVES it into formatted documentation (LaTeX or Markdown).\n\n");
          printf("  The .weft files are the ONLY source of truth.  The tangled output\n");
          printf("  files (*.c, *.go, *.js, etc.) are GENERATED -- never edit them.\n");
          printf("  All changes must go through the .weft source.\n\n");
          printf("  Default behavior: tangle only (generate output files from @o directives).\n\n");
          printf("HOW IT WORKS\n");
          printf("  A .weft file contains prose with embedded code chunks:\n");
          printf("    @o file.c @{ ... @}    Output file: tangle writes this to file.c\n");
          printf("    @d Name   @{ ... @}    Fragment: reusable code block, invoked by name\n");
          printf("    @<Name@>                Invocation: inserts the named fragment here\n");
          printf("    @@                      Literal @: escapes the meta-character\n");
          printf("    @i other.weft            Include: pulls in another .weft file\n\n");
          printf("  Fragments compose hierarchically.  An @o chunk may invoke @d fragments,\n");
          printf("  which invoke other fragments, forming a tree that weft flattens into\n");
          printf("  the final output file.\n\n");
          printf("OPTIONS\n");
          printf("  Tangle control:\n");
          printf("    -o              Suppress output file generation (parse only)\n");
          printf("    -c              Overwrite files without comparing first\n");
          printf("    -p path         Prepend path to all output file names\n");
          printf("    -I path         Add directory to include search path (repeatable)\n");
          printf("    -x              Include cross-reference numbers in section markers\n\n");
          printf("  Weave control:\n");
          printf("    -w [format]     Weave documentation (format: md or tex)\n");
          printf("                    Without format, uses @W directive from source\n");
          printf("    -t              Suppress weave output (even if -w given)\n");
          printf("    -n              Sequential scrap numbering (auto for Markdown)\n");
          printf("    -s              Omit scrap composition lists\n\n");
          printf("  Analysis (AI-friendly tools):\n");
          printf("    -m              Emit JSON structure map to stdout (all fragments,\n");
          printf("                    files, definitions, and dependency graph)\n");
          printf("    -e name         Extract named fragment and all its transitive\n");
          printf("                    dependencies as self-contained Markdown to stdout\n");
          printf("    -R file[:line]  Reverse map: given a tangled output file (and\n");
          printf("                    optional line number), prints the .weft source\n");
          printf("                    location that generated it.  Needs NO .weft files\n");
          printf("                    -- reads section markers embedded in the output\n\n");
          printf("  LaTeX:\n");
          printf("    -r              Enable hyperref cross-reference links\n");
          printf("    -h options      Pass options to hyperref package\n");
          printf("    -l              Use listings package for scrap formatting\n");
          printf("    -d              List dangling identifier references\n\n");
          printf("  General:\n");
          printf("    -V string       Set version string (expands @v in source)\n");
          printf("    -v              Verbose progress output\n");
          printf("    --help          Show this help message\n\n");
          printf("EXAMPLES\n");
          printf("  %s project.weft                        Tangle (default)\n", command_name);
          printf("  %s -w md project.weft                  Weave to Markdown\n", command_name);
          printf("  %s -w tex project.weft                 Weave to LaTeX\n", command_name);
          printf("  %s -m project.weft | jq '.files'       JSON structure map\n", command_name);
          printf("  %s -e \"Validate\" project.weft           Extract fragment\n", command_name);
          printf("  %s -R server.js:47                     Reverse map line\n", command_name);
          printf("  %s -R server.js                        All regions in file\n", command_name);
          printf("  %s -v -p build/ project.weft           Verbose + path prefix\n", command_name);
          printf("  %s -o project.weft                     Parse only, no output\n\n", command_name);
          printf("RULES FOR AI ASSISTANTS\n");
          printf("  1. NEVER edit tangled output files (*.c, *.go, *.js, etc.)\n");
          printf("     They are generated.  Your changes will be overwritten.\n");
          printf("  2. ALWAYS edit .weft source files instead.\n");
          printf("     Use -R to find which .weft file generates a given output line.\n");
          printf("  3. After editing .weft files, re-tangle:\n");
          printf("       %s project.weft\n", command_name);
          printf("  4. To understand a project's structure before editing:\n");
          printf("       %s -m project.weft | jq '.fragments'\n", command_name);
          printf("  5. To get the code of a specific fragment:\n");
          printf("       %s -e \"fragment name\" project.weft\n", command_name);
          printf("  6. When a compiler reports an error in tangled output:\n");
          printf("       %s -R output.c:42   # find the .weft source location\n", command_name);
          printf("     Then fix the code in the .weft file, not in output.c.\n\n");
          printf("STEP-BY-STEP AI WORKFLOW\n");
          printf("  1. Map:     %s -m project.weft | jq    Discover structure\n", command_name);
          printf("  2. Extract: %s -e \"name\" project.weft   Read specific fragment\n", command_name);
          printf("  3. Edit:    modify the .weft source       (never tangled output!)\n");
          printf("  4. Tangle:  %s project.weft              Regenerate output files\n", command_name);
          printf("  5. Build:   compile/test as usual         Verify the change works\n");
          printf("  6. Debug:   %s -R file:line              Trace errors to source\n\n", command_name);
          printf("DIRECTIVES QUICK REFERENCE\n");
          printf("  @o file flags @{ ... @}   Define output file (tangled to disk)\n");
          printf("  @d Name       @{ ... @}   Define reusable fragment\n");
          printf("  @<Name@>                   Invoke fragment (insert its contents)\n");
          printf("  @@                         Literal @ character in output\n");
          printf("  @i file                     Include another .weft file\n");
          printf("  @W fmt                      Declare weave format (md or tex)\n");
          printf("  @l tag                      Set language for next @o/@d\n");
          printf("  @L name style [+d]          Define custom language mapping\n");
          printf("  @v                          Expand to version string (-V)\n");
          printf("  @%%                          Comment (omitted from output)\n");
          printf("  @f @m @u                    Emit file/fragment/identifier indexes\n");/* {:26} */

#line 219 "literate/main.weft"

          exit(0);
        }
      }/* {:25} */

#line 193 "literate/main.weft"

      /* Interpret the argument string \verb|s| */
      
#line 336 "literate/main.weft"
      /* {27: literate/main.weft:336} */
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
            default:  fprintf(stderr, "%s: unexpected argument ignored.  "
                              "Try '%s --help' for usage information.\n",
                              command_name, command_name);
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
      }/* {:27} */

#line 194 "literate/main.weft"

      arg++;
      /* Perhaps get the prepend path */
      
#line 418 "literate/main.weft"
      /* {28: literate/main.weft:418} */
if (prepend_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        dirpath = s;
        prepend_flag = FALSE;
      }
      /* {:28} */

#line 196 "literate/main.weft"

      /* Perhaps get the version info string */
      
#line 446 "literate/main.weft"
      /* {30: literate/main.weft:446} */
if (version_info_flag)
      {
         if (*s == '\0')
           s = argv[arg++];
         version_string = s;
         version_info_flag = FALSE;
      }
      /* {:30} */

#line 197 "literate/main.weft"

      /* Perhaps get the hyperref options */
      
#line 456 "literate/main.weft"
      /* {31: literate/main.weft:456} */
if (hyperopt_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        hyperoptions = s;
        hyperopt_flag = FALSE;
        hyperref_flag = TRUE;
      }
      /* {:31} */

#line 198 "literate/main.weft"

      /* Perhaps add an include path */
      
#line 428 "literate/main.weft"
      /* {29: literate/main.weft:428} */
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
      /* {:29} */

#line 199 "literate/main.weft"

      /* Perhaps get the extract name */
      
#line 467 "literate/main.weft"
      /* {32: literate/main.weft:467} */
if (extract_flag) {
        extract_name = (*s != '\0') ? s : argv[arg++];
        extract_flag = FALSE;
      }/* {:32} */

#line 200 "literate/main.weft"

      /* Perhaps get the reverse map argument */
      
#line 473 "literate/main.weft"
      /* {33: literate/main.weft:473} */
if (reverse_map_flag) {
        reverse_map_arg = (*s != '\0') ? s : argv[arg++];
        reverse_map_flag = FALSE;
      }/* {:33} */

#line 201 "literate/main.weft"

    }
    else break;
  }/* {:24} */

#line 12 "literate/main.weft"

  /* Set locale information */
  
#line 493 "literate/main.weft"
/* {35: literate/main.weft:493} */

  {
    /* try to get locale information */
    char *s=getenv("LC_CTYPE");
    if (s==NULL) s=getenv("LC_ALL");

    /* set it */
    if (s!=NULL)
      if(setlocale(LC_CTYPE, s)==NULL)
        fprintf(stderr, "Setting locale failed\n");
  }
  /* {:35} */

#line 13 "literate/main.weft"

  /* Handle reverse map mode */
  
#line 482 "literate/main.weft"
  /* {34: literate/main.weft:482} */
if (reverse_map_arg) {
    write_reverse_map(reverse_map_arg);
    exit(0);
  }/* {:34} */

#line 14 "literate/main.weft"

  initialise_delimit_scrap_array();
  /* Process the remaining arguments (file names) */
  
#line 512 "literate/main.weft"
  /* {36: literate/main.weft:512} */
{
    if (arg >= argc) {
      fprintf(stderr, "%s: expected a file name.\n"
              "Usage: %s [options] file...\n"
              "Try '%s --help' for more information.\n",
              command_name, command_name, command_name);
      exit(-1);
    }
    do {
      /* Handle the file name in \verb|argv[arg]| */
      
#line 536 "literate/main.weft"
      /* {37: literate/main.weft:536} */
{
        char source_name[FILENAME_MAX];
        char tex_name[FILENAME_MAX];
        char aux_name[FILENAME_MAX];
        char *trim;
        char *dot;
        /* Build \verb|source_name| and \verb|tex_name| */
        
#line 560 "literate/main.weft"
        /* {38: literate/main.weft:560} */
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
          
#line 606 "literate/main.weft"
          /* {39: literate/main.weft:606} */
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
          /* {:39} */

#line 577 "literate/main.weft"

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
        }/* {:38} */

#line 542 "literate/main.weft"

        /* Process a file */
        
#line 638 "literate/main.weft"
        /* {40: literate/main.weft:638} */
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
        }/* {:40} */

#line 543 "literate/main.weft"

      }/* {:37} */

#line 521 "literate/main.weft"

      arg++;
    } while (arg < argc);
  }/* {:36} */

#line 16 "literate/main.weft"

  exit(0);
}
/* {:15} */
