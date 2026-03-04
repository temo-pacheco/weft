
#line 128 "literate/architecture.weft"
/* {12: literate/architecture.weft:128} */
#include "global.h"
/* {:12} */

#line 90 "literate/names.weft"
/* {302: literate/names.weft:90} */
enum { LESS, GREATER, EQUAL, PREFIX, EXTENSION };

static int compare(char *x, char *y)
{
  int len, result;
  int xl = strlen(x);
  int yl = strlen(y);
  int xp = x[xl - 1] == ' ';
  int yp = y[yl - 1] == ' ';
  if (xp) xl--;
  if (yp) yl--;
  len = xl < yl ? xl : yl;
  result = strncmp(x, y, len);
  if (result < 0) return GREATER;
  else if (result > 0) return LESS;
  else if (xl < yl) {
    if (xp) return EXTENSION;
    else return LESS;
  }
  else if (xl > yl) {
    if (yp) return PREFIX;
    else return GREATER;
  }
  else return EQUAL;
}
/* {:302} */

#line 119 "literate/names.weft"
/* {303: literate/names.weft:119} */
char *save_string(char *s)
{
  char *new_str = (char *) arena_getmem((strlen(s) + 1) * sizeof(char));
  strcpy(new_str, s);
  return new_str;
}
/* {:303} */

#line 128 "literate/names.weft"
/* {304: literate/names.weft:128} */
static int ambiguous_prefix(Name *node, char *spelling, unsigned char sector);

static char * found_name = NULL;

Name *prefix_add(Name **rt, char *spelling, unsigned char sector)
{
  Name *node = *rt;
  int cmp;

  while (node) {
    switch ((cmp = compare(node->spelling, spelling))) {
    case GREATER:   rt = &node->rlink;
                    break;
    case LESS:      rt = &node->llink;
                    break;
    case EQUAL:
                    found_name = node->spelling;
    case EXTENSION: if (node->sector > sector) {
                       rt = &node->rlink;
                       break;
                    }
                    else if (node->sector < sector) {
                       rt = &node->llink;
                       break;
                    }
                    if (cmp == EXTENSION)
                       node->spelling = save_string(spelling);
                    return node;
    case PREFIX:    
#line 170 "literate/names.weft"
                    /* {305: literate/names.weft:170} */
{
                      if (ambiguous_prefix(node->llink, spelling, sector) ||
                          ambiguous_prefix(node->rlink, spelling, sector))
                        fprintf(stderr,
                                "%s: ambiguous prefix %c<%s...%c> (%s, line %d)\n",
                                command_name, nw_char, spelling, nw_char, source_name, source_line);
                    }/* {:305} */

#line 156 "literate/names.weft"

                    return node;
    }
    node = *rt;
  }
  /* Create new name entry */
  
#line 293 "literate/names.weft"
  /* {311: literate/names.weft:293} */
{
    node = (Name *) arena_getmem(sizeof(Name));
    if (found_name && robs_strcmp(found_name, spelling) == 0)
       node->spelling = found_name;
    else
       node->spelling = save_string(spelling);
    node->mark = FALSE;
    node->llink = NULL;
    node->rlink = NULL;
    node->uses = NULL;
    node->defs = NULL;
    node->arg[0] =
    node->arg[1] =
    node->arg[2] =
    node->arg[3] =
    node->arg[4] =
    node->arg[5] =
    node->arg[6] =
    node->arg[7] =
    node->arg[8] = NULL;
    node->tab_flag = TRUE;
    node->indent_flag = TRUE;
    node->debug_flag = FALSE;
    node->location_flag = FALSE;
    node->suppress_markers = FALSE;
    node->comment_flag = 0;
    node->lang[0] = '\0';
    node->sector = sector;
    *rt = node;
    return node;
  }/* {:311} */

#line 161 "literate/names.weft"

}
/* {:304} */

#line 179 "literate/names.weft"
/* {306: literate/names.weft:179} */
static int ambiguous_prefix(Name *node, char *spelling, unsigned char sector)
{
  while (node) {
    switch (compare(node->spelling, spelling)) {
    case GREATER:   node = node->rlink;
                    break;
    case LESS:      node = node->llink;
                    break;
    case EXTENSION:
    case PREFIX:
    case EQUAL:     if (node->sector > sector) {
                       node = node->rlink;
                       break;
                    }
                    else if (node->sector < sector) {
                       node = node->llink;
                       break;
                    }
                    return TRUE;
    }
  }
  return FALSE;
}
/* {:306} */

#line 227 "literate/names.weft"
/* {308: literate/names.weft:227} */
int robs_strcmp(char* x, char* y)
{
   int cmp = 0;

   for (; *x && *y; x++, y++)
   {
      /* Skip invisibles on 'x' */
      
#line 261 "literate/names.weft"
      /* {309: literate/names.weft:261} */
if (*x == '|')
         x++;
      /* {:309} */

#line 233 "literate/names.weft"

      /* Skip invisibles on 'y' */
      
#line 261 "literate/names.weft"
      /* {309: literate/names.weft:261} */
if (*y == '|')
         y++;
      /* {:309} */

#line 234 "literate/names.weft"

      if (*x == *y)
         continue;
      if (islower(*x) && toupper(*x) == *y)
      {
         if (!cmp) cmp = 1;
         continue;
      }
      if (islower(*y) && *x == toupper(*y))
      {
         if (!cmp) cmp = -1;
         continue;
      }
      return 2*(toupper(*x) - toupper(*y));
   }
   if (*x)
      return 2;
   if (*y)
      return -2;
   return cmp;
}
/* {:308} */

