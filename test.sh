#!/bin/bash

set -e

if [ ! -f ./pwgc ]; then
    echo "Error: pwgc binary not found"
    echo "Run 'make' first"
    exit 1
fi

echo "Default length (20 characters)"
result=$(./pwgc)
len=${#result}
if [ $len -ne 20 ]; then
    echo "FAIL: Expected length 20, got $len"
    exit 1
fi
echo "PASS"
echo ""

echo "Custom length with -l 30"
result=$(./pwgc -l 30)
len=${#result}
if [ $len -ne 30 ]; then
    echo "FAIL: Expected length 30, got $len"
    exit 1
fi
echo "PASS"
echo ""

echo "Custom length with -l 15"
result=$(./pwgc -l 15)
len=${#result}
if [ $len -ne 15 ]; then
    echo "FAIL: Expected length 15, got $len"
    exit 1
fi
echo "PASS"
echo ""

echo "Invalid length (0)"
if ./pwgc -l 0 2>/dev/null; then
    echo "FAIL: Should reject length 0"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "Invalid length (101)"
if ./pwgc -l 101 2>/dev/null; then
    echo "FAIL: Should reject length > 100"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "Randomness check"
result1=$(./pwgc)
result2=$(./pwgc)
if [ "$result1" = "$result2" ]; then
    echo "FAIL: Generated passwords should be different"
    exit 1
fi
echo "PASS (passwords are different)"
echo ""

echo "Numbers only mode (-n)"
result=$(./pwgc -n)
if echo "$result" | grep -qE '[^a-zA-Z0-9]'; then
    echo "FAIL: -n option should not include symbols"
    exit 1
fi
echo "PASS (no symbols found)"
echo ""

echo "Symbols only mode (-s)"
result=$(./pwgc -s)
if echo "$result" | grep -q '[0-9]'; then
    echo "FAIL: -s option should not include numbers"
    exit 1
fi
if ! echo "$result" | grep -qE '[^a-zA-Z]'; then
    echo "FAIL: -s option should include symbols"
    exit 1
fi
echo "PASS (no numbers, symbols included)"
echo ""

echo "Alphabetic only mode (-a)"
result=$(./pwgc -a)
if echo "$result" | grep -q '[0-9]'; then
    echo "FAIL: -a option should not include numbers"
    exit 1
fi
if echo "$result" | grep -qE '[^a-zA-Z]'; then
    echo "FAIL: -a option should not include symbols"
    exit 1
fi
if ! echo "$result" | grep -q '[a-zA-Z]'; then
    echo "FAIL: -a option should include alphabetic characters"
    exit 1
fi
echo "PASS (only alphabetic characters)"
echo ""

echo "Combined options (-a -l 25)"
result=$(./pwgc -a -l 25)
len=${#result}
if [ $len -ne 25 ]; then
    echo "FAIL: Expected length 25, got $len"
    exit 1
fi
if echo "$result" | grep -qE '[^a-zA-Z]'; then
    echo "FAIL: Should only contain alphabetic characters"
    exit 1
fi
echo "PASS"
echo ""

echo "Invalid option (-aaa)"
if ./pwgc -aaa 2>/dev/null; then
    echo "FAIL: Should reject invalid option -aaa"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "Mutually exclusive options (-a -n)"
if ./pwgc -a -n 2>/dev/null; then
    echo "FAIL: Should reject -a and -n together"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "Mutually exclusive options (-a -s)"
if ./pwgc -a -s 2>/dev/null; then
    echo "FAIL: Should reject -a and -s together"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "Mutually exclusive options (-n -s)"
if ./pwgc -n -s 2>/dev/null; then
    echo "FAIL: Should reject -n and -s together"
    exit 1
fi
echo "PASS (rejected as expected)"
echo ""

echo "All tests passed"
