
#line 23 "literate/architecture.weft"
/* {2: literate/architecture.weft:23} */
/* Include files */

#line 35 "literate/architecture.weft"
/* {3: literate/architecture.weft:35} */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <locale.h>
/* {:3} */

#line 23 "literate/architecture.weft"

/* Type declarations */

#line 56 "literate/architecture.weft"
/* {4: literate/architecture.weft:56} */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#if defined(__GNUC__) && __GNUC__ >= 7
#define FALLTHROUGH __attribute__((fallthrough))
#elif defined(__clang__)
#define FALLTHROUGH __attribute__((fallthrough))
#else
#define FALLTHROUGH ((void)0)
#endif
/* {:4} */

#line 535 "literate/parser.weft"
/* {79: literate/parser.weft:535} */
typedef int *Parameters;
/* {:79} */

#line 37 "literate/output-files.weft"
/* {235: literate/output-files.weft:37} */

#define MAX_INDENT 8192
/* {:235} */

#line 5 "literate/names.weft"
/* {334: literate/names.weft:5} */
typedef struct scrap_node {
  struct scrap_node *next;
  int scrap;
  char quoted;
} Scrap_Node;
/* {:334} */

#line 34 "literate/names.weft"
/* {335: literate/names.weft:34} */
typedef struct name {
  char *spelling;
  struct name *llink;
  struct name *rlink;
  Scrap_Node *defs;
  Scrap_Node *uses;
  char * arg[9];
  int mark;
  char tab_flag;
  char indent_flag;
  char debug_flag;
  char location_flag;
  char suppress_markers;
  unsigned char comment_flag;
  char lang[64];
  unsigned char sector;
} Name;
/* {:335} */

#line 1167 "literate/names.weft"
/* {361: literate/names.weft:1167} */
#define ARG_CHR '\001'
/* {:361} */

#line 1228 "literate/names.weft"
/* {367: literate/names.weft:1228} */
typedef struct arglist
{Name * name;
struct arglist * args;
struct arglist * next;
} Arglist;
/* {:367} */

#line 1392 "literate/names.weft"
/* {374: literate/names.weft:1392} */
typedef struct embed {
   Scrap_Node * defs;
   Arglist * args;
} Embed_Node;
/* {:374} */

#line 433 "literate/search-labels.weft"
/* {405: literate/search-labels.weft:433} */
typedef struct uses {
  struct uses *next;
  Name *defn;
} Uses;
/* {:405} */

#line 6 "literate/arena.weft"
/* {428: literate/arena.weft:6} */
typedef struct l_node
{
   struct l_node * left, * right;
   int scrap, seq;
   char name[1];
} label_node;
/* {:428} */

#line 24 "literate/architecture.weft"

/* Limits */

#line 90 "literate/architecture.weft"
/* {5: literate/architecture.weft:90} */

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN 1024
#endif

#define WEFT_VERSION "1.1.1"
/* {:5} */

#line 25 "literate/architecture.weft"

/* Global variable declarations */

#line 93 "literate/main.weft"
/* {18: literate/main.weft:93} */
extern int tex_flag;      /* if FALSE, don't emit the documentation file */
extern int output_flag;   /* if FALSE, don't emit the output files */
extern int compare_flag;  /* if FALSE, overwrite without comparison */
extern int verbose_flag;  /* if TRUE, write progress information */
extern int number_flag;   /* if TRUE, use a sequential numbering scheme */
extern int scrap_flag;    /* if FALSE, don't print list of scraps */
extern int dangling_flag;    /* if FALSE, don't print dangling identifiers */
extern int xref_flag; /* If TRUE, print cross-references in scrap comments */
extern int prepend_flag;  /* If TRUE, prepend a path to the output file names */
extern char * dirpath;    /* The prepended directory path */
extern char * path_sep;   /* How to join path to filename */
extern int listings_flag;   /* if TRUE, use listings package for scrap formatting */
extern int version_info_flag; /* If TRUE, set up version string */
extern char *  version_string; /* What to print for @v */
extern int hyperref_flag; /* Are we preparing for hyperref
                             package. */
extern int hyperopt_flag; /* Are we preparing for hyperref options */
extern char * hyperoptions; /* The options to pass to the
                               hyperref package */
extern int includepath_flag; /* Do we have an include path? */
extern struct incl{char * name; struct incl * next;} * include_list;
                       /* The list of include paths */
