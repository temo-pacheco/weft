#!/bin/sh
#
# $RCSfile: t0025a.sh,v $-- Test test/00/t0024a.sh
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
@{Base sector line one.
@}
@s
@d Frag 1
@{First sector line one.
@}
@o test.actual.c
@{Use first local
@<Frag 1@>
@<+ Frag 2@>
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
@{Base sector line two.
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
@{Base sector line three.
@}
@o test.actual.c
@{ Last use in global
@<Frag 1@>
@}
@d+ Frag 2
@{Here is frag 2
@}
@m
@m+
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
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\small\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\providecommand{\WEFTlstsetup}{\lstset{basicstyle=\ttfamily\small,breaklines=true,breakatwhitespace=false,columns=fullflexible,keepspaces=true,showstringspaces=false,keywordstyle=\bfseries,commentstyle=\itshape,tabsize=8,xleftmargin=1.5em}}
\newcommand{\WEFTuseHyperlinks}{}
\documentclass{article}
\usepackage{listings}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
 First use in global
(*<\hbox{\normalfont $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2b}{, 2b}\WEFTlink{weft2d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Base sector line one.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2c}{, 2c}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2d}{, 2d}.

\item{}
\end{list}
\WEFTend

\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1c}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
First sector line one.

\end{lstlisting}
{\WEFTsep}
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
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use first local
(*<\hbox{\normalfont $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Frag 2}\nobreak\ {\footnotesize \WEFTlink{weft2e}{2e}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2b}{, 2b}\WEFTlink{weft2d}{d}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1d}{1d}.}
\end{list}}

\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1e}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {1e}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use second local
(*<\hbox{\normalfont $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2b}{, 2b}\WEFTlink{weft2d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1f}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {1f}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Second sector line one.

\end{lstlisting}
{\WEFTsep}
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
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Base sector line two.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2c}{, 2c}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2d}{, 2d}.

\item{}
\end{list}
\WEFTend

\WEFTbegin
\label{scrap8}
\WEFTtarget{weft2a}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {2a}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Third sector line one.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft2b}{2b}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap9}
\WEFTtarget{weft2b}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {2b}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Use second local
(*<\hbox{\normalfont $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft2a}{2a}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2b}{, 2b}\WEFTlink{weft2d}{d}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft2a}{2a}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft2b}{2b}.}
\end{list}}

\WEFTbegin
\label{scrap10}
\WEFTtarget{weft2c}{} $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize {2c}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Base sector line three.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2c}{, 2c}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft2d}{, 2d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap11}
\WEFTtarget{weft2d}{} \verb@"test.actual.c"@\nobreak\ {\footnotesize {2d}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
 Last use in global
(*<\hbox{\normalfont $\langle\,${\itshape Frag 1}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTlink{weft1e}{e}\WEFTlink{weft2b}{, 2b}\WEFTlink{weft2d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap12}
\WEFTtarget{weft2e}{} $\langle\,${\itshape Frag 2}\nobreak\ {\footnotesize {2e}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Here is frag 2

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1d}{1d}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 1\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}\WEFTlink{weft1g}{g}\WEFTlink{weft2c}{, 2c}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}\WEFTlink{weft2d}{, 2d}.
}
\end{list}}

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Frag 2\nobreak\ {\footnotesize \WEFTlink{weft2e}{2e}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1d}{1d}.}
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
Base sector line one.
/* {:2} */

#line 32 "test.w"
/* {7: test.w:32} */
Base sector line two.
/* {:7} */

#line 45 "test.w"
/* {10: test.w:45} */
Base sector line three.
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

/* Frag 2 */

#line 52 "test.w"
/* {12: test.w:52} */
Here is frag 2
/* {:12} */

#line 18 "test.w"

/* {:4} */

#line 23 "test.w"
/* {5: test.w:23} */
Use second local
/* Frag 1 */

#line 27 "test.w"
/* {6: test.w:27} */
Second sector line one.
/* {:6} */

#line 24 "test.w"

/* {:5} */

#line 39 "test.w"
/* {9: test.w:39} */
Use second local
/* Frag 1 */

#line 36 "test.w"
/* {8: test.w:36} */
Third sector line one.
/* {:8} */

#line 40 "test.w"

/* {:9} */

#line 48 "test.w"
/* {11: test.w:48} */
 Last use in global
/* Frag 1 */

#line 9 "test.w"
/* {2: test.w:9} */
Base sector line one.
/* {:2} */

#line 32 "test.w"
/* {7: test.w:32} */
Base sector line two.
/* {:7} */

#line 45 "test.w"
/* {10: test.w:45} */
Base sector line three.
/* {:10} */

#line 49 "test.w"

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
