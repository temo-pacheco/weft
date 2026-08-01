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
\mbox{\strut}Test\WEFTsp of\WEFTsp quoted\WEFTsp fragments.\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert first fragment}\nobreak\ {\footnotesize \WEFTlink{weft1b}{1b}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert second fragment}\nobreak\ {\footnotesize \WEFTlink{weft1c}{1c}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert third fragment}\nobreak\ {\footnotesize \WEFTlink{weft1d}{1d}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter whatsit fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTsp of\WEFTsp test.\WEFTbrk \WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft1b}{} $\langle\,${\itshape Insert first fragment}\nobreak\ {\footnotesize {1b}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}This\WEFTsp fragment\WEFTsp is\WEFTsp not\WEFTsp quoted.\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert unquoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1e}{1e}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert quoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter 1 fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter thing fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTsp of\WEFTsp first\WEFTsp fragment.\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft1c}{} $\langle\,${\itshape Insert second fragment}\nobreak\ {\footnotesize {1c}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}This\WEFTsp fragment\WEFTsp is\WEFTsp quoted.\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert unquoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1e}{1e}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert quoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter 2 fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter thing fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTsp of\WEFTsp second\WEFTsp fragment.\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft1d}{} $\langle\,${\itshape Insert third fragment}\nobreak\ {\footnotesize {1d}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}This\WEFTsp fragment\WEFTsp is\WEFTsp not\WEFTsp quoted.\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert unquoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1e}{1e}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert quoted fragment}\nobreak\ {\footnotesize \WEFTlink{weft1f}{1f}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter 3 fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Insert parameter thing fragment}\nobreak\ {\footnotesize \WEFTlink{weft1g}{1g}}$\,\rangle$}\WEFTeol
\mbox{\strut}End\WEFTsp of\WEFTsp third\WEFTsp fragment.\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft1e}{} $\langle\,${\itshape Insert unquoted fragment}\nobreak\ {\footnotesize {1e}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}This\WEFTsp fragment\WEFTsp in\WEFTsp file\WEFTsp \hbox{\sffamily\slshape file name}\WEFTsp is\WEFTsp not\WEFTsp quoted{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft1f}{} $\langle\,${\itshape Insert quoted fragment}\nobreak\ {\footnotesize {1f}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}This\WEFTsp fragment\WEFTsp in\WEFTsp file\WEFTsp \hbox{\sffamily\slshape file name}\WEFTsp is\WEFTsp quoted{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1b}{1b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap7}
\WEFTtarget{weft1g}{} $\langle\,${\itshape Insert parameter \hbox{\slshape\sffamily thing\/} fragment}\nobreak\ {\footnotesize {1g}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}Here\WEFTsp >\WEFTbrk >\WEFTbrk \hbox{\slshape\sffamily thing\/}<\WEFTbrk <\WEFTbrk \WEFTsp is\WEFTsp the\WEFTsp parameter{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft1a}{1a}\WEFTlink{weft1b}{b}\WEFTlink{weft1c}{c}\WEFTlink{weft1d}{d}.

\item{}
\end{list}
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
