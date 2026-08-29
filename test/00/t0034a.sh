#!/bin/sh
#
# $RCSfile: t0034a.sh,v $-- Test test/00/t0034a.sh
#
#
# Test of Listing package
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
	echo "FAILED test of Listing package" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Listing package" 1>&2
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
# test Listing package
#

cat > test.w <<"EOF"
\documentclass{article}
\usepackage{listings}
\begin{document}

\lstset{extendedchars=true,keepspaces=true,language=C}

@o test.c -cc
@{int
main(int argc, char ** argv)
{
   @<Body of main@>
}
@}

@d Body...
@{int in;
unsigned char out[20];

while (scanf("%x", &in) == 1)
{
   @<Do one item@>
}
return 0;@}

@d Do...
@{int n = mangle(in, out);

for (int i = 0; i < n; i++)
   printf("%02x", out[i]);
printf("\n");@}

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

\lstset{extendedchars=true,keepspaces=true,language=C}

\WEFTbegin
\label{scrap1}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\verb@test.c@}\nobreak\,{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
int
main(int argc, char ** argv)
{
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Body of main}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft?}{?}}\WEFTrangle}>*)
}

\end{lstlisting}
\WEFTmetabegin{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Body of main}\nobreak\,{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
int in;
unsigned char out[20];

while (scanf("%x", &in) == 1)
{
   (*<\hbox{\normalfont \WEFTlangle{\WEFTtint{weftaccent}\itshape Do one item}\nobreak\,{\footnotesize\WEFTtint{weftcom}\WEFTlink{weft?}{?}}\WEFTrangle}>*)
}
return 0;
\end{lstlisting}
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{}\WEFTlangle{\WEFTtint{weftaccent}\itshape Do one item}\nobreak\,{\footnotesize\WEFTtint{weftcom}?}\WEFTrangle\WEFTeq\par\nobreak\vspace{0.6ex}\nobreak
\begin{lstlisting}[escapeinside={(*<}{>*)}]
int n = mangle(in, out);

for (int i = 0; i < n; i++)
   printf("%02x", out[i]);
printf("\n");
\end{lstlisting}
\WEFTmetabegin{?}\WEFTdot \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}\WEFTmetaend
\WEFTend
\end{document}
EOF

# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

$bin/weft -w tex -l test.w
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