extern int markdown_flag; /* if TRUE, generate Markdown instead of LaTeX */
extern int weave_flag;          /* if TRUE, weave was requested via -w */
extern int weave_format;        /* 0=none, 1=tex, 2=md */
extern int weave_format_from_cli; /* if TRUE, CLI override of @W */
extern int map_flag;       /* if TRUE, emit JSON map to stdout */
extern int extract_flag;   /* if TRUE, next arg is extract name */
extern char *extract_name; /* fragment name to extract */
extern int reverse_map_flag; /* if TRUE, reverse map mode */
extern char *reverse_map_arg; /* file[:line] argument for -R */
extern int bodies_flag;    /* if TRUE, include scrap bodies in -m JSON */
extern int prose_flag;     /* if TRUE, include prose context in -m JSON */
extern int diff_flag;      /* if TRUE, show diff instead of writing files */
extern int callers_flag;   /* if TRUE, show callers with -e */
extern int lint_flag;      /* if TRUE, run static analysis */
extern int errors_flag;    /* if TRUE, annotate compiler errors from stdin */
/* {:18} */

#line 181 "literate/main.weft"
/* {20: literate/main.weft:181} */
extern int nw_char;
/* {:20} */

#line 191 "literate/main.weft"
/* {22: literate/main.weft:191} */
extern char *command_name;
/* {:22} */

#line 214 "literate/parser.weft"
/* {55: literate/parser.weft:214} */
extern unsigned char current_sector;
extern unsigned char prev_sector;
/* {:55} */

#line 423 "literate/parser.weft"
/* {69: literate/parser.weft:423} */
extern char blockBuff[6400];
/* {:69} */

#line 632 "literate/latex-output.weft"
/* {107: literate/latex-output.weft:632} */
extern int extra_scraps;
/* {:107} */

#line 19 "literate/source-io.weft"
/* {243: literate/source-io.weft:19} */
extern char *source_name;  /* name of the current file */
extern int source_line;    /* current line in the source file */
/* {:243} */

#line 161 "literate/scraps.weft"
/* {271: literate/scraps.weft:161} */
extern int already_warned;
/* {:271} */

#line 60 "literate/names.weft"
/* {336: literate/names.weft:60} */
extern Name *file_names;
extern Name *macro_names;
extern Name *user_names;
extern int scrap_name_has_parameters;
extern int scrap_ended_with;
extern char pending_lang[64];
/* {:336} */

#line 15 "literate/arena.weft"
/* {429: literate/arena.weft:15} */
extern label_node * label_tab;
/* {:429} */

#line 26 "literate/architecture.weft"

/* Function prototypes */

#line 7 "literate/parser.weft"
/* {42: literate/parser.weft:7} */
extern void pass1(char *file_name);
/* {:42} */

#line 15 "literate/latex-output.weft"
/* {84: literate/latex-output.weft:15} */
extern void write_tex(char *file_name, char *tex_name, unsigned char sector);
/* {:84} */

#line 855 "literate/latex-output.weft"
/* {120: literate/latex-output.weft:855} */
void initialise_delimit_scrap_array(void);
/* {:120} */

#line 976 "literate/latex-output.weft"
/* {126: literate/latex-output.weft:976} */
void update_delimit_scrap(void);
/* {:126} */

#line 1459 "literate/latex-output.weft"
/* {154: literate/latex-output.weft:1459} */
extern int has_sector(Name *, unsigned char);
/* {:154} */

#line 10 "literate/markdown-output.weft"
/* {159: literate/markdown-output.weft:10} */
extern void write_md(char *file_name, char *md_name, unsigned char sector);
/* {:159} */

#line 4 "literate/output-files.weft"
/* {232: literate/output-files.weft:4} */
extern void write_files(Name *files);
/* {:232} */

#line 8 "literate/output-files.weft"
/* {233: literate/output-files.weft:8} */
extern void write_map(Name *file_names, Name *macro_names);
extern void write_extract(char *name, Name *file_names, Name *macro_names);
extern void write_reverse_map(const char *arg);
extern void write_callers(char *name, Name *file_names, Name *macro_names);
extern void write_lint(Name *file_names, Name *macro_names);
extern void write_error_annotator(void);
/* {:233} */

#line 9 "literate/source-io.weft"
/* {242: literate/source-io.weft:9} */
extern void source_open(char *name); /* pass in the name of the source file */
extern int source_get(void);   /* no args; returns the next char or EOF */
extern int source_last;   /* what last source_get() returned. */
extern int source_peek;   /* The next character to get */
/* {:242} */

