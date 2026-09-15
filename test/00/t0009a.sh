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
\newcommand{\WEFTsep}{\par\nobreak\noindent{\WEFTtint{weftcom}\fboxsep=0pt\fboxrule=.3pt\fbox{\rule{0pt}{.85ex}\kern.85ex}}\par}
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
\usepackage{listings}
\begin{document}
Define a thing.
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1a}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
This defines ident.
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Use the thing defined}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1a}\WEFTdot \WEFTtxtIdentsDefed\nobreak\  {\WEFTtint{weftkw}\verb@ident@}\nobreak\ \WEFTlink{weft1b}{1b}\WEFTmetaend
\WEFTend
Now use it without it actually appearing in the text.

\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Use the thing defined}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1b}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Pretend that we use it here.

\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1b}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTdot \WEFTtxtIdentsUsed\nobreak\  {\WEFTtint{weftkw}\verb@ident@}\nobreak\ \WEFTlink{weft1a}{1a}\WEFTmetaend
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
