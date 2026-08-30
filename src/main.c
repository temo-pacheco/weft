
#line 106 "literate/architecture.weft"
/* {6: literate/architecture.weft:106} */
#include "global.h"
/* {:6} */

#line 7 "literate/main.weft"
/* {16: literate/main.weft:7} */

#include <stdlib.h>
int main(int argc, char **argv)
{
  int arg = 1;
  /* Interpret command-line arguments */
  
#line 200 "literate/main.weft"
  /* {24: literate/main.weft:200} */
command_name = argv[0];
  /* {:24} */

#line 206 "literate/main.weft"
  /* {25: literate/main.weft:206} */
while (arg < argc) {
    char *s = argv[arg];
    if (*s++ == '-') {
      /* Check for long options */
      
#line 233 "literate/main.weft"
      /* {26: literate/main.weft:233} */
if (*s == '-') {
        if (strcmp(s + 1, "help") == 0) {
          /* Print help message */
          
#line 281 "literate/main.weft"
          /* {27: literate/main.weft:281} */
printf("weft %s -- A Literate Programming Tool\n\n", WEFT_VERSION);
          printf("USAGE\n");
          printf("  %s [options] file...\n\n", command_name);
          printf("WHAT IS WEFT?\n");
          printf("  weft is a literate programming tool: write .weft files where\n");
          printf("  documentation and code live as one narrative.  weft TANGLES them\n");
          printf("  into executable output files and optionally WEAVES formatted\n");
          printf("  documentation (LaTeX or Markdown).  .weft files are the ONLY\n");
          printf("  source of truth; tangled outputs are GENERATED -- never edit them.\n\n");
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
          printf("                    -- reads section markers embedded in the output\n");
          printf("    --bodies        Include raw scrap body text in -m JSON output\n");
          printf("    --prose         Include preceding documentation in -m JSON output\n");
          printf("    --diff          Show unified diff of what tangle would change\n");
          printf("                    (no files written)\n");
          printf("    --lint          Static analysis: unused fragments, undefined refs,\n");
          printf("                    and fuzzy-match suggestions for likely typos\n");
          printf("    --callers       With -e, show call chain from %co roots to fragment\n", nw_char);
          printf("    --errors        Annotate compiler errors from stdin with .weft\n");
          printf("                    source locations (reads section markers, no\n");
          printf("                    .weft files needed)\n");
          printf("                    Example: make 2>&1 | weft --errors\n\n");
          printf("  LaTeX:\n");
          printf("    -r              Enable hyperref cross-reference links\n");
          printf("    -h options      Pass options to hyperref package\n");
          printf("    -l              Deprecated no-op (listings is always used)\n");
          printf("    -d              List dangling identifier references\n\n");
          printf("  General:\n");
          printf("    -V string       Set version string (expands @v in source)\n");
          printf("    -v              Verbose progress output\n");
          printf("    --help          Show this help message\n");
          printf("    --version       Show version number\n\n");
          printf("PER-FILE FLAGS (on @o)\n");
          printf("  @o file [-d] [-i] [-t] [-s] [-cc|-c+|-cp] @{ ... @}\n");
          printf("    -d           Emit #line N \"file\" directives (C/C++ debuggers)\n");
          printf("    -i           Suppress auto-indentation of expanded fragments\n");
          printf("    -t           Suppress tab expansion (write tabs literally)\n");
          printf("    -cc          C block-comment markers:   /* {N: file:line} */\n");
          printf("    -c+          C++ line-comment markers:  // {N: file:line}\n");
          printf("    -cp          Hash-comment markers:      # {N: file:line}\n");
          printf("    -s           Suppress all section markers in output\n");
          printf("  Without explicit flags, weft auto-detects from file extension\n");
          printf("  (250+ extensions recognized).  Use @l tag before @o to override.\n\n");
          printf("SECTION MARKERS\n");
          printf("  When comment style is known, tangled output includes markers:\n");
          printf("    /* {5: source.weft:42} */   opening -- scrap 5, from line 42\n");
          printf("    /* {:5} */                  closing\n");
          printf("  Comment adapts to language: // for C++/Go, # for Python/shell, etc.\n");
          printf("  Use weft -R to parse these markers and reverse-map to source.\n\n");
          printf("DIRECTIVES REFERENCE\n");
          printf("  Core structure:\n");
          printf("    @o file [flags] @{ code @}  Output file (tangled to disk)\n");
          printf("    @d Name @{ code @}          Define reusable named fragment\n");
          printf("    @d+ Name @{ code @}         Global fragment (visible across sectors)\n");
          printf("    @q Name @{ code @}          Quoted fragment (no expansion on include)\n");
          printf("    @O / @D / @Q                Uppercase = big-format scrap in LaTeX\n");
          printf("    @<Name@>                     Invoke (expand) a named fragment\n");
          printf("    @<+Name@>                    Invoke the global version of a fragment\n");
          printf("    @i file                       Include another .weft file\n");
          printf("    @@                           Literal @ character\n\n");
          printf("  Fragment parameters:\n");
          printf("    @'text@'                     Pass literal text argument to a fragment\n");
          printf("    @1 .. @9                     Substitute nth argument inside fragment\n");
          printf("    @{ code @} as argument       Pass anonymous inline scrap as argument\n\n");
          printf("  Inside scraps:\n");
          printf("    @f                            Current output file name\n");
          printf("    @t                            Current fragment title (as invoked)\n");
          printf("    @v                            Version string (defaults to weft's version; -V overrides)\n");
          printf("    @c                            Expand block comment at current indent\n");
          printf("    @#                            Suppress auto-indent for this line\n");
          printf("    @%%                            Comment to end of line (omitted)\n");
          printf("    @| name                       Mark identifier(s) as defined here\n");
          printf("    @+ @- @*                      Global-export / import / used markers\n");
          printf("    @_ text @_                   Bold (LaTeX weave); skipped in tangle\n");
          printf("    @x label @x                  Cross-reference label (anchor position)\n\n");
          printf("  Scrap delimiters (LaTeX weave):\n");
          printf("    @{ ... @}                    Verbatim code (default)\n");
          printf("    @[ ... @]                    Paragraph mode\n");
          printf("    @( ... @)                    Math mode\n\n");
          printf("  Sections and structure:\n");
          printf("    @s                            Start a new sector (section boundary)\n");
          printf("    @S                            Close current sector (return to base)\n");
          printf("    @W fmt                        Declare weave format: md or tex\n\n");
          printf("  Language and formatting:\n");
          printf("    @l tag                        Set language for next @o or @d\n");
          printf("    @L name style [+d]            Define custom language (// # -- /* <!--)\n");
          printf("    @r x                          Change escape character from @ to x\n\n");
          printf("  Indexes (weave output only):\n");
          printf("    @f @m @u                      File / fragment / identifier indexes\n");
          printf("    @m+ @u+                       Global fragment / identifier indexes\n\n");
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
          printf("AI ASSISTANT WORKFLOW\n");
          printf("  GOLDEN RULE: .weft files are the ONLY source of truth.\n");
          printf("  Tangled outputs (*.c, *.go, etc.) are GENERATED -- never edit them.\n");
          printf("  If you edit a tangled file, your changes WILL be lost on next tangle.\n\n");
          printf("  Step 1 -- Understand the project structure:\n");
          printf("    %s -m project.weft > map.json\n", command_name);
          printf("    The JSON contains: files (all @o output files), fragments (all @d\n");
          printf("    named blocks), and a dependency graph (which fragments invoke which).\n");
          printf("    Useful jq queries:\n");
          printf("      jq '.files'                         List output files\n");
          printf("      jq '.fragments'                     List all fragments\n");
          printf("      jq '.fragments[] | select(.name | test(\"Auth\"))'  Search\n\n");
          printf("  Step 2 -- Read specific code without opening .weft files:\n");
          printf("    %s -e \"fragment name\" project.weft\n", command_name);
          printf("    Prints the fragment AND all its transitive dependencies as\n");
          printf("    self-contained Markdown to stdout.  Ideal for understanding\n");
          printf("    one piece of the system without reading the whole source.\n\n");
          printf("  Step 3 -- Edit the .weft source (NEVER tangled output):\n");
          printf("    Locate the right .weft file (use -m to find it), then edit\n");
          printf("    the @d or @o scrap that contains the code you need to change.\n");
          printf("    Use standard text editing -- the prose around code is LaTeX or\n");
          printf("    plain text, and code lives between @{ and @}.\n\n");
          printf("  Step 4 -- Regenerate and verify:\n");
          printf("    %s project.weft           Tangle (regenerate all output files)\n", command_name);
          printf("    Then build and test as usual.\n\n");
          printf("  Step 5 -- Debug errors with reverse map:\n");
          printf("    %s -R output.c:42          Find .weft source for line 42\n", command_name);
          printf("    %s -R output.c             Show ALL .weft regions in file\n", command_name);
          printf("    Reads section markers in the tangled output -- needs NO .weft files.\n");
          printf("    Fix errors in the .weft source, then re-tangle (step 4).\n");/* {:27} */

#line 235 "literate/main.weft"

          exit(0);
        }
        if (strcmp(s + 1, "version") == 0) {
          printf("weft %s\n", WEFT_VERSION);
          exit(0);
        }
        if (strcmp(s + 1, "bodies") == 0) {
          bodies_flag = TRUE;
          arg++;
          continue;
        }
        if (strcmp(s + 1, "prose") == 0) {
          prose_flag = TRUE;
          arg++;
          continue;
        }
        if (strcmp(s + 1, "diff") == 0) {
          diff_flag = TRUE;
          arg++;
          continue;
        }
        if (strcmp(s + 1, "callers") == 0) {
          callers_flag = TRUE;
          arg++;
          continue;
        }
        if (strcmp(s + 1, "lint") == 0) {
          lint_flag = TRUE;
          arg++;
          continue;
        }
        if (strcmp(s + 1, "errors") == 0) {
          errors_flag = TRUE;
          arg++;
          continue;
        }
      }/* {:26} */

#line 209 "literate/main.weft"

      /* Interpret the argument string \verb|s| */
      
#line 450 "literate/main.weft"
      /* {28: literate/main.weft:450} */
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
      }/* {:28} */

#line 210 "literate/main.weft"

      arg++;
      /* Perhaps get the prepend path */
      
#line 532 "literate/main.weft"
      /* {29: literate/main.weft:532} */
if (prepend_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        dirpath = s;
        prepend_flag = FALSE;
      }
      /* {:29} */

#line 212 "literate/main.weft"

      /* Perhaps get the version info string */
      
#line 560 "literate/main.weft"
      /* {31: literate/main.weft:560} */
if (version_info_flag)
      {
         if (*s == '\0')
           s = argv[arg++];
         version_string = s;
         version_info_flag = FALSE;
      }
      /* {:31} */

#line 213 "literate/main.weft"

      /* Perhaps get the hyperref options */
      
#line 570 "literate/main.weft"
      /* {32: literate/main.weft:570} */
if (hyperopt_flag)
      {
        if (*s == '\0')
          s = argv[arg++];
        hyperoptions = s;
        hyperopt_flag = FALSE;
        hyperref_flag = TRUE;
      }
      /* {:32} */

#line 214 "literate/main.weft"

      /* Perhaps add an include path */
      
#line 542 "literate/main.weft"
      /* {30: literate/main.weft:542} */
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
      /* {:30} */

#line 215 "literate/main.weft"

      /* Perhaps get the extract name */
      
#line 581 "literate/main.weft"
      /* {33: literate/main.weft:581} */
if (extract_flag) {
        extract_name = (*s != '\0') ? s : argv[arg++];
        extract_flag = FALSE;
      }/* {:33} */

#line 216 "literate/main.weft"

      /* Perhaps get the reverse map argument */
      
#line 587 "literate/main.weft"
      /* {34: literate/main.weft:587} */
if (reverse_map_flag) {
        reverse_map_arg = (*s != '\0') ? s : argv[arg++];
        reverse_map_flag = FALSE;
      }/* {:34} */

#line 217 "literate/main.weft"

    }
    else break;
  }/* {:25} */

