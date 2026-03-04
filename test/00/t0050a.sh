#!/bin/sh
#
# Test of @l c in LaTeX mode
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
	echo "FAILED test of @l c in LaTeX mode" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @l c in LaTeX mode" 1>&2
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
# test @l c in LaTeX mode generates #line + section markers
#

cat > test.w <<'EOF'
\documentclass{article}
\begin{document}
@l c
@o test.c
@{#include <stdio.h>
int main() { return 0; }
@}
\end{document}
EOF

cat > test.expected.c <<'EOF'

#line 5 "test.w"
/* {1: test.w:5} */
#include <stdio.h>
int main() { return 0; }
/* {:1} */
EOF

$bin/weft test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.c
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
