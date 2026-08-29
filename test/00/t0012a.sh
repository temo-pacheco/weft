#!/bin/sh
#
# $RCSfile: t0012a.sh,v $-- Test test/00/t0012a.sh
#
#
# Test of block comments
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
	echo "FAILED test of block comments" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of block comments" 1>&2
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
# test block comments
#

cat > test.w <<"EOF"
\documentclass{article}
\begin{document}
@c Here is a block comment which is long enough to need line
breaking. (And a bit extra.)
Here is a block comment which is long enough to need line
breaking.
Here is a block comment which is long enough to need line
breaking. (Some more extra.)
Here is a block comment which is long enough to need line
breaking.

Here is more of the block comment. It is also long enough to
need line-breaking.

@o test.c -cc
@{Here is some stuff.
   @c
Here is the end of the stuff.
Here (@c) is a block comment in code.
@}

@c This is another block comment that shouldn't end in a
newline. (So long as its length is right.
@o test.c -cc
@{This --@c-- is where it is used.
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
\WEFTbegin
 Here is a block comment which is long enough to need line
breaking. (And a bit extra.)
Here is a block comment which is long enough to need line
breaking.
Here is a block comment which is long enough to need line
breaking. (Some more extra.)
Here is a block comment which is long enough to need line
breaking.

Here is more of the block comment. It is also long enough to
need line-breaking.

\par\vspace{\baselineskip}
\label{scrap1}
\WEFTtarget{weft?}{} \verb@"test.c"@\nobreak\ {\footnotesize {?}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Here is some stuff.
   (*<\hbox{\sffamily\slshape (Comment)}>*)
Here is the end of the stuff.
Here ((*<\hbox{\sffamily\slshape (Comment)}>*)) is a block comment in code.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft?}{?}\WEFTlink{weft?}{, ?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
 This is another block comment that shouldn't end in a
newline. (So long as its length is right.
\par\vspace{\baselineskip}
\label{scrap2}
\WEFTtarget{weft?}{} \verb@"test.c"@\nobreak\ {\footnotesize {?}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
This --(*<\hbox{\sffamily\slshape (Comment)}>*)-- is where it is used.

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft?}{?}\WEFTlink{weft?}{, ?}.

\item{}
\end{list}
\WEFTend
\end{document}
EOF

cat > test.expected.c <<"EOF"
Here is some stuff.
   /* Here is a block comment which is long enough to need line
    * breaking. (And a bit extra.) Here is a block comment which
    * is long enough to need line breaking. Here is a block comment
    * which is long enough to need line breaking. (Some more extra.)
    * Here is a block comment which is long enough to need line
    * breaking.
    * Here is more of the block comment. It is also long enough
    * to need line-breaking.
    */
Here is the end of the stuff.
Here (/* Here is a block comment which is long enough to need
       * line breaking. (And a bit extra.) Here is a block comment
       * which is long enough to need line breaking. Here is a
       * block comment which is long enough to need line breaking.
       * (Some more extra.) Here is a block comment which is long
       * enough to need line breaking.
       * Here is more of the block comment. It is also long enough
       * to need line-breaking.
       */) is a block comment in code.
This --/* This is another block comment that shouldn't end in
        * a newline. (So long as its length is right.  */-- is where it is used.
EOF

# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

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
