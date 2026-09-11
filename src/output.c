
#line 142 "literate/architecture.weft"
/* {11: literate/architecture.weft:142} */
#include "global.h"
/* {:11} */

#line 37 "literate/map-output.weft"
/* {234: literate/map-output.weft:37} */
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
/* {:234} */

#line 67 "literate/map-output.weft"
/* {235: literate/map-output.weft:67} */
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
    fprintf(f, ", \"line\": %d, \"end_line\": %d",
            scrap_file_line(sn->scrap), scrap_end_line(sn->scrap));
    if (bodies_flag) {
      fputs(", \"body\": ", f);
      dump_scrap_json(f, sn->scrap);
    }
    if (prose_flag && scrap_prose(sn->scrap)) {
      fputs(", \"prose\": ", f);
      json_puts(f, scrap_prose(sn->scrap));
    }
    fputc('}', f);
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
/* {:235} */

#line 154 "literate/map-output.weft"
/* {236: literate/map-output.weft:154} */
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
/* {:236} */

#line 181 "literate/map-output.weft"
/* {237: literate/map-output.weft:181} */
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
/* {:237} */

#line 198 "literate/map-output.weft"
/* {238: literate/map-output.weft:198} */
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
/* {:238} */

#line 253 "literate/map-output.weft"
/* {239: literate/map-output.weft:253} */
static void clear_marks(Name *node)
{
  if (!node) return;
  clear_marks(node->llink);
  node->mark = FALSE;
  clear_marks(node->rlink);
}
/* {:239} */

#line 271 "literate/map-output.weft"
/* {240: literate/map-output.weft:271} */
static int is_file_name(Name *root, Name *target)
{
  if (!root) return 0;
  if (root == target) return 1;
  if (is_file_name(root->llink, target)) return 1;
  return is_file_name(root->rlink, target);
}
/* {:240} */

#line 281 "literate/map-output.weft"
/* {241: literate/map-output.weft:281} */
static void print_callers(Name *name, Name *file_names, int depth)
{
  Scrap_Node *usn;
  int i;

  if (!name || name->mark) return;
  name->mark = TRUE;

  for (i = 0; i < depth; i++) fputs("  ", stdout);
  if (is_file_name(file_names, name))
    printf("[file] %s\n", name->spelling);
  else
    printf("%s\n", name->spelling);

  for (usn = name->uses; usn; usn = usn->next) {
    Name *owner = scrap_owner(usn->scrap);
    if (owner && !owner->mark)
      print_callers(owner, file_names, depth + 1);
  }
}
/* {:241} */

#line 304 "literate/map-output.weft"
/* {242: literate/map-output.weft:304} */
void write_callers(char *name, Name *files, Name *macros)
{
  Name *found = find_name(macros, name);
  if (!found)
    found = find_name(files, name);
  if (!found) {
    fprintf(stderr, "%s: fragment \"%s\" not found\n", command_name, name);
    exit(1);
  }
  printf("Callers of \"%s\":\n\n", found->spelling);
  printf("%s\n", found->spelling);
  {
    Scrap_Node *usn;
    for (usn = found->uses; usn; usn = usn->next) {
      Name *owner = scrap_owner(usn->scrap);
      if (owner)
        print_callers(owner, files, 1);
    }
  }
  clear_marks(files);
  clear_marks(macros);
}
/* {:242} */

#line 329 "literate/map-output.weft"
/* {243: literate/map-output.weft:329} */
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
/* {:243} */

#line 368 "literate/map-output.weft"
/* {244: literate/map-output.weft:368} */
static int levenshtein(const char *a, const char *b)
{
  int la = (int)strlen(a), lb = (int)strlen(b);
  int prev[256], curr[256];
  int i, j;
  if (la > 255 || lb > 255) return 999;
  for (j = 0; j <= lb; j++) prev[j] = j;
  for (i = 1; i <= la; i++) {
    curr[0] = i;
    for (j = 1; j <= lb; j++) {
      int cost = (a[i-1] != b[j-1]) ? 1 : 0;
      int del = prev[j] + 1;
      int ins = curr[j-1] + 1;
      int sub = prev[j-1] + cost;
      curr[j] = del < ins ? (del < sub ? del : sub) : (ins < sub ? ins : sub);
    }
    for (j = 0; j <= lb; j++) prev[j] = curr[j];
  }
  return prev[lb];
}
/* {:244} */

