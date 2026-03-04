#!/bin/sh
#
# Test of -e (extract) producing fragment output
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
	echo "FAILED test of -e extract output" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of -e extract output" 1>&2
	cd $here
	rm -rf $work
	exit 2
}
trap "no_result" 1 2 3 15

mkdir $work
if test $? -ne 0 ; then no_result; fi
cd $work
if test $? -ne 0 ; then no_result; fi

#
# test -e extract output
#

cat > test.w <<"EOF"
@o hello.c
@{#include <stdio.h>
int main() {
  @<Print greeting@>
  return 0;
}
@}

@d Print greeting
@{printf("Hello, world!\n");
@}
EOF

$bin/weft -e "Print greeting" test.w > extract.md 2>/dev/null
if test $? -ne 0 ; then fail; fi

# Verify the output contains the fragment name and its code
grep -q "Print greeting" extract.md
if test $? -ne 0 ; then fail; fi

grep -q 'printf("Hello, world!' extract.md
if test $? -ne 0 ; then fail; fi

# Verify that extracting a nonexistent fragment fails
$bin/weft -e "Nonexistent fragment" test.w > /dev/null 2>&1
if test $? -eq 0 ; then fail; fi

pass
