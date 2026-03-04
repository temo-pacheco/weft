#!/bin/sh
#
# Test of nested fragment expansion with section markers
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
	echo "FAILED test of nested fragment expansion with section markers" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of nested fragment expansion with section markers" 1>&2
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
# test nested fragments with section markers
#

cat > test.w <<'EOF'
\documentclass{article}
\begin{document}
@l js
@o app.js
@{// app start
@<Setup@>
@}
```

@d Setup
@{const x = 1;
@<Init@>
@}

@d Init
@{console.log(x);
@}

\end{document}
EOF

cat > test.expected.js <<'EOF'
// {1: test.w:5}
// app start
// Setup
// {2: test.w:11}
const x = 1;
// Init
// {3: test.w:16}
console.log(x);
// {:3}

// {:2}

// {:1}
EOF

$bin/weft test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.js app.js
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
