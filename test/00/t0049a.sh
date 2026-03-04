#!/bin/sh
#
# Test that explicit -cp flag overrides @l c
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
	echo "FAILED test that explicit -cp flag overrides @l c" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test that explicit -cp flag overrides @l c" 1>&2
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
# test that -cp overrides @l c (no #line, no section markers)
#

cat > test.w <<'EOF'
\documentclass{article}
\begin{document}
@l c
@o test.c -cp
@{print("hello")
@}
```
\end{document}
EOF

cat > test.expected.c <<'EOF'
print("hello")
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
