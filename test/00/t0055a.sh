#!/bin/sh
#
# Test of -s per-file flag: suppress markers but keep lang for Markdown
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
	echo "FAILED test of -s per-file flag" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of -s per-file flag" 1>&2
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
# test -s flag: no markers in tangle, but class="language-c" in Markdown
#

cat > test.w <<'EOF'
@l c
@o test.c -s
@{int main() { return 0; }
@}
EOF

# Tangle: no #line, no section markers, just raw code
cat > test.expected.c <<'EOF'
int main() { return 0; }
EOF

# Markdown: still gets class="language-c"
cat > test.expected.md <<'EOF'

<a id="weft1"></a>
**"test.c"** **1** =
<pre><code class="language-c">int main() { return 0; }
</code></pre>
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.c
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
