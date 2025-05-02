#!/bin/sh

compile () {
	OUT="$1"
	SCAN="${2:-$1}"
	CC=""
	CFLAGS=""
	RESULT=""

	OLD="`pwd`"
	cd $SCAN

	ARR="`ls -d *.make 2>/dev/null`"
	if [ "x$ARR" = "x" ]; then
		echo 'print-cflags:' > scan_$$.mk
		echo '	@echo "$(ALL_CFLAGS) $(FORCE_INCLUDE)"' >> scan_$$.mk
		echo 'print-cc:' >> scan_$$.mk
		echo '	@echo "$(CC)"' >> scan_$$.mk
		echo "include Makefile" >> scan_$$.mk
		CC="`gmake -f scan_$$.mk print-cc`"
		CFLAGS="$CFLAGS `${MAKE:-gmake} -f scan_$$.mk print-cflags ${MAKEFLAGS}`"
		rm -f scan_*.mk
	else
		for i in $ARR; do
			echo 'print-cflags:' > scan_$$.mk
			echo '	@echo "$(ALL_CFLAGS) $(FORCE_INCLUDE)"' >> scan_$$.mk
			echo 'print-cc:' >> scan_$$.mk
			echo '	@echo "$(CC)"' >> scan_$$.mk
			echo "include $i" >> scan_$$.mk
			CC="`gmake -f scan_$$.mk print-cc`"
			CFLAGS="$CFLAGS `${MAKE:-gmake} -f scan_$$.mk print-cflags ${MAKEFLAGS}`"
			rm -f scan_*.mk
		done
	fi

	cd $OLD

	cd $OUT
	rm -f compile_flags.txt
	for i in $CFLAGS; do
		has=false
		case "$i" in
			-I*)
				case "$i" in
					-I/*)
						;;
					*)
						i="-I$OLD/$SCAN/`echo $i | sed s/-I//`"
						;;
				esac
			;;
		esac
		for j in $RESULT; do
			if [ "x$i" = "x$j" ]; then
				has=true
				break
			fi
		done
		if $has; then
			:
		else
			RESULT="$RESULT $i"
			echo "$i" >> compile_flags.txt
		fi
	done
	cd $OLD
}

compile "src"
compile "util"

cp src/compile_flags.txt include/
