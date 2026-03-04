#!/bin/sh
#
# Test of Markdown indices: @f, @m, @u
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
	echo "FAILED test of Markdown indices" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Markdown indices" 1>&2
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
# test Markdown indices
#

cat > test.w <<"EOF"
@o test.c
@{int x = 1;
@| x @}

@f

@m

@u
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**"test.c"** **1** =
<pre><code class="language-c">int x = 1;
</code></pre>
> *Defines:*  `x` *Never used*.

- `"test.c"` *Defined by* <a href="#weft1">1</a>.



None.



- `x`: **<a href="#weft1">1</a>**.


EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