#line 391 "literate/map-output.weft"
/* {245: literate/map-output.weft:391} */

#define MAX_LINT_NAMES 4096
static Name *lint_names[MAX_LINT_NAMES];
static int lint_name_count;

static void collect_lint_names(Name *node)
{
  if (!node) return;
  collect_lint_names(node->llink);
  if (node->defs && lint_name_count < MAX_LINT_NAMES)
    lint_names[lint_name_count++] = node;
  collect_lint_names(node->rlink);
}

static const char *find_closest(const char *target)
{
  int i, best_dist = 4;
  const char *best = NULL;
  for (i = 0; i < lint_name_count; i++) {
    int d = levenshtein(target, lint_names[i]->spelling);
    if (d < best_dist) {
      best_dist = d;
      best = lint_names[i]->spelling;
    }
  }
  return best;
}
/* {:245} */

#line 421 "literate/map-output.weft"
/* {246: literate/map-output.weft:421} */
static int lint_name(Name *node, int check_unused, int *issues)
{
  if (!node) return 0;
  lint_name(node->llink, check_unused, issues);

  if (node->defs == NULL && node->uses != NULL) {
    /* undefined reference */
    const char *suggestion = find_closest(node->spelling);
    fprintf(stderr, "lint: undefined fragment \"%s\"", node->spelling);
    if (node->uses) {
      fprintf(stderr, " (used at %s:%d)",
              scrap_file_name(node->uses->scrap),
              scrap_file_line(node->uses->scrap));
    }
    if (suggestion)
      fprintf(stderr, " -- did you mean \"%s\"?", suggestion);
    fputc('\n', stderr);
    (*issues)++;
  }
  else if (node->defs != NULL && node->uses == NULL && check_unused) {
    fprintf(stderr, "lint: unused fragment \"%s\" (defined at %s:%d)\n",
            node->spelling,
            scrap_file_name(node->defs->scrap),
            scrap_file_line(node->defs->scrap));
    (*issues)++;
  }

  lint_name(node->rlink, check_unused, issues);
  return 0;
}
/* {:246} */

#line 454 "literate/map-output.weft"
/* {247: literate/map-output.weft:454} */
void write_lint(Name *file_names, Name *macro_names)
{
  int issues = 0;
  lint_name_count = 0;
  collect_lint_names(file_names);
  collect_lint_names(macro_names);

  /* check files for undefined refs (not unused -- files are roots) */
  lint_name(file_names, 0, &issues);
  /* check macros for both undefined and unused */
  lint_name(macro_names, 1, &issues);

  if (issues)
    fprintf(stderr, "lint: %d issue(s) found\n", issues);
  else
    fprintf(stderr, "lint: no issues found\n");
}
/* {:247} */

#line 499 "literate/map-output.weft"
/* {248: literate/map-output.weft:499} */
typedef struct {
  int scrap;
  char file[FILENAME_MAX];
  int line;
  int start_line;  /* line in the tangled file where this marker opened */
} MarkerFrame;
/* {:248} */

#line 513 "literate/map-output.weft"
/* {249: literate/map-output.weft:513} */
static int parse_open_marker(const char *line, MarkerFrame *mf)
{
  const char *p = line;
  while (*p) {
    if (*p == '{') {
      const char *start = p + 1;
      /* scrap number */
      if (*start < '0' || *start > '9') { p++; continue; }
      int scrap = 0;
      while (*start >= '0' && *start <= '9')
        scrap = scrap * 10 + (*start++ - '0');
      if (*start != ':' || *(start+1) != ' ') { p++; continue; }
      start += 2; /* skip ": " */
      /* file:line -- find the closing brace, then work backwards for :line */
      const char *end = strchr(start, '}');
      if (!end) { p++; continue; }
      /* find last ':' between start and end */
      const char *colon = NULL;
      for (const char *q = end - 1; q >= start; q--) {
        if (*q == ':') { colon = q; break; }
      }
      if (!colon) { p++; continue; }
      /* digits after colon? */
      int src_line = 0;
      const char *d = colon + 1;
      if (d == end) { p++; continue; }
      while (d < end) {
        if (*d < '0' || *d > '9') break;
        src_line = src_line * 10 + (*d++ - '0');
      }
      if (d != end) { p++; continue; }
      /* success */
      mf->scrap = scrap;
      mf->line = src_line;
      {
        size_t len = (size_t)(colon - start);
        if (len >= FILENAME_MAX) len = FILENAME_MAX - 1;
        memcpy(mf->file, start, len);
        mf->file[len] = '\0';
      }
      return 1;
    }
    p++;
  }
  return 0;
}
/* {:249} */

