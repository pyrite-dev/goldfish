/**
 * @file gf_type/graphic.h
 * @~english
 * @brief Type definitions related to graphic interface
 */

#ifndef __GF_TYPE_GRAPHIC_H__
#define __GF_TYPE_GRAPHIC_H__

#include <gf_pre.h>
#include <gf_macro.h>

#ifdef GF_EXPOSE_GRAPHIC
#define gf_graphic_color_t struct gf_graphic_color

gf_graphic_color_t;

/* External library */

/* Engine */

/* Standard */

/**
 * @struct gf_graphic_color
 * @~english
 * @brief Color
 *
 * @var gf_graphic_color::r
 * @brief Red
 *
 * @var gf_graphic_color::g
 * @brief Green
 *
 * @var gf_graphic_color::b
 * @brief Blue
 *
 * @var gf_graphic_color::a
 * @brief Alpha
 */
GF_DECLARE_TYPE(graphic_color, {
	double r;
	double g;
	double b;
	double a;
});
#else
#error "should not happen!"
#endif

#endif
