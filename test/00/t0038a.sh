#!/bin/sh
#
# Test of basic Markdown output: a single fragment definition
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
	echo "FAILED test of basic Markdown fragment definition" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of basic Markdown fragment definition" 1>&2
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
# test basic Markdown fragment definition
#

cat > test.w <<"EOF"
@d Hello World
@{printf("Hello, World!\n");
@}
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**&lt;*Hello World* 1&gt;** =
<pre><code>printf("Hello, World!\n");
</code></pre>
> *Fragment never referenced*.
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
