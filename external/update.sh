#!/bin/sh
OLD=`pwd`
for i in `ls -d *`; do
	if [ "$i" = "update.sh" ]; then
		continue
	fi
	cd $i
	if [ "x`ls -d .git 2>/dev/null`" = "x.git" ]; then
		cd $OLD
		echo "Git: $i"
		git submodule update --init --remote --force $i >/dev/null 2>&1
	elif [ -f get.sh ]; then
		echo "Shell: $i"
		./get.sh >/dev/null 2>&1
	fi
	cd $OLD
done
