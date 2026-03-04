#!/bin/sh
#
# Test of Markdown output file definition with HTML escaping
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
	echo "FAILED test of Markdown file definition" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of Markdown file definition" 1>&2
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
# test Markdown file definition with HTML escaping
#

cat > test.w <<"EOF"
@o test.c
@{#include <stdio.h>
int main() { return 0; }
@}
EOF

cat > test.expected.md <<"EOF"

<a id="weft1"></a>
**"test.c"** **1** =
<pre><code class="language-c">#include &lt;stdio.h&gt;
int main() { return 0; }
</code></pre>
EOF

$bin/weft -w md test.w
if test $? -ne 0 ; then fail; fi

diff -a --context test.expected.md test.md
if test $? -ne 0 ; then fail; fi

pass
