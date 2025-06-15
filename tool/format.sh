#!/bin/sh
exec clang-format --verbose -i `find src include util "(" -name "*.c" -or -name "*.h" ")" -and -not -path "external/*" -and -not -path "gf_powered.c"` external/jar/*.h examples/src/*.c
