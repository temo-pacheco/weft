#!/bin/sh
#
# Test of @l on @d macro: lang stored, used for Markdown class
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
	echo "FAILED test of @l on @d macro for Markdown" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @l on @d macro for Markdown" 1>&2
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
# test: @l python before @d -> class="language-python" in Markdown
# Without @l, @d has no extension to infer from -> plain <pre><code>
#

cat > test.w <<'EOF'
@o main.py
@{@<Setup@>
@}

@l python
@d Setup
@{import sys
@}
EOF

cat > test.expected.md <<'EOF'

<a id="weft1"></a>
**"main.py"** **1** =
<pre><code class="language-py">&lt;<a href="#weft2"><i>Setup</i> 2</a>&gt;
</code></pre>

<a id="weft2"></a>
**&lt;*Setup* 2&gt;** =
<pre><code class="language-python">import sys
</code></pre>
> *Fragment referenced in* <a href="#weft1">1</a>.
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
