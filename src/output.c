
#line 104 "literate/architecture.weft"
/* {9: literate/architecture.weft:104} */
#include "global.h"
/* {:9} */

#line 37 "literate/map-output.weft"
/* {196: literate/map-output.weft:37} */
static void json_puts(FILE *f, const char *s)
{
  fputc('"', f);
  while (*s) {
    switch (*s) {
      case '"':  fputs("\\\"", f); break;
      case '\\': fputs("\\\\", f); break;
      case '\n': fputs("\\n", f);  break;
      case '\r': fputs("\\r", f);  break;
      case '\t': fputs("\\t", f);  break;
      default:
        if ((unsigned char)*s < 0x20)
          fprintf(f, "\\u%04x", (unsigned char)*s);
        else
          fputc(*s, f);
    }
    s++;
  }
  fputc('"', f);
}
/* {:196} */

#line 67 "literate/map-output.weft"
/* {197: literate/map-output.weft:67} */
static void emit_name_json(FILE *f, Name *name, int is_file, int *first)
{
  Scrap_Node *sn;
  int first_inner;

  if (!name) return;
  emit_name_json(f, name->llink, is_file, first);

  if (!*first) fputs(",\n", f);
  *first = 0;

  fputs("    {\n", f);
  fputs("      \"name\": ", f); json_puts(f, name->spelling);
  if (name->lang[0]) {
    fputs(",\n      \"lang\": ", f); json_puts(f, name->lang);
  }
  fprintf(f, ",\n      \"sector\": %d", name->sector);

  /* defs array */
  fputs(",\n      \"defs\": [", f);
  first_inner = 1;
  for (sn = name->defs; sn; sn = sn->next) {
    if (!first_inner) fputs(", ", f);
    first_inner = 0;
    fprintf(f, "{\"scrap\": %d, \"file\": ", sn->scrap);
    json_puts(f, scrap_file_name(sn->scrap));
    fprintf(f, ", \"line\": %d}", scrap_file_line(sn->scrap));
  }
  fputs("]", f);

  if (is_file) {
    /* uses_fragments: fragments referenced inside this file's scraps */
    fputs(",\n      \"uses_fragments\": [", f);
    first_inner = 1;
    for (sn = name->defs; sn; sn = sn->next) {
      Uses *u;
      for (u = get_scrap_uses(sn->scrap); u; u = u->next) {
        if (!first_inner) fputs(", ", f);
        first_inner = 0;
        json_puts(f, u->defn->spelling);
      }
    }
    fputs("]", f);
  } else {
    /* used_by: scraps that invoke this fragment */
    fputs(",\n      \"used_by\": [", f);
    first_inner = 1;
    for (sn = name->uses; sn; sn = sn->next) {
      if (!first_inner) fputs(", ", f);
      first_inner = 0;
      fprintf(f, "{\"scrap\": %d, \"file\": ", sn->scrap);
      json_puts(f, scrap_file_name(sn->scrap));
      fprintf(f, ", \"line\": %d}", scrap_file_line(sn->scrap));
    }
    fputs("]", f);

    /* uses: fragments referenced inside this fragment's def scraps */
    fputs(",\n      \"uses\": [", f);
    first_inner = 1;
    for (sn = name->defs; sn; sn = sn->next) {
      Uses *u;
      for (u = get_scrap_uses(sn->scrap); u; u = u->next) {
        if (!first_inner) fputs(", ", f);
        first_inner = 0;
        json_puts(f, u->defn->spelling);
      }
    }
    fputs("]", f);
  }

  fputs("\n    }", f);

  emit_name_json(f, name->rlink, is_file, first);
}
/* {:197} */

#line 144 "literate/map-output.weft"
/* {198: literate/map-output.weft:144} */
void write_map(Name *files, Name *macros)
{
  int first;

  fputs("{\n  \"files\": [\n", stdout);
  first = 1;
  emit_name_json(stdout, files, 1, &first);
  fputs("\n  ],\n  \"fragments\": [\n", stdout);
  first = 1;
  emit_name_json(stdout, macros, 0, &first);
  fputs("\n  ]\n}\n", stdout);
}
/* {:198} */

#line 171 "literate/map-output.weft"
/* {199: literate/map-output.weft:171} */
static Name *find_name(Name *node, const char *spelling)
{
  Name *found;
  if (!node) return NULL;
  if (robs_strcmp(node->spelling, (char *)spelling) == 0)
    return node;
  found = find_name(node->llink, spelling);
  if (found) return found;
  return find_name(node->rlink, spelling);
}
/* {:199} */

#line 188 "literate/map-output.weft"
/* {200: literate/map-output.weft:188} */
static void extract_fragment(FILE *f, Name *name, Name *files, Name *macros)
{
  Scrap_Node *sn;

  if (!name || name->mark) return;
  name->mark = TRUE;

  for (sn = name->defs; sn; sn = sn->next) {
    fprintf(f, "\n## \"%s\" --- %s:%d\n\n",
            name->spelling,
            scrap_file_name(sn->scrap),
            scrap_file_line(sn->scrap));
    if (name->lang[0])
      fprintf(f, "```%s\n", name->lang);
    else
      fputs("```\n", f);
    dump_scrap_text(f, sn->scrap);
    fputs("\n```\n", f);

    /* Show dependencies */
    {
      Uses *u = get_scrap_uses(sn->scrap);
      if (u) {
        fputs("\n### Uses\n", f);
        for (; u; u = u->next)
          fprintf(f, "- %s\n", u->defn->spelling);
      }
    }
  }

  /* Show used_by */
  if (name->uses) {
    Scrap_Node *usn;
    fputs("\n### Used by\n", f);
    for (usn = name->uses; usn; usn = usn->next) {
      fprintf(f, "- scrap %d (%s:%d)\n",
              usn->scrap,
              scrap_file_name(usn->scrap),
              scrap_file_line(usn->scrap));
    }
  }

  /* Recurse into dependencies */
  for (sn = name->defs; sn; sn = sn->next) {
    Uses *u;
    for (u = get_scrap_uses(sn->scrap); u; u = u->next) {
      extract_fragment(f, u->defn, files, macros);
    }
  }
}
/* {:200} */

