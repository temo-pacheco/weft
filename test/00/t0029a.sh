#!/bin/sh
#
# $RCSfile: t0029a.sh,v $-- Test test/00/t0029a.sh
#
#
# Test of Bar in fragment names is invisible
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
	echo "FAILED test of Bar in fragment names is invisible" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Bar in fragment names is invisible" 1>&2
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
# test Bar in fragment names is invisible
#

cat > test.w <<"EOF"
\documentclass{article}
\begin{document}
@d Atom
@{@}

@d atom
@{@}

@d Atomic
@{@}

@d atomi...
@{
@}

@d Save |file| abc
@{@}

@d Save file uvw
@{@}

@d Adam
@{@}

@d atoms
@{@}

@o test.c
@{@<Atom@>
@<atom@>
@<Save |file| abc@>
@<Save file uvw@>
@<Adam@>
@<atomic@>
@<atoms@>
@<Atomic@>
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
\newcommand{\WEFTbegin}{\par\addvspace{2.3ex plus .6ex}\begingroup\small\raggedright}
\newcommand{\WEFTend}{\par\endgroup\addvspace{2.3ex plus .6ex}}
\providecommand{\WEFTlstsetup}{\lstset{basicstyle=\ttfamily\small,breaklines=true,breakatwhitespace=false,columns=fullflexible,keepspaces=true,showstringspaces=false,keywordstyle=\bfseries,commentstyle=\itshape,tabsize=8,xleftmargin=1.5em}}
\newcommand{\WEFTuseHyperlinks}{}
\documentclass{article}
\begin{document}
\WEFTbegin
\label{scrap1}
\WEFTtarget{weft?}{} $\langle\,${\itshape Atom}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap2}
\WEFTtarget{weft?}{} $\langle\,${\itshape atom}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap3}
\WEFTtarget{weft?}{} $\langle\,${\itshape Atomic}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap4}
\WEFTtarget{weft?}{} $\langle\,${\itshape atomic}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]


\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap5}
\WEFTtarget{weft?}{} $\langle\,${\itshape Save |file| abc}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap6}
\WEFTtarget{weft?}{} $\langle\,${\itshape Save file uvw}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap7}
\WEFTtarget{weft?}{} $\langle\,${\itshape Adam}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap8}
\WEFTtarget{weft?}{} $\langle\,${\itshape atoms}\nobreak\ {\footnotesize {?}}$\,\rangle\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)}]

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item \WEFTtxtMacroRefIn\ \WEFTlink{weft?}{?}.

\item{}
\end{list}
\WEFTend
\WEFTbegin
\label{scrap9}
\WEFTtarget{weft?}{} \verb@"test.c"@\nobreak\ {\footnotesize {?}}$\equiv$
\begin{lstlisting}[escapeinside={(*<}{>*)},language=C]
(*<\hbox{\normalfont $\langle\,${\itshape Atom}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape atom}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Save |file| abc}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Save file uvw}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Adam}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape atomic}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape atoms}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)
(*<\hbox{\normalfont $\langle\,${\itshape Atomic}\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$}>*)

\end{lstlisting}
{\WEFTsep}
\vspace{-1.5ex}
\footnotesize
\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}

\item{}
\end{list}
\WEFTend

{\small\begin{list}{}{\setlength{\itemsep}{-\parsep}\setlength{\itemindent}{-\leftmargin}}
\item $\langle\,$Adam\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$Atom\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$atom\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$Atomic\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$atomic\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$atoms\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$Save |file| abc\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\item $\langle\,$Save file uvw\nobreak\ {\footnotesize \WEFTlink{weft?}{?}}$\,\rangle$ {\footnotesize {\WEFTtxtRefIn} \WEFTlink{weft?}{?}.}
\end{list}}

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