#line 266 "literate/names.weft"
/* {310: literate/names.weft:266} */
Name *name_add(Name **rt, char *spelling, unsigned char sector)
{
  Name *node = *rt;
  while (node) {
    int result = robs_strcmp(node->spelling, spelling);
    if (result > 0)
      rt = &node->llink;
    else if (result < 0)
      rt = &node->rlink;
    else
    {
       found_name = node->spelling;
       if (node->sector > sector)
         rt = &node->llink;
       else if (node->sector < sector)
         rt = &node->rlink;
       else
         return node;
    }
    node = *rt;
  }
  /* Create new name entry */
  
#line 293 "literate/names.weft"
  /* {311: literate/names.weft:293} */
{
    node = (Name *) arena_getmem(sizeof(Name));
    if (found_name && robs_strcmp(found_name, spelling) == 0)
       node->spelling = found_name;
    else
       node->spelling = save_string(spelling);
    node->mark = FALSE;
    node->llink = NULL;
    node->rlink = NULL;
    node->uses = NULL;
    node->defs = NULL;
    node->arg[0] =
    node->arg[1] =
    node->arg[2] =
    node->arg[3] =
    node->arg[4] =
    node->arg[5] =
    node->arg[6] =
    node->arg[7] =
    node->arg[8] = NULL;
    node->tab_flag = TRUE;
    node->indent_flag = TRUE;
    node->debug_flag = FALSE;
    node->location_flag = FALSE;
    node->suppress_markers = FALSE;
    node->comment_flag = 0;
    node->lang[0] = '\0';
    node->sector = sector;
    *rt = node;
    return node;
  }/* {:311} */

#line 287 "literate/names.weft"

}
/* {:310} */

#line 354 "literate/names.weft"
/* {312: literate/names.weft:354} */
typedef struct {
  const char *name;
  unsigned char comment;
  char line_directive;
} LangEntry;

