#!/bin/sh
#
# $RCSfile: t0035a.sh,v $-- Test test/00/t0018a.sh
#
#
# Test of hyperlinks using -r
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
	echo "FAILED test of hyperlinks using -r" 1>&2
	cd $here
        rm -rf $work
        exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of hyperlinks using -r" 1>&2
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
# test hyperlinks using -r
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\usepackage[pdftex,colorlinks=true]{hyperref}
\begin{document}
@o test.c -cc
@{@<Use a fragment@>
@}
@d Use...
@{Here is a fragment.
   Make sure it is referenced properly.@}
\end{document}
EOF

cat > test.expected.tex <<"EOF"
\newcommand{\WEFTtarget}[2]{\hypertarget{#1}{#2}}
\newcommand{\WEFTlink}[2]{\hyperlink{#1}{#2}}
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
\usepackage[pdftex,colorlinks=true]{hyperref}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} \verb@"test.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
(*<\hbox{\normalfont $\langle\,${\itshape Use a fragment}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Use a fragment}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Here is a fragment.
   Make sure it is referenced properly.
\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend
\end{document}
EOF


# [Add other files here.  Avoid any extra processing such as
# decompression until after weft has run.  If weft fails this script
# can save time by not decompressing. ]

$bin/weft -w tex -r test.w
if test $? -ne 0 ; then fail; fi

pdflatex test
if test $? -ne 0 ; then fail; fi

$bin/weft -w tex -r test.w
if test $? -ne 0 ; then fail; fi

pdflatex test
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.tex test.tex
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
