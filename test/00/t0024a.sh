#!/bin/sh
#
# $RCSfile: t0024a.sh,v $-- Test test/00/t0024a.sh
#
#
# Test of Local and global sectors
#
work=${TMPDIR:-/tmp}/$$
PAGER=cat
export PAGER
umask 022
here=`pwd`
if test $? -ne 0 ; then exit 2; fi
SHELL=/bin/sh
export SHELL

bin="$here/${1-.}"

pass()
{
	set +x
	cd $here
	rm -rf $work
	exit 0
}
fail()
{
	set +x
	echo "FAILED test of Local and global sectors" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Local and global sectors" 1>&2
	cd $here
	rm -rf $work
	exit 2
}
trap \"no_result\" 1 2 3 15

mkdir $work
if test $? -ne 0 ; then no_result; fi
cd $work
if test $? -ne 0 ; then no_result; fi

#
# test Local and global sectors
#

cat > test.w <<"EOF"
\documentclass{article}
\begin{document}
@o test.actual.c
@{ First use in global
@<Frag 1@>
@}
@d Frag 1
@{Global sector line one.
@}
@s
@d Frag 1
@{First sector line one.
@}
@o test.actual.c
@{Use first local
@<Frag 1@>
@}
@m
@s
@o test.actual.c
@{Use second local
@<Frag 1@>
@}
@d Frag 1
@{Second sector line one.
@}
@m
@S
@d Frag 1
@{Global sector line two.
@}
@s
@d Frag 1
@{Third sector line one.
@}
@o test.actual.c
@{Use second local
@<Frag 1@>
@}
@m
@S
@d Frag 1
@{Global sector line three.
@}
@o test.actual.c
@{ Last use in global
@<Frag 1@>
@}
@m
\end{document}
EOF

cat > test.expected.tex <<"EOF"
\newcommand{\WEFTtarget}[2]{#2}
\newcommand{\WEFTlink}[2]{#2}
\newcommand{\WEFTtxtMacroDefBy}{Fragment defined by}
\newcommand{\WEFTtxtMacroRefIn}{Fragment referenced in}
\newcommand{\WEFTtxtMacroNoRef}{Fragment never referenced}
\newcommand{\WEFTtxtDefBy}{Defined by}
\newcommand{\WEFTtxtRefIn}{Referenced in}
\newcommand{\WEFTtxtNoRef}{Not referenced}
\newcommand{\WEFTtxtFileDefBy}{File defined by}
\newcommand{\WEFTtxtIdentsUsed}{Uses:}
\newcommand{\WEFTtxtIdentsNotUsed}{Never used}
\newcommand{\WEFTtxtIdentsDefed}{Defines:}
\newcommand{\WEFTsep}{${\diamond}$}
\newcommand{\WEFTnotglobal}{(not defined globally)}
\newcommand{\WEFTbreakpenalty}{500}
\newcommand{\WEFTsp}{\hskip\fontdimen2\font\relax}
\newcommand{\WEFTbrk}{\discretionary{}{}{}}
\newlength{\WEFTindent}\setlength{\WEFTindent}{1.5em}
\newlength{\WEFThang}\setlength{\WEFThang}{2em}
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\small\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\newcommand{\WEFTcode}{\par\nobreak\vspace{-.5ex}\begingroup\ttfamily\small\parindent0pt\parskip0pt\raggedright\leftskip\WEFTindent\hangindent\WEFThang\hangafter1\relax\everypar{\hangindent\WEFThang\hangafter1\relax}}
\newcommand{\WEFTendcode}{\par\endgroup}
\newcommand{\WEFTeol}{\par\penalty\WEFTbreakpenalty\relax}
\newcommand{\WEFTuseHyperlinks}{}
\documentclass{article}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\WEFTcode
\mbox{\strut}\WEFTsp First\WEFTsp use\WEFTsp in\WEFTsp global\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2a}{, 2a}\WEFTlink{weft2c}{c}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Global\WEFTsp sector\WEFTsp line\WEFTsp one.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2b}{, 2b}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2c}{, 2c}.

\item{}
\end{list}
\WEFTend

