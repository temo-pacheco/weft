#!/bin/sh
#
# $RCSfile: t0005a.sh,v $-- Test test/00/t0005a.sh
#
#
# Test of Commenting macroes as argument uses
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
	echo "FAILED test of Commenting macroes as argument uses" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Commenting macroes as argument uses" 1>&2
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
# test Commenting macroes as argument uses
#

cat > test.w <<"EOF"
\documentclass{article}
\begin{document}
@o test.c -cc
@{Call the macro
   @<Fragment with @<A macro argument@> as parameter@>
   @<Second frag with @<A macro argument@> as parameter@>
   @<Third frag with @<A macro argument@> as parameter@>
@}

@d Fragment with @'Begin macro@'...
@{@1<<<Here 'tis.
That argument was at the beginning of the fragment@}

@d Second frag with @'Begin line@'...
@{Here is the beginning of the second macro
@1<<<That is the argument
And this is the end of the second frag@}

@d Third frag with @'Embedded@'...
@{Here is the argument>>>@1<<<That was it.@}

@d A macro argument
@{Hello folks@}
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
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Call the macro
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Fragment with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft?}{?}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Second frag with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft?}{?}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Third frag with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft?}{?}}\WEFTrangle}>*)

\end{lstlisting}
\WEFTsep
\WEFTmetabegin{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Fragment with \hbox{\slshape\sffamily Begin macro\/} as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
(*<\hbox{\slshape\sffamily Begin macro\/}>*)<<<Here 'tis.
That argument was at the beginning of the fragment
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Second frag with \hbox{\slshape\sffamily Begin line\/} as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Here is the beginning of the second macro
(*<\hbox{\slshape\sffamily Begin line\/}>*)<<<That is the argument
And this is the end of the second frag
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Third frag with \hbox{\slshape\sffamily Embedded\/} as parameter}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Here is the argument>>>(*<\hbox{\slshape\sffamily Embedded\/}>*)<<<That was it.
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape A macro argument}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Hello folks
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\end{document}
EOF

cat > test.expected.c <<"EOF"
Call the macro
   /* Fragment with <A macro argument> as parameter */
   /* A macro argument */
   Hello folks<<<Here 'tis.
   That argument was at the beginning of the fragment
   /* Second frag with <A macro argument> as parameter */
   Here is the beginning of the second macro
   /* A macro argument */
   Hello folks<<<That is the argument
   And this is the end of the second frag
   /* Third frag with <A macro argument> as parameter */
   Here is the argument>>>Hello folks<<<That was it.
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
