#! /usr/bin/env bash

set -ue

mkdir -p tmp
expected_file="tmp/expected.txt"
actual_file="tmp/actual.txt"

cat > "$expected_file" <<EOF
dump dgtvec---------
digits: (none)
ndigits: 0
cap: 0
--------------------
dump dgtvec---------
digits: 1
ndigits: 1
cap: 1
--------------------
dump dgtvec---------
digits: 3, 1, 2
ndigits: 3
cap: 4
--------------------
EOF
./test_bignum --dumper > "$actual_file"
diff -u "$expected_file" "$actual_file"

echo done