#line 565 "literate/map-output.weft"
/* {250: literate/map-output.weft:565} */
static int parse_close_marker(const char *line)
{
  const char *p = line;
  while (*p) {
    if (*p == '{' && *(p+1) == ':') {
      const char *start = p + 2;
      if (*start < '0' || *start > '9') { p++; continue; }
      int scrap = 0;
      while (*start >= '0' && *start <= '9')
        scrap = scrap * 10 + (*start++ - '0');
      if (*start == '}')
        return scrap;
    }
    p++;
  }
  return -1;
}
/* {:250} */

#line 598 "literate/map-output.weft"
/* {251: literate/map-output.weft:598} */
void write_reverse_map(const char *arg)
{
  char filename[FILENAME_MAX];
  int target_line = -1;
  FILE *fp;
  char buf[8192];
  MarkerFrame stack[256];
  int depth = 0;
  int current_line = 0;
  int first_region = 1;

  /* Parse arg: find last ':' where suffix is all digits */
  {
    const char *colon = NULL;
    const char *p;
    for (p = arg + strlen(arg) - 1; p > arg; p--) {
      if (*p == ':') { colon = p; break; }
    }
    if (colon) {
      const char *d = colon + 1;
      int all_digits = (*d != '\0');
      while (*d) {
        if (*d < '0' || *d > '9') { all_digits = 0; break; }
        d++;
      }
      if (all_digits) {
        target_line = atoi(colon + 1);
        snprintf(filename, FILENAME_MAX, "%.*s", (int)(colon - arg), arg);
      } else {
        snprintf(filename, FILENAME_MAX, "%s", arg);
      }
    } else {
      snprintf(filename, FILENAME_MAX, "%s", arg);
    }
  }

  fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "weft: cannot open %s: ", filename);
    perror(NULL);
    exit(1);
  }

  if (target_line < 0) {
    /* Full mode header */
    fputs("{\"source\": ", stdout);
    json_puts(stdout, filename);
    fputs(", \"regions\": [\n", stdout);
  }

  while (fgets(buf, sizeof(buf), fp)) {
    MarkerFrame mf;
    int close_scrap;
    current_line++;

    if (parse_open_marker(buf, &mf)) {
      mf.start_line = current_line;
      if (depth < 256)
        stack[depth++] = mf;
      continue;
    }

    close_scrap = parse_close_marker(buf);
    if (close_scrap >= 0) {
      /* Find matching frame on stack */
      int i;
      for (i = depth - 1; i >= 0; i--) {
        if (stack[i].scrap == close_scrap) {
          if (target_line < 0) {
            /* Full mode: emit region */
            if (!first_region) fputs(",\n", stdout);
            first_region = 0;
            fprintf(stdout,
              "  {\"start\": %d, \"end\": %d, \"scrap\": %d, \"file\": ",
              stack[i].start_line + 1, current_line - 1, stack[i].scrap);
            json_puts(stdout, stack[i].file);
            fprintf(stdout, ", \"line\": %d}", stack[i].line);
          }
          /* Remove frame by shifting */
          depth--;
          for (; i < depth; i++)
            stack[i] = stack[i + 1];
          break;
        }
      }
      continue;
    }

    /* Regular line -- check for line mode match */
    if (target_line > 0 && current_line == target_line) {
      if (depth > 0) {
        MarkerFrame *inner = &stack[depth - 1];
        fprintf(stdout, "{\"scrap\": %d, \"file\": ", inner->scrap);
        json_puts(stdout, inner->file);
        fprintf(stdout, ", \"line\": %d}\n", inner->line);
      } else {
        fputs("{}\n", stdout);
      }
      fclose(fp);
      return;
    }
  }

  fclose(fp);

  if (target_line > 0) {
    /* Line was beyond end of file or not inside a scrap */
    fputs("{}\n", stdout);
  } else {
    fputs("\n]}\n", stdout);
  }
}
/* {:251} */

