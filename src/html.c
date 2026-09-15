
#line 135 "literate/architecture.weft"
/* {10: literate/architecture.weft:135} */
#include "global.h"
static int scraps = 1;
/* {:10} */

#line 30 "literate/html-output.weft"
/* {215: literate/html-output.weft:30} */
static void html_esc(FILE *f, int c)
{
  if (c == '<') fputs("&lt;", f);
  else if (c == '>') fputs("&gt;", f);
  else if (c == '&') fputs("&amp;", f);
  else putc(c, f);
}

static void html_puts(FILE *f, const char *s)
{
  while (s && *s)
    html_esc(f, (unsigned char)*s++);
}

static void html_emit_name(FILE *f, Name *name)
{
  char *p = name->spelling;
  int i = 0;
  while (*p != '\0') {
    if (*p == ARG_CHR) { html_puts(f, name->arg[i++]); p++; }
    else html_esc(f, (unsigned char)*p++);
  }
}

static void html_ref(FILE *f, Name *name)
{
  fputs("&lt;<a href=\"#weft", f);
  if (name->defs) write_single_scrap_ref(f, name->defs->scrap);
  else putc('?', f);
  fputs("\"><i>", f);
  html_emit_name(f, name);
  fputs("</i> ", f);
  if (name->defs) {
    write_single_scrap_ref(f, name->defs->scrap);
    if (name->defs->next) fputs(", ...", f);
  } else putc('?', f);
  fputs("</a>&gt;", f);
}
/* {:215} */

#line 73 "literate/html-output.weft"
/* {216: literate/html-output.weft:73} */
static void html_format_file_entry(Name *name, FILE *f);
static void html_format_entry(Name *name, FILE *f, unsigned char sector);
static void html_format_user_entry(Name *name, FILE *f, unsigned char sector);

