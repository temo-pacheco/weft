
#line 23 "literate/architecture.weft"
/* {1: literate/architecture.weft:23} */
/* Include files */

#line 35 "literate/architecture.weft"
/* {2: literate/architecture.weft:35} */

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <locale.h>
/* {:2} */

#line 23 "literate/architecture.weft"

/* Type declarations */

#line 56 "literate/architecture.weft"
/* {3: literate/architecture.weft:56} */
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
/* {:3} */

#line 492 "literate/parser.weft"
/* {75: literate/parser.weft:492} */
typedef int *Parameters;
/* {:75} */

#line 34 "literate/output-files.weft"
/* {214: literate/output-files.weft:34} */

#define MAX_INDENT 8192
/* {:214} */

#line 5 "literate/names.weft"
/* {305: literate/names.weft:5} */
typedef struct scrap_node {
  struct scrap_node *next;
  int scrap;
  char quoted;
} Scrap_Node;
/* {:305} */

#line 34 "literate/names.weft"
/* {306: literate/names.weft:34} */
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
/* {:306} */

#line 1167 "literate/names.weft"
/* {332: literate/names.weft:1167} */
#define ARG_CHR '\001'
/* {:332} */

#line 1228 "literate/names.weft"
/* {338: literate/names.weft:1228} */
typedef struct arglist
{Name * name;
struct arglist * args;
struct arglist * next;
} Arglist;
/* {:338} */

#line 1392 "literate/names.weft"
/* {345: literate/names.weft:1392} */
typedef struct embed {
   Scrap_Node * defs;
   Arglist * args;
} Embed_Node;
/* {:345} */

#line 433 "literate/search-labels.weft"
/* {376: literate/search-labels.weft:433} */
typedef struct uses {
  struct uses *next;
  Name *defn;
} Uses;
/* {:376} */

#line 6 "literate/arena.weft"
/* {399: literate/arena.weft:6} */
typedef struct l_node
{
   struct l_node * left, * right;
   int scrap, seq;
   char name[1];
} label_node;
/* {:399} */

#line 24 "literate/architecture.weft"

/* Limits */

#line 81 "literate/architecture.weft"
/* {4: literate/architecture.weft:81} */

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN 1024
#endif
/* {:4} */

#line 25 "literate/architecture.weft"

/* Global variable declarations */

#line 90 "literate/main.weft"
/* {17: literate/main.weft:90} */
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
/* {:17} */

#line 165 "literate/main.weft"
/* {19: literate/main.weft:165} */
extern int nw_char;
/* {:19} */

#line 175 "literate/main.weft"
/* {21: literate/main.weft:175} */
extern char *command_name;
/* {:21} */

#line 182 "literate/parser.weft"
/* {51: literate/parser.weft:182} */
extern unsigned char current_sector;
extern unsigned char prev_sector;
/* {:51} */

#line 380 "literate/parser.weft"
/* {65: literate/parser.weft:380} */
extern char blockBuff[6400];
/* {:65} */

#line 402 "literate/latex-output.weft"
/* {100: literate/latex-output.weft:402} */
extern int extra_scraps;
/* {:100} */

#line 19 "literate/source-io.weft"
/* {221: literate/source-io.weft:19} */
extern char *source_name;  /* name of the current file */
extern int source_line;    /* current line in the source file */
/* {:221} */

#line 140 "literate/scraps.weft"
/* {246: literate/scraps.weft:140} */
extern int already_warned;
/* {:246} */

#line 60 "literate/names.weft"
/* {307: literate/names.weft:60} */
extern Name *file_names;
extern Name *macro_names;
extern Name *user_names;
extern int scrap_name_has_parameters;
extern int scrap_ended_with;
extern char pending_lang[64];
/* {:307} */

#line 15 "literate/arena.weft"
/* {400: literate/arena.weft:15} */
extern label_node * label_tab;
/* {:400} */

#line 26 "literate/architecture.weft"

/* Function prototypes */

#line 7 "literate/parser.weft"
/* {41: literate/parser.weft:7} */
extern void pass1(char *file_name);
/* {:41} */

#line 15 "literate/latex-output.weft"
/* {80: literate/latex-output.weft:15} */
extern void write_tex(char *file_name, char *tex_name, unsigned char sector);
/* {:80} */

#line 566 "literate/latex-output.weft"
/* {111: literate/latex-output.weft:566} */
void initialise_delimit_scrap_array(void);
/* {:111} */

#line 660 "literate/latex-output.weft"
/* {116: literate/latex-output.weft:660} */
void update_delimit_scrap(void);
/* {:116} */

