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
\WEFTtarget{weft1a}{} \verb@"test.c"@\nobreak\ {\footnotesize {1a}}$\equiv$
\WEFTcode
\mbox{\strut}Begin\WEFTeol
\mbox{\strut}Define\WEFTsp abc\WEFTsp cba\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Outer abc and def retuO}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Outer cba and fed retuO}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}, \ldots\ }$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}.
\item \WEFTtxtIdentsDefed\nobreak\  \verb@abc@\nobreak\ \WEFTlink{weft1b}{1b}, \verb@cba@\nobreak\ \WEFTlink{weft1b}{1b}.\item \WEFTtxtIdentsUsed\nobreak\  \verb@def@\nobreak\ \WEFTlink{weft1b}{1b}, \verb@fed@\nobreak\ \WEFTlink{weft1b}{1b}.
\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Start\WEFTeol
\mbox{\strut}Define\WEFTsp def\WEFTsp fed\WEFTeol
\mbox{\strut}Use\WEFTsp abc\WEFTsp cba\WEFTeol
\mbox{\strut}\hbox{$\langle\,${\itshape Inner xArg1yArg2z rennI}\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$}\WEFTeol
\mbox{\strut}Finish\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.
\item \WEFTtxtIdentsDefed\nobreak\  \verb@def@\nobreak\ \WEFTlink{weft1a}{1a}, \verb@fed@\nobreak\ \WEFTlink{weft1a}{1a}.\item \WEFTtxtIdentsUsed\nobreak\  \verb@abc@\nobreak\ \WEFTlink{weft1a}{1a}, \verb@cba@\nobreak\ \WEFTlink{weft1a}{1a}.
\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1c}{} $\langle\,${\itshape Inner \hbox{\slshape\sffamily Stuff\/} rennI}\nobreak\ {\footnotesize {1c}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}XX>\WEFTbrk >\WEFTbrk \hbox{\slshape\sffamily Stuff\/}<\WEFTbrk <\WEFTbrk YY{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1d}{} \verb@"test.c"@\nobreak\ {\footnotesize {1d}}$\equiv$
\WEFTcode
\mbox{\strut}More\WEFTsp stuff\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtFileDefBy\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1e}{} $\langle\,${\itshape Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO}\nobreak\ {\footnotesize {1e}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Added\WEFTsp stuff\WEFTsp to\WEFTsp force\WEFTsp fragment\WEFTsp defined\WEFTeol
\mbox{\strut}cross-\WEFTbrk reference\WEFTsp entry.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroDefBy\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}.
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Inner \hbox{\slshape\sffamily Stuff\/} rennI\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1b}{1b}.}
\item $\langle\,$Outer \hbox{\slshape\sffamily Arg1\/} and \hbox{\slshape\sffamily Arg2\/} retuO\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}\WEFTlink{weft1e}{e}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft1a}{1a}.}
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