#line 243 "literate/map-output.weft"
/* {201: literate/map-output.weft:243} */
static void clear_marks(Name *node)
{
  if (!node) return;
  clear_marks(node->llink);
  node->mark = FALSE;
  clear_marks(node->rlink);
}
/* {:201} */

#line 253 "literate/map-output.weft"
/* {202: literate/map-output.weft:253} */
void write_extract(char *name, Name *files, Name *macros)
{
  Name *found = NULL;

  /* Search in macro_names first, then file_names */
  found = find_name(macros, name);
  if (!found)
    found = find_name(files, name);

  if (!found) {
    fprintf(stderr, "%s: fragment \"%s\" not found\n", command_name, name);
    exit(1);
  }

  fprintf(stdout, "# Extract: \"%s\"\n", found->spelling);
  extract_fragment(stdout, found, files, macros);

  /* Clean up marks */
  clear_marks(files);
  clear_marks(macros);
}
/* {:202} */

#line 13 "literate/output-files.weft"
/* {205: literate/output-files.weft:13} */
void write_files(Name *files)
{
  while (files) {
    write_files(files->llink);
    /* Write out \verb|files->spelling| */
    
#line 38 "literate/output-files.weft"
    /* {207: literate/output-files.weft:38} */
{
      static char temp_name[FILENAME_MAX];
      static char real_name[FILENAME_MAX];
      static int temp_name_count = 0;
      char indent_chars[MAX_INDENT];
      FILE *temp_file;

      /* Find a free temporary file */
      
#line 59 "literate/output-files.weft"
/* {208: literate/output-files.weft:59} */

      for( temp_name_count = 0; temp_name_count < 10000; temp_name_count++) {
        sprintf(temp_name,"%s%snw%06d", dirpath, path_sep, temp_name_count);
      #ifdef O_EXCL
        if (-1 != (temp_file_fd = open(temp_name, O_CREAT|O_WRONLY|O_EXCL))) {
           temp_file = fdopen(temp_file_fd, "w");
           break;
        }
      #else
        if (0 != (temp_file = fopen(temp_name, "a"))) {
           if ( 0L == ftell(temp_file)) {
              break;
           } else {
              fclose(temp_file);
              temp_file = 0;
           }
        }
      #endif
      }
      if (!temp_file) {
        fprintf(stderr, "%s: can't create %s for a temporary file\n",
                command_name, temp_name);
        exit(-1);
      }
      /* {:208} */

#line 45 "literate/output-files.weft"


      sprintf(real_name, "%s%s%s", dirpath, path_sep, files->spelling);
      if (verbose_flag)
        fprintf(stderr, "writing %s [%s]\n", files->spelling, temp_name);
      write_scraps(temp_file, files->spelling, files->defs, 0, indent_chars,
                   files->debug_flag, files->tab_flag, files->indent_flag,
                   files->comment_flag, files->location_flag,
                   NULL, NULL, 0, files->spelling);
      fclose(temp_file);

      /* Move the temporary file to the target, if required */
      
#line 89 "literate/output-files.weft"
/* {209: literate/output-files.weft:89} */

      if (compare_flag)
        /* Compare the temp file and the old file */
        
#line 100 "literate/output-files.weft"
        /* {210: literate/output-files.weft:100} */
{
          FILE *old_file = fopen(real_name, "r");
          if (old_file) {
            int x, y;
            temp_file = fopen(temp_name, "r");
            do {
              x = getc(old_file);
              y = getc(temp_file);
            } while (x == y && x != EOF);
            fclose(old_file);
            fclose(temp_file);
            if (x == y)
              remove(temp_name);
            else {
              remove(real_name);
              /* Rename the temporary file to the target */
              
#line 122 "literate/output-files.weft"
/* {211: literate/output-files.weft:122} */

              if (0 != rename(temp_name, real_name)) {
                fprintf(stderr, "%s: can't rename output file to %s\n",
                        command_name, real_name);
              }
              /* {:211} */

#line 115 "literate/output-files.weft"

            }
          }
          else
            /* Rename the temporary file to the target */
            
#line 122 "literate/output-files.weft"
/* {211: literate/output-files.weft:122} */

            if (0 != rename(temp_name, real_name)) {
              fprintf(stderr, "%s: can't rename output file to %s\n",
                      command_name, real_name);
            }
            /* {:211} */

#line 119 "literate/output-files.weft"

        }/* {:210} */

#line 91 "literate/output-files.weft"

      else {
        remove(real_name);
        /* Rename the temporary file to the target */
        
#line 122 "literate/output-files.weft"
/* {211: literate/output-files.weft:122} */

        if (0 != rename(temp_name, real_name)) {
          fprintf(stderr, "%s: can't rename output file to %s\n",
                  command_name, real_name);
        }
        /* {:211} */

#line 94 "literate/output-files.weft"

      }
      /* {:209} */

#line 56 "literate/output-files.weft"

    }/* {:207} */

#line 17 "literate/output-files.weft"

    files = files->rlink;
  }
}
/* {:205} */
