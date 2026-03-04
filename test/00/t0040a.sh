#!/bin/sh
#
# Test of Markdown cross-references: fragment defined in 2 scraps, referenced in 1
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
	echo "FAILED test of Markdown cross-references" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Markdown cross-references" 1>&2
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
# test Markdown cross-references
#

cat > test.w <<"EOF"
@o test.c
@{@<Part A@>
@<Part B@>
@}

@d Part A
@{int x = 1;
@}

@d Part A
@{int y = 2;
@}

@d Part B
@{int z = 3;
@}
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**"test.c"** **1** =
<pre><code class="language-c">&lt;<a href="#weft2"><i>Part A</i> 2, ...</a>&gt;
&lt;<a href="#weft4"><i>Part B</i> 4</a>&gt;
</code></pre>

<a id="weft2"></a>
**&lt;*Part A* 2&gt;** =
<pre><code>int x = 1;
</code></pre>
> *Fragment defined by* <a href="#weft2">2</a><a href="#weft3">, 3</a>.
> *Fragment referenced in* <a href="#weft1">1</a>.

<a id="weft3"></a>
**&lt;*Part A* 3&gt;** =
<pre><code>int y = 2;
</code></pre>
> *Fragment defined by* <a href="#weft2">2</a><a href="#weft3">, 3</a>.
> *Fragment referenced in* <a href="#weft1">1</a>.

<a id="weft4"></a>
**&lt;*Part B* 4&gt;** =
<pre><code>int z = 3;
</code></pre>
> *Fragment referenced in* <a href="#weft1">1</a>.
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
