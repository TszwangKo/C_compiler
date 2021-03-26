#!/bin/bash

FILE_DIR=$1
COND=${2:-"null"}

if [[ "${COND}" = "null" ]] 
then
    echo "single mode"
fi

TEST_NAME=$(basename ${FILE_DIR})
mkdir -p output

# # Try to find a portable way of getting rid of
# # any stray carriage returns
# if which dos2unix ; then
#     DOS2UNIX="dos2unix"
# elif which fromdos ; then
#     DOS2UNIX="fromdos"
# else
#     # Tested for combinations of:
#     # - Ubuntu
#     # - Cygwin
#     # and inputs formats:
#     # - LF
#     # - CRLF
# fi

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean

echo "========================================"
echo " Force building compiler"
make -B bin/c_compiler
if [[ "$?" -ne "0" ]]; then
    echo "Error while building compiler."
    exit 1;
fi

echo "========================================"
if [[ "${COND}" = "2" ]]
then
    for i in compiler_tests/${FILE_DIR}/*_driver.c; do
        b=$(basename ${i});
        IFS='_'
        read -a strarr <<< "${b}"
        if [[ ${#strarr[*]} -eq 3 ]]
        then 
            c="${strarr[0]}_${strarr[1]}"
        else
            c="${strarr[0]}"
        fi
        IFS=''

        ./bin/c_compiler -S compiler_tests/${FILE_DIR}/${c}.c -o output/${c}.s > /dev/null 2> /dev/null
        mips-linux-gnu-gcc -mfp32 -o output/${c}.o -c output/${c}.s > /dev/null 2> /dev/null
        mips-linux-gnu-gcc -mfp32 -static -o ./output/${c} ./output/${c}.o ./compiler_tests/${FILE_DIR}/${c}_driver.c > /dev/null 2> /dev/null
        qemu-mips ./output/${c}
        
        RESULT=$?
        echo "Test : ${c}"
        if [[ RESULT -ne "0" ]]; then
            echo "RESULT: FAIL"
            echo "OUPUT: $RESULT"
        else 
            echo "RESULT: PASS "
            PASSED=$(( ${PASSED}+1 ));
        fi
        CHECKED=$(( ${CHECKED}+1 ));
        echo "================================"
    done
    echo "Passed ${PASSED} out of ${CHECKED}".
# elif [[  "${COND}" = "c"  ]] 
#     then
#          ./bin/c_compiler -S compiler_tests/${FILE_DIR}.c -o output/${TEST_NAME}.s  2> /dev/null
#          mips-linux-gnu-gcc -mfp32 -o output/${TEST_NAME}.o -c output/${TEST_NAME}.s
else
    ./bin/c_compiler -S compiler_tests/${FILE_DIR}.c -o output/${TEST_NAME}.s  2> /dev/null
    mips-linux-gnu-gcc -mfp32 -o output/${TEST_NAME}.o -c output/${TEST_NAME}.s
    mips-linux-gnu-gcc -mfp32 -static -o ./output/${TEST_NAME} ./output/${TEST_NAME}.o ./compiler_tests/${FILE_DIR}_driver.c
    qemu-mips ./output/${TEST_NAME}
    RESULT=$?
    echo "Test : $TEST_NAME"
    if [[ RESULT -ne "0" ]]; then
        echo "RESULT: FAIL"
        echo "OUPUT: $RESULT"
    else 
        echo "RESULT: PASS"
    fi
fi