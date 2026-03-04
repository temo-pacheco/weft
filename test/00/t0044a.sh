#!/bin/sh
#
# Test of simultaneous tangle and Markdown weave
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
	echo "FAILED test of tangle plus Markdown weave" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of tangle plus Markdown weave" 1>&2
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
# test simultaneous tangle and Markdown weave
#

cat > test.w <<"EOF"
@o test.c
@{int main() { return 0; }
@}
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**"test.c"** **1** =
<pre><code class="language-c">int main() { return 0; }
</code></pre>
EOF

cat > test.expected.c <<"EOF"

#line 2 "test.w"
/* {1: test.w:2} */
int main() { return 0; }
/* {:1} */
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.c test.c
if test $? -ne 0 ; then fail; fi

pass
