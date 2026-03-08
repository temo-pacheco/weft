
#line 160 "literate/architecture.weft"
/* {14: literate/architecture.weft:160} */
#include "global.h"
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

#line 161 "literate/architecture.weft"

/* Global variable definitions */

#line 130 "literate/main.weft"
/* {18: literate/main.weft:130} */
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
char default_version_string[] = "no version";
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
/* {:18} */

#line 168 "literate/main.weft"
/* {20: literate/main.weft:168} */
int nw_char='@';
/* {:20} */

#line 179 "literate/main.weft"
/* {22: literate/main.weft:179} */
char *command_name = NULL;
/* {:22} */

#line 187 "literate/parser.weft"
/* {52: literate/parser.weft:187} */
unsigned char current_sector = 1;
unsigned char prev_sector = 1;
/* {:52} */

#line 384 "literate/parser.weft"
/* {66: literate/parser.weft:384} */
char blockBuff[6400];
/* {:66} */

#line 406 "literate/latex-output.weft"
/* {101: literate/latex-output.weft:406} */
int extra_scraps = 0;
 /* {:101} */

#line 24 "literate/source-io.weft"
/* {222: literate/source-io.weft:24} */
char *source_name = NULL;
int source_line = 0;
/* {:222} */

#line 144 "literate/scraps.weft"
/* {247: literate/scraps.weft:144} */
int already_warned = 0;
/* {:247} */

#line 69 "literate/names.weft"
/* {308: literate/names.weft:69} */
Name *file_names = NULL;
Name *macro_names = NULL;
Name *user_names = NULL;
int scrap_name_has_parameters;
int scrap_ended_with;
char pending_lang[64] = "";
/* {:308} */

#line 162 "literate/architecture.weft"

/* {:14} */

#line 2 "literate/arena.weft"
/* {398: literate/arena.weft:2} */
label_node * label_tab = NULL;
/* {:398} */
