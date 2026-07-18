#!/bin/sh
#
# Test auto-suppression of markers for single-line @d fragments
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
	echo "FAILED test of single-line @d auto-suppression ($1)" 1>&2
	cd $here
	rm -rf $work
	exit 1
}
no_result()
{
	set +x
	echo "NO RESULT for test of single-line @d auto-suppression" 1>&2
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
# Test 1: single-line @d expands inline without markers
#

cat > test1.w <<"EOF"
@d port @{4000@}
@d host @{"localhost"@}

@o config.c -s
@{const char *host = @<host@>;
int port = @<port@>;
@}
EOF

$bin/weft -t test1.w > /dev/null 2>&1
if test $? -ne 0 ; then fail "test1 weft"; fi

cat > expected1 <<"EOF"
const char *host = "localhost";
int port = 4000;
EOF

diff config.c expected1
if test $? -ne 0 ; then fail "test1 diff"; fi

rm -f config.c

#
# Test 2: multi-line @d produces markers when parent allows them
#

cat > test2.w <<"EOF"
@d compute hash
@{int h = 0;
for (int i = 0; i < n; i++)
  h = h * 31 + s[i];
@}

@o hash.c -s
@{@<compute hash@>
@}
EOF

$bin/weft -t test2.w > /dev/null 2>&1
if test $? -ne 0 ; then fail "test2 weft"; fi

# Multi-line fragment is NOT auto-suppressed, so the parent's
# comment_flag (C-style) produces a comment with the fragment name
grep -q 'compute hash' hash.c
if test $? -ne 0 ; then fail "test2 comment"; fi

rm -f hash.c

#
# Test 3: @d on separate line from @{ but single-line content
#

cat > test3.w <<"EOF"
@d version
@{2.0@}

@o ver.c -s
@{const char *v = "@<version@>";
@}
EOF

$bin/weft -t test3.w > /dev/null 2>&1
if test $? -ne 0 ; then fail "test3 weft"; fi

cat > expected3 <<"EOF"
const char *v = "2.0";
EOF

diff ver.c expected3
if test $? -ne 0 ; then fail "test3 diff"; fi

rm -f ver.c

#
# Test 4: subsequent multi-line scrap clears auto-suppression
#

cat > test4.w <<"EOF"
@d stuff @{first@}
@d stuff
@{second
third@}

@o out.c -s
@{@<stuff@>
@}
EOF

$bin/weft -t test4.w > /dev/null 2>&1
if test $? -ne 0 ; then fail "test4 weft"; fi

# After the multi-line scrap, auto-suppress is cleared.
# The fragment name comment should appear since comment_flag
# is inherited from the C parent.
grep -q 'stuff' out.c
if test $? -ne 0 ; then fail "test4 markers"; fi

rm -f out.c

pass
