#!/bin/sh
#
# Test of Markdown parameter formatting
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
	echo "FAILED test of Markdown parameter formatting" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Markdown parameter formatting" 1>&2
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
# test Markdown parameter formatting
#

cat > test.w <<"EOF"
@d Test with @'alpha@'
@{Use @1 here.
@}
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**&lt;*Test with <i>alpha</i>* 1&gt;** =
<pre><code>Use <i>alpha</i> here.
</code></pre>
> *Fragment never referenced*.
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