static const LangEntry lang_table[] = {
  /* ---- Style 1: C block comments WITH #line ---- */
  {"c",            1, TRUE},
  {"h",            1, TRUE},
  {"hpp",          2, TRUE},
  {"hxx",          2, TRUE},
  {"hh",           2, TRUE},
  {"cpp",          2, TRUE},
  {"c++",          2, TRUE},
  {"cxx",          2, TRUE},
  {"cc",           2, TRUE},
  {"objc",         1, TRUE},
  {"objcpp",       2, TRUE},
  {"m",            1, TRUE},
  {"mm",           2, TRUE},
  {"ino",          2, TRUE},   /* Arduino */
  {"pde",          2, TRUE},   /* Processing (C-like) */
  /* ---- Style 1: C block comments WITHOUT #line ---- */
  {"css",          1, FALSE},
  {"scss",         1, FALSE},
  {"sass",         1, FALSE},
  {"less",         1, FALSE},
  {"stylus",       1, FALSE},
  {"styl",         1, FALSE},
  /* ---- Style 2: C++ line comments (//) ---- */
  /* --- JavaScript / TypeScript --- */
  {"js",           2, FALSE},
  {"javascript",   2, FALSE},
  {"jsx",          2, FALSE},
  {"mjs",          2, FALSE},
  {"cjs",          2, FALSE},
  {"ts",           2, FALSE},
  {"typescript",   2, FALSE},
  {"tsx",          2, FALSE},
  {"mts",          2, FALSE},
  {"cts",          2, FALSE},
  {"svelte",       2, FALSE},
  {"vue",          2, FALSE},
  /* --- JVM languages --- */
  {"java",         2, FALSE},
  {"scala",        2, FALSE},
  {"sc",           2, FALSE},
  {"kotlin",       2, FALSE},
  {"kt",           2, FALSE},
  {"kts",          2, FALSE},
  {"groovy",       2, FALSE},
  {"gradle",       2, FALSE},
  {"clj",          2, FALSE},  /* Clojure (;; but // is close enough) */
  {"cljs",         2, FALSE},
  {"cljc",         2, FALSE},
  /* --- .NET --- */
  {"cs",           2, FALSE},
  {"csharp",       2, FALSE},
  {"fs",           2, FALSE},
  {"fsharp",       2, FALSE},
  {"fsx",          2, FALSE},
  {"vb",           2, FALSE},  /* VB.NET uses ' but // is a safe fallback */
  /* --- Systems languages --- */
  {"go",           2, FALSE},
  {"rust",         2, FALSE},
  {"rs",           2, FALSE},
  {"zig",          2, FALSE},
  {"d",            2, FALSE},
  {"v",            2, FALSE},
  {"vlang",        2, FALSE},
  {"swift",        2, FALSE},
  {"dart",         2, FALSE},
  {"odin",         2, FALSE},
  {"jai",          2, FALSE},
  {"carbon",       2, FALSE},
  {"mojo",         2, FALSE},
  /* --- Web / general --- */
  {"php",          2, FALSE},
  {"hack",         2, FALSE},
  {"jsonc",        2, FALSE},
  {"json5",        2, FALSE},
  {"graphql",      2, FALSE},
  {"gql",          2, FALSE},
  {"prisma",       2, FALSE},
  {"wgsl",         2, FALSE},  /* WebGPU shading language */
  /* --- GPU / shaders --- */
  {"glsl",         2, FALSE},
  {"hlsl",         2, FALSE},
  {"vert",         2, FALSE},
  {"frag",         2, FALSE},
  {"comp",         2, FALSE},
  {"geom",         2, FALSE},
  {"tesc",         2, FALSE},
  {"tese",         2, FALSE},
  {"metal",        2, FALSE},
  {"cu",           2, TRUE},   /* CUDA: supports #line */
  {"cuh",          2, TRUE},
  {"cl",           1, FALSE},  /* OpenCL: C-style block comments */
  /* --- Protocol / IDL / blockchain --- */
  {"proto",        2, FALSE},
  {"protobuf",     2, FALSE},
  {"thrift",       2, FALSE},
  {"flatbuffers",  2, FALSE},
  {"fbs",          2, FALSE},
  {"capnp",       2, FALSE},
  {"avdl",         2, FALSE},  /* Avro IDL */
  {"solidity",     2, FALSE},
  {"sol",          2, FALSE},
  {"vyper",        3, FALSE},
  {"vy",           3, FALSE},
  {"move",         2, FALSE},
  /* --- Functional (// style) --- */
  {"reason",       2, FALSE},
  {"re",           2, FALSE},
  {"res",          2, FALSE},  /* ReScript */
  {"ml",           2, FALSE},  /* OCaml also uses (* *) but // in comments */
  {"mli",          2, FALSE},
  {"sml",          2, FALSE},
  {"sig",          2, FALSE},
  {"fut",          2, FALSE},  /* Futhark */
  /* --- Other // languages --- */
  {"actionscript", 2, FALSE},
  {"as",           2, FALSE},
  {"apex",         2, FALSE},
  {"bicep",        2, FALSE},
  {"ceu",          2, FALSE},
  {"chapel",       2, FALSE},
  {"chpl",         2, FALSE},
  {"cilk",         2, TRUE},
  {"jsonnet",      2, FALSE},
  {"libsonnet",    2, FALSE},
  {"p4",           2, FALSE},
  {"qml",          2, FALSE},
  {"slang",        2, FALSE},
  /* ---- Style 3: Shell / hash comments (#) ---- */
  /* --- Python --- */
  {"py",           3, FALSE},
  {"python",       3, FALSE},
  {"pyx",          3, FALSE},  /* Cython */
  {"pxd",          3, FALSE},
  {"pyi",          3, FALSE},  /* type stubs */
  {"sage",         3, FALSE},  /* SageMath */
  /* --- Shell --- */
  {"sh",           3, FALSE},
  {"bash",         3, FALSE},
  {"zsh",          3, FALSE},
  {"fish",         3, FALSE},
  {"ksh",          3, FALSE},
  {"csh",          3, FALSE},
  {"tcsh",         3, FALSE},
  {"dash",         3, FALSE},
  {"nu",           3, FALSE},  /* Nushell */
  /* --- Classic scripting --- */
  {"perl",         3, FALSE},
  {"pl",           3, FALSE},
  {"pm",           3, FALSE},
  {"ruby",         3, FALSE},
  {"rb",           3, FALSE},
  {"rake",         3, FALSE},
  {"gemspec",      3, FALSE},
  {"r",            3, FALSE},
  {"rmd",          3, FALSE},
  {"tcl",          3, FALSE},
  {"awk",          3, FALSE},
  {"sed",          3, FALSE},
  /* --- Build / make --- */
  {"makefile",     3, FALSE},
  {"make",         3, FALSE},
  {"mk",           3, FALSE},
  {"cmake",        3, FALSE},
  {"just",         3, FALSE},
  {"justfile",     3, FALSE},
  {"meson",        3, FALSE},
  {"scons",        3, FALSE},
  {"bazel",        3, FALSE},
  {"bzl",          3, FALSE},
  {"buck",         3, FALSE},
  {"pants",        3, FALSE},
  {"gnumakefile",  3, FALSE},
  /* --- Scientific / modern --- */
  {"julia",        3, FALSE},
  {"jl",           3, FALSE},
  {"nim",          3, FALSE},
  {"nimble",       3, FALSE},
  {"crystal",      3, FALSE},
  {"cr",           3, FALSE},
  {"elixir",       3, FALSE},
  {"ex",           3, FALSE},
  {"exs",          3, FALSE},
  {"coco",         3, FALSE},  /* Coconut (Python superset) */
  {"coffee",       3, FALSE},  /* CoffeeScript */
  {"litcoffee",    3, FALSE},
  /* --- Config / DevOps --- */
  {"yaml",         3, FALSE},
  {"yml",          3, FALSE},
  {"toml",         3, FALSE},
  {"dockerfile",   3, FALSE},
  {"docker",       3, FALSE},
  {"containerfile",3, FALSE},
  {"terraform",    3, FALSE},
  {"tf",           3, FALSE},
  {"tfvars",       3, FALSE},
  {"hcl",          3, FALSE},
  {"nix",          3, FALSE},
  {"conf",         3, FALSE},
  {"ini",          3, FALSE},
  {"properties",   3, FALSE},
  {"env",          3, FALSE},
  {"editorconfig", 3, FALSE},
  {"gitignore",    3, FALSE},
  {"dockerignore", 3, FALSE},
  {"flake8",       3, FALSE},
  {"pylintrc",     3, FALSE},
  {"powershell",   3, FALSE},
  {"ps1",          3, FALSE},
  {"psm1",         3, FALSE},
  {"psd1",         3, FALSE},
  /* --- Hardware description (# style) --- */
  {"tcl",          3, FALSE},
  {"xdc",          3, FALSE},  /* Xilinx constraints */
  {"sdc",          3, FALSE},  /* Synopsys constraints */
  /* --- Data / query (# style) --- */
  {"gn",           3, FALSE},  /* GN build */
  {"cfg",          3, FALSE},
  {"pip",          3, FALSE},
  {"cask",         3, FALSE},  /* Homebrew */
  {"gemfile",      3, FALSE},
  {"vagrantfile",  3, FALSE},
  {"procfile",     3, FALSE},
  /* ---- Style 4: double-dash comments (--) ---- */
  {"lua",          4, FALSE},
  {"luau",         4, FALSE},  /* Roblox Lua */
  {"sql",          4, FALSE},
  {"psql",         4, FALSE},
  {"plsql",        4, FALSE},
  {"plpgsql",      4, FALSE},
  {"mysql",        4, FALSE},
  {"sqlite",       4, FALSE},
  {"hive",         4, FALSE},
  {"presto",       4, FALSE},
  {"sparksql",     4, FALSE},
  {"hs",           4, FALSE},
  {"haskell",      4, FALSE},
  {"lhs",          4, FALSE},
  {"elm",          4, FALSE},
  {"purescript",   4, FALSE},
  {"purs",         4, FALSE},
  {"ada",          4, FALSE},
  {"adb",          4, FALSE},
  {"ads",          4, FALSE},
  {"vhdl",         4, FALSE},
  {"vhd",          4, FALSE},
  {"lean",         4, FALSE},
  {"lean4",        4, FALSE},
  {"agda",         4, FALSE},
  {"idr",          4, FALSE},  /* Idris */
  {"idris",        4, FALSE},
  {"erl",          4, FALSE},  /* Erlang uses % but -- is common in embedded SQL */
  {"erlang",       4, FALSE},
  /* ---- Style 5: HTML comments (<!-- -->) ---- */
  {"html",         5, FALSE},
  {"htm",          5, FALSE},
  {"xml",          5, FALSE},
  {"svg",          5, FALSE},
  {"xhtml",        5, FALSE},
  {"xsl",          5, FALSE},
  {"xslt",         5, FALSE},
  {"xsd",          5, FALSE},
  {"dtd",          5, FALSE},
  {"rss",          5, FALSE},
  {"atom",         5, FALSE},
  {"mathml",       5, FALSE},
  {"plist",        5, FALSE},
  {"xaml",         5, FALSE},
  {"csproj",       5, FALSE},
  {"fsproj",       5, FALSE},
  {"vbproj",       5, FALSE},
  {"vcxproj",      5, FALSE},
  {"sln",          5, FALSE},
  {"props",        5, FALSE},
  {"targets",      5, FALSE},
  {"nuspec",       5, FALSE},
  {"resx",         5, FALSE},
  {"wxml",         5, FALSE},
  {"axml",         5, FALSE},
  {"jspx",         5, FALSE},
  {"jsp",          5, FALSE},
  {"asp",          5, FALSE},
  {"ascx",         5, FALSE},
  {"aspx",         5, FALSE},
  {"cshtml",       5, FALSE},  /* Razor */
  {"vbhtml",       5, FALSE},
  {"erb",          5, FALSE},
  {"ejs",          5, FALSE},
  {"njk",          5, FALSE},  /* Nunjucks */
  {"hbs",          5, FALSE},  /* Handlebars */
  {"mustache",     5, FALSE},
  {"liquid",       5, FALSE},
  {"twig",         5, FALSE},
  {"blade",        5, FALSE},  /* Laravel Blade */
  {"pug",          5, FALSE},  /* Pug/Jade */
  {"slim",         5, FALSE},
  {"haml",         5, FALSE},
  {"md",           5, FALSE},  /* Markdown: HTML comments are valid */
  {"markdown",     5, FALSE},
  {"mdx",          5, FALSE},
  {"rst",          5, FALSE},  /* reStructuredText (.. is native but <!-- works) */
  {"tex",          5, FALSE},  /* LaTeX: % is native but no style for it */
  {"latex",        5, FALSE},
  /* ---- Style 0: no comments (data formats) ---- */
  {"json",         0, FALSE},
  {"json5",        0, FALSE},
  {"csv",          0, FALSE},
  {"tsv",          0, FALSE},
  {"txt",          0, FALSE},
  {"text",         0, FALSE},
  {"log",          0, FALSE},
  {"diff",         0, FALSE},
  {"patch",        0, FALSE},
  /* Sentinel */
  {NULL,           0, FALSE}
};

