#!/bin/sh
#
# $RCSfile: t0033a.sh,v $-- Test test/00/t0032a.sh
#
#
# Test of Cross-reference environment
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
	echo "FAILED test of Cross-reference environment" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Cross-reference environment" 1>&2
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
# test Cross-reference environment
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}
@o test.c -cc
@{Begin
Define abc cba
@<Outer @'abc@' and @'def@' retuO@>
@<Outer @'cba@' and @'fed@' retuO@>
End
@| abc cba @}

@d Outer @'Arg1@' and @'Arg2@' retuO
@{Start
Define def fed
Use abc cba
@<Inner @{x@1y@2z@} rennI@>
Finish
@| def fed@}

@d Inner @'Stuff@' rennI
@{XX>>@1<<YY@}

@o test.c -cc
@{More stuff
@}

@d Outer...
@{Added stuff to force fragment defined
cross-reference entry.
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
\WEFTtarget{weft1a}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1a}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Begin
Define abc cba
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Outer abc and def retuO}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}, \ldots\ }\WEFTrangle}>*)
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Outer cba and fed retuO}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}, \ldots\ }\WEFTrangle}>*)
End

\end{lstlisting}
\WEFTmetabegin{1a}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTdot \WEFTtxtIdentsDefed\nobreak\  {\WEFTtint{weftkw}\verb@abc@}\nobreak\ \WEFTlink{weft1b}{1b}, {\WEFTtint{weftkw}\verb@cba@}\nobreak\ \WEFTlink{weft1b}{1b}\WEFTdot \WEFTtxtIdentsUsed\nobreak\  {\WEFTtint{weftkw}\verb@def@}\nobreak\ \WEFTlink{weft1b}{1b}, {\WEFTtint{weftkw}\verb@fed@}\nobreak\ \WEFTlink{weft1b}{1b}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\,{\footnotesize\WEFTtint{weftcom}1b}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Start
Define def fed
Use abc cba
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Inner \hbox{\ttfamily xArg1yArg2z} rennI}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1c}{1c}}\WEFTrangle}>*)
Finish

\end{lstlisting}
\WEFTmetabegin{1b}\WEFTdot \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTdot \WEFTtxtIdentsDefed\nobreak\  {\WEFTtint{weftkw}\verb@def@}\nobreak\ \WEFTlink{weft1a}{1a}, {\WEFTtint{weftkw}\verb@fed@}\nobreak\ \WEFTlink{weft1a}{1a}\WEFTdot \WEFTtxtIdentsUsed\nobreak\  {\WEFTtint{weftkw}\verb@abc@}\nobreak\ \WEFTlink{weft1a}{1a}, {\WEFTtint{weftkw}\verb@cba@}\nobreak\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1c}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Inner \hbox{\slshape\sffamily Stuff\/} rennI}\nobreak\,{\footnotesize\WEFTtint{weftcom}1c}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
XX>>(*<\hbox{\slshape\sffamily Stuff\/}>*)<<YY
\end{lstlisting}
\WEFTmetabegin{1c}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1d}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1d}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
More stuff

\end{lstlisting}
\WEFTmetabegin{1d}\WEFTdot \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1e}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\,{\footnotesize\WEFTtint{weftcom}1e}\WEFTrangle\WEFTpluseq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Added stuff to force fragment defined
cross-reference entry.

\end{lstlisting}
\WEFTmetabegin{1e}\WEFTdot \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Inner \hbox{\slshape\sffamily Stuff\/} rennI}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1c}{1c}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1b}{1b}}
\item \WEFTlangle{\WEFTtint{weftaccent}\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}}\WEFTrangle\ {\footnotesize\WEFTtint{weftcom} {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}}
\end{list}}

\end{document}
EOF

cat > test.expected.c <<"EOF"
Begin
Define abc cba
/* Outer 'abc' and 'def' retuO 1b */
Start
Define def fed
Use abc cba
/* Inner {xabcydefz} rennI 1c */
XX>>xabcydefz<<YY
Finish
Added stuff to force fragment defined
cross-reference entry.

/* Outer 'cba' and 'fed' retuO 1b */
Start
Define def fed
Use abc cba
/* Inner {xcbayfedz} rennI 1c */
XX>>xcbayfedz<<YY
Finish
Added stuff to force fragment defined
cross-reference entry.

End
More stuff
EOF

$bin/weft -w tex -x test.w
if test $? -ne 0 ; then fail; fi

latex test

$bin/weft -w tex -x test.w
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
