#!/bin/sh
#
# $RCSfile: t0015a.sh,v $-- Test test/00/t0015a.sh
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
\begin{document}
@d Sort @'key@' of size @'n@' for @'ordering@'
@{for (int j = 1; j < @2; j++)
{
   int i = j - 1;
   int kj = @1[j];

   do
   {
      int ki = @1[i];

      if (@3)
         break;
      @1[i + 1] = ki;
      i -= 1;
   } while (i >= 0);
   @1[i + 1] = kj;
}
@}

Test in-text @{@<Sort @'key@'...@>@} usage.
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
\WEFTtarget{weft?}{} $\langle\,${\itshape Sort \hbox{\slshape\sffamily key\/} of size \hbox{\slshape\sffamily n\/} for \hbox{\slshape\sffamily ordering\/}}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}for\WEFTsp (\WEFTbrk int\WEFTsp j\WEFTsp =\WEFTbrk \WEFTsp 1;\WEFTbrk \WEFTsp j\WEFTsp <\WEFTbrk \WEFTsp \hbox{\slshape\sffamily n\/};\WEFTbrk \WEFTsp j+\WEFTbrk +\WEFTbrk )\WEFTbrk \WEFTeol
\mbox{\strut}\{\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp int\WEFTsp i\WEFTsp =\WEFTbrk \WEFTsp j\WEFTsp -\WEFTbrk \WEFTsp 1;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp int\WEFTsp kj\WEFTsp =\WEFTbrk \WEFTsp \hbox{\slshape\sffamily key\/}[\WEFTbrk j]\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp do\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \{\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp int\WEFTsp ki\WEFTsp =\WEFTbrk \WEFTsp \hbox{\slshape\sffamily key\/}[\WEFTbrk i]\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp if\WEFTsp (\WEFTbrk \hbox{\slshape\sffamily ordering\/})\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp break;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \hbox{\slshape\sffamily key\/}[\WEFTbrk i\WEFTsp +\WEFTbrk \WEFTsp 1]\WEFTbrk \WEFTsp =\WEFTbrk \WEFTsp ki;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp \WEFTsp i\WEFTsp -\WEFTbrk =\WEFTbrk \WEFTsp 1;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \}\WEFTsp while\WEFTsp (\WEFTbrk i\WEFTsp >\WEFTbrk =\WEFTbrk \WEFTsp 0)\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{\slshape\sffamily key\/}[\WEFTbrk i\WEFTsp +\WEFTbrk \WEFTsp 1]\WEFTbrk \WEFTsp =\WEFTbrk \WEFTsp kj;\WEFTbrk \WEFTeol
\mbox{\strut}\}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item {\WEFTtxtMacroNoRef}.

\item{}
\end{list}
\WEFTend
Test in-text $\langle\,${\itshape Sort key of size n for ordering}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ usage.
\end{document}
EOF

# [Add other files here.  Avoid any extra processing such as
# decompression until after demo has run.  If demo fails this script
# can save time by not decompressing. ]

$bin/weft -w tex test.w
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