/* User-defined languages via @L command */
#define MAX_USER_LANGS 32
static LangEntry user_langs[MAX_USER_LANGS];
static char user_lang_names[MAX_USER_LANGS][64];
static int user_lang_count = 0;

static const LangEntry *find_lang(const char *name)
{
  int i;
  const LangEntry *p;
  /* User-defined languages override built-in */
  for (i = 0; i < user_lang_count; i++)
    if (strcmp(user_langs[i].name, name) == 0) return &user_langs[i];
  for (p = lang_table; p->name; p++)
    if (strcmp(p->name, name) == 0) return p;
  return NULL;
}

static void lang_to_flags(const char *lang,
                           unsigned char *comment_flag,
                           char *debug_flag,
                           char *location_flag)
{
  const LangEntry *e = find_lang(lang);
  if (e) {
    *comment_flag = e->comment;
    *debug_flag = e->line_directive;
    *location_flag = TRUE;
  } else {
    /* Unknown language: default to // comments, no #line */
    *comment_flag = 2;
    *debug_flag = FALSE;
    *location_flag = TRUE;
  }
}
/* {:312} */

#line 752 "literate/names.weft"
/* {313: literate/names.weft:752} */
void collect_lang_def(void)
{
  char name[64];
  char *p = name;
  char style[16];
  char *sp;
  int c;
  unsigned char comment = 0;
  char line_dir = FALSE;

  /* Skip whitespace after @L */
  c = source_get();
  while (c == ' ' || c == '\t')
    c = source_get();

  /* Read language name */
  while (c != EOF && c != ' ' && c != '\t' && c != '\n'
         && (p - name) < 62) {
    *p++ = c;
    c = source_get();
  }
  *p = '\0';

  if (name[0] == '\0') {
    fprintf(stderr, "%s: expected language name after %cL (%s, %d)\n",
            command_name, nw_char, source_name, source_line);
    while (c != EOF && c != '\n') c = source_get();
    return;
  }

  /* Skip whitespace */
  while (c == ' ' || c == '\t')
    c = source_get();

  /* Read comment style token */
  sp = style;
  while (c != EOF && c != ' ' && c != '\t' && c != '\n'
         && (sp - style) < 14) {
    *sp++ = c;
    c = source_get();
  }
  *sp = '\0';

  /* Map style string to comment index */
  if (strcmp(style, "//") == 0)        comment = 2;
  else if (strcmp(style, "#") == 0)    comment = 3;
  else if (strcmp(style, "--") == 0)   comment = 4;
  else if (strcmp(style, "/*") == 0)   comment = 1;
  else if (strcmp(style, "<!--") == 0) comment = 5;
  else {
    fprintf(stderr,
            "%s: unknown comment style '%s' for %cL (%s, %d)\n",
            command_name, style, nw_char, source_name, source_line);
    fprintf(stderr, "  Valid styles: // # -- /* <!--\n");
    while (c != EOF && c != '\n') c = source_get();
    return;
  }

  /* Check for optional +d flag (#line directives) */
  while (c == ' ' || c == '\t')
    c = source_get();
  if (c == '+') {
    c = source_get();
    if (c == 'd') line_dir = TRUE;
  }

  /* Consume rest of line */
  while (c != EOF && c != '\n') c = source_get();

  /* Register the user language */
  if (user_lang_count < MAX_USER_LANGS) {
    strncpy(user_lang_names[user_lang_count], name, 63);
    user_lang_names[user_lang_count][63] = '\0';
    user_langs[user_lang_count].name = user_lang_names[user_lang_count];
    user_langs[user_lang_count].comment = comment;
    user_langs[user_lang_count].line_directive = line_dir;
    user_lang_count++;
  } else {
    fprintf(stderr, "%s: too many %cL definitions (max %d) (%s, %d)\n",
            command_name, nw_char, MAX_USER_LANGS,
            source_name, source_line);
  }
}
/* {:313} */