#line 12 "literate/main.weft"

  /* Set locale information */
  
#line 611 "literate/main.weft"
/* {36: literate/main.weft:611} */

  {
    /* try to get locale information */
    char *s=getenv("LC_CTYPE");
    if (s==NULL) s=getenv("LC_ALL");

    /* set it */
    if (s!=NULL)
      if(setlocale(LC_CTYPE, s)==NULL)
        fprintf(stderr, "Setting locale failed\n");
  }
  /* {:36} */

#line 13 "literate/main.weft"

  /* Handle reverse map mode */
  
#line 596 "literate/main.weft"
  /* {35: literate/main.weft:596} */
if (errors_flag) {
    write_error_annotator();
    exit(0);
  }
  if (reverse_map_arg) {
    write_reverse_map(reverse_map_arg);
    exit(0);
  }/* {:35} */

#line 14 "literate/main.weft"

  initialise_delimit_scrap_array();
  /* Process the remaining arguments (file names) */
  
#line 630 "literate/main.weft"
  /* {37: literate/main.weft:630} */
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
      
#line 654 "literate/main.weft"
      /* {38: literate/main.weft:654} */
{
        char source_name[FILENAME_MAX];
        char tex_name[FILENAME_MAX];
        char aux_name[FILENAME_MAX];
        char *trim;
        char *dot;
        /* Build \verb|source_name| and \verb|tex_name| */
        
#line 678 "literate/main.weft"
        /* {39: literate/main.weft:678} */
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
          
#line 724 "literate/main.weft"
          /* {40: literate/main.weft:724} */
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
          /* {:40} */

#line 695 "literate/main.weft"

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
        }/* {:39} */

#line 660 "literate/main.weft"

        /* Process a file */
        
#line 756 "literate/main.weft"
        /* {41: literate/main.weft:756} */
{
          int do_weave = FALSE;
          int effective_format = 0;

          /* Reset weave_format for this file unless CLI locked it */
          if (!weave_format_from_cli)
            weave_format = 0;

          pass1(source_name);
          current_sector = 1;
          prev_sector = 1;

          if (lint_flag) {
            write_lint(file_names, macro_names);
            /* skip weave and tangle */
          } else if (map_flag) {
            write_map(file_names, macro_names);
            /* skip weave and tangle */
          } else if (extract_name) {
            if (callers_flag)
              write_callers(extract_name, file_names, macro_names);
            else
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
        }/* {:41} */

#line 661 "literate/main.weft"

      }/* {:38} */

#line 639 "literate/main.weft"

      arg++;
    } while (arg < argc);
  }/* {:37} */

#line 16 "literate/main.weft"

  exit(0);
}
/* {:16} */
