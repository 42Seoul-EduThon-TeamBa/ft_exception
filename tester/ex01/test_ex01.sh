#!/bin/bash

## ft_exception dash ex01 tester

# # 현재 디렉토리로 복사
echo 'Copy files:'
cp ../../ex01/try_me.c ./ex01.c

# 컴파일 검사
echo 'Compile test'
if ! cc -Wall -Wextra -Werror ex01.c test_ex01.c -o test_generated
then
    echo 'Does not compiled. KO :('
    exit 1
fi

# 프로그램 검사
# 8: Floating point error
# 11: Segmentation fault
ARR='8 11'
idx=1
for i in $ARR
do
    ./test_generated $i
    if [ $? -ne $i ]
    then
        echo "Program test $idx failed. KO :("
        exit 1
    fi
    echo "Program test $((idx++)). OK :)"
done

echo "Exercise success!"
