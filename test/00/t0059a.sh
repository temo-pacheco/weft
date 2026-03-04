#!/bin/sh
#
# Test of @d without @l or @o: no language class in Markdown
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
	echo "FAILED test of @d without language" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @d without language" 1>&2
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
# test: @d without @l and without @o -> plain <pre><code> (no class)
#

cat > test.w <<'EOF'
@d My fragment
@{some code here
@}
EOF

cat > test.expected.md <<'EOF'

<a id="weft1"></a>
**&lt;*My fragment* 1&gt;** =
<pre><code>some code here
</code></pre>
> *Fragment never referenced*.
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
