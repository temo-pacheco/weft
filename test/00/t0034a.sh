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
\usepackage{listings}
\begin{document}

\lstset{extendedchars=true,keepspaces=true,language=C}

\WEFTbegin
\label{scrap1}
\WEFTtarget{weft?}{} \verb@"test.c"@\nobreak\ {\footnotesize {?}}$\equiv$
\WEFTcode
\mbox{\strut}int\WEFTeol
\mbox{\strut}main(\WEFTbrk int\WEFTsp argc,\WEFTbrk \WEFTsp char\WEFTsp **\WEFTsp argv)\WEFTbrk \WEFTeol
\mbox{\strut}\{\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Body of main}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}\}\WEFTeol
\mbox{\strut}{\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{} $\langle\,${\itshape Body of main}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}int\WEFTsp in;\WEFTbrk \WEFTeol
\mbox{\strut}unsigned\WEFTsp char\WEFTsp out[\WEFTbrk 20]\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTeol
\mbox{\strut}while\WEFTsp (\WEFTbrk scanf(\WEFTbrk "\%x",\WEFTbrk \WEFTsp \&in)\WEFTbrk \WEFTsp =\WEFTbrk =\WEFTbrk \WEFTsp 1)\WEFTbrk \WEFTeol
\mbox{\strut}\{\WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp \hbox{$\langle\,${\itshape Do one item}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}\WEFTeol
\mbox{\strut}\}\WEFTeol
\mbox{\strut}return\WEFTsp 0;\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{} $\langle\,${\itshape Do one item}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\WEFTcode
\mbox{\strut}int\WEFTsp n\WEFTsp =\WEFTbrk \WEFTsp mangle(\WEFTbrk in,\WEFTbrk \WEFTsp out)\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTeol
\mbox{\strut}for\WEFTsp (\WEFTbrk int\WEFTsp i\WEFTsp =\WEFTbrk \WEFTsp 0;\WEFTbrk \WEFTsp i\WEFTsp <\WEFTbrk \WEFTsp n;\WEFTbrk \WEFTsp i+\WEFTbrk +\WEFTbrk )\WEFTbrk \WEFTeol
\mbox{\strut}\WEFTsp \WEFTsp \WEFTsp printf(\WEFTbrk "\%02x",\WEFTbrk \WEFTsp out[\WEFTbrk i]\WEFTbrk )\WEFTbrk ;\WEFTbrk \WEFTeol
\mbox{\strut}printf(\WEFTbrk "\textbackslash{}n")\WEFTbrk ;\WEFTbrk {\WEFTsep}\WEFTendcode
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
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
