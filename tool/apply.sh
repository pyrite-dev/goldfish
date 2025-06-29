#!/bin/sh
for i in `find src -name "*.c"`; do
	echo $i
	( echo $i | grep "bindgen" >/dev/null 2>&1 ) || ./tool/license.pl $i
done
for i in `find include -name "*.h"`; do
	echo $i
	./tool/license.pl $i --only-license
done
