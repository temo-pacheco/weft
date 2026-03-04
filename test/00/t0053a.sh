#!/bin/sh
#
# Test of @L overriding built-in language (c -> // comments, no #line)
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
	echo "FAILED test of @L overriding built-in language" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @L overriding built-in language" 1>&2
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
# test @L c // overrides built-in C (which normally uses /* */ + #line)
# Now C should use // comments and no #line
#

cat > test.w <<'EOF'
\documentclass{article}
@L c //
\begin{document}
@l c
@o test.c
@{int main() { return 0; }
@}
```
\end{document}
EOF

cat > test.expected.c <<'EOF'
// {1: test.w:6}
int main() { return 0; }
// {:1}
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