#line 845 "literate/names.weft"
/* {314: literate/names.weft:845} */
void collect_weave_format(void)
{
  char token[64];
  char *p = token;
  int c;

  /* Skip whitespace after @W */
  c = source_get();
  while (c == ' ' || c == '\t')
    c = source_get();

  /* Read format token */
  while (c != EOF && c != ' ' && c != '\t' && c != '\n'
         && (p - token) < 62) {
    *p++ = c;
    c = source_get();
  }
  *p = '\0';

  /* Consume rest of line */
  while (c != EOF && c != '\n') c = source_get();

  if (token[0] == '\0') {
    fprintf(stderr, "%s: expected format after %cW (md or tex) (%s, %d)\n",
            command_name, nw_char, source_name, source_line);
    return;
  }

  /* If CLI already set the format, skip silently */
  if (weave_format_from_cli)
    return;

  if (strcmp(token, "md") == 0)
    weave_format = 2;
  else if (strcmp(token, "tex") == 0)
    weave_format = 1;
  else {
    fprintf(stderr, "%s: unknown weave format '%s' for %cW (%s, %d)\n",
            command_name, token, nw_char, source_name, source_line);
    fprintf(stderr, "  Valid formats: md tex\n");
  }
}
/* {:314} */

#line 897 "literate/names.weft"
/* {315: literate/names.weft:897} */
Name *collect_file_name(void)
{
  Name *new_name;
  char name[MAX_NAME_LEN];
  char *p = name;
  int start_line = source_line;
  int c = source_get(), c2;
  while (isspace(c))
    c = source_get();
  while (isgraph(c)) {
    *p++ = c;
    c = source_get();
  }
  if (p == name) {
    fprintf(stderr, "%s: expected file name (%s, %d)\n",
            command_name, source_name, start_line);
    exit(-1);
  }
  *p = '\0';
  /* File names are always global. */
  new_name = name_add(&file_names, name, 0);
  /* Handle optional per-file flags */
  
#line 931 "literate/names.weft"
  /* {316: literate/names.weft:931} */
{
    while (1) {
      while (isspace(c))
        c = source_get();
      if (c == '-') {
        c = source_get();
        do {
          switch (c) {
            case 't': new_name->tab_flag = FALSE;
                      break;
            case 'd': new_name->debug_flag = TRUE;
                      break;
            case 'i': new_name->indent_flag = FALSE;
                      break;
            case 's': new_name->suppress_markers = TRUE;
                      break;
            case 'c': 
#line 972 "literate/names.weft"
                      /* {317: literate/names.weft:972} */
c = source_get();
                      if (c == 'c')
                         new_name->comment_flag = 1;
                      else if (c == '+')
                         new_name->comment_flag = 2;
                      else if (c == 'p')
                         new_name->comment_flag = 3;
                      else
                         fprintf(stderr, "%s: Unrecognised comment flag (%s, %d)\n",
                                 command_name, source_name, source_line);
                      /* {:317} */

#line 947 "literate/names.weft"

                      break;
            default : fprintf(stderr, "%s: unexpected per-file flag (%s, %d)\n",
                              command_name, source_name, source_line);
                      break;
          }
          c = source_get();
        } while (!isspace(c));
      }
      else break;
    }
  }/* {:316} */

#line 918 "literate/names.weft"

  /* Apply pending language if no explicit flags */
  
#line 988 "literate/names.weft"
  /* {318: literate/names.weft:988} */
if (pending_lang[0] != '\0') {
    strncpy(new_name->lang, pending_lang, 63);
    new_name->lang[63] = '\0';
    if (new_name->comment_flag == 0 && !new_name->debug_flag) {
      lang_to_flags(pending_lang,
                    &new_name->comment_flag,
                    &new_name->debug_flag,
                    &new_name->location_flag);
    }
    pending_lang[0] = '\0';
  } else if (new_name->lang[0] == '\0') {
    const char *dot = strrchr(new_name->spelling, '.');
    if (dot && dot[1]) {
      const LangEntry *e = find_lang(dot + 1);
      if (e) {
        strncpy(new_name->lang, dot + 1, 63);
        new_name->lang[63] = '\0';
        if (new_name->comment_flag == 0 && !new_name->debug_flag) {
          lang_to_flags(new_name->lang,
                        &new_name->comment_flag,
                        &new_name->debug_flag,
                        &new_name->location_flag);
        }
      }
    }
  }
  if (new_name->suppress_markers) {
    new_name->debug_flag = FALSE;
    new_name->location_flag = FALSE;
  }/* {:318} */

#line 919 "literate/names.weft"

  c2 = source_get();
  if (c != nw_char || (c2 != '{' && c2 != '(' && c2 != '[')) {
    fprintf(stderr, "%s: expected %c{, %c[, or %c( after file name (%s, %d)\n",
            command_name, nw_char, nw_char, nw_char, source_name, start_line);
    exit(-1);
  }
  return new_name;
}
/* {:315} */

