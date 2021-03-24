#!/bin/bash
echo "Compiling to MIPS..."
echo "compiler_test/basic/basic.c" | ./bin/compiler 1> $4
echo "compiler_test/basic/basic.c"