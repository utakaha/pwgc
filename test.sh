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

echo "All tests passed"
