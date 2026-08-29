#!/bin/sh
#
# $RCSfile: t0032a.sh,v $-- Test test/00/t0032a.sh
#
#
# Test of Embedded fragments as arguments
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
	echo "FAILED test of Embedded fragments as arguments" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Embedded fragments as arguments" 1>&2
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
# test Embedded fragments as arguments
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}
@o test.c -cc
@{Begin
@<Outer @'abc@' and @'def@' retuO@>
@<Outer @'cba@' and @'fed@' retuO@>
End
@}

@d Outer @'Arg1@' and @'Arg2@' retuO
@{Start
@<Inner @{x@1y@2z@} rennI@>
Finish
@}

@d Inner @'Stuff@' rennI
@{XX>>@1<<YY@}

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
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\small\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\providecommand{\WEFTlstsetup}{\lstset{basicstyle=\ttfamily\small,breaklines=true,breakatwhitespace=false,columns=fullflexible,keepspaces=true,showstringspaces=false,keywordstyle=\bfseries,commentstyle=\itshape,tabsize=8,xleftmargin=1.5em}}
\newcommand{\WEFTuseHyperlinks}{}
\documentclass{article}
\usepackage{listings}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{} \verb@"test.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Begin
(*<\hbox{\normalfont $\langle\,${\itshape Outer abc and def retuO}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Outer cba and fed retuO}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}>*)
End

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
\WEFTtarget{weft1b}{} $\langle\,${\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Start
(*<\hbox{\normalfont $\langle\,${\itshape Inner \hbox{\ttfamily xArg1yArg2z} rennI}\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$}>*)
Finish

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1c}{} $\langle\,${\itshape Inner \hbox{\slshape\sffamily Stuff\/} rennI}\nobreak\ {\footnotesize {1c}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]
XX>>(*<\hbox{\slshape\sffamily Stuff\/}>*)<<YY
\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Inner \hbox{\slshape\sffamily Stuff\/} rennI\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1b}{1b}.}
\item $\langle\,$Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}.}
\end{list}}

\end{document}
EOF

cat > test.expected.c <<"EOF"
Begin
/* Outer 'abc' and 'def' retuO */
Start
/* Inner {xabcydefz} rennI */
XX>>xabcydefz<<YY
Finish

/* Outer 'cba' and 'fed' retuO */
Start
/* Inner {xcbayfedz} rennI */
XX>>xcbayfedz<<YY
Finish

End
EOF

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.tex test.tex
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.c
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