\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1c}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}First\WEFTsp sector\WEFTsp line\WEFTsp one.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1d}{1d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1d}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {1d}}$\equiv$
\WEFTcode
\mbox{\strut}Use\WEFTsp first\WEFTsp local\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2a}{, 2a}\WEFTlink{weft2c}{c}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1d}{1d}.}
\end{list}}

\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1e}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {1e}}$\equiv$
\WEFTcode
\mbox{\strut}Use\WEFTsp second\WEFTsp local\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2a}{, 2a}\WEFTlink{weft2c}{c}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1f}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1f}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Second\WEFTsp sector\WEFTsp line\WEFTsp one.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1e}{1e}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1e}{1e}.}
\end{list}}

\WEFTbegin
\label{scrap7}
\WEFTtarget{weft1g}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1g}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Global\WEFTsp sector\WEFTsp line\WEFTsp two.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2b}{, 2b}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2c}{, 2c}.

\item{}
\end{list}
\WEFTend

\WEFTbegin
\label{scrap8}
\WEFTtarget{weft1h}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1h}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Third\WEFTsp sector\WEFTsp line\WEFTsp one.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft2a}{2a}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap9}
\WEFTtarget{weft2a}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {2a}}$\equiv$
\WEFTcode
\mbox{\strut}Use\WEFTsp second\WEFTsp local\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1h}{1h}}$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2a}{, 2a}\WEFTlink{weft2c}{c}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1h}{1h}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft2a}{2a}.}
\end{list}}

\WEFTbegin
\label{scrap10}
\WEFTtarget{weft2b}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {2b}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Global\WEFTsp sector\WEFTsp line\WEFTsp three.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2b}{, 2b}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2c}{, 2c}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap11}
\WEFTtarget{weft2c}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {2c}}$\equiv$
\WEFTcode
\mbox{\strut}\WEFTsp Last\WEFTsp use\WEFTsp in\WEFTsp global\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2a}{, 2a}\WEFTlink{weft2c}{c}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2b}{, 2b}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}\WEFTlink{weft2c}{, 2c}.
}
\end{list}}
\end{document}
EOF

cat > test.expected.c <<"EOF"

#line 4 "test.w"
/* {1: test.w:4} */
 First use in global
/* Frag 1 */

#line 8 "test.w"
/* {2: test.w:8} */
Global sector line one.
/* {:2} */

#line 30 "test.w"
/* {7: test.w:30} */
Global sector line two.
/* {:7} */

#line 43 "test.w"
/* {10: test.w:43} */
Global sector line three.
/* {:10} */

#line 5 "test.w"

/* {:1} */

#line 15 "test.w"
/* {4: test.w:15} */
Use first local
/* Frag 1 */

#line 12 "test.w"
/* {3: test.w:12} */
First sector line one.
/* {:3} */

#line 16 "test.w"

/* {:4} */

#line 21 "test.w"
/* {5: test.w:21} */
Use second local
/* Frag 1 */

#line 25 "test.w"
/* {6: test.w:25} */
Second sector line one.
/* {:6} */

#line 22 "test.w"

/* {:5} */

#line 37 "test.w"
/* {9: test.w:37} */
Use second local
/* Frag 1 */

#line 34 "test.w"
/* {8: test.w:34} */
Third sector line one.
/* {:8} */

#line 38 "test.w"

/* {:9} */

#line 46 "test.w"
/* {11: test.w:46} */
 Last use in global
/* Frag 1 */

#line 8 "test.w"
/* {2: test.w:8} */
Global sector line one.
/* {:2} */

#line 30 "test.w"
/* {7: test.w:30} */
Global sector line two.
/* {:7} */

#line 43 "test.w"
/* {10: test.w:43} */
Global sector line three.
/* {:10} */

#line 47 "test.w"

/* {:11} */
EOF

# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test
if test $? -ne 0 ; then fail; fi

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.tex test.tex
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.actual.c
if test $? -ne 0 ; then fail; fi

# [Add other sub-tests that might be failed here.  If they need files
# created above to be decompressed, decompress them here ; this saves
# time if demo fails or the text-based sub-test fails.]

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
