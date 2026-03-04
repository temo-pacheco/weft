#!/bin/sh
#
# Test that long lines (>500 chars) in tangle do not crash
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
	echo "FAILED test of long lines in tangle" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of long lines in tangle" 1>&2
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
# test: a line with 600+ characters should tangle without error
# (old MAX_INDENT=500 would crash with "maximum indentation exceeded")
#

# Generate a long SVG path attribute (800 chars on a single line)
longattr=$(python3 -c "print('M ' + ' '.join(f'L {i} {i*2}' for i in range(100)))")

cat > test.w <<EOF
@o output.svg
@{<svg xmlns="http://www.w3.org/2000/svg">
<path d="$longattr"/>
</svg>
@}
EOF

$bin/weft test.w
if test $? -ne 0 ; then fail; fi

# Verify the output file exists and contains the long line
test -f output.svg
if test $? -ne 0 ; then fail; fi

grep -q "M " output.svg
if test $? -ne 0 ; then fail; fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass
