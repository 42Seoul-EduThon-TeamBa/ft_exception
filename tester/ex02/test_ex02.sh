#!/bin/bash

## ft_exception dash ex02 tester

# # 현재 디렉토리로 복사
echo 'Copy files:'
cp ../../ex02/safe_replace.c ./ex02.c

# 컴파일 검사
if ! cc -Wall -Wextra -Werror ex02.c test_ex02.c -o test_generated
then
    echo 'Does not compiled. KO :('
    exit 1
fi

# 프로그램 검사

function timeout() { perl -e 'alarm shift; exec @ARGV' "$@"; }

ARR='1 2 3 4 5 6'
idx=1
for i in $ARR
do
    timeout 5s ./test_generated $i
    if [ $? -ne 0 ]
    then
        echo "Program test $((idx++)) failed. KO :("
        exit 1
    fi
    echo "Program test $((idx++)). OK :)"
done

echo "Exercise success!"
