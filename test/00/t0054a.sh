#!/bin/sh
#
# Test of @L with +d flag enabling #line directives
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
	echo "FAILED test of @L with +d flag" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @L with +d flag" 1>&2
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
# test @L glsl // +d → // markers AND #line directives
#

cat > test.w <<'EOF'
\documentclass{article}
@L glsl // +d
\begin{document}
@l glsl
@o shader.glsl
@{void main() { gl_FragColor = vec4(1.0); }
@}
```
\end{document}
EOF

cat > test.expected.glsl <<'EOF'

#line 6 "test.w"
// {1: test.w:6}
void main() { gl_FragColor = vec4(1.0); }
// {:1}
EOF

$bin/weft test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.glsl shader.glsl
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
