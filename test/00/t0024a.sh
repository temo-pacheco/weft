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
\usepackage{listings}
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
\newcommand{\WEFTtxtMacroDefBy}{defined at}
\newcommand{\WEFTtxtMacroRefIn}{used in}
\newcommand{\WEFTtxtMacroNoRef}{unreferenced}
\newcommand{\WEFTtxtDefBy}{defined at}
\newcommand{\WEFTtxtRefIn}{used in}
\newcommand{\WEFTtxtNoRef}{unreferenced}
\newcommand{\WEFTtxtFileDefBy}{defined at}
\newcommand{\WEFTtxtIdentsUsed}{uses}
\newcommand{\WEFTtxtIdentsNotUsed}{never used}
\newcommand{\WEFTtxtIdentsDefed}{defines}
\newcommand{\WEFTsep}{}
\newcommand{\WEFTnotglobal}{(not defined globally)}
\newcommand{\WEFTtint}[1]{}
\newcommand{\WEFTtintOn}[1]{\color{#1}}
\newcommand{\WEFTlangle}{{\WEFTtint{weftcom}$\langle$}\,}
\newcommand{\WEFTrangle}{\,{\WEFTtint{weftcom}$\rangle$}}
\newcommand{\WEFTeq}{\ {\WEFTtint{weftcom}$\equiv$}}
\newcommand{\WEFTpluseq}{\ {\WEFTtint{weftcom}$\mathord{+}\equiv$}}
\newcommand{\WEFTdot}{\ {\WEFTtint{weftcom}\textperiodcentered}\ }
\newcommand{\WEFTmetabegin}[1]{\par\vspace{3pt}\begingroup\footnotesize\WEFTtint{weftcom}#1}
\newcommand{\WEFTmetaend}{\endgroup\par}
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\providecommand{\WEFTlstsetup}{\lstset{basicstyle=\ttfamily\fontsize{10pt}{10.8pt}\selectfont,keywordstyle={\WEFTtint{weftkw}\bfseries},commentstyle={\WEFTtint{weftcom}\itshape},stringstyle={\WEFTtint{weftstr}},breaklines=true,breakatwhitespace=false,columns=fullflexible,keepspaces=true,showstringspaces=false,tabsize=8,frame=none,xleftmargin=0pt,aboveskip=0pt,belowskip=0pt}}
\newcommand{\WEFTuseHyperlinks}{}
\AtBeginDocument{%
\ifdefined\definecolor
\definecolor{weftaccent}{rgb}{0.184,0.435,0.922}%
\definecolor{weftkw}{rgb}{0.020,0.314,0.682}%
\definecolor{weftstr}{rgb}{0.067,0.388,0.161}%
\definecolor{weftcom}{rgb}{0.431,0.467,0.506}%
\let\WEFTtint\WEFTtintOn
\fi
\ifdefined\lstset\WEFTlstsetup\fi
}
\documentclass{article}
\usepackage{listings}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.actual.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1a}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
 First use in global
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}, \ldots\ }\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{1a}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft1i}{i}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}1b}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Global sector line one.

\end{lstlisting}
\WEFTmetabegin{1b}\WEFTdot \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2a}{, 2a}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend

\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}1c}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
First sector line one.

\end{lstlisting}
\WEFTmetabegin{1c}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1d}{1d}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1d}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.actual.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1d}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use first local
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1c}{1c}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{1d}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft1i}{i}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1c}{1c}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1d}{1d}}
\end{list}}

\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1e}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.actual.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1e}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use second local
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1f}{1f}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{1e}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft1i}{i}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1f}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}1f}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Second sector line one.

\end{lstlisting}
\WEFTmetabegin{1f}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1e}{1e}\WEFTmetaend
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1f}{1f}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1e}{1e}}
\end{list}}

\WEFTbegin
\label{scrap7}
\WEFTtarget{weft1g}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}1g}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Global sector line two.

\end{lstlisting}
\WEFTmetabegin{1g}\WEFTdot \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2a}{, 2a}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend

\WEFTbegin
\label{scrap8}
\WEFTtarget{weft1h}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}1h}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Third sector line one.

\end{lstlisting}
\WEFTmetabegin{1h}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1i}{1i}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap9}
\WEFTtarget{weft1i}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.actual.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1i}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use second local
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1h}{1h}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{1i}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft1i}{i}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1h}{1h}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1i}{1i}}
\end{list}}

\WEFTbegin
\label{scrap10}
\WEFTtarget{weft2a}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}2a}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Global sector line three.

\end{lstlisting}
\WEFTmetabegin{2a}\WEFTdot \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2a}{, 2a}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap11}
\WEFTtarget{weft2b}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.actual.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}2b}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
 Last use in global
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}, \ldots\ }\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{2b}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft1i}{i}\WEFTlink{weft2b}{, 2b}\WEFTmetaend
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Frag 1}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2a}{, 2a}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}\WEFTlink{weft2b}{, 2b}}
\end{list}}
\end{document}
EOF

cat > test.expected.c <<"EOF"

#line 5 "test.w"
/* {1: test.w:5} */
 First use in global
/* Frag 1 */

#line 9 "test.w"
/* {2: test.w:9} */
Global sector line one.
/* {:2} */

#line 31 "test.w"
/* {7: test.w:31} */
Global sector line two.
/* {:7} */

#line 44 "test.w"
/* {10: test.w:44} */
Global sector line three.
/* {:10} */

#line 6 "test.w"

/* {:1} */

#line 16 "test.w"
/* {4: test.w:16} */
Use first local
/* Frag 1 */

#line 13 "test.w"
/* {3: test.w:13} */
First sector line one.
/* {:3} */

#line 17 "test.w"

/* {:4} */

#line 22 "test.w"
/* {5: test.w:22} */
Use second local
/* Frag 1 */

#line 26 "test.w"
/* {6: test.w:26} */
Second sector line one.
/* {:6} */

#line 23 "test.w"

/* {:5} */

#line 38 "test.w"
/* {9: test.w:38} */
Use second local
/* Frag 1 */

#line 35 "test.w"
/* {8: test.w:35} */
Third sector line one.
/* {:8} */

#line 39 "test.w"

/* {:9} */

#line 47 "test.w"
/* {11: test.w:47} */
 Last use in global
/* Frag 1 */

#line 9 "test.w"
/* {2: test.w:9} */
Global sector line one.
/* {:2} */

#line 31 "test.w"
/* {7: test.w:31} */
Global sector line two.
/* {:7} */

#line 44 "test.w"
/* {10: test.w:44} */
Global sector line three.
/* {:10} */

#line 48 "test.w"

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
