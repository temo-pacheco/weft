#!/bin/sh
#
# $RCSfile: t0009a.sh,v $-- Test test/00/t0009a.sh
#
#
# Test of user specified use of identifiers
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
	echo "FAILED test of user specified use of identifiers" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of user specified use of identifiers" 1>&2
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
# test user specified use of identifiers
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}
Define a thing.
@o test.c -cc
@{This defines ident.
@<Use the thing defined@>
@| ident @}

Now use it without it actually appearing in the text.

@d Use the thing defined
@{Pretend that we use it here.
@* ident @}
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
Define a thing.
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} \verb@"test.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
This defines ident.
(*<\hbox{\normalfont $\langle\,${\itshape Use the thing defined}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtIdentsDefed\nobreak\  \verb@ident@\nobreak\ \WEFTlink{weft1b}{1b}.
\item{}
\end{list}
\WEFTend
Now use it without it actually appearing in the text.

\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Use the thing defined}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Pretend that we use it here.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.
\item \WEFTtxtIdentsUsed\nobreak\  \verb@ident@\nobreak\ \WEFTlink{weft1a}{1a}.
\item{}
\end{list}
\WEFTend
\end{document}
EOF

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test
latex test

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.tex test.tex
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
