#!/bin/sh
#
# $RCSfile: t0016a.sh,v $-- Test test/00/t0016a.sh
#
#
# Test no pagebreak before scraps
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
	echo "FAILED test no pagebreak before scraps" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test no pagebreak before scraps" 1>&2
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
\begin{document}
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

@c Here comes a block comment just before a scrap and we don't
want this separated from it. (Thinks:
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:)

@d Here is a scrap.
@{This is the contents of the scrap.

More stuff, not related to songs without ends.

"This is a song that will get on your nerves,
   Get on your nerves,
   Get on your nerves,"

Ad infinitum.
@}

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
\newcommand{\WEFTnumsep}{\ }
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
\begin{document}
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:

\WEFTbegin
 Here comes a block comment just before a scrap and we don't
want this separated from it. (Thinks:
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:
This is a song without and end. It goes on and on my friend. Some
people started singing it, not knowing what it was, and they'll
go on forever just because:)

\par\vspace{\baselineskip}
\label{scrap1}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Here is a scrap.}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This is the contents of the scrap.

More stuff, not related to songs without ends.

"This is a song that will get on your nerves,
   Get on your nerves,
   Get on your nerves,"

Ad infinitum.

\end{lstlisting}
\WEFTmetabegin{?}\WEFTdot {\WEFTtxtMacroNoRef}\WEFTmetaend
\WEFTend
\end{document}
EOF

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
