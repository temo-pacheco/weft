#!/bin/sh
#
# $RCSfile: t0007a.sh,v $-- Test test/00/t0007a.sh
#
#
# Test of ???
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
	echo "FAILED test of ???" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of ???" 1>&2
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
# test ???
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}
Here is a macro that defines something.
@d Define something
@{something anything
@| something anything @}

Here is a macro that uses an argument
@d Use the @'thing@'
@{Use @1
Use anything
@}

Now use the something in an argument
@o test.c -cc
@{
@<Define something@>
@<Use the @'something@'@>
@}
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
Here is a macro that defines something.
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} $\langle\,${\itshape Define something}\nobreak\ {\footnotesize {1a}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
something anything

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1c}{1c}.
\item \WEFTtxtIdentsDefed\nobreak\  \verb@anything@\nobreak\ \WEFTlink{weft1b}{1b}, \verb@something@\nobreak\ \WEFTlink{weft1c}{1c}.
\item{}
\end{list}
\WEFTend
Here is a macro that uses an argument
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Use the \hbox{\slshape\sffamily thing\/}}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Use (*<\hbox{\slshape\sffamily thing\/}>*)
Use anything

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1c}{1c}.
\item \WEFTtxtIdentsUsed\nobreak\  \verb@anything@\nobreak\ \WEFTlink{weft1a}{1a}.
\item{}
\end{list}
\WEFTend
Now use the something in an argument
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{} \verb@"test.c"@\nobreak\ {\footnotesize {1c}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]

(*<\hbox{\normalfont $\langle\,${\itshape Define something}\nobreak\ {\footnotesize \WEFTlink{weft1a}{1a}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Use the something}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtIdentsUsed\nobreak\  \verb@something@\nobreak\ \WEFTlink{weft1a}{1a}.
\item{}
\end{list}
\WEFTend
\end{document}
EOF

cat > test.expected.c <<"EOF"

/* Define something */
something anything

/* Use the 'something' */
Use something
Use anything

EOF


# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.tex test.tex
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.c
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
