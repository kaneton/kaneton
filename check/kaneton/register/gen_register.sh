#! /bin/sh

test_filename="$1"

cat <<EOF
#include <kaneton.h>

void	check_register_tests(void)
{
EOF

for i in `grep 'symbol:' "$test_filename" | cut -d':' -f2`
do
	echo "void $i(void);"
	echo "debug_callbacks_register(\"$i\", $i);"
	echo ""
done

cat <<EOF
}
EOF
