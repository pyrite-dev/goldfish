#!/bin/sh
for i in `find src -name "*.c"`; do
	./tool/license.pl $i
done
for i in `find include -name "*.h"`; do
	./tool/license.pl $i --only-license
done
