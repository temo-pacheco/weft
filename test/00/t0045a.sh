#!/bin/sh
#
# Test of @l command with Markdown cosmetic fence stripping
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
	echo "FAILED test of @l command with Markdown cosmetic fence stripping" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of @l command with Markdown cosmetic fence stripping" 1>&2
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
# test @l command with Markdown cosmetic fence stripping
# Fences (```) around @d/@o definitions should be silently removed
#

printf 'Some prose.\n\n@l c\n```c\n@d My fragment\n@{code here\n@}\n```\n\nMore prose.\n' > test.w

cat > test.expected.md <<"EOF"
Some prose.


<a id="weft1"></a>
**&lt;*My fragment* 1&gt;** =
<pre><code class="language-c">code here
</code></pre>
> *Fragment never referenced*.

More prose.
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
