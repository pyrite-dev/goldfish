#!/bin/sh
exec clang-format --verbose -i `find src util "(" -name "*.c" -or -name "*.h" ")" -and -not -path "external/*"` external/jar/*.h examples/src/*.c
