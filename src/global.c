
#line 178 "literate/architecture.weft"
/* {16: literate/architecture.weft:178} */
#include "global.h"
/* Operating System Dependencies */

#line 25 "literate/main.weft"
/* {18: literate/main.weft:25} */

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
/* {:18} */

#line 179 "literate/architecture.weft"

/* Global variable definitions */

#line 147 "literate/main.weft"
/* {20: literate/main.weft:147} */
int tex_flag = TRUE;
int output_flag = TRUE;
int compare_flag = TRUE;
int verbose_flag = FALSE;
int number_flag = FALSE;
int scrap_flag = TRUE;
int dangling_flag = FALSE;
int xref_flag = FALSE;
int prepend_flag = FALSE;
char * dirpath = DEFAULT_PATH; /* Default directory path */
char * path_sep = PATH_SEP_CHAR;
int listings_flag = FALSE;
int version_info_flag = FALSE;
char default_version_string[] = WEFT_VERSION;
char *  version_string = default_version_string;
int hyperref_flag = FALSE;
int hyperopt_flag = FALSE;
char * hyperoptions = "";
int includepath_flag = FALSE; /* Do we have an include path? */
struct incl * include_list = NULL;
                       /* The list of include paths */
int markdown_flag = FALSE;
int weave_flag = FALSE;
int weave_format = 0;
int weave_format_from_cli = FALSE;
int map_flag = FALSE;
int extract_flag = FALSE;
char *extract_name = NULL;
int reverse_map_flag = FALSE;
char *reverse_map_arg = NULL;
int bodies_flag = FALSE;
int prose_flag = FALSE;
int diff_flag = FALSE;
int callers_flag = FALSE;
int lint_flag = FALSE;
int errors_flag = FALSE;
int mkdirs_flag = FALSE;
/* {:20} */

#line 192 "literate/main.weft"
/* {22: literate/main.weft:192} */
int nw_char='@';
/* {:22} */

#line 203 "literate/main.weft"
/* {24: literate/main.weft:203} */
char *command_name = NULL;
/* {:24} */

#line 219 "literate/parser.weft"
/* {57: literate/parser.weft:219} */
unsigned char current_sector = 1;
unsigned char prev_sector = 1;
/* {:57} */

#line 427 "literate/parser.weft"
/* {71: literate/parser.weft:427} */
char blockBuff[6400];
/* {:71} */

#line 657 "literate/latex-output.weft"
/* {109: literate/latex-output.weft:657} */
int extra_scraps = 0;
 /* {:109} */

#line 24 "literate/source-io.weft"
/* {267: literate/source-io.weft:24} */
char *source_name = NULL;
int source_line = 0;
/* {:267} */

#line 165 "literate/scraps.weft"
/* {295: literate/scraps.weft:165} */
int already_warned = 0;
/* {:295} */

#line 69 "literate/names.weft"
/* {360: literate/names.weft:69} */
Name *file_names = NULL;
Name *macro_names = NULL;
Name *user_names = NULL;
int scrap_name_has_parameters;
int scrap_ended_with;
char pending_lang[64] = "";
/* {:360} */

#line 180 "literate/architecture.weft"

/* {:16} */

#line 2 "literate/arena.weft"
/* {450: literate/arena.weft:2} */
label_node * label_tab = NULL;
/* {:450} */
