#!/bin/sh
cat > include/gf_all.h << EOF
/**
 * @file gf_all.h
 * @~english
 * @brief Include all GoldFish headers
 */

EOF
cd include
for i in *.h; do
	case "$i" in
		gf_*drv.h)
			:
			;;
		gf_*_static.h)
			:
			;;
		gf_opengl.h)
			:
			;;
		gf_all.h)
			:
			;;
		gf_config.h)
			:
			;;
		gf_pre.h)
			:
			;;
		gf_macro.h)
			:
			;;
		gf_gload.h)
			:
			;;
		*)
			echo "#include <$i>" >> gf_all.h
			;;
	esac
done
