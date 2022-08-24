#!/bin/bash

# Build
gcc -Wall -Wextra -Werror -o test_generated signal_practice.c test_ex00.c
if [ $? -ne 0 ]
then
    echo "Compilation failed"
    exit 1
fi

# Run

function timeout() { perl -e 'alarm shift; exec @ARGV' "$@"; }

function test_case {
    echo "Test $1"
    timeout 5s ./test_generated $3
    if [ $? -ne $2 ]
    then
        echo "Test $1 KO :("
        exit 1
    fi
    echo "Test $1 OK :D"
}

# case 1 - no call register fpe
test_case 1 136

# case 2 - call register fpe
test_case 2 42 "catch"