#line 723 "literate/map-output.weft"
/* {252: literate/map-output.weft:723} */
static int reverse_lookup_in_file(const char *filename, int target_line,
                                   MarkerFrame *result)
{
  FILE *fp = fopen(filename, "r");
  char buf[8192];
  MarkerFrame stack[256];
  int depth = 0, current_line = 0;

  if (!fp) return 0;

  while (fgets(buf, sizeof(buf), fp)) {
    MarkerFrame mf;
    int close_scrap;
    current_line++;

    if (parse_open_marker(buf, &mf)) {
      mf.start_line = current_line;
      if (depth < 256)
        stack[depth++] = mf;
      continue;
    }

    close_scrap = parse_close_marker(buf);
    if (close_scrap >= 0) {
      int i;
      for (i = depth - 1; i >= 0; i--) {
        if (stack[i].scrap == close_scrap) {
          depth--;
          for (; i < depth; i++)
            stack[i] = stack[i + 1];
          break;
        }
      }
      continue;
    }

    if (current_line == target_line && depth > 0) {
      *result = stack[depth - 1];
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}
/* {:252} */

#line 771 "literate/map-output.weft"
/* {253: literate/map-output.weft:771} */
void write_error_annotator(void)
{
  char line[8192];

  while (fgets(line, sizeof(line), stdin)) {
    const char *p = line;
    char err_file[FILENAME_MAX];
    int err_line = -1;
    int parsed = 0;

    /* Try to parse file:digits: pattern */
    /* Skip optional drive letter on Windows (e.g. C:) */
    if (p[0] && p[1] == ':' && ((p[0] >= 'A' && p[0] <= 'Z') ||
        (p[0] >= 'a' && p[0] <= 'z'))) {
      p += 2;
    }
    {
      const char *colon1 = strchr(p, ':');
      if (colon1 && colon1 > p) {
        const char *d = colon1 + 1;
        int num = 0;
        int has_digits = 0;
        while (*d >= '0' && *d <= '9') {
          num = num * 10 + (*d - '0');
          has_digits = 1;
          d++;
        }
        if (has_digits && *d == ':') {
          size_t flen = (size_t)(colon1 - line);
          if (flen < FILENAME_MAX) {
            memcpy(err_file, line, flen);
            err_file[flen] = '\0';
            err_line = num;
            parsed = 1;
          }
        }
      }
    }

    fputs(line, stdout);

    if (parsed) {
      MarkerFrame mf;
      if (reverse_lookup_in_file(err_file, err_line, &mf)) {
        int offset = err_line - mf.start_line;
        printf("  [weft: %s:%d, scrap %d",
               mf.file, mf.line + offset, mf.scrap);
        printf("]\n");
      }
    }
  }
}
/* {:253} */

#line 17 "literate/output-files.weft"
/* {256: literate/output-files.weft:17} */
static void make_parent_dirs(const char *path);   /* defined below */

void write_files(Name *files)
{
  while (files) {
    write_files(files->llink);
    /* Write out \verb|files->spelling| */
    
#line 81 "literate/output-files.weft"
    /* {259: literate/output-files.weft:81} */
{
      static char temp_name[FILENAME_MAX];
      static char real_name[FILENAME_MAX];
      static int temp_name_count = 0;
      char indent_chars[MAX_INDENT];
      FILE *temp_file;

      sprintf(real_name, "%s%s%s", dirpath, path_sep, files->spelling);
      if (mkdirs_flag)
        make_parent_dirs(real_name);

      /* Find a free temporary file */
      
#line 105 "literate/output-files.weft"
/* {260: literate/output-files.weft:105} */

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
      /* {:260} */

#line 92 "literate/output-files.weft"


      if (verbose_flag)
        fprintf(stderr, "writing %s [%s]\n", files->spelling, temp_name);
      write_scraps(temp_file, files->spelling, files->defs, 0, indent_chars,
                   files->debug_flag, files->tab_flag, files->indent_flag,
                   files->comment_flag, files->location_flag,
                   NULL, NULL, 0, files->spelling);
      fclose(temp_file);

      /* Move the temporary file to the target, if required */
      
#line 135 "literate/output-files.weft"
/* {261: literate/output-files.weft:135} */

      if (diff_flag) {
        /* Show diff of temp file vs real file */
        
#line 147 "literate/output-files.weft"
        /* {262: literate/output-files.weft:147} */
{
          FILE *old_file = fopen(real_name, "r");
          if (!old_file) {
            /* New file: show all lines with + prefix */
            FILE *tf = fopen(temp_name, "r");
            if (tf) {
              char dbuf[8192];
              printf("new file: %s\n", real_name);
              while (fgets(dbuf, sizeof(dbuf), tf))
                printf("+%s", dbuf);
              fclose(tf);
            }
          } else {
            char cmd[FILENAME_MAX * 2 + 32];
            FILE *dp;
            fclose(old_file);
            sprintf(cmd, "diff -u \"%s\" \"%s\"", real_name, temp_name);
            dp = popen(cmd, "r");
            if (dp) {
              char dbuf[8192];
              while (fgets(dbuf, sizeof(dbuf), dp))
                fputs(dbuf, stdout);
              pclose(dp);
            }
          }
          remove(temp_name);
        }/* {:262} */

#line 137 "literate/output-files.weft"

      } else if (compare_flag)
        /* Compare the temp file and the old file */
        
#line 177 "literate/output-files.weft"
        /* {263: literate/output-files.weft:177} */
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
              
#line 199 "literate/output-files.weft"
/* {264: literate/output-files.weft:199} */

              if (0 != rename(temp_name, real_name)) {
                fprintf(stderr, "%s: can't rename output file to %s\n",
                        command_name, real_name);
                if (mkdirs_flag == FALSE)
                  fprintf(stderr, "  (does its directory exist? try --mkdirs)\n");
                remove(temp_name);
                exit(1);
              }
              /* {:264} */

#line 192 "literate/output-files.weft"

            }
          }
          else
            /* Rename the temporary file to the target */
            
#line 199 "literate/output-files.weft"
/* {264: literate/output-files.weft:199} */

            if (0 != rename(temp_name, real_name)) {
              fprintf(stderr, "%s: can't rename output file to %s\n",
                      command_name, real_name);
              if (mkdirs_flag == FALSE)
                fprintf(stderr, "  (does its directory exist? try --mkdirs)\n");
              remove(temp_name);
              exit(1);
            }
            /* {:264} */

#line 196 "literate/output-files.weft"

        }/* {:263} */

#line 139 "literate/output-files.weft"

      else {
        remove(real_name);
        /* Rename the temporary file to the target */
        
#line 199 "literate/output-files.weft"
/* {264: literate/output-files.weft:199} */

        if (0 != rename(temp_name, real_name)) {
          fprintf(stderr, "%s: can't rename output file to %s\n",
                  command_name, real_name);
          if (mkdirs_flag == FALSE)
            fprintf(stderr, "  (does its directory exist? try --mkdirs)\n");
          remove(temp_name);
          exit(1);
        }
        /* {:264} */

#line 142 "literate/output-files.weft"

      }
      /* {:261} */

#line 102 "literate/output-files.weft"

    }/* {:259} */

#line 23 "literate/output-files.weft"

    files = files->rlink;
  }
}
/* {:256} */

#line 54 "literate/output-files.weft"
/* {258: literate/output-files.weft:54} */
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define WEFT_MKDIR(p) _mkdir(p)
#else
#define WEFT_MKDIR(p) mkdir((p), 0777)
#endif

static void make_parent_dirs(const char *path)
{
  char buf[FILENAME_MAX];
  char *p;
  if (strlen(path) >= sizeof(buf))
    return;
  strcpy(buf, path);
  for (p = buf + 1; *p != '\0'; p++) {
    if (*p == path_sep[0]) {
      *p = '\0';
      if (buf[0] != '\0')
        (void) WEFT_MKDIR(buf);
      *p = path_sep[0];
    }
  }
}
/* {:258} */
