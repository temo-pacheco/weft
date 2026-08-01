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
\newcommand{\WEFTbreakpenalty}{500}
\newcommand{\WEFTsp}{\hskip\fontdimen2\font\relax}
\newcommand{\WEFTbrk}{\discretionary{}{}{}}
\newlength{\WEFTindent}\setlength{\WEFTindent}{1.5em}
\newlength{\WEFThang}\setlength{\WEFThang}{2em}
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\small\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\newcommand{\WEFTcode}{\par\nobreak\vspace{-.5ex}\begingroup\ttfamily\small\parindent0pt\parskip0pt\raggedright\leftskip\WEFTindent\hangindent\WEFThang\hangafter1\relax\everypar{\hangindent\WEFThang\hangafter1\relax}}
\newcommand{\WEFTendcode}{\par\endgroup}
\newcommand{\WEFTeol}{\par\penalty\WEFTbreakpenalty\relax}
\newcommand{\WEFTuseHyperlinks}{}
\documentclass{article}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft?}{} \verb@"test.c"@\nobreak\ {\footnotesize {?}}$\equiv$
\WEFTcode
\mbox{\strut}Call\WEFTsp the\WEFTsp macro\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Fragment with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Second frag with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Third frag with $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ as parameter}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{} $\langle\,${\itshape Fragment with \hbox{\slshape\sffamily Begin macro\/} as parameter}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}\hbox{\slshape\sffamily Begin macro\/}<\WEFTbrk <\WEFTbrk <\WEFTbrk Here\WEFTsp 'tis.\WEFTbrk \WEFTeol
\mbox{\strut}That\WEFTsp argument\WEFTsp was\WEFTsp at\WEFTsp the\WEFTsp beginning\WEFTsp of\WEFTsp the\WEFTsp fragment{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{} $\langle\,${\itshape Second frag with \hbox{\slshape\sffamily Begin line\/} as parameter}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Here\WEFTsp is\WEFTsp the\WEFTsp beginning\WEFTsp of\WEFTsp the\WEFTsp second\WEFTsp macro\WEFTeol
\mbox{\strut}\hbox{\slshape\sffamily Begin line\/}<\WEFTbrk <\WEFTbrk <\WEFTbrk That\WEFTsp is\WEFTsp the\WEFTsp argument\WEFTeol
\mbox{\strut}And\WEFTsp this\WEFTsp is\WEFTsp the\WEFTsp end\WEFTsp of\WEFTsp the\WEFTsp second\WEFTsp frag{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft?}{} $\langle\,${\itshape Third frag with \hbox{\slshape\sffamily Embedded\/} as parameter}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Here\WEFTsp is\WEFTsp the\WEFTsp argument>\WEFTbrk >\WEFTbrk >\WEFTbrk \hbox{\slshape\sffamily Embedded\/}<\WEFTbrk <\WEFTbrk <\WEFTbrk That\WEFTsp was\WEFTsp it.\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft?}{} $\langle\,${\itshape A macro argument}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Hello\WEFTsp folks{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
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