#line 1066 "literate/names.weft"
/* {321: literate/names.weft:1066} */
Name *collect_macro_name(void)
{
  char name[MAX_NAME_LEN];
  char args[1000];
  char * arg[9];
  char * argp = args;
  int argc = 0;
  char *p = name;
  int start_line = source_line;
  int c = source_get(), c2;
  unsigned char sector = current_sector;

  if (c == '+') {
    sector = 0;
    c = source_get();
  }
  while (isspace(c))
    c = source_get();
  while (c != EOF) {
    Name * node;
    switch (c) {
      case '\t':
      case ' ':  *p++ = ' ';
                 do
                   c = source_get();
                 while (c == ' ' || c == '\t');
                 break;
      case '\n': 
#line 1185 "literate/names.weft"
                 /* {327: literate/names.weft:1185} */
{
                   do
                     c = source_get();
                   while (isspace(c));
                   c2 = source_get();
                   if (c != nw_char || (c2 != '{' && c2 != '(' && c2 != '[')) {
                     fprintf(stderr, "%s: expected %c{ after fragment name (%s, %d)\n",
                             command_name, nw_char, source_name, start_line);
                     exit(-1);
                   }
                   /* Cleanup and install name */
                   
#line 1168 "literate/names.weft"
                   /* {326: literate/names.weft:1168} */
{
                     if (p > name && p[-1] == ' ')
                       p--;
                     if (p - name > 3 && p[-1] == '.' && p[-2] == '.' && p[-3] == '.') {
                       p[-3] = ' ';
                       p -= 2;
                     }
                     if (p == name || name[0] == ' ') {
                       fprintf(stderr, "%s: empty name (%s, %d)\n",
                               command_name, source_name, source_line);
                       exit(-1);
                     }
                     *p = '\0';
                     node = prefix_add(&macro_names, name, sector);
                   }/* {:326} */

#line 1195 "literate/names.weft"

                   /* Apply pending language to macro */
                   
#line 1024 "literate/names.weft"
                   /* {319: literate/names.weft:1024} */
if (pending_lang[0] != '\0') {
                     strncpy(node->lang, pending_lang, 63);
                     node->lang[63] = '\0';
                     pending_lang[0] = '\0';
                   }/* {:319} */

#line 1196 "literate/names.weft"

                   return install_args(node, argc, arg);
                 }/* {:327} */

#line 1093 "literate/names.weft"

      default:
         if (c==nw_char)
           {
             /* Check for terminating at-sequence and return name */
             
#line 1114 "literate/names.weft"
             /* {322: literate/names.weft:1114} */
{
               c = source_get();
               switch (c) {
                 case '(':
                 case '[':
                 case '{': 
#line 1168 "literate/names.weft"
                           /* {326: literate/names.weft:1168} */
{
                             if (p > name && p[-1] == ' ')
                               p--;
                             if (p - name > 3 && p[-1] == '.' && p[-2] == '.' && p[-3] == '.') {
                               p[-3] = ' ';
                               p -= 2;
                             }
                             if (p == name || name[0] == ' ') {
                               fprintf(stderr, "%s: empty name (%s, %d)\n",
                                       command_name, source_name, source_line);
                               exit(-1);
                             }
                             *p = '\0';
                             node = prefix_add(&macro_names, name, sector);
                           }/* {:326} */

#line 1119 "literate/names.weft"

                          /* Apply pending language to macro */
                          
#line 1024 "literate/names.weft"
                          /* {319: literate/names.weft:1024} */
if (pending_lang[0] != '\0') {
                            strncpy(node->lang, pending_lang, 63);
                            node->lang[63] = '\0';
                            pending_lang[0] = '\0';
                          }/* {:319} */

#line 1120 "literate/names.weft"

                          return install_args(node, argc, arg);
                 case '\'': 
#line 1138 "literate/names.weft"
                            /* {323: literate/names.weft:1138} */
arg[argc] = argp;
                            while ((c = source_get()) != EOF) {
                               if (c==nw_char) {
                                  c2 = source_get();
                                  if (c2=='\'') {
                                    /* Make this argument */
                                    
#line 1161 "literate/names.weft"
                                    /* {325: literate/names.weft:1161} */
if (argc < 9) {
                                      *argp++ = '\000';
                                      argc += 1;
                                    }
                                    /* {:325} */

#line 1143 "literate/names.weft"

                                    c = source_get();
                                    break;
                                  }
                                  else
                                    *argp++ = c2;
                               }
                               else
                                 *argp++ = c;
                            }
                            *p++ = ARG_CHR;
                            /* {:323} */

#line 1122 "literate/names.weft"

                            break;
                 default:
                       if (c==nw_char)
                         {
                           *p++ = c;
                           break;
                         }
                       fprintf(stderr,
                                   "%s: unexpected %c%c in fragment definition name (%s, %d)\n",
                                   command_name, nw_char, c, source_name, start_line);
                           exit(-1);
               }
             }/* {:322} */

#line 1097 "literate/names.weft"

             break;
           }
         *p++ = c;
                 c = source_get();
                 break;
    }
  }
  fprintf(stderr, "%s: expected fragment name (%s, %d)\n",
          command_name, source_name, start_line);
  exit(-1);
  return NULL;  /* unreachable return to avoid warnings on some compilers */
}
/* {:321} */

