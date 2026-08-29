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
\WEFTtarget{weft?}{} $\langle\,${\itshape Here is a scrap.}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This is the contents of the scrap.

More stuff, not related to songs without ends.

"This is a song that will get on your nerves,
   Get on your nerves,
   Get on your nerves,"

Ad infinitum.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item {\WEFTtxtMacroNoRef}.

\item{}
\end{list}
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