void write_html(char *file_name, char *html_name, unsigned char sector)
{
  (void)sector;
  FILE *html_file = fopen(html_name, "w");
  if (html_file) {
    if (verbose_flag)
      fprintf(stderr, "writing %s\n", html_name);
    /* Write the HTML document head */
    
#line 102 "literate/html-output.weft"
    /* {217: literate/html-output.weft:102} */
fputs(
    "<!doctype html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
    "<title>weft</title>\n<style>\n"
    ":root{--acc:#2f6feb;--kw:#0550ae;--com:#6e7781;--bg:#f6f8fa;--rule:#d0d7de}\n"
    "body{max-width:820px;margin:2rem auto;padding:0 1.2rem;color:#1f2328;\n"
    " font:16px/1.6 -apple-system,Segoe UI,Roboto,Helvetica,Arial,sans-serif}\n"
    "h1,h2,h3{font-weight:700;line-height:1.25;margin:1.8rem 0 .6rem}\n"
    "a{color:var(--acc);text-decoration:none} a:hover{text-decoration:underline}\n"
    ".weft-name{margin:1.4rem 0 .1rem;font-size:1.02rem}\n"
    ".weft-name .b{color:var(--com)} .weft-name i{color:var(--acc);font-style:italic}\n"
    ".weft-name .f{color:var(--acc);font-family:ui-monospace,Menlo,Consolas,monospace}\n"
    ".weft-name .n{color:var(--com);font-size:.85em} .weft-name .eq{color:var(--com)}\n"
    "pre{background:var(--bg);border:1px solid var(--rule);border-radius:8px;\n"
    " padding:.8rem 1rem;overflow:auto;margin:.2rem 0}\n"
    "pre code{font:0.86em/1.45 ui-monospace,SFMono-Regular,Menlo,Consolas,monospace}\n"
    "pre code i{color:var(--acc);font-style:italic} pre code a{color:var(--acc)}\n"
    ".weft-meta{color:var(--com);font-size:.82rem;margin:.15rem 0 .6rem}\n"
    ".weft-meta a{color:var(--acc)} .weft-meta code{color:var(--kw)}\n"
    ".weft-end{color:var(--com);text-align:left;line-height:1;\n"
    " font-size:.75rem;margin:-.05rem 0 .1rem}\n"
    "code{background:var(--bg);padding:.1em .35em;border-radius:5px;\n"
    " font:0.9em ui-monospace,Menlo,Consolas,monospace}\n"
    "@media(prefers-color-scheme:dark){body{background:#0d1117;color:#e6edf3}\n"
    " :root{--bg:#161b22;--rule:#30363d;--com:#8b949e}}\n"
    "</style>\n</head>\n<body>\n", html_file);
    /* {:217} */

#line 84 "literate/html-output.weft"

    source_open(file_name);
    /* Copy source into \verb|html_file| */
    
#line 138 "literate/html-output.weft"
    /* {219: literate/html-output.weft:138} */
{
      int c = source_get();
      while (c != EOF) {
        if (c == nw_char)
          {
          /* Interpret html at-sequence */
          
#line 153 "literate/html-output.weft"
          /* {220: literate/html-output.weft:153} */
{
            int big_definition = FALSE;
            c = source_get();
            switch (c) {
              case 'r': c = source_get(); nw_char = c; update_delimit_scrap(); break;
              case 'O': big_definition = TRUE; FALLTHROUGH;
              case 'o': {
                          Name *name = collect_file_name();
                          int additive = (name->defs && name->defs->scrap != scraps);
                          (void)big_definition;
                          fputs("<div class=\"weft-name\"><a id=\"weft", html_file);
                          write_single_scrap_ref(html_file, scraps);
                          fputs("\"></a><span class=\"b\">&lt;</span> <span class=\"f\">", html_file);
                          html_puts(html_file, name->spelling);
                          fputs("</span> <span class=\"n\">", html_file);
                          write_single_scrap_ref(html_file, scraps);
                          fputs("</span> <span class=\"b\">&gt;</span> ", html_file);
                          fputs(additive ? "<span class=\"eq\">+&equiv;</span>"
                                         : "<span class=\"eq\">&equiv;</span>", html_file);
                          fputs("</div>\n", html_file);
                          {
                            if (name->lang[0] != '\0')
                              fprintf(html_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", html_file);
                            extra_scraps = 0;
                            md_copy_scrap(html_file, TRUE, name);
                            fputs("</code></pre>\n", html_file);
                            /* end-of-scrap tombstone: a small grey square flushed left, the HTML
                               twin of the LaTeX \WEFTsep, closing the code before the metadata line */
                            fputs("<div class=\"weft-end\">&#9633;</div>\n", html_file);
                          }
                          {
                            fputs("<div class=\"weft-meta\">", html_file);
                            write_single_scrap_ref(html_file, scraps);
                            if (scrap_flag && name->defs && name->defs->next) {
                              fputs(" &middot; also at ", html_file);
                              md_print_scrap_numbers(html_file, name->defs);
                            }
                            fputs("</div>\n", html_file);
                            scraps++;
                          }
                          {
                            scraps += extra_scraps;
                            do c = source_get(); while (isspace(c));
                          }
                        } break;
              case 'Q':
              case 'D': big_definition = TRUE; FALLTHROUGH;
              case 'q':
              case 'd': {
                          Name *name = collect_macro_name();
                          int additive = (name->defs && name->defs->scrap != scraps);
                          (void)big_definition;
                          fputs("<div class=\"weft-name\"><a id=\"weft", html_file);
                          write_single_scrap_ref(html_file, scraps);
                          fputs("\"></a><span class=\"b\">&lt;</span> <i>", html_file);
                          html_emit_name(html_file, name);
                          fputs("</i> <span class=\"n\">", html_file);
                          write_single_scrap_ref(html_file, scraps);
                          fputs("</span> <span class=\"b\">&gt;</span> ", html_file);
                          fputs(additive ? "<span class=\"eq\">+&equiv;</span>"
                                         : "<span class=\"eq\">&equiv;</span>", html_file);
                          fputs("</div>\n", html_file);
                          {
                            if (name->lang[0] != '\0')
                              fprintf(html_file, "<pre><code class=\"language-%s\">", name->lang);
                            else
                              fputs("<pre><code>", html_file);
                            extra_scraps = 0;
                            md_copy_scrap(html_file, TRUE, name);
                            fputs("</code></pre>\n", html_file);
                            /* end-of-scrap tombstone: a small grey square flushed left, the HTML
                               twin of the LaTeX \WEFTsep, closing the code before the metadata line */
                            fputs("<div class=\"weft-end\">&#9633;</div>\n", html_file);
                          }
                          {
                            fputs("<div class=\"weft-meta\">", html_file);
                            write_single_scrap_ref(html_file, scraps);
                            if (name->defs && name->defs->next) {
                              fputs(" &middot; also at ", html_file);
                              md_print_scrap_numbers(html_file, name->defs);
                            }
                            if (name->uses) {
                              fputs(" &middot; used in ", html_file);
                              md_print_scrap_numbers(html_file, name->uses);
                            } else {
                              fputs(" &middot; unreferenced", html_file);
                            }
                            fputs("</div>\n", html_file);
                            scraps++;
                          }
                          {
                            scraps += extra_scraps;
                            do c = source_get(); while (isspace(c));
                          }
                        } break;
              case 's': 
                        prev_sector += 1;
                        current_sector = prev_sector;
                        c = source_get();
                         break;
              case 'S': current_sector = 1;
                        c = source_get();
                         break;
              case '{':
              case '[':
              case '(': md_copy_scrap(html_file, FALSE, NULL); c = source_get(); break;
              case '<': { Arglist *a = collect_scrap_name(0);
                          html_ref(html_file, a->name); c = source_get(); } break;
              case 'x': {
                           char  label_name[MAX_NAME_LEN];
                           char * p = label_name;
                           while (c = source_get(), c != nw_char) /* Here is ?-01 */
                              *p++ = c;
                           *p = '\0';
                           c = source_get();
                           
                           write_label(label_name, html_file);
                        } c = source_get(); break;
              case 'c': c = source_get(); break;
              case 'f': {
                          if (file_names) {
                            fputs("<ul class=\"weft-index\">\n", html_file);
                            html_format_file_entry(file_names, html_file);
                            fputs("</ul>\n", html_file);
                          }
                          c = source_get();
                        } break;
              case 'm': {
                          unsigned char sector = current_sector;
                          int cc = source_get();
                          if (cc == '+') sector = 0; else source_ungetc(&cc);
                          if (has_sector(macro_names, sector)) {
                            fputs("<ul class=\"weft-index\">\n", html_file);
                            html_format_entry(macro_names, html_file, sector);
                            fputs("</ul>\n", html_file);
                          }
                          c = source_get();
                        } break;
              case 'u': {
                          unsigned char sector = current_sector;
                          c = source_get();
                          if (c == '+') { sector = 0; c = source_get(); }
                          if (has_sector(user_names, sector)) {
                            fputs("<ul class=\"weft-index\">\n", html_file);
                            html_format_user_entry(user_names, html_file, sector);
                            fputs("</ul>\n", html_file);
                          }
                        } break;
              case 'v': fputs(version_string, html_file); c = source_get(); break;
              case 'l':
              case 'L': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get(); break;
              case 'W': { int lc = source_get();
                          while (lc != '\n' && lc != EOF)
                            lc = source_get();
                        }
                        c = source_get(); break;
              default:
                if (c==nw_char) putc(c, html_file);
                c = source_get();
                break;
            }
          }/* {:220} */

#line 143 "literate/html-output.weft"

          }
        else {
          putc(c, html_file);
          c = source_get();
        }
      }
    }/* {:219} */

#line 86 "literate/html-output.weft"

    /* Write the HTML document tail */
    
#line 131 "literate/html-output.weft"
    /* {218: literate/html-output.weft:131} */
fputs("</body>\n</html>\n", html_file);
    /* {:218} */

#line 87 "literate/html-output.weft"

    fclose(html_file);
  }
  else
    fprintf(stderr, "%s: can't open %s\n", command_name, html_name);
}
/* {:216} */

#line 305 "literate/html-output.weft"
/* {229: literate/html-output.weft:305} */
static void html_format_file_entry(Name *name, FILE *f)
{
  while (name) {
    html_format_file_entry(name->llink, f);
    fputs("<li><span class=\"f\">", f);
    html_puts(f, name->spelling);
    fputs("</span> ", f);
    if (name->defs) md_print_scrap_numbers(f, name->defs);
    fputs("</li>\n", f);
    name = name->rlink;
  }
}
/* {:229} */

#line 333 "literate/html-output.weft"
/* {231: literate/html-output.weft:333} */
static void html_format_entry(Name *name, FILE *f, unsigned char sector)
{
  while (name) {
    html_format_entry(name->llink, f, sector);
    if (name->sector == sector) {
      fputs("<li><span class=\"b\">&lt;</span> <i>", f);
      html_emit_name(f, name);
      fputs("</i> <span class=\"b\">&gt;</span> ", f);
      if (name->defs) md_print_scrap_numbers(f, name->defs);
      else putc('?', f);
      fputs("</li>\n", f);
    }
    name = name->rlink;
  }
}
/* {:231} */

#line 363 "literate/html-output.weft"
/* {233: literate/html-output.weft:363} */
static void html_format_user_entry(Name *name, FILE *f, unsigned char sector)
{
  while (name) {
    html_format_user_entry(name->llink, f, sector);
    if (name->sector == sector && (name->uses || dangling_flag)) {
      fputs("<li><code>", f);
      html_puts(f, name->spelling);
      fputs("</code>: ", f);
      if (name->uses) md_print_scrap_numbers(f, name->uses);
      fputs("</li>\n", f);
    }
    name = name->rlink;
  }
}
/* {:233} */
