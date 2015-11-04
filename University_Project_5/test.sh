#=============================================================================
#     FileName: test.sh
#         Desc: Test script for Assignment 2 
#       Author: Max Lee (Ho Suk Lee)
#        Email: hoso1312@gmail.com
#     HomePage: http://imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2015-10-08 21:09:55
#=============================================================================

vals=3 # until test file
type=1 # default test type
ARR=("pg11-in" "pg2600" "pg4000" "2pg4000" "4pg4000")
#Custom test file names

# Script =====================================================================


if [ ! -f "$1".c ];
then
    echo "Invalid File"
    echo ">usage  runtest.sh [file_name]"
    exit 1
fi

gcc -Wall -o "$1" "$1".c -lm

if [ $? -ne 0 ];
then
    echo "Compile failed"
    exit 1
else
    echo "******************"
    echo " Freshly Compiled"
    echo "******************"
fi

echo "Test Initiated"
echo "File Name : $1"
echo "----------------"

if [ ! $2 ];
then
    echo "Default Test : Compare"
else
    echo "Default Test : Decode"
    let type=2
fi

echo "Starting Default Test"
echo "----------------"
i=0
while [ $i -le $vals ];
do
    echo "Default test$i"

    if [ $type -ne 1 ];
    then
        echo './'$1' < test'$i'.txt | ./ass2d | diff - test'$i'.txt'
        time ./$1 < test$i".txt" | ./ass2d | diff - test$i".txt"
    else
        echo './$1 < test'$i'-in.txt | diff - test'$i'.txt'
        time ./$1 < test$i-in.txt | diff - test$i.txt
    fi

    if [ $? -ne 0 ]
    then
        echo ""
        echo "Failed at test$i"
        exit $?
    else
        echo "> Success"
    fi

    echo "----------------"

    let i+=1
done

echo "*****************"
echo "----------------"

echo "Starting Custom Test"
for j in ${ARR[*]};
do
    echo "Testing $j"
    echo './'$1' < '$j'.txt | ./ass2d | diff - '$j'.txt'
    time ./$1 < $j".txt" | ./ass2d | diff - $j".txt"

    if [ $? -ne 0 ]
    then
        echo ""
        echo "Failed at $j"
        exit $?
    else
        echo "> Success"
    fi

    echo "----------------"
done

echo "*****************************"
echo " All tests passed. Good job."
echo "*****************************"

exit 0
