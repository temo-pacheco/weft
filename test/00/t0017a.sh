#!/bin/sh
#
# $RCSfile: t0017a.sh,v $-- Test test/00/t0017a.sh
#
#
# Test of quoted scraps
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
	echo "FAILED test of quoted scraps" 1>&2
	cd $here
        rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of quoted scraps" 1>&2
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
# test quoted scraps
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}
@o test.c -cc
@{Test of quoted fragments.
   @<Insert first fragment@>
   @<Insert second fragment@>
   @<Insert third fragment@>
   @<Insert parameter @'whatsit@'...@>
End of test.
@}

@d Insert first fragment
@{This fragment is not quoted.
   @<Insert unquoted fragment@>
   @<Insert quoted fragment@>
   @<Insert parameter @'1@'...@>
   @<Insert param...@>
End of first fragment.@}

@q Insert second fragment
@{This fragment is quoted.
   @<Insert unquoted fragment@>
   @<Insert quoted fragment@>
   @<Insert parameter @'2@'...@>
   @<Insert param...@>
End of second fragment.@}

@d Insert third fragment
@{This fragment is not quoted.
   @<Insert unquoted fragment@>
   @<Insert quoted fragment@>
   @<Insert parameter @'3@'...@>
   @<Insert param...@>
End of third fragment.@}

@d Insert unquoted fragment
@{This fragment in file @f is not quoted@}

@q Insert quoted fragment
@{This fragment in file @f is quoted@}

@d Insert parameter @'thing@' fragment
@{Here >>@1<< is the parameter@}

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
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft1a}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1a}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
Test of quoted fragments.
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert first fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1b}{1b}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert second fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1c}{1c}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert third fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1d}{1d}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter whatsit fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
End of test.

\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1a}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert first fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1b}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This fragment is not quoted.
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert unquoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1e}{1e}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert quoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1f}{1f}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter 1 fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter thing fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
End of first fragment.
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1b}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert second fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1c}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This fragment is quoted.
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert unquoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1e}{1e}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert quoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1f}{1f}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter 2 fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter thing fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
End of second fragment.
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1c}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1d}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert third fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1d}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This fragment is not quoted.
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert unquoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1e}{1e}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert quoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft1f}{1f}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter 3 fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter thing fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft2}{2}}\WEFTrangle}>*)
End of third fragment.
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1d}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1e}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert unquoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1e}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This fragment in file (*<\hbox{\sffamily\slshape file name}>*) is not quoted
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1e}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1f}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert quoted fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}1f}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
This fragment in file (*<\hbox{\sffamily\slshape file name}>*) is quoted
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{1f}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap7}
\WEFTtarget{weft2}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Insert parameter \hbox{\slshape\sffamily thing\/} fragment}\nobreak\WEFTnumsep{\footnotesize\WEFTtint{weftcom}2}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
Here >>(*<\hbox{\slshape\sffamily thing\/}>*)<< is the parameter
\end{lstlisting}
\WEFTsep
\WEFTmetabegin{2}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1b}{b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}\WEFTmetaend
\WEFTend
\end{document}
EOF

cat > test.expected.c <<"EOF"
Test of quoted fragments.
   /* Insert first fragment */
   This fragment is not quoted.
      /* Insert unquoted fragment */
      This fragment in file test.c is not quoted
      /* Insert quoted fragment */
      This fragment in file @f is quoted
      /* Insert parameter '1' fragment */
      Here >>1<< is the parameter
      /* Insert parameter 'thing' fragment */
      Here >>thing<< is the parameter
   End of first fragment.
   /* Insert second fragment */
   This fragment is quoted.
      @<Insert unquoted fragment@>
      @<Insert quoted fragment@>
      @<Insert parameter @'2@' fragment@>
      @<Insert parameter @'thing@' fragment@>
   End of second fragment.
   /* Insert third fragment */
   This fragment is not quoted.
      /* Insert unquoted fragment */
      This fragment in file test.c is not quoted
      /* Insert quoted fragment */
      This fragment in file @f is quoted
      /* Insert parameter '3' fragment */
      Here >>3<< is the parameter
      /* Insert parameter 'thing' fragment */
      Here >>thing<< is the parameter
   End of third fragment.
   /* Insert parameter 'whatsit' fragment */
   Here >>whatsit<< is the parameter
End of test.
EOF

# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

$bin/weft -w tex test.w
if test $? -ne 0 ; then fail; fi

latex test
if test $? -ne 0 ; then fail; fi

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
