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
Here is a macro that defines something.
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Define something}\nobreak\,{\footnotesize\WEFTtint{weftcom}1a}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
something anything

\end{lstlisting}
\WEFTmetabegin{1a}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1c}{1c}\WEFTdot \WEFTtxtIdentsDefed\nobreak\  {\WEFTtint{weftkw}\verb@anything@}\nobreak\ \WEFTlink{weft1b}{1b}, {\WEFTtint{weftkw}\verb@something@}\nobreak\ \WEFTlink{weft1c}{1c}\WEFTmetaend
\WEFTend
Here is a macro that uses an argument
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Use the \hbox{\slshape\sffamily thing\/}}\nobreak\,{\footnotesize\WEFTtint{weftcom}1b}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Use (*<\hbox{\slshape\sffamily thing\/}>*)
Use anything

\end{lstlisting}
\WEFTmetabegin{1b}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1c}{1c}\WEFTdot \WEFTtxtIdentsUsed\nobreak\  {\WEFTtint{weftkw}\verb@anything@}\nobreak\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend
Now use the something in an argument
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}1c}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]

(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Define something}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1a}{1a}}\WEFTrangle}>*)
(*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Use the something}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTmetabegin{1c}\WEFTdot \WEFTtxtIdentsUsed\nobreak\  {\WEFTtint{weftkw}\verb@something@}\nobreak\ \WEFTlink{weft1a}{1a}\WEFTmetaend
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
