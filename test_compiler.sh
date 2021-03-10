#!/bin/bash

 file_dir = $1
 test_name = $(basename ${file_dir})
mkdir -p output

# Try to find a portable way of getting rid of
# any stray carriage returns
if which dos2unix ; then
    DOS2UNIX="dos2unix"
elif which fromdos ; then
    DOS2UNIX="fromdos"
else
    # Tested for combinations of:
    # - Ubuntu
    # - Cygwin
    # and inputs formats:
    # - LF
    # - CRLF
fi

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean

echo "========================================"
echo " Force building compiler"
make -B bin/compiler
if [[ "$?" -ne "0" ]]; then
    echo "Error while building compiler."
    exit 1;
fi

echo "========================================"

./bincompiler -S compiler_tests/${test_dir} -o output/${test_name}.s
mips-linux-gnu-gcc -mfp32 -o output/${test_name}.o -c output/${test_name}.s
mips-linux-gnu-gcc -mfp32 -static -o ${test_name} ${test_name}.o ${test_dir}_driver.c
qemu-mips test_program ${test_name}
RESULT = $?
echo "Test : {$test_name}"
if [[ RESULT -ne "0" }}; then
    echo "RESULT: FAIL"
    echo "OUPUT: {$RESULT}"
else 
    echo "RESULT: PASS"
fi