#line 1085 "literate/latex-output.weft"
/* {142: literate/latex-output.weft:1085} */
extern int has_sector(Name *, unsigned char);
/* {:142} */

#line 10 "literate/markdown-output.weft"
/* {147: literate/markdown-output.weft:10} */
extern void write_md(char *file_name, char *md_name, unsigned char sector);
/* {:147} */

#line 4 "literate/output-files.weft"
/* {211: literate/output-files.weft:4} */
extern void write_files(Name *files);
/* {:211} */

#line 8 "literate/output-files.weft"
/* {212: literate/output-files.weft:8} */
extern void write_map(Name *file_names, Name *macro_names);
extern void write_extract(char *name, Name *file_names, Name *macro_names);
extern void write_reverse_map(const char *arg);
/* {:212} */

#line 9 "literate/source-io.weft"
/* {220: literate/source-io.weft:9} */
extern void source_open(char *name); /* pass in the name of the source file */
extern int source_get(void);   /* no args; returns the next char or EOF */
extern int source_last;   /* what last source_get() returned. */
extern int source_peek;   /* The next character to get */
/* {:220} */

#line 83 "literate/source-io.weft"
/* {226: literate/source-io.weft:83} */
extern void source_ungetc(int*);
/* {:226} */

#line 48 "literate/scraps.weft"
/* {236: literate/scraps.weft:48} */
extern void init_scraps(void);
extern int collect_scrap(void);
extern int write_scraps(FILE *file, char *spelling, Scrap_Node *defs, int global_indent, char *indent_chars, char debug_flag, char tab_flag, char indent_flag, unsigned char comment_flag, char location_flag, Arglist *inArgs, char *inParams[9], Parameters parameters, char *title);
extern void write_scrap_ref(FILE *file, int num, int first, int *page);
extern void write_single_scrap_ref(FILE *file, int num);
extern int num_scraps(void);
extern Uses * get_scrap_uses(int scrap);
extern Uses * get_scrap_defs(int scrap);
/* {:236} */

#line 117 "literate/scraps.weft"
/* {242: literate/scraps.weft:117} */
extern const char *scrap_file_name(int i);
extern int scrap_file_line(int i);
extern void dump_scrap_text(FILE *file, int scrap_idx);
/* {:242} */

#line 404 "literate/scraps.weft"
/* {263: literate/scraps.weft:404} */
extern void add_to_use(Name * name, int current_scrap);
/* {:263} */

#line 538 "literate/scraps.weft"
/* {269: literate/scraps.weft:538} */
Arglist *instance(Arglist *a, Arglist *par, char *arg[9], int *ch);
/* {:269} */

#line 1016 "literate/scraps.weft"
/* {298: literate/scraps.weft:1016} */
extern void collect_numbers(char *aux_name);
/* {:298} */

#line 78 "literate/names.weft"
/* {309: literate/names.weft:78} */
extern Name *collect_file_name(void);
extern Name *collect_macro_name(void);
extern Arglist *collect_scrap_name(int current_scrap);
extern Name *name_add(Name **rt, char *spelling, unsigned char sector);
extern Name *prefix_add(Name **rt, char *spelling, unsigned char sector);
extern char *save_string(char *s);
extern void reverse_lists(Name *names);
extern void collect_lang_def(void);
extern void collect_weave_format(void);
/* {:309} */

#line 224 "literate/names.weft"
/* {315: literate/names.weft:224} */
extern int robs_strcmp(char*, char*);
/* {:315} */

#line 1211 "literate/names.weft"
/* {336: literate/names.weft:1211} */
extern Name *install_args(Name *name, int argc, char *arg[9]);
/* {:336} */

#line 193 "literate/search-labels.weft"
/* {365: literate/search-labels.weft:193} */
extern void search(void);
/* {:365} */

#line 440 "literate/search-labels.weft"
/* {377: literate/search-labels.weft:440} */
extern void format_uses_refs(FILE *, int);
/* {:377} */

#line 495 "literate/search-labels.weft"
/* {382: literate/search-labels.weft:495} */
extern void format_defs_refs(FILE *, int);
/* {:382} */

#line 606 "literate/search-labels.weft"
/* {390: literate/search-labels.weft:606} */
void write_label(char label_name[], FILE * file);
/* {:390} */

#line 30 "literate/arena.weft"
/* {401: literate/arena.weft:30} */
extern void *arena_getmem(size_t n);
extern void arena_free(void);
extern void arglist_reset(void);
/* {:401} */

#line 27 "literate/architecture.weft"

/* Operating System Dependencies */

#line 25 "literate/main.weft"
/* {16: literate/main.weft:25} */

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
/* {:16} */

#line 28 "literate/architecture.weft"
/* {:1} */