#line 1205 "literate/names.weft"
/* {329: literate/names.weft:1205} */
Name *install_args(Name * name, int argc, char *arg[9])
{
  int i;

  for (i = 0; i < argc; i++) {
    if (name->arg[i] == NULL)
      name->arg[i] = save_string(arg[i]);
  }
  return name;
}
/* {:329} */

#line 1226 "literate/names.weft"
/* {331: literate/names.weft:1226} */
Arglist * buildArglist(Name * name, Arglist * a)
{
  Arglist * args = (Arglist *)arena_getmem(sizeof(Arglist));

  args->args = a;
  args->next = NULL;
  args->name = name;
  return args;
}
/* {:331} */

#line 1239 "literate/names.weft"
/* {332: literate/names.weft:1239} */
Arglist * collect_scrap_name(int current_scrap)
{
  char name[MAX_NAME_LEN];
  char *p = name;
  int c = source_get();
  unsigned char sector = current_sector;
  Arglist * head = NULL;
  Arglist ** tail = &head;

  if (c == '+')
  {
    sector = 0;
    c = source_get();
  }
  while (c == ' ' || c == '\t')
    c = source_get();
  while (c != EOF) {
    switch (c) {
      case '\t':
      case ' ':  *p++ = ' ';
                 do
                   c = source_get();
                 while (c == ' ' || c == '\t');
                 break;
      default:
         if (c==nw_char)
           {
             /* Look for end of scrap name and return */
             
#line 1288 "literate/names.weft"
             /* {333: literate/names.weft:1288} */
{
               Name * node;

               c = source_get();
               switch (c) {

                 case '\'': {
                       /* Add plain string argument */
                       
#line 1345 "literate/names.weft"
                       /* {334: literate/names.weft:1345} */
char buff[MAX_NAME_LEN];
                       char * s = buff;
                       int c, c2;

                       while ((c = source_get()) != EOF) {
                         if (c==nw_char) {
                           c2 = source_get();
                           if (c2=='\'')
                             break;
                           *s++ = c2;
                         }
                         else
                           *s++ = c;
                       }
                       *s = '\000';
                       /* Add buff to current arg list */

#line 1396 "literate/names.weft"
                       /* {339: literate/names.weft:1396} */
                       *tail = buildArglist(NULL, (Arglist *)save_string(buff));
                       tail = &(*tail)->next;
                       /* {:339} */

#line 1360 "literate/names.weft"
                       /* {:334} */

#line 1295 "literate/names.weft"

                     }
                     *p++ = ARG_CHR;
                     c = source_get();
                     break;
                 case '1': case '2': case '3':
                 case '4': case '5': case '6':
                 case '7': case '8': case '9': {
                       /* Add a propagated argument */
                       
#line 1366 "literate/names.weft"
                       /* {335: literate/names.weft:1366} */
char buff[3];
                       buff[0] = ARG_CHR;
                       buff[1] = c;
                       buff[2] = '\000';
                       /* Add buff to current arg list */

#line 1396 "literate/names.weft"
                       /* {339: literate/names.weft:1396} */
                       *tail = buildArglist(NULL, (Arglist *)save_string(buff));
                       tail = &(*tail)->next;
                       /* {:339} */

#line 1370 "literate/names.weft"
                       /* {:335} */

#line 1303 "literate/names.weft"

                     }
                     *p++ = ARG_CHR;
                     c = source_get();
                     break;
                 case '{': {
                     /* Add an inline scrap argument */
                     
#line 1373 "literate/names.weft"
                     /* {336: literate/names.weft:1373} */
int s = collect_scrap();
                     Scrap_Node * d = (Scrap_Node *)arena_getmem(sizeof(Scrap_Node));
                     d->scrap = s;
                     d->quoted = 0;
                     d->next = NULL;
                     *tail = buildArglist((Name *)1, (Arglist *)d);
                     tail = &(*tail)->next;/* {:336} */

#line 1309 "literate/names.weft"

                     }
                     *p++ = ARG_CHR;
                     c = source_get();
                     break;
                 case '<':
                     /* Add macro call argument */
                     
#line 1389 "literate/names.weft"
                     /* {338: literate/names.weft:1389} */
*tail = collect_scrap_name(current_scrap);
                     if (current_scrap >= 0)
                       add_to_use((*tail)->name, current_scrap);
                     tail = &(*tail)->next;
                     /* {:338} */

#line 1315 "literate/names.weft"

                     *p++ = ARG_CHR;
                     c = source_get();
                     break;
                 case '(':
                     scrap_name_has_parameters = 1;
                     /* Cleanup and install name */
                     
#line 1168 "literate/names.weft"
                     /* {326: literate/names.weft:1168} */
{
                       if (p > name && p[-1] == ' ')
                         p--;
                       if (p - name > 3 && p[-1] == '.' && p[-2] == '.' && p[-3] == '.') {
                         p[-3] = ' ';
                         p -= 2;
                       }
                       if (p == name || name[0] == ' ') {
                         fprintf(stderr, "%s: empty name (%s, %d)\n",
                                 command_name, source_name, source_line);
                         exit(-1);
                       }
                       *p = '\0';
                       node = prefix_add(&macro_names, name, sector);
                     }/* {:326} */

#line 1321 "literate/names.weft"

                     return buildArglist(node, head);
                 case '>':
                     scrap_name_has_parameters = 0;
                     /* Cleanup and install name */
                     
#line 1168 "literate/names.weft"
                     /* {326: literate/names.weft:1168} */
{
                       if (p > name && p[-1] == ' ')
                         p--;
                       if (p - name > 3 && p[-1] == '.' && p[-2] == '.' && p[-3] == '.') {
                         p[-3] = ' ';
                         p -= 2;
                       }
                       if (p == name || name[0] == ' ') {
                         fprintf(stderr, "%s: empty name (%s, %d)\n",
                                 command_name, source_name, source_line);
                         exit(-1);
                       }
                       *p = '\0';
                       node = prefix_add(&macro_names, name, sector);
                     }/* {:326} */

#line 1325 "literate/names.weft"

                     return buildArglist(node, head);

                 default:
                    if (c==nw_char)
                      {
                        *p++ = c;
                           c = source_get();
                           break;
                      }
                    fprintf(stderr,
                                   "%s: unexpected %c%c in fragment invocation name (%s, %d)\n",
                                   command_name, nw_char, c, source_name, source_line);
                           exit(-1);
               }
             }/* {:333} */

#line 1266 "literate/names.weft"

             break;
           }
         if (!isgraph(c)) {
                   fprintf(stderr,
                           "%s: unexpected character in fragment name (%s, %d)\n",
                           command_name, source_name, source_line);
                   exit(-1);
                 }
                 *p++ = c;
                 c = source_get();
                 break;
    }
  }
  fprintf(stderr, "%s: unexpected end of file (%s, %d)\n",
          command_name, source_name, source_line);
  exit(-1);
  return NULL;  /* unreachable return to avoid warnings on some compilers */
}
/* {:332} */

#line 1401 "literate/names.weft"
/* {340: literate/names.weft:1401} */
static Scrap_Node *reverse(Scrap_Node *a); /* a forward declaration */

void reverse_lists(Name *names)
{
  while (names) {
    reverse_lists(names->llink);
    names->defs = reverse(names->defs);
    names->uses = reverse(names->uses);
    names = names->rlink;
  }
}
/* {:340} */

#line 1418 "literate/names.weft"
/* {341: literate/names.weft:1418} */
static Scrap_Node *reverse(Scrap_Node *a)
{
  if (a) {
    Scrap_Node *b = a->next;
    a->next = NULL;
    while (b) {
      Scrap_Node *c = b->next;
      b->next = a;
      a = b;
      b = c;
    }
  }
  return a;
}
/* {:341} */