#line 83 "literate/source-io.weft"
/* {248: literate/source-io.weft:83} */
extern void source_ungetc(int*);
/* {:248} */

#line 51 "literate/scraps.weft"
/* {258: literate/scraps.weft:51} */
extern void init_scraps(void);
extern int collect_scrap(void);
extern int write_scraps(FILE *file, char *spelling, Scrap_Node *defs, int global_indent, char *indent_chars, char debug_flag, char tab_flag, char indent_flag, unsigned char comment_flag, char location_flag, Arglist *inArgs, char *inParams[9], Parameters parameters, char *title);
extern void write_scrap_ref(FILE *file, int num, int first, int *page);
extern void write_single_scrap_ref(FILE *file, int num);
extern int num_scraps(void);
extern Uses * get_scrap_uses(int scrap);
extern Uses * get_scrap_defs(int scrap);
/* {:258} */

#line 120 "literate/scraps.weft"
/* {264: literate/scraps.weft:120} */
extern const char *scrap_file_name(int i);
extern int scrap_file_line(int i);
extern int scrap_end_line(int i);
extern const char *scrap_prose(int i);
extern Name *scrap_owner(int i);
extern void dump_scrap_text(FILE *file, int scrap_idx);
extern void dump_scrap_json(FILE *f, int scrap_idx);
extern void set_scrap_prose(int i, char *p);
extern void set_scrap_owner(int i, Name *n);
/* {:264} */

#line 429 "literate/scraps.weft"
/* {288: literate/scraps.weft:429} */
extern void add_to_use(Name * name, int current_scrap);
/* {:288} */

#line 605 "literate/scraps.weft"
/* {297: literate/scraps.weft:605} */
Arglist *instance(Arglist *a, Arglist *par, char *arg[9], int *ch);
/* {:297} */

#line 1108 "literate/scraps.weft"
/* {327: literate/scraps.weft:1108} */
extern void collect_numbers(char *aux_name);
/* {:327} */

#line 78 "literate/names.weft"
/* {338: literate/names.weft:78} */
extern Name *collect_file_name(void);
extern Name *collect_macro_name(void);
extern Arglist *collect_scrap_name(int current_scrap);
extern Name *name_add(Name **rt, char *spelling, unsigned char sector);
extern Name *prefix_add(Name **rt, char *spelling, unsigned char sector);
extern char *save_string(char *s);
extern void reverse_lists(Name *names);
extern void collect_lang_def(void);
extern void collect_weave_format(void);
/* {:338} */

#line 224 "literate/names.weft"
/* {344: literate/names.weft:224} */
extern int robs_strcmp(char*, char*);
/* {:344} */

#line 1211 "literate/names.weft"
/* {365: literate/names.weft:1211} */
extern Name *install_args(Name *name, int argc, char *arg[9]);
/* {:365} */

#line 193 "literate/search-labels.weft"
/* {394: literate/search-labels.weft:193} */
extern void search(void);
/* {:394} */

#line 440 "literate/search-labels.weft"
/* {406: literate/search-labels.weft:440} */
extern void format_uses_refs(FILE *, int);
/* {:406} */

#line 494 "literate/search-labels.weft"
/* {411: literate/search-labels.weft:494} */
extern void format_defs_refs(FILE *, int);
/* {:411} */

#line 604 "literate/search-labels.weft"
/* {419: literate/search-labels.weft:604} */
void write_label(char label_name[], FILE * file);
/* {:419} */

#line 30 "literate/arena.weft"
/* {430: literate/arena.weft:30} */
extern void *arena_getmem(size_t n);
extern void arena_free(void);
extern void arglist_reset(void);
/* {:430} */

#line 27 "literate/architecture.weft"

/* Operating System Dependencies */

#line 25 "literate/main.weft"
/* {17: literate/main.weft:25} */

#if defined(VMS)
#define PATH_SEP(c) (c==']'||c==':')
#define PATH_SEP_CHAR ""
#define DEFAULT_PATH ""
#elif defined(MSDOS)
#define PATH_SEP(c) (c=='\\')
#define PATH_SEP_CHAR "\\"
#define DEFAULT_PATH "."
#else
#define PATH_SEP(c) (c=='/')
#define PATH_SEP_CHAR "/"
#define DEFAULT_PATH "."
#endif
/* {:17} */

#line 28 "literate/architecture.weft"
/* {:2} */
