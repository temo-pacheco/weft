#!/bin/sh
#
# $RCSfile: t0004a.sh,v $-- Test test/00/t0004a.sh
#
#
# Test of indent before parameter
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
	echo "FAILED test of indent before parameter" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of indent before parameter" 1>&2
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
# test indent before parameter
#

cat > test.w <<"EOF"
\documentclass{article}
\begin{document}
@o test.c
@{Start
   @<Here is the first call@>
End
@}

@d Here is the first call
@{@<Use @'xxx@' as parameter@>
@<Use @'ZZZ@' as parameter@>
@}

@d Use @'yyy@' as...
@{@1 is here.
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
\mbox{\strut}Start\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Here is the first call}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{} $\langle\,${\itshape Here is the first call}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}\hbox{$\langle\,${\itshape Use xxx as parameter}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Use ZZZ as parameter}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{} $\langle\,${\itshape Use \hbox{\slshape\sffamily yyy\/} as parameter}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}\hbox{\slshape\sffamily yyy\/}\WEFTsp is\WEFTsp here.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
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

#line 4 "test.w"
/* {1: test.w:4} */
Start
   /* Here is the first call */
   
#line 10 "test.w"
/* {2: test.w:10} */
   /* Use 'xxx' as parameter */

#line 15 "test.w"
/* {3: test.w:15} */
   xxx is here.
   /* {:3} */

#line 10 "test.w"

   /* Use 'ZZZ' as parameter */

#line 15 "test.w"
/* {3: test.w:15} */
   ZZZ is here.
   /* {:3} */

#line 11 "test.w"

   /* {:2} */

#line 5 "test.w"

End
/* {:1} */
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
