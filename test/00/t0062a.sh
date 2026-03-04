#!/bin/sh
#
# Test of -m (map) producing valid JSON with files and fragments
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
	echo "FAILED test of -m map output" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of -m map output" 1>&2
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
# test -m map output
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

$bin/weft -m test.w > map.json 2>/dev/null
if test $? -ne 0 ; then fail; fi

# Verify valid JSON with at least one file and one fragment
python3 -c "
import json, sys
d = json.load(open('map.json'))
assert 'files' in d, 'missing files key'
assert 'fragments' in d, 'missing fragments key'
assert len(d['files']) >= 1, 'no files found'
assert len(d['fragments']) >= 1, 'no fragments found'
# Check that hello.c is in files
fnames = [f['name'] for f in d['files']]
assert 'hello.c' in fnames, 'hello.c not in files'
# Check that Print greeting is in fragments
mnames = [f['name'] for f in d['fragments']]
assert 'Print greeting' in mnames, 'Print greeting not in fragments'
"
if test $? -ne 0 ; then fail; fi

pass
