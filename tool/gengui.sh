#!/bin/sh

n=0

COMPONENTS=""
for i in `ls -d src/gui/*.c`; do
	COMPONENTS="$COMPONENTS `echo $i | rev | cut -d"_" -f1 | rev | cut -d"." -f1`"
	n=`expr $n + 1`
done

##### gf_gui_static.h #####

generate () {
cat >> include/gf_gui_static.h << EOF
/**
 * @~english
 * @brief Run \`$1\` procedure for all components
 * @param gui GUI
 * @param c Component
 */
EOF
	echo "static void gf_gui_all_$1(gf_gui_t* gui, gf_gui_component_t* c){" >> include/gf_gui_static.h
	for i in $COMPONENTS; do
		echo "	extern void gf_gui_${i}_$1(gf_gui_t* gui, gf_gui_component_t* c);" >> include/gf_gui_static.h
	done
	for i in $COMPONENTS; do
		echo "	gf_gui_${i}_$1(gui, c);" >> include/gf_gui_static.h
	done
	echo "}" >> include/gf_gui_static.h
	echo >> include/gf_gui_static.h
}

cat > include/gf_gui_static.h << EOF
/**
 * @file gf_gui_static.h
 * @~english
 * @brief GUI static stuff
 * @note User should not include this, this header is used internally
 * @warning This file is auto-generated using ./tool/gengui.sh at \``LC_ALL= LANG= date`\`
 */

#ifndef __GF_GUI_STATIC_H__
#define __GF_GUI_STATIC_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/gui.h>
#include <gf_gui_component.h>

/* Standard */
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

EOF

echo "#define GF_GUI_COMPONENTS $n" >> include/gf_gui_static.h

cat >> include/gf_gui_static.h << EOF
/**
 * @~english
 * @brief Initialize GUI component calls
 */
EOF
incr=0
echo "static void gf_gui_init_calls(void){" >> include/gf_gui_static.h
echo "	extern gf_gui_call_t gf_gui_calls[$n];" >> include/gf_gui_static.h
for i in $COMPONENTS; do
	echo "	/*** Begin $i ***/" >> include/gf_gui_static.h
	echo "	gf_gui_calls[$incr].name = malloc(512);" >> include/gf_gui_static.h
	echo "	strcpy(gf_gui_calls[$incr].name, \"$i\");" >> include/gf_gui_static.h
	echo "	gf_gui_calls[$incr].call = gf_gui_create_${i};" >> include/gf_gui_static.h
	echo "	/***  End  $i ***/" >> include/gf_gui_static.h
	incr=`expr $incr + 1`
done
echo "}" >> include/gf_gui_static.h

generate "render"
generate "drag"
generate "click"

cat >> include/gf_gui_static.h << EOF
#ifdef __cplusplus
}
#endif

#endif
EOF

##### gf_gui_component.h #####

generate2 () {
cat >> include/gf_gui_component.h << EOF
/**
 * @~english
 * @brief Create $1 component
 * @param gui GUI
 * @param x X
 * @param y Y
 * @param w Width
 * @param h Height
 * @return Component ID
 */
EOF
	echo "GF_EXPORT gf_gui_id_t GFAPI gf_gui_create_$1(gf_gui_t* gui, double x, double y, double w, double h);" >> include/gf_gui_component.h
	echo >> include/gf_gui_component.h
}

cat > include/gf_gui_component.h << EOF
/**
 * @file gf_gui_component.h
 * @~english
 * @brief GUI component stuff
 * @warning This file is auto-generated using ./tool/gengui.sh at \``LC_ALL= LANG= date`\`
 */

#ifndef __GF_GUI_COMPONENT_H__
#define __GF_GUI_COMPONENT_H__

#include <gf_pre.h>
#include <gf_macro.h>

/* Type */

/* Engine */
#include <gf_type/gui.h>

/* Standard */

#ifdef __cplusplus
extern "C" {
#endif

EOF

for i in $COMPONENTS; do
	generate2 "$i"
done

echo "GF_EXPORT gf_gui_call_t gf_gui_calls[$n];" >> include/gf_gui_component.h
echo >> include/gf_gui_component.h

cat >> include/gf_gui_component.h << EOF
#ifdef __cplusplus
}
#endif

#endif
EOF